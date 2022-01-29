#include "tui.h"

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "modalwindow.h"

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Renderer, Horizontal, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, Element, filler, text, hbox, separator, center, vbox, bold, border, clear_under, dbox, size, GREATER_THAN, HEIGHT
#include "ftxui/component/captured_mouse.hpp"  // for ftxui

void Tui::runTDPClient() {
    do {
        showLoginView();
    } while (needToReconnect && !terminateProgram);
}

void Tui::showLoginView() {
    using namespace ftxui;

    if (needToReconnect) {
        connectToServer();
        if (terminateProgram) return;
    }

    string login, passwd;
    auto inputLogin = Input(&login, "");
    InputOption password_opt;
    password_opt.password = true;
    auto inputPassword = Input(&passwd, "", password_opt);

    auto screen = ScreenInteractive::TerminalOutput();

    auto quitButton = Button("  Quit  ", screen.ExitLoopClosure());
    auto loginButton = Button("  Sign in  ", [&] {
        logInUser(login, passwd);
        login = "";
        passwd = "";
        if (needToReconnect) quitButton->OnEvent(Event::Return);
    });

    auto header = center(bold(text("TinDox Console Client")));

    auto inputs_components = Container::Vertical({inputLogin, inputPassword});
    auto render_input_section = Renderer(inputs_components, [&] {
        return vbox({
                            window(text("Login"), inputLogin->Render()) | size(WIDTH, GREATER_THAN, 15),
                            window(text("Password"), inputPassword->Render()) | size(WIDTH, GREATER_THAN, 15)
                    }) | center;
    });

    auto buttons_components = Container::Horizontal({loginButton, quitButton});
    auto render_buttons = Renderer(buttons_components, [&] {
        return hbox(loginButton->Render(), quitButton->Render()) | center;
    });

    auto main_view_components = Container::Vertical({inputs_components, buttons_components});

    auto main_view = Renderer(main_view_components, [&] {
        return vbox({
                            header,
                            separator(),
                            render_input_section->Render(),
                            render_buttons->Render()
                    }) | border;
    });

    screen.Loop(main_view);
}

void Tui::connectToServer() {
    using namespace ftxui;

    ModalWindow unableToConnectModal("Cannot connect to server.", "Try again.");

    for (int attempt = 1; attempt <= 5 && !tdpService.initConnection(server_ip, server_port); ++attempt) {
        unableToConnectModal.showModalWindow();
        if (attempt == 5) {
            terminateProgram = true;
            return;
        }
    }

    needToReconnect = false;
    system("clear");
}

void Tui::logInUser(string &login, string &password) {
    if (!tdpService.auth(login, password)) {
        ModalWindow wrongCredentialsModal("Wrong login and/or password.", "Try again.");

        if (tdpService.error_code == ConnectionError::E_INVALID_CREDENTIALS || tdpService.error_code == E_BAD_FIELD) {
            wrongCredentialsModal.showModalWindow();

        } else if (!needToReconnect && tdpService.error_code == E_CLOSED_CON) {
            wrongCredentialsModal.showModalWindow();
            needToReconnect = true;
        }
    } else {
        showFilesView();
        if (!tdpService.logout())
            needToReconnect = true;
    }
}

void Tui::showFilesView() {
    using namespace ftxui;

    vector<string> file_entries;
    string location, user_name;
    int selected_file = 0;

    if (!getUserName(user_name) || !updateFilesEntries(location, file_entries))
        return;

    auto screen = ScreenInteractive::TerminalOutput();

    auto header = center(bold(text("TinDox Console Client")));
    auto user_info = vbox({
                                  text(" User: ") | center | color(Color::Cyan) | bold,
                                  text(" " + user_name + " ") | center | color(Color::CyanLight)
                          });

    auto logoutButton = Button("  Logout  ", screen.ExitLoopClosure());
    auto no_border_opt = ButtonOption();
    no_border_opt.border = false;
    auto createButton = Button("Create", screen.ExitLoopClosure(), no_border_opt);
    auto copyButton = Button("Copy", screen.ExitLoopClosure(), no_border_opt);
    auto moveButton = Button("Move", screen.ExitLoopClosure(), no_border_opt);
    auto deleteButton = Button("Delete", screen.ExitLoopClosure(), no_border_opt);
    auto uploadButton = Button("Upload", screen.ExitLoopClosure(), no_border_opt);
    auto downloadButton = Button("Download", screen.ExitLoopClosure(), no_border_opt);

    auto bottom_buttons = Container::Horizontal(
            {createButton, copyButton, moveButton, deleteButton, uploadButton, downloadButton});

    MenuOption menu_opt;
    menu_opt.style_selected = bgcolor(Color::Blue);
    menu_opt.style_focused = bgcolor(Color::BlueLight);
    menu_opt.style_selected_focused = bgcolor(Color::BlueLight);
    menu_opt.on_enter = screen.ExitLoopClosure();
    auto files_view_menu = Menu(&file_entries, &selected_file, &menu_opt);

    auto main_view_components = Container::Vertical({
                                                            logoutButton,
                                                            files_view_menu,
                                                            bottom_buttons
                                                    });

    auto main_view = Renderer(main_view_components, [&] {
        return vbox({
                            hbox(user_info, separator(), header | flex, separator(), logoutButton->Render()),
                            separator(),
                            text("Location: " + location) | dim,
                            separator(),
                            files_view_menu->Render() | vscroll_indicator | frame |
                            size(HEIGHT, LESS_THAN, 15),
                            separator(),
                            hbox(createButton->Render() | center | flex, separator(),
                                 copyButton->Render() | center | flex, separator(),
                                 moveButton->Render() | center | flex, separator(),
                                 deleteButton->Render() | center | flex, separator(),
                                 uploadButton->Render() | center | flex, separator(),
                                 downloadButton->Render() | center | flex)

                    }) | border;
    });

    screen.Loop(main_view);
}

bool Tui::updateFilesEntries(string &path, vector<string> &entries) {
    if (!tdpService.pwd()) {
        needToReconnect = true;
        return false;
    }
    std::stringstream buf(tdpService.response_body);
    buf >> path;

    if (!tdpService.ls(path, "true", "true")) {
        needToReconnect = true;
        return false;
    }
    entries.clear();
    if (path != "/")
        entries.emplace_back("../");

    std::stringstream ls_result(tdpService.response_body);
    int word_counter = 0, space_numb;
    string word, entry;

    while (ls_result >> word) {

        if (word_counter == 0 || word_counter == 2)
            word.erase(word.begin());

        if (word_counter == 0 || word_counter == 3)
            word.pop_back();

        entry += word;

        if (word_counter == 0 || word_counter == 1) {
            if (word_counter == 0)
                space_numb = 40 - word.size();
            else {
                space_numb = 15 - word.size();
                if (word == "-")
                    entry.replace(entry.find_first_of(' '), 1, "/");
            }

            if (space_numb > 0)
                entry += string(space_numb, ' ');
            else
                entry += ' ';

        } else if (word_counter == 2)
            entry += ' ';
        else {
            entries.push_back(entry);
            entry.clear();
        }

        word_counter = (word_counter + 1) % 4;
    }

    return true;
}

bool Tui::getUserName(string &name) {
    if (!tdpService.name()) {
        needToReconnect = true;
        return false;
    }
    std::stringstream buf(tdpService.response_body);
    buf >> name;
    return true;
}

//----------------------------------------------------------------------------------------------------------------

void Tui::modal() {
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    // There are two layers. One at depth = 0 and the modal window at depth = 1;
    int depth = 0;

    // The current rating of FTXUI.
    std::string rating = "3/5 stars";

    // At depth=0, two buttons. One for rating FTXUI and one for quitting.
    auto button_rate_ftxui = Button("Rate FTXUI", [&] { depth = 1; });
    auto button_quit = Button("Quit", screen.ExitLoopClosure());

    auto depth_0_container = Container::Horizontal({
                                                           button_rate_ftxui,
                                                           button_quit,
                                                   });
    auto depth_0_renderer = Renderer(depth_0_container, [&] {
        return vbox({
                            text("Modal dialog example"),
                            separator(),
                            text("☆☆☆ FTXUI:" + rating + " ☆☆☆") | bold,
                            filler(),
                            hbox({
                                         button_rate_ftxui->Render(),
                                         filler(),
                                         button_quit->Render(),
                                 }),
                    }) |
               border | size(HEIGHT, GREATER_THAN, 18) | center;
    });

    // At depth=1, The "modal" window.
    std::vector<std::string> rating_labels = {
            "1/5 stars", "2/5 stars", "3/5 stars", "4/5 stars", "5/5 stars",
    };

    auto on_rating = [&](std::string new_rating) {
        rating = new_rating;
        depth = 0;
    };

    auto depth_1_container = Container::Horizontal({
                                                           Button(&rating_labels[0],
                                                                  [&] { on_rating(rating_labels[0]); }),
                                                           Button(&rating_labels[1],
                                                                  [&] { on_rating(rating_labels[1]); }),
                                                           Button(&rating_labels[2],
                                                                  [&] { on_rating(rating_labels[2]); }),
                                                           Button(&rating_labels[3],
                                                                  [&] { on_rating(rating_labels[3]); }),
                                                           Button(&rating_labels[4],
                                                                  [&] { on_rating(rating_labels[4]); }),
                                                   });

    auto depth_1_renderer = Renderer(depth_1_container, [&] {
        return vbox({
                            text("Do you like FTXUI?"),
                            separator(),
                            hbox(depth_1_container->Render()),
                    }) |
               border;
    });

    auto main_container = Container::Tab(
            {
                    depth_0_renderer,
                    depth_1_renderer,
            },
            &depth);

    auto main_renderer = Renderer(main_container, [&] {
        Element document = depth_0_renderer->Render();

        if (depth == 1) {
            document = dbox({
                                    document,
                                    depth_1_renderer->Render() | clear_under | center,
                            });
        }
        return document;
    });

    screen.Loop(main_renderer);
}

void Tui::menu_framed() {
    using namespace ftxui;
    std::vector<std::string> entries;
    int selected = 0;

    for (int i = 0; i < 30; ++i)
        entries.push_back("Entry " + std::to_string(i));
    auto radiobox = Menu(&entries, &selected);
    auto renderer = Renderer(radiobox, [&] {
        return radiobox->Render() | vscroll_indicator | frame |
               size(HEIGHT, LESS_THAN, 10) | border;
    });

    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(renderer);

}

void Tui::select_menu() {
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
            "entry 1",
            "entry 2",
            "entry 3",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, &option);

    screen.Loop(menu);

    std::cout << "Selected element = " << selected << std::endl;
}

void Tui::menu_style() {
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
            "Monkey", "Dog", "Cat", "Bird", "Elephant",
    };
    int menu_1_selected_ = 0;
    int menu_2_selected_ = 0;
    int menu_3_selected_ = 0;
    int menu_4_selected_ = 0;
    int menu_5_selected_ = 0;
    int menu_6_selected_ = 0;

    MenuOption option_1;
    option_1.style_focused = bold | color(Color::Blue);
    option_1.style_selected = color(Color::Blue);
    option_1.style_selected_focused = bold | color(Color::Blue);
    option_1.on_enter = screen.ExitLoopClosure();
    auto menu_1_ = Menu(&entries, &menu_1_selected_, &option_1);

    MenuOption option_2;
    option_2.style_focused = bold | color(Color::Blue);
    option_2.style_selected = color(Color::Blue);
    option_2.style_selected_focused = bold | color(Color::Blue);
    option_2.on_enter = screen.ExitLoopClosure();
    auto menu_2_ = Menu(&entries, &menu_2_selected_, &option_2);

    MenuOption option_3;
    option_3.style_selected = color(Color::Blue);
    option_3.style_focused = bgcolor(Color::Blue);
    option_3.style_selected_focused = bgcolor(Color::Blue);
    option_3.on_enter = screen.ExitLoopClosure();
    auto menu_3_ = Menu(&entries, &menu_3_selected_, &option_3);

    MenuOption option_4;
    option_4.style_selected = bgcolor(Color::Blue);
    option_4.style_focused = bgcolor(Color::BlueLight);
    option_4.style_selected_focused = bgcolor(Color::BlueLight);
    option_4.on_enter = screen.ExitLoopClosure();
    auto menu_4_ = Menu(&entries, &menu_4_selected_, &option_4);

    MenuOption option_5;
    option_5.style_normal = bgcolor(Color::Blue);
    option_5.style_selected = bgcolor(Color::Yellow);
    option_5.style_focused = bgcolor(Color::Red);
    option_5.style_selected_focused = bgcolor(Color::Red);
    option_5.on_enter = screen.ExitLoopClosure();
    auto menu_5_ = Menu(&entries, &menu_5_selected_, &option_5);

    MenuOption option_6;
    option_6.style_normal = dim | color(Color::Blue);
    option_6.style_selected = color(Color::Blue);
    option_6.style_focused = bold | color(Color::Blue);
    option_6.style_selected_focused = bold | color(Color::Blue);
    option_6.on_enter = screen.ExitLoopClosure();
    auto menu_6_ = Menu(&entries, &menu_6_selected_, &option_6);

    auto container = Container::Horizontal({
                                                   menu_1_,
                                                   menu_2_,
                                                   menu_3_,
                                                   menu_4_,
                                                   menu_5_,
                                                   menu_6_,
                                           });

    // clang-format off
    auto renderer = Renderer(container, [&] {
        return
                hbox({
                             menu_1_->Render() | flex, separator(),
                             menu_2_->Render() | flex, separator(),
                             menu_3_->Render() | flex, separator(),
                             menu_4_->Render() | flex, separator(),
                             menu_5_->Render() | flex, separator(),
                             menu_6_->Render() | flex,
                     }) | border;
    });
    // clang-format on

    screen.Loop(renderer);
}

void Tui::nested_menus() {
    using namespace ftxui;
    auto screen = ScreenInteractive::FitComponent();
    auto button_quit = Button("Quit", screen.ExitLoopClosure());
    auto button_nested = Button("Nested", [] { Nested(""); });
    screen.Loop(Container::Vertical({
                                            button_quit,
                                            button_nested,
                                    }));
}

void Tui::Nested(std::string path) {
    using namespace ftxui;
    auto screen = ScreenInteractive::FitComponent();
    auto back_button = Button("Back", screen.ExitLoopClosure());
    auto goto_1 = Button("Goto /1", [path] { Nested(path + "/1"); });
    auto goto_2 = Button("Goto /2", [path] { Nested(path + "/2"); });
    auto goto_3 = Button("Goto /3", [path] { Nested(path + "/3"); });
    auto layout = Container::Vertical({
                                              back_button,
                                              goto_1,
                                              goto_2,
                                              goto_3,
                                      });
    auto renderer = Renderer(layout, [&] {
        return vbox({
                            text("path: " + path),
                            separator(),
                            back_button->Render(),
                            goto_1->Render(),
                            goto_2->Render(),
                            goto_3->Render(),
                    }) |
               border;
    });
    screen.Loop(renderer);
}

void Tui::dropdown() {
    using namespace ftxui;

    std::vector<std::string> entries = {
            "tribute", "clearance", "ally", "bend", "electronics",
            "module", "era", "cultural", "sniff", "nationalism",
            "negotiation", "deliver", "figure", "east", "tribute",
            "clearance", "ally", "bend", "electronics", "module",
            "era", "cultural", "sniff", "nationalism", "negotiation",
            "deliver", "figure", "east", "tribute", "clearance",
            "ally", "bend", "electronics", "module", "era",
            "cultural", "sniff", "nationalism", "negotiation", "deliver",
            "figure", "east",
    };

    int selected_1 = 0;
    int selected_2 = 0;
    int selected_3 = 0;
    int selected_4 = 0;

    auto layout = Container::Vertical({
                                              Container::Horizontal({
                                                                            Dropdown(&entries, &selected_1),
                                                                            Dropdown(&entries, &selected_2),
                                                                    }),
                                              Container::Horizontal({
                                                                            Dropdown(&entries, &selected_3),
                                                                            Dropdown(&entries, &selected_4),
                                                                    }),
                                      });

    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(layout);
}

#include <cmath>

void Tui::test() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    // ---------------------------------------------------------------------------
    // HTOP
    // ---------------------------------------------------------------------------

    int shift = 0;

    auto my_graph = [&shift](int width, int height) {
        std::vector<int> output(width);
        for (int i = 0; i < width; ++i) {
            float v = 0.5f;
            v += 0.1f * sin((i + shift) * 0.1f);
            v += 0.2f * sin((i + shift + 10) * 0.15f);
            v += 0.1f * sin((i + shift) * 0.03f);
            v *= height;
            output[i] = (int) v;
        }
        return output;
    };

    auto htop = Renderer([&] {
        auto frequency = vbox({
                                      text("Frequency [Mhz]") | hcenter,
                                      hbox({
                                                   vbox({
                                                                text("2400 "),
                                                                filler(),
                                                                text("1200 "),
                                                                filler(),
                                                                text("0 "),
                                                        }),
                                                   graph(std::ref(my_graph)) | flex,
                                           }) | flex,
                              });

        auto utilization = vbox({
                                        text("Utilization [%]") | hcenter,
                                        hbox({
                                                     vbox({
                                                                  text("100 "),
                                                                  filler(),
                                                                  text("50 "),
                                                                  filler(),
                                                                  text("0 "),
                                                          }),
                                                     graph(std::ref(my_graph)) | color(Color::RedLight),
                                             }) | flex,
                                });

        auto ram = vbox({
                                text("Ram [Mo]") | hcenter,
                                hbox({
                                             vbox({
                                                          text("8192"),
                                                          filler(),
                                                          text("4096 "),
                                                          filler(),
                                                          text("0 "),
                                                  }),
                                             graph(std::ref(my_graph)) | color(Color::BlueLight),
                                     }) | flex,
                        });

        return hbox({
                            vbox({
                                         frequency | flex,
                                         separator(),
                                         utilization | flex,
                                 }) | flex,
                            separator(),
                            ram | flex,
                    }) |
               flex | border;
    });

    // ---------------------------------------------------------------------------
    // Compiler
    // ---------------------------------------------------------------------------

    const std::vector<std::string> compiler_entries = {
            "gcc",
            "clang",
            "emcc",
            "game_maker",
            "Ada compilers",
            "ALGOL 60 compilers",
            "ALGOL 68 compilers",
            "Assemblers (Intel *86)",
            "Assemblers (Motorola 68*)",
            "Assemblers (Zilog Z80)",
            "Assemblers (other)",
            "BASIC Compilers",
            "BASIC interpreters",
            "Batch compilers",
            "C compilers",
            "Source-to-source compilers",
            "C++ compilers",
            "C# compilers",
            "COBOL compilers",
            "Common Lisp compilers",
            "D compilers",
            "DIBOL/DBL compilers",
            "ECMAScript interpreters",
            "Eiffel compilers",
            "Fortran compilers",
            "Go compilers",
            "Haskell compilers",
            "Java compilers",
            "Pascal compilers",
            "Perl Interpreters",
            "PHP compilers",
            "PL/I compilers",
            "Python compilers",
            "Scheme compilers and interpreters",
            "Smalltalk compilers",
            "Tcl Interpreters",
            "VMS Interpreters",
            "Rexx Interpreters",
            "CLI compilers",
    };

    int compiler_selected = 0;
    Component compiler = Radiobox(&compiler_entries, &compiler_selected);

    std::array<std::string, 8> options_label = {
            "-Wall",
            "-Werror",
            "-lpthread",
            "-O3",
            "-Wabi-tag",
            "-Wno-class-conversion",
            "-Wcomma-subscript",
            "-Wno-conversion-null",
    };
    std::array<bool, 8> options_state = {
            false, false, false, false, false, false, false, false,
    };

    std::vector<std::string> input_entries;
    int input_selected = 0;
    Component input = Menu(&input_entries, &input_selected);

    auto input_option = InputOption();
    std::string input_add_content;
    input_option.on_enter = [&] {
        input_entries.push_back(input_add_content);
        input_add_content = "";
    };
    Component input_add = Input(&input_add_content, "input files", input_option);

    std::string executable_content_ = "";
    Component executable_ = Input(&executable_content_, "executable");

    Component flags = Container::Vertical({
                                                  Checkbox(&options_label[0], &options_state[0]),
                                                  Checkbox(&options_label[1], &options_state[1]),
                                                  Checkbox(&options_label[2], &options_state[2]),
                                                  Checkbox(&options_label[3], &options_state[3]),
                                                  Checkbox(&options_label[4], &options_state[4]),
                                                  Checkbox(&options_label[5], &options_state[5]),
                                                  Checkbox(&options_label[6], &options_state[6]),
                                                  Checkbox(&options_label[7], &options_state[7]),
                                          });

    auto compiler_component = Container::Horizontal({
                                                            compiler,
                                                            flags,
                                                            Container::Vertical({
                                                                                        executable_,
                                                                                        Container::Horizontal({
                                                                                                                      input_add,
                                                                                                                      input,
                                                                                                              }),
                                                                                }),
                                                    });

    auto render_command = [&] {
        Elements line;
        // Compiler
        line.push_back(text(compiler_entries[compiler_selected]) | bold);
        // flags
        for (int i = 0; i < 8; ++i) {
            if (options_state[i]) {
                line.push_back(text(" "));
                line.push_back(text(options_label[i]) | dim);
            }
        }
        // Executable
        if (!executable_content_.empty()) {
            line.push_back(text(" -o ") | bold);
            line.push_back(text(executable_content_) | color(Color::BlueLight) |
                           bold);
        }
        // Input
        for (auto &it: input_entries) {
            line.push_back(text(" " + it) | color(Color::RedLight));
        }
        return line;
    };

    auto compiler_renderer = Renderer(compiler_component, [&] {
        auto compiler_win = window(text("Compiler"),
                                   compiler->Render() | vscroll_indicator | frame);
        auto flags_win =
                window(text("Flags"), flags->Render() | vscroll_indicator | frame);
        auto executable_win = window(text("Executable:"), executable_->Render());
        auto input_win =
                window(text("Input"), hbox({
                                                   vbox({
                                                                hbox({
                                                                             text("Add: "),
                                                                             input_add->Render(),
                                                                     }) | size(WIDTH, EQUAL, 20) |
                                                                size(HEIGHT, EQUAL, 1),
                                                                filler(),
                                                        }),
                                                   separator(),
                                                   input->Render() | vscroll_indicator | frame |
                                                   size(HEIGHT, EQUAL, 3) | flex,
                                           }));
        return vbox({
                            hbox({
                                         compiler_win,
                                         flags_win,
                                         vbox({
                                                      executable_win | size(WIDTH, EQUAL, 20),
                                                      input_win | size(WIDTH, EQUAL, 60),
                                              }),
                                         filler(),
                                 }) | size(HEIGHT, LESS_THAN, 8),
                            hflow(render_command()) | flex_grow,
                    }) |
               flex_grow | border;
    });

    // ---------------------------------------------------------------------------
    // Spiner
    // ---------------------------------------------------------------------------
    auto spinner_tab_renderer = Renderer([&] {
        Elements entries;
        for (int i = 0; i < 22; ++i) {
            entries.push_back(spinner(i, shift / 2) | bold |
                              size(WIDTH, GREATER_THAN, 2) | border);
        }
        return hflow(std::move(entries)) | border;
    });

    // ---------------------------------------------------------------------------
    // Colors
    // ---------------------------------------------------------------------------
    auto color_tab_renderer = Renderer([] {
        return hbox({
                            vbox({
                                         color(Color::Default, text("Default")),
                                         color(Color::Black, text("Black")),
                                         color(Color::GrayDark, text("GrayDark")),
                                         color(Color::GrayLight, text("GrayLight")),
                                         color(Color::White, text("White")),
                                         color(Color::Blue, text("Blue")),
                                         color(Color::BlueLight, text("BlueLight")),
                                         color(Color::Cyan, text("Cyan")),
                                         color(Color::CyanLight, text("CyanLight")),
                                         color(Color::Green, text("Green")),
                                         color(Color::GreenLight, text("GreenLight")),
                                         color(Color::Magenta, text("Magenta")),
                                         color(Color::MagentaLight, text("MagentaLight")),
                                         color(Color::Red, text("Red")),
                                         color(Color::RedLight, text("RedLight")),
                                         color(Color::Yellow, text("Yellow")),
                                         color(Color::YellowLight, text("YellowLight")),
                                 }),
                            vbox({
                                         bgcolor(Color::Default, text("Default")),
                                         bgcolor(Color::Black, text("Black")),
                                         bgcolor(Color::GrayDark, text("GrayDark")),
                                         bgcolor(Color::GrayLight, text("GrayLight")),
                                         bgcolor(Color::White, text("White")),
                                         bgcolor(Color::Blue, text("Blue")),
                                         bgcolor(Color::BlueLight, text("BlueLight")),
                                         bgcolor(Color::Cyan, text("Cyan")),
                                         bgcolor(Color::CyanLight, text("CyanLight")),
                                         bgcolor(Color::Green, text("Green")),
                                         bgcolor(Color::GreenLight, text("GreenLight")),
                                         bgcolor(Color::Magenta, text("Magenta")),
                                         bgcolor(Color::MagentaLight, text("MagentaLight")),
                                         bgcolor(Color::Red, text("Red")),
                                         bgcolor(Color::RedLight, text("RedLight")),
                                         bgcolor(Color::Yellow, text("Yellow")),
                                         bgcolor(Color::YellowLight, text("YellowLight")),
                                 }),
                    }) |
               hcenter | border;
    });

    // ---------------------------------------------------------------------------
    // Gauges
    // ---------------------------------------------------------------------------
    auto render_gauge = [&shift](int delta) {
        float progress = (shift + delta) % 1000 / 1000.f;
        return hbox({
                            text(std::to_string(int(progress * 100)) + "% ") |
                            size(WIDTH, EQUAL, 5),
                            gauge(progress),
                    });
    };

    auto gauge_component = Renderer([render_gauge] {
        return vbox({
                            render_gauge(0) | color(Color::Black),
                            render_gauge(100) | color(Color::GrayDark),
                            render_gauge(50) | color(Color::GrayLight),
                            render_gauge(6894) | color(Color::White),
                            separator(),
                            render_gauge(6841) | color(Color::Blue),
                            render_gauge(9813) | color(Color::BlueLight),
                            render_gauge(98765) | color(Color::Cyan),
                            render_gauge(98) | color(Color::CyanLight),
                            render_gauge(9846) | color(Color::Green),
                            render_gauge(1122) | color(Color::GreenLight),
                            render_gauge(84) | color(Color::Magenta),
                            render_gauge(645) | color(Color::MagentaLight),
                            render_gauge(568) | color(Color::Red),
                            render_gauge(2222) | color(Color::RedLight),
                            render_gauge(220) | color(Color::Yellow),
                            render_gauge(348) | color(Color::YellowLight),
                    }) |
               border;
    });

    // ---------------------------------------------------------------------------
    // Paragraph
    // ---------------------------------------------------------------------------
    auto make_box = [](size_t dimx, size_t dimy) {
        std::string title = std::to_string(dimx) + "x" + std::to_string(dimy);
        return window(text(title) | hcenter | bold,
                      text("content") | hcenter | dim) |
               size(WIDTH, EQUAL, dimx) | size(HEIGHT, EQUAL, dimy);
    };

    auto paragraph_renderer_left = Renderer([&] {
        auto title_style = bold | bgcolor(Color::Blue) | color(Color::Black);
        std::string str =
                "Lorem Ipsum is simply dummy text of the printing and typesetting "
                "industry. Lorem Ipsum has been the industry's standard dummy text "
                "ever since the 1500s, when an unknown printer took a galley of type "
                "and scrambled it to make a type specimen book.";
        return vbox({
                            // [ Left ]
                            text("Align left:") | title_style,
                            paragraphAlignLeft(str),
                            // [ Center ]
                            text("Align center:") | title_style,
                            paragraphAlignCenter(str),
                            // [ Right ]
                            text("Align right:") | title_style,
                            paragraphAlignRight(str),
                            // [ Justify]
                            text("Align justify:") | title_style,
                            paragraphAlignJustify(str),
                            // [ Side by side ]
                            text("Side by side:") | title_style,
                            hbox({
                                         paragraph(str),
                                         separator() | color(Color::Blue),
                                         paragraph(str),
                                 }),
                            // [ Misc ]
                            text("Elements with different size:") | title_style,
                            flexbox({
                                            make_box(10, 5),
                                            make_box(9, 4),
                                            make_box(8, 4),
                                            make_box(6, 3),
                                            make_box(10, 5),
                                            make_box(9, 4),
                                            make_box(8, 4),
                                            make_box(6, 3),
                                            make_box(10, 5),
                                            make_box(9, 4),
                                            make_box(8, 4),
                                            make_box(6, 3),
                                    }),
                    }) |
               // vscroll_indicator | yflex;
               yflex | vscroll_indicator;
    });

    auto paragraph_renderer_right = Renderer([] {
        return paragraph("<--- This vertical bar is resizable using the  mouse") |
               center;
    });

    int paragraph_renderer_split_position = Terminal::Size().dimx / 2;
    auto paragraph_renderer_group =
            ResizableSplitLeft(paragraph_renderer_left, paragraph_renderer_right,
                               &paragraph_renderer_split_position);
    auto paragraph_renderer_group_renderer =
            Renderer(paragraph_renderer_group,
                     [&] { return paragraph_renderer_group->Render() | border; });

    // ---------------------------------------------------------------------------
    // Tabs
    // ---------------------------------------------------------------------------

    int tab_index = 0;
    std::vector<std::string> tab_entries = {
            "htop", "color", "spinner", "gauge", "compiler", "paragraph",
    };
    auto tab_selection = Toggle(&tab_entries, &tab_index);
    auto tab_content = Container::Tab(
            {
                    htop,
                    color_tab_renderer,
                    spinner_tab_renderer,
                    gauge_component,
                    compiler_renderer,
                    paragraph_renderer_group_renderer,
            },
            &tab_index);

    auto main_container = Container::Vertical({
                                                      tab_selection,
                                                      tab_content,
                                              });

    auto main_renderer = Renderer(main_container, [&] {
        return vbox({
                            text("FTXUI Demo") | bold | hcenter,
                            tab_selection->Render() | hcenter,
                            tab_content->Render() | flex,
                    });
    });

    bool refresh_ui_continue = true;
    std::thread refresh_ui([&] {
        while (refresh_ui_continue) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0.05s);
            shift++;
            screen.PostEvent(Event::Custom);
        }
    });

    screen.Loop(main_renderer);
    refresh_ui_continue = false;
    refresh_ui.join();

}