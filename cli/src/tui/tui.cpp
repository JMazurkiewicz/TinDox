#include "tui.h"

#include <string>
#include <vector>
#include <sstream>
#include "modalwarningwin.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

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

    ModalWarningWin unableToConnectModal("Cannot connect to server.", "Try again.");

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
        ModalWarningWin wrongCredentialsModal("Wrong login and/or password.", "Try again.");

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

    vector<string> file_names;
    vector<string> file_entries;
    string location, user_name;
    int selected_file = 0;

    if (!getUserName(user_name) || !updateFilesEntries(location, file_entries, file_names)) {
        needToReconnect = true;
        return;
    }

    auto screen = ScreenInteractive::TerminalOutput();

    auto header = center(bold(text("TinDox Console Client")));
    auto user_info = vbox({
                                  text(" User: ") | center | color(Color::Cyan) | bold,
                                  text(" " + user_name + " ") | center | color(Color::CyanLight)
                          });

    auto logoutButton = Button("  Logout  ", screen.ExitLoopClosure());
    auto no_border_opt = ButtonOption();
    no_border_opt.border = false;
    auto createButton = Button("Create", [&] {
        if (!createDir() || !updateFilesEntries(location, file_entries, file_names))
            logoutButton->OnEvent(Event::Return);
    }, no_border_opt);
    auto copyButton = Button("Copy", screen.ExitLoopClosure(), no_border_opt);
    auto moveButton = Button("Move", screen.ExitLoopClosure(), no_border_opt);
    auto deleteButton = Button("Delete", [&] {
        if (!deleteFileView(file_names[selected_file]) || !updateFilesEntries(location, file_entries, file_names))
            logoutButton->OnEvent(Event::Return);
    }, no_border_opt);
    auto uploadButton = Button("Upload", screen.ExitLoopClosure(), no_border_opt);
    auto downloadButton = Button("Download", screen.ExitLoopClosure(), no_border_opt);

    auto bottom_buttons = Container::Horizontal(
            {createButton, copyButton, moveButton, deleteButton, uploadButton, downloadButton});

    MenuOption menu_opt;
    menu_opt.style_selected = bgcolor(Color::Blue);
    menu_opt.style_focused = bgcolor(Color::BlueLight);
    menu_opt.style_selected_focused = bgcolor(Color::BlueLight);
    menu_opt.on_enter = [&] {
        if (!changeDirectory(location, file_names[selected_file], file_entries, file_names, selected_file))
            logoutButton->OnEvent(Event::Return);
    };
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
                            size(HEIGHT, LESS_THAN, 15) | size(HEIGHT, GREATER_THAN, 15),
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

bool Tui::updateFilesEntries(string &path, vector<string> &entries, vector<string> &f_names) {
    if (!tdpService.pwd()) {
        needToReconnect = true;
        return false;
    }
    std::stringstream buf(tdpService.response_body);
    string tmp;
    path.clear();
    while (buf >> tmp) {
        path += tmp + " ";
    }
    path.pop_back();

    if (!tdpService.ls(path, "true", "true")) {
        needToReconnect = true;
        return false;
    }
    entries.clear();
    f_names.clear();
    if (path != "/") {
        entries.emplace_back("../");
        f_names.emplace_back("../");
    }

    std::stringstream ls_result(tdpService.response_body);
    int word_counter = 0, space_numb;
    string word, entry;

    while (ls_result >> word) {

        if (word_counter == 0 || word_counter == 2)
            word.erase(word.begin());

        if (word_counter == 0 || word_counter == 3) {
            if (word_counter == 0) {
                while (!word.ends_with("\"")) {
                    entry += word + ' ';
                    ls_result >> word;
                }
            }
            word.pop_back();
        }

        entry += word;

        if (word_counter == 0 || word_counter == 1) {
            if (word_counter == 0) {
                f_names.push_back(entry);
                space_numb = 40 - entry.size();
            } else {
                space_numb = 15 - word.size();
                if (word == "-") {
                    f_names.back() += "/";
                    entry.replace(0, f_names.back().size(), f_names.back());
                }
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

bool Tui::changeDirectory(string &current_location, const string &dir_name, vector<string> &entries,
                          vector<string> &names, int &selected_entry) {

    if (dir_name.ends_with('/')) {

        if (!tdpService.cd(dir_name)) {
            if (!checkIfShouldReconnect()) {
                ModalWarningWin cannotChangeDirModal(" Error occured while ", " changing directory. ");
                cannotChangeDirModal.showModalWindow();
                return true;
            } else
                return false;
        }

        if (!updateFilesEntries(current_location, entries, names))
            return false;

        selected_entry = 0;
    }

    return true;
}

bool Tui::checkIfShouldReconnect() {
    if (tdpService.error_code == ConnectionError::E_CLOSED_CON ||
        tdpService.error_code == ConnectionError::E_EXCHANGE_MESSAGE) {
        needToReconnect = true;
        return true;
    }
    return false;
}

bool Tui::deleteFileView(const string &file_to_delete) {
    using namespace ftxui;

    string file_name = file_to_delete;
    if (file_name.ends_with('/'))
        file_name.pop_back();

    auto screen = ScreenInteractive::TerminalOutput();
    auto cancelButton = Button("   Cancel   ", screen.ExitLoopClosure());
    auto okButton = Button("   Ok   ", [&] {
        if (!tdpService.rm(file_name)) {
            if (!checkIfShouldReconnect()) {
                ModalWarningWin errWind = ModalWarningWin("Cannot delete file:", file_name);
                errWind.showModalWindow();
            }
        }
        cancelButton->OnEvent(Event::Return);
    });

    auto buttons = Container::Horizontal({okButton, cancelButton});

    auto modalWindow = Renderer(buttons, [&] {
        return vbox({
                            center(text(" Do you want to delete file/directory ")),
                            center(text(" " + file_name + "? ")),
                            separator(),
                            buttons->Render() | center
                    }) | border | bgcolor(Color::GrayDark) | clear_under;
    });

    screen.Loop(modalWindow);

    if (needToReconnect)
        return false;
    else
        return true;
}

bool Tui::createDir() {
    using namespace ftxui;

    string dir_name;

    auto inputDirName = Input(&dir_name, "");
    auto screen = ScreenInteractive::TerminalOutput();
    auto cancelButton = Button("   Cancel   ", screen.ExitLoopClosure());
    auto createButton = Button("   Create   ", [&] {
        if (dir_name.empty() || !tdpService.mkdir(dir_name)) {
            if (!checkIfShouldReconnect()) {
                ModalWarningWin errWind = ModalWarningWin("Cannot create directory", dir_name);
                errWind.showModalWindow();
            }
        }
        cancelButton->OnEvent(Event::Return);
    });

    auto buttons = Container::Horizontal({createButton, cancelButton});
    auto components = Container::Vertical({inputDirName, buttons});

    auto modalWindow = Renderer(components, [&] {
        return vbox({
                            center(text(" Enter name of new directory: ")),
                            window(text(""), inputDirName->Render()) | size(WIDTH, GREATER_THAN, 15) | center,
                            separator(),
                            buttons->Render() | center
                    }) | border | bgcolor(Color::GrayDark) | clear_under;
    });

    screen.Loop(modalWindow);

    if (needToReconnect)
        return false;
    else
        return true;
}