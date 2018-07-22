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
#include "../../../core/mixer.h"
#include "../../../core/sampleChannel.h"
#include "../../../utils/log.h"
#include "../../dialogs/actionEditor/baseActionEditor.h"
#include "sampleActionEditor.h"
#include "sampleAction.h"


using std::vector;
using namespace giada;
using namespace giada::m;


geSampleAction::geSampleAction(int X, int Y, int W, int H, const SampleChannel* ch,
	const recorder::action a1, const recorder::action a2)
: Fl_Box     (X, Y, W, H),
  ch         (ch),
  onRightEdge(false),
  onLeftEdge (false),
  hovered    (false),
  pick       (0),
  a1         (a1),
  a2         (a2)
{
	/* A singlepress action narrower than MIN_WIDTH pixel is useless. So check it. 
	Warning: if an action is MIN_WIDTH px narrow, it has no body space to drag it. 
	It's up to the user to zoom in and drag it. */

	if (w() < MIN_WIDTH)
		size(MIN_WIDTH, h());
}


/* -------------------------------------------------------------------------- */


void geSampleAction::draw()
{
	int color = hovered ? G_COLOR_LIGHT_2 : G_COLOR_LIGHT_1; 

	if (ch->mode == ChannelMode::SINGLE_PRESS) {
		fl_rectf(x(), y(), w(), h(), (Fl_Color) color);
	}
	else {
		if (a1.type == G_ACTION_KILL)
			fl_rect(x(), y(), MIN_WIDTH, h(), (Fl_Color) color);
		else {
			fl_rectf(x(), y(), MIN_WIDTH, h(), (Fl_Color) color);
			if (a1.type == G_ACTION_KEYPRESS)
				fl_rectf(x()+3, y()+h()-11, 2, 8, G_COLOR_GREY_4);
			else
			if  (a1.type == G_ACTION_KEYREL)
				fl_rectf(x()+3, y()+3, 2, 8, G_COLOR_GREY_4);
		}
	}

}


/* -------------------------------------------------------------------------- */


int geSampleAction::handle(int e)
{
	switch (e) {
		case FL_ENTER: {
			hovered = true;
			redraw();
			return 1;
		}
		case FL_LEAVE: {
			fl_cursor(FL_CURSOR_DEFAULT, FL_WHITE, FL_BLACK);
			hovered = false;
			redraw();
			return 1;
		}
		case FL_MOVE: {
			if (ch->mode == ChannelMode::SINGLE_PRESS) {
				onLeftEdge  = false;
				onRightEdge = false;
				if (Fl::event_x() >= x() && Fl::event_x() < x() + HANDLE_WIDTH) {
					onLeftEdge = true;
					fl_cursor(FL_CURSOR_WE, FL_WHITE, FL_BLACK);
				}
				else
				if (Fl::event_x() >= x() + w() - HANDLE_WIDTH && 
					  Fl::event_x() <= x() + w()) {
					onRightEdge = true;
					fl_cursor(FL_CURSOR_WE, FL_WHITE, FL_BLACK);
				}
				else
					fl_cursor(FL_CURSOR_DEFAULT, FL_WHITE, FL_BLACK);
			}
			return 1;
		}
		default:
			return Fl_Widget::handle(e);
	}
}


/* -------------------------------------------------------------------------- */


void geSampleAction::setLeftEdge(int p)
{
	resize(p, y(), x() - p + w(), h());
	if (w() < MIN_WIDTH)
		size(MIN_WIDTH, h());
}


/* -------------------------------------------------------------------------- */


void geSampleAction::setRightEdge(int p)
{
	size(p, h());
	if (w() < MIN_WIDTH)
		size(MIN_WIDTH, h());
}


/* -------------------------------------------------------------------------- */


bool geSampleAction::isOnEdges() const
{
	return onLeftEdge || onRightEdge;
}