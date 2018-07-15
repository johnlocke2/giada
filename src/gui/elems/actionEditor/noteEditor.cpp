/* -----------------------------------------------------------------------------
 *
 * Giada - Your Hardcore Loopmachine
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2018 Giovanni A. Zuliani | Monocasual
 *
 * This file is part of Giada - Your Hardcore Loopmachine.
 *
 * Giada - Your Hardcore Loopmachine is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Giada - Your Hardcore Loopmachine is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Giada - Your Hardcore Loopmachine. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------- */


#include <FL/fl_draw.H>
#include "../../../core/const.h"
#include "../../../core/conf.h"
#include "../../../utils/log.h"
#include "../../dialogs/gd_actionEditor.h"
#include "pianoItem.h"
#include "pianoRoll.h"
#include "noteEditor.h"


using namespace giada::m;


geNoteEditor::geNoteEditor(int x, int y)
  : Fl_Scroll(x, y, 200, 422)
{
	gdActionEditor* ae = static_cast<gdActionEditor*>(window());

	size(ae->totalWidth, conf::pianoRollH);
	pianoRoll = new gePianoRoll(x, y, ae->totalWidth);
}


/* -------------------------------------------------------------------------- */


geNoteEditor::~geNoteEditor()
{
	clear();
	conf::pianoRollH = h();
	conf::pianoRollY = pianoRoll->y();
}


/* -------------------------------------------------------------------------- */


void geNoteEditor::rebuild()
{
	pianoRoll->rebuild();
}


/* -------------------------------------------------------------------------- */


void geNoteEditor::draw()
{
	pianoRoll->size(this->w(), pianoRoll->h());  /// <--- not optimal

	/* clear background */

	fl_rectf(x(), y(), w(), h(), G_COLOR_GREY_1);

	/* clip pianoRoll to pianoRollContainer size */

	fl_push_clip(x(), y(), w(), h());
	draw_child(*pianoRoll);
	fl_pop_clip();

	fl_color(G_COLOR_GREY_4);
	fl_line_style(0);
	fl_rect(x(), y(), static_cast<gdActionEditor*>(window())->totalWidth, h());
}
