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
  selected   (false),
  hovered    (false),
  pick       (0),
  a1         (a1),
  a2         (a2)
{
		/* A singlepress action narrower than 8 pixel is useless. So check it. 
		Warning: if an action is 8 px narrow, it has no body space to drag it. It's 
		up to the user to zoom in and drag it. */

		if (w() < MIN_WIDTH)
			size(MIN_WIDTH, h());
}


/* -------------------------------------------------------------------------- */


void geSampleAction::draw()
{
	int color;
	if (hovered)  /// && geActionEditor !disabled
		color = G_COLOR_LIGHT_2;
	else
		color = G_COLOR_LIGHT_1;

	if (ch->mode == ChannelMode::SINGLE_PRESS) {
		fl_rectf(x(), y(), w(), h(), (Fl_Color) color);
	}
	else {
		if (type == G_ACTION_KILL)
			fl_rect(x(), y(), MIN_WIDTH, h(), (Fl_Color) color);
		else {
			fl_rectf(x(), y(), MIN_WIDTH, h(), (Fl_Color) color);
			if (type == G_ACTION_KEYPRESS)
				fl_rectf(x()+3, y()+h()-11, 2, 8, G_COLOR_GREY_4);
			else
			if  (type == G_ACTION_KEYREL)
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
			/* handling of the two margins, left & right. 4 pixels are good enough */
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


bool geSampleAction::isOnEdges() const
{
	return onLeftEdge || onRightEdge;
}


/* -------------------------------------------------------------------------- */


void geSampleAction::addAction()
{
#if 0
	/* TODO action still not on screen, window() returns nullptr */
	gdActionEditor* ae = static_cast<gdActionEditor*>(window());

	/* anatomy of an action
	 * ____[#######]_____ (a) is the left margin, G_ACTION_KEYPRESS. (b) is
	 *     a       b      the right margin, the G_ACTION_KEYREL. This is the
	 * theory behind the singleshot.press actions; for any other kind the
	 * (b) is just a graphical and meaningless point. */

	if (ch->mode == ChannelMode::SINGLE_PRESS) {
		recorder::rec(ae->chan->index, G_ACTION_KEYPRESS, frame_a);
		recorder::rec(ae->chan->index, G_ACTION_KEYREL, frame_a+4096);
		//gu_log("action added, [%d, %d]\n", frame_a, frame_a+4096);
	}
	else {
		recorder::rec(ae->chan->index, ae->getActionType(), frame_a);
		//gu_log("action added, [%d]\n", frame_a);
	}

  ae->chan->hasActions = true;

	recorder::sortActions();

	index++; // important!
#endif
}


/* -------------------------------------------------------------------------- */


void geSampleAction::delAction()
{
#if 0
	gdActionEditor* ae = static_cast<gdActionEditor*>(window());

	/* if SINGLE_PRESS you must delete both the keypress and the keyrelease
	 * actions. */

	if (ch->mode == ChannelMode::SINGLE_PRESS) {
		recorder::deleteAction(ae->chan->index, frame_a, G_ACTION_KEYPRESS,
      false, &mixer::mutex);
		recorder::deleteAction(ae->chan->index, frame_b, G_ACTION_KEYREL,
      false, &mixer::mutex);
	}
	else
		recorder::deleteAction(ae->chan->index, frame_a, type, false,
      &mixer::mutex);

  ae->chan->hasActions = recorder::hasActions(ae->chan->index);


	/* restore the initial cursor shape, in case you delete an action and
	 * the double arrow (for resizing) is displayed */

	fl_cursor(FL_CURSOR_DEFAULT, FL_WHITE, FL_BLACK);
#endif
}


/* -------------------------------------------------------------------------- */


void geSampleAction::moveAction(int frame_a)
{
#if 0
	/* easy one: delete previous action and record the new ones. As usual,
	 * SINGLE_PRESS requires two jobs. If frame_a is valid, use that frame
	 * value. */

	gdActionEditor* ae = static_cast<gdActionEditor*>(window());

	delAction();

	if (frame_a != -1)
		this->frame_a = frame_a;
	else
		this->frame_a = xToFrame_a();


	/* always check frame parity */

	if (this->frame_a % 2 != 0)
		this->frame_a++;

	recorder::rec(ae->chan->index, type, this->frame_a);

	if (ch->mode == ChannelMode::SINGLE_PRESS) {
		frame_b = xToFrame_b();
		recorder::rec(ae->chan->index, G_ACTION_KEYREL, frame_b);
	}

  ae->chan->hasActions = true;

	recorder::sortActions();
#endif
}


/* -------------------------------------------------------------------------- */


int geSampleAction::absx()
{
	//return x() - static_cast<gdActionEditor*>(window())->ac->x();
}


/* -------------------------------------------------------------------------- */


int geSampleAction::xToFrame_a()
{
	//return (absx()) * static_cast<gdActionEditor*>(window())->zoom;
}


/* -------------------------------------------------------------------------- */


int geSampleAction::xToFrame_b()
{
	//return (absx() + w()) * static_cast<gdActionEditor*>(window())->zoom;
}
