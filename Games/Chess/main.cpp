/*
 * Copyright (c) 2020, the SerenityOS developers.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ChessWidget.h"
#include <LibCore/ConfigFile.h>
#include <LibCore/DirIterator.h>
#include <LibGUI/ActionGroup.h>
#include <LibGUI/Application.h>
#include <LibGUI/Menu.h>
#include <LibGUI/MenuBar.h>
#include <LibGUI/Window.h>

int main(int argc, char** argv)
{
    auto app = GUI::Application::construct(argc, argv);

    auto window = GUI::Window::construct();
    auto& widget = window->set_main_widget<ChessWidget>();
    widget.set_side(Chess::Colour::White);

    RefPtr<Core::ConfigFile> config = Core::ConfigFile::get_for_app("Chess");

    auto size = config->read_num_entry("Display", "size", 512);
    window->set_title("Chess");
    window->resize(size, size);
    window->set_resizable(false);

    auto icon = Gfx::Bitmap::load_from_file("/res/icons/16x16/app-chess.png");
    window->set_icon(icon);

    widget.set_piece_set(config->read_entry("Style", "PieceSet", "test"));
    widget.set_board_theme(config->read_entry("Style", "BoardTheme", "Beige"));

    auto menubar = GUI::MenuBar::construct();
    auto& app_menu = menubar->add_menu("Chess");

    app_menu.add_action(GUI::Action::create("New game", { Mod_None, Key_F2 }, [&](auto&) {
        widget.reset();
    }));
    app_menu.add_action(GUI::CommonActions::make_quit_action([](auto&) {
        GUI::Application::the()->quit();
    }));

    auto& style_menu = menubar->add_menu("Style");
    GUI::ActionGroup piece_set_action_group;
    piece_set_action_group.set_exclusive(true);
    auto& piece_set_menu = style_menu.add_submenu("Piece Set");
    piece_set_menu.set_icon(icon);

    Core::DirIterator di("/res/icons/chess/sets/", Core::DirIterator::SkipParentAndBaseDir);
    while (di.has_next()) {
        auto set = di.next_path();
        auto action = GUI::Action::create_checkable(set, [&](auto& action) {
            widget.set_piece_set(action.text());
            widget.update();
            config->write_entry("Style", "PieceSet", action.text());
            config->sync();
        });

        piece_set_action_group.add_action(*action);
        if (widget.piece_set() == set)
            action->set_checked(true);
        piece_set_menu.add_action(*action);
    }

    GUI::ActionGroup board_theme_action_group;
    board_theme_action_group.set_exclusive(true);
    auto& board_theme_menu = style_menu.add_submenu("Board Theme");
    board_theme_menu.set_icon(Gfx::Bitmap::load_from_file("/res/icons/chess/mini-board.png"));

    for (auto& theme : Vector({ "Beige", "Green", "Blue" })) {
        auto action = GUI::Action::create_checkable(theme, [&](auto& action) {
            widget.set_board_theme(action.text());
            widget.update();
            config->write_entry("Style", "BoardTheme", action.text());
            config->sync();
        });
        board_theme_action_group.add_action(*action);
        if (widget.board_theme().name == theme)
            action->set_checked(true);
        board_theme_menu.add_action(*action);
    }

    app->set_menubar(move(menubar));

    window->show();

    return app->exec();
}
