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


#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include "baseAction.h"


geBaseAction::geBaseAction(int X, int Y, int W, int H, bool resizable,
	const Channel* ch)
: Fl_Box     (X, Y, W, H),
  m_ch       (ch),
  m_resizable(resizable),
  onRightEdge(false),
  onLeftEdge (false),
  hovered    (false),
  pick       (0)
{
	if (w() < MIN_WIDTH)
		size(MIN_WIDTH, h());
}


/* -------------------------------------------------------------------------- */


int geBaseAction::handle(int e)
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
			if (m_resizable) {
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


void geBaseAction::setLeftEdge(int p)
{
	resize(p, y(), x() - p + w(), h());
	if (w() < MIN_WIDTH)
		size(MIN_WIDTH, h());
}


/* -------------------------------------------------------------------------- */


void geBaseAction::setRightEdge(int p)
{
	size(p, h());
	if (w() < MIN_WIDTH)
		size(MIN_WIDTH, h());
}


/* -------------------------------------------------------------------------- */


void geBaseAction::setPosition(int p)
{
	position(p, y());
}


/* -------------------------------------------------------------------------- */


bool geBaseAction::isOnEdges() const
{
	return onLeftEdge || onRightEdge;
}