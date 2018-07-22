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
#include "../../../core/clock.h"
#include "../../../core/sampleChannel.h"
#include "../../../utils/log.h"
#include "../../../glue/recorder.h"
#include "../../dialogs/gd_mainWindow.h"
#include "../../dialogs/actionEditor/baseActionEditor.h"
#include "../mainWindow/keyboard/keyboard.h"
#include "gridTool.h"
#include "sampleAction.h"
#include "sampleActionEditor.h"


extern gdMainWindow* G_MainWin;


using std::vector;
using namespace giada;
using namespace giada::m;


geSampleActionEditor::geSampleActionEditor(int x, int y, SampleChannel* ch)
  : geBaseActionEditor(x, y, 200, 40),
    ch                (ch),
    action            (nullptr)
{
	rebuild();

	/* If channel is LOOP_ANY, deactivate it: a loop mode channel cannot hold 
	keypress/keyrelease actions. */
	
	if (ch->isAnyLoopMode())
		deactivate();
}


/* -------------------------------------------------------------------------- */


geSampleAction* geSampleActionEditor::getActionAtCursor()
{
	for (int i=0; i<children(); i++) {
		geSampleAction* a = static_cast<geSampleAction*>(child(i));
		if (a->hovered)
			return a;
	}
	return nullptr;
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::rebuild()
{
	namespace mr = m::recorder;
	namespace cr = c::recorder;

	gdBaseActionEditor* ae = static_cast<gdBaseActionEditor*>(window());

	/* Remove all existing actions and set a new width, according to the current
	zoom level. */

	clear();
	size(ae->fullWidth, h());

	vector<mr::Composite> comps = cr::getSampleActions(ch);

	for (mr::Composite comp : comps) {
		gu_log("Action [%d, %d)\n", comp.a1.frame, comp.a2.frame);
		Pixel px = x() + ae->frameToPixel(comp.a1.frame);
		Pixel py = y() + 4;
		Pixel pw = 0;
		Pixel ph = h() - 8;
		if (comp.a2.frame != -1)
				pw = ae->frameToPixel(comp.a2.frame - comp.a1.frame);

		geSampleAction* a = new geSampleAction(px, py, pw, ph, ch, comp.a1, comp.a2);
		add(a);
		resizable(a);
	}

	redraw();
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::draw()
{
	/* Draw basic boundaries (+ beat bars) and hide the unused area. Then draw 
	children (the actions). */

	baseDraw();

	/* Print label. */

	fl_color(G_COLOR_GREY_4);
	fl_font(FL_HELVETICA, 12);
	if (active())
		fl_draw("start/stop", x()+4, y(), w(), h(), (Fl_Align) (FL_ALIGN_LEFT | FL_ALIGN_CENTER));
	else
		fl_draw("start/stop (disabled)", x()+4, y(), w(), h(), (Fl_Align) (FL_ALIGN_LEFT | FL_ALIGN_CENTER));

	draw_children();
}


/* -------------------------------------------------------------------------- */


int geSampleActionEditor::onPush()
{
	gdBaseActionEditor* ae = static_cast<gdBaseActionEditor*>(window());
	
	action = getActionAtCursor();

	if (Fl::event_button1()) {  // Left button
		if (action == nullptr) {  // No action under cursor: add a new one
			if (Fl::event_x() >= ae->loopWidth) // Avoid click on grey area
				return 0;
			Frame f = ae->pixelToFrame(Fl::event_x() - x());
			c::recorder::recordSampleAction(ch, ae->getActionType(), f);
			rebuild();
		}
		else {                     // Action under cursor: get ready for move/resize
			action->pick = Fl::event_x() - action->x();
		}
	}
	else
	if (Fl::event_button3()) {  // Right button
		if (action != nullptr) {
			c::recorder::deleteSampleAction(ch, action->a1, action->a2);
			action = nullptr;
			rebuild();	
		}
	}

	return 1;
}


/* -------------------------------------------------------------------------- */


int geSampleActionEditor::onDrag()
{
	if (action == nullptr)
		return 0;
	if (action->isOnEdges())
		resizeAction();
	else
		moveAction();
	return 1;
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::moveAction()
{
	gdBaseActionEditor* ae = static_cast<gdBaseActionEditor*>(window());

	Pixel ex = Fl::event_x() - action->pick;
	if      (ex < x()) ex = x();
	else if (ex + action->w() > ae->loopWidth + x()) ex = ae->loopWidth + x() - action->w();

	action->setPosition(ex);
	redraw();
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::resizeAction()
{
	gdBaseActionEditor* ae = static_cast<gdBaseActionEditor*>(window());

	Pixel ex = Fl::event_x();
	if      (ex < x()) ex = x();
	else if (ex > ae->loopWidth + x()) ex = ae->loopWidth + x();

	if (action->onRightEdge) 
		action->setRightEdge(ex - action->x());
	else
		action->setLeftEdge(ex);
	redraw();
}


/* -------------------------------------------------------------------------- */


int geSampleActionEditor::onRelease()
{
	if (action == nullptr)
		return 0;

	/* TODO - do this only if the action has been really altered */

	gdBaseActionEditor* ae = static_cast<gdBaseActionEditor*>(window());

	Frame f1 = ae->pixelToFrame(action->x() - x());
	Frame f2 = ae->pixelToFrame(action->x() + action->w() - x());
	c::recorder::deleteSampleAction(ch, action->a1, action->a2);
	c::recorder::recordSampleAction(ch, ae->getActionType(), f1, f2);
	action = nullptr;

	rebuild();

	
	return 1;
}


/* -------------------------------------------------------------------------- */


int geSampleActionEditor::handle(int e)
{
	switch (e) {
		case FL_PUSH:
			return onPush();
		case FL_DRAG:
			return onDrag();
		case FL_RELEASE:
			fl_cursor(FL_CURSOR_DEFAULT, FL_WHITE, FL_BLACK); // Make sure cursor returns normal
			return onRelease();
		default:
			return Fl_Group::handle(e);
	}
}