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


#include "../../../core/kernelMidi.h"
#include "../../../core/const.h"
#include "../../../core/mixer.h"
#include "../../../core/channel.h"
#include "../../../core/clock.h"
#include "../../../core/midiChannel.h"
#include "../../dialogs/gd_actionEditor.h"
#include "noteEditor.h"
#include "pianoRoll.h"
#include "gridTool.h"
#include "pianoItem.h"


using namespace giada::m;


gePianoItem::gePianoItem(int X, int Y, int W, int H, recorder::action a1,
	recorder::action a2)
: geBaseAction(X, Y, W, H, /*resizable=*/true, a1, a2)
{
}


/* -------------------------------------------------------------------------- */


void gePianoItem::draw()
{
	if (a2.frame == -1)
		fl_rect(x(), y()+2, MIN_WIDTH, h()-3, (Fl_Color) hovered ? G_COLOR_LIGHT_2 : G_COLOR_LIGHT_1);
	else
		fl_rectf(x(), y()+2, w(), h()-3, (Fl_Color) hovered ? G_COLOR_LIGHT_2 : G_COLOR_LIGHT_1);
}