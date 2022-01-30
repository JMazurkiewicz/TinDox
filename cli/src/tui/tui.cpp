#include "tui.h"

#include <string>
#include <vector>
#include <sstream>
#include "modalwarningwin.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "modalinputwin.h"

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

    auto renameButton = Button("Rename", [&] {
        if (file_names[selected_file] != "../") {
            if (!renameFile(file_names[selected_file]) || !updateFilesEntries(location, file_entries, file_names))
                logoutButton->OnEvent(Event::Return);
        }
    }, no_border_opt);

    auto copyButton = Button("Copy", [&] {
        if (file_names[selected_file] != "../") {
            if (!copyFile(file_names[selected_file], location) ||
                !updateFilesEntries(location, file_entries, file_names))
                logoutButton->OnEvent(Event::Return);
        }
    }, no_border_opt);
    auto moveButton = Button("Move", [&] {
        if (file_names[selected_file] != "../") {
            if (!moveFile(file_names[selected_file], location) ||
                !updateFilesEntries(location, file_entries, file_names))
                logoutButton->OnEvent(Event::Return);
        }
    }, no_border_opt);

    auto deleteButton = Button("Delete", [&] {
        if (file_names[selected_file] != "../") {
            if (!deleteFileView(file_names[selected_file]) || !updateFilesEntries(location, file_entries, file_names))
                logoutButton->OnEvent(Event::Return);
        }
    }, no_border_opt);
    auto uploadButton = Button("Upload", screen.ExitLoopClosure(), no_border_opt);
    auto downloadButton = Button("Download", screen.ExitLoopClosure(), no_border_opt);

    auto bottom_buttons = Container::Horizontal(
            {createButton, renameButton, copyButton, moveButton, deleteButton, uploadButton, downloadButton});

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
                                 renameButton->Render() | center | flex, separator(),
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
    int word_counter = 0;
    size_t space_numb;
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
    string tmp;
    while (buf >> tmp) {
        name += tmp + " ";
    }
    name.pop_back();
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

    return !needToReconnect;
}

bool Tui::createDir() {
    using namespace ftxui;

    string dir_name;
    ModalInputWin createWind = ModalInputWin(" Enter name of new directory: ", "Create");

    if (createWind.showModalWindow(dir_name)) {
        if (dir_name.empty() || !tdpService.mkdir(dir_name)) {
            if (!checkIfShouldReconnect()) {
                ModalWarningWin errWind = ModalWarningWin("Cannot create directory", dir_name);
                errWind.showModalWindow();
            }
        }
    }

    return !needToReconnect;
}

bool Tui::renameFile(const string &old_name) {
    using namespace ftxui;

    string o_name = old_name;
    if (o_name.ends_with('/'))
        o_name.pop_back();

    string new_name = o_name;
    ModalInputWin createWind = ModalInputWin(" Enter new name for file/directory '" + o_name + "':", "Rename");

    if (createWind.showModalWindow(new_name)) {
        if (new_name.empty() || !tdpService.rename(o_name, new_name)) {
            if (!checkIfShouldReconnect()) {
                ModalWarningWin errWind = ModalWarningWin("Cannot rename file/directory", o_name);
                errWind.showModalWindow();
            }
        }
    }

    return !needToReconnect;
}

bool Tui::copyFile(const string &file_name, const string &current_location) {
    if (!file_name.ends_with('/')) {
        string selected_path = current_location;
        ModalWarningWin errWind = ModalWarningWin(" Cannot copy file ", file_name);

        if (showPathSelectWind("Copy file '" + file_name + "' to directory - enter  path:", "Copy", selected_path)) {

            if (selected_path.empty() ||
                ((!tdpService.cd(current_location) || !tdpService.cp(file_name, selected_path)) &&
                 !checkIfShouldReconnect()))
                errWind.showModalWindow();

        } else if (!needToReconnect && !tdpService.cd(current_location) && !checkIfShouldReconnect()) {
            errWind.showModalWindow();
        }
    }
    return !needToReconnect;
}

bool Tui::moveFile(const string &file_name, const string &current_location) {
    if (!file_name.ends_with('/')) {
        string selected_path = current_location;
        ModalWarningWin errWind = ModalWarningWin(" Cannot move file ", file_name);

        if (showPathSelectWind("Move file '" + file_name + "' to directory - enter  path:", "Move", selected_path)) {

            if (selected_path.empty() ||
                ((!tdpService.cd(current_location) || !tdpService.mv(file_name, selected_path)) &&
                 !checkIfShouldReconnect()))
                errWind.showModalWindow();

        } else if (!needToReconnect && !tdpService.cd(current_location) && !checkIfShouldReconnect()) {
            errWind.showModalWindow();
        }
    }
    return !needToReconnect;
}

bool Tui::showPathSelectWind(const string &instr_text, const string &oper_button_text, string &path) {
    using namespace ftxui;

    vector<string> file_names;
    vector<string> file_entries;
    string location;
    bool proceedWithOperation = false;
    int selected_file = 0, wind_depth = 0;

    if (!updateFilesEntries(location, file_entries, file_names)) {
        needToReconnect = true;
        return false;
    }

    auto input_opt = InputOption();
    input_opt.on_enter = [&] { wind_depth = 1; };
    auto inputName = Input(&path, "", input_opt);
    auto screen = ScreenInteractive::TerminalOutput();
    auto cancelButton = Button("   Cancel   ", screen.ExitLoopClosure());
    auto operationButton = Button("   " + oper_button_text + "   ", [&] {
        proceedWithOperation = true;
        cancelButton->OnEvent(Event::Return);
    });

    auto buttons = Container::Horizontal({operationButton, cancelButton});
    auto input_win_comp = Container::Vertical({inputName, buttons});

    auto inputWindow = Renderer(input_win_comp, [&] {
        return vbox({
                            center(text(instr_text)),
                            window(text(""), inputName->Render()) | size(WIDTH, GREATER_THAN, 15) | center,
                            separator(),
                            buttons->Render() | center
                    }) | border | bgcolor(Color::GrayDark) | clear_under;
    });


    MenuOption menu_opt;
    menu_opt.style_selected = bgcolor(Color::Blue);
    menu_opt.style_focused = bgcolor(Color::BlueLight);
    menu_opt.style_selected_focused = bgcolor(Color::BlueLight);
    menu_opt.on_enter = [&] {
        if (!changeDirectory(location, file_names[selected_file], file_entries, file_names,
                             selected_file))
            cancelButton->OnEvent(Event::Return);
    };
    auto files_view_menu = Menu(&file_entries, &selected_file, &menu_opt);

    auto okButton = Button("   Ok   ", [&] {
        path = location;
        wind_depth = 0;
    });

    auto menu_comp = Container::Vertical({files_view_menu, okButton});
    auto fileSelectWin = Renderer(menu_comp, [&] {
        return vbox(
                text("Choose location") | bold | center,
                separator(),
                text("Location: " + location),
                separator(),
                files_view_menu->Render() | vscroll_indicator | frame |
                size(HEIGHT, LESS_THAN, 15) | size(HEIGHT, GREATER_THAN, 15),
                separator(),
                okButton->Render() | center
        ) | border | bgcolor(Color::DarkGreen) | size(WIDTH, GREATER_THAN, screen.dimx() - 5);
    });

    auto modalWindComp = Container::Tab({
                                                input_win_comp,
                                                menu_comp
                                        }, &wind_depth);

    auto modalWind = Renderer(modalWindComp, [&] {
        Element document = inputWindow->Render();
        if (wind_depth == 1) {
            document = dbox({
                                    document,
                                    fileSelectWin->Render() | clear_under | center,
                            });
        }
        return document;
    });

    screen.Loop(modalWind);

    return proceedWithOperation;
}