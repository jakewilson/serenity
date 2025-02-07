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

#include "PromotionDialog.h"
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/Frame.h>

PromotionDialog::PromotionDialog(ChessWidget& chess_widget)
    : Dialog(chess_widget.window())
{
    set_title("Choose piece to promote to");
    resize(70 * 4, 70);

    auto& main_widget = set_main_widget<GUI::Frame>();
    main_widget.set_frame_shape(Gfx::FrameShape::Container);
    main_widget.set_fill_with_background_color(true);
    main_widget.set_layout<GUI::HorizontalBoxLayout>();

    for (auto& type : Vector({ Chess::Type::Queen, Chess::Type::Knight, Chess::Type::Rook, Chess::Type::Bishop })) {
        auto& button = main_widget.add<GUI::Button>("");
        button.set_icon(chess_widget.get_piece_graphic({ chess_widget.board().turn(), type }));
        button.on_click = [this, type](auto) {
            m_selected_piece = type;
            done(ExecOK);
        };
    }
}

void PromotionDialog::event(Core::Event& event)
{
    Dialog::event(event);
}
