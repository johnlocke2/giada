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
    selected          (nullptr)
{
	rebuild();

	/* If channel is LOOP_ANY, deactivate it: a loop mode channel cannot hold 
	keypress/keyrelease actions. */
	
	if (ch->isAnyLoopMode())
		deactivate();

#if 0
	/* add actions when the window opens. Their position is zoom-based;
	 * each frame is / 2 because we don't care about stereo infos. */

	for (unsigned i=0; i<recorder::frames.size(); i++) {
		for (unsigned j=0; j<recorder::global.at(i).size(); j++) {

		  recorder::action *action = recorder::global.at(i).at(j);

      /* Don't show actions:
      - that don't belong to the displayed channel (!= ae->chan->index);
      - that are covered by the grey area (> G_Mixer.totalFrames);
      - of type G_ACTION_KILL in a SINGLE_PRESS channel. They cannot be
        recorded in such mode, but they can exist if you change from another
        mode to singlepress;
      - of type G_ACTION_KEYREL in a SINGLE_PRESS channel. It's up to geSampleAction to
        find the other piece (namely frame_b)
      - not of types G_ACTION_KEYPRESS | G_ACTION_KEYREL | G_ACTION_KILL */

      if ((action->chan != ae->chan->index)                          ||
          (recorder::frames.at(i) > clock::getFramesInLoop())             ||
          (action->type == G_ACTION_KILL && ch->mode == ChannelMode::SINGLE_PRESS)     ||
          (action->type == G_ACTION_KEYREL && ch->mode == ChannelMode::SINGLE_PRESS)   ||
          (action->type & ~(G_ACTION_KEYPRESS | G_ACTION_KEYREL | G_ACTION_KILL))
      )
        continue;

			int ax = x + (action->frame / ae->zoom);
			geSampleAction *a = new geSampleAction(
					ax,               // x
					y + 4,            // y
					h() - 8,          // h
					action->frame,	  // frame_a
					i,                // n. of recordings
					ch,               // pointer to SampleChannel
					false,            // record = false: don't record it, we are just displaying it!
					action->type);    // type of action
			add(a);
		}
	}
	end(); // mandatory when you add widgets to a fl_group, otherwise mega malfunctions
#endif
}


/* -------------------------------------------------------------------------- */


geSampleAction *geSampleActionEditor::getSelectedAction()
{
	for (int i=0; i<children(); i++) {
		int action_x  = ((geSampleAction*)child(i))->x();
		int action_w  = ((geSampleAction*)child(i))->w();
		if (Fl::event_x() >= action_x && Fl::event_x() <= action_x + action_w)
			return (geSampleAction*)child(i);
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
printf("Action [%d, %d)\n", comp.a1.frame, comp.a2.frame);
		Pixel px = ae->frameToPixel(comp.a1.frame);
		Pixel py = y() + 4;
		Pixel pw = 0;
		Pixel ph = h() - 8;
		if (comp.a2.frame != -1)
				pw = ae->frameToPixel(comp.a2.frame - comp.a1.frame);
		add(new geSampleAction(px, py, pw, ph, ch));
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


int geSampleActionEditor::handle(int e)
{
	gdBaseActionEditor* ae = static_cast<gdBaseActionEditor*>(window());

	switch (e) {
		case FL_PUSH: {
			if (Fl::event_button1()) {  // Left
				Frame f = ae->pixelToFrame(Fl::event_x() - x());
				c::recorder::recordSampleAction(ch, ae->getActionType(), f);
				rebuild();
			}
			else
			if (Fl::event_button3()) {  // Right
			}
			return 1;
		}

		default:
			return Fl_Widget::handle(e);
	}


#if 0
	gdActionEditor* ae = static_cast<gdActionEditor*>(window());

	int ret = Fl_Group::handle(e);

	/* do nothing if the widget is deactivated. It could happen for loopmode
	 * channels */

	if (!active())
		return 1;

	switch (e) {

		case FL_DRAG: {

      if (selected == nullptr) {  // if you drag an empty area
        ret = 1;
        break;
      }

			/* if onLeftEdge o onRightEdge are true it means that you're resizing
			 * an action. Otherwise move the widget. */

			if (selected->onLeftEdge || selected->onRightEdge) {

				/* some checks: a) cannot resize an action < N pixels, b) no beyond zero,
				 * c) no beyond bar maxwidth. Checks for overlap are done in FL_RELEASE */

				if (selected->onRightEdge) {

					int aw = Fl::event_x()-selected->x();
					int ah = selected->h();

					if (Fl::event_x() < selected->x()+geSampleAction::MIN_WIDTH)
						aw = geSampleAction::MIN_WIDTH;
					else
					if (Fl::event_x() > ae->coverX)
						aw = ae->coverX-selected->x();

					selected->size(aw, ah);
				}
				else {

					int ax = Fl::event_x();
					int ay = selected->y();
					int aw = selected->x()-Fl::event_x()+selected->w();
					int ah = selected->h();

					if (Fl::event_x() < x()) {
						ax = x();
						aw = selected->w()+selected->x()-x();
					}
					else
					if (Fl::event_x() > selected->x()+selected->w()-geSampleAction::MIN_WIDTH) {
						ax = selected->x()+selected->w()-geSampleAction::MIN_WIDTH;
						aw = geSampleAction::MIN_WIDTH;
					}
					selected->resize(ax, ay, aw, ah);
				}
			}

      /* move the widget around */

			else {
				int real_x = Fl::event_x() - actionPickPoint;
				if (real_x < x())                                  // don't go beyond the left border
					selected->position(x(), selected->y());
				else
				if (real_x+selected->w() > ae->coverX+x())         // don't go beyond the right border
					selected->position(ae->coverX+x()-selected->w(), selected->y());
				else {
					if (ae->gridTool->isOn()) {
						int snpx = ae->gridTool->getSnapPoint(real_x-x()) + x() -1;
						selected->position(snpx, selected->y());
					}
					else
						selected->position(real_x, selected->y());
				}
			}
			redraw();
			ret = 1;
			break;
		}

		case FL_PUSH:	{

			if (Fl::event_button1()) {

				/* avoid at all costs two overlapping actions. We use 'selected' because
				 * the selected action can be reused in FL_DRAG, in case you want to move
				 * it. */

				selected = getSelectedAction();

				if (selected == nullptr) {

					/* avoid click on grey area */

					if (Fl::event_x() >= ae->coverX) {
						ret = 1;
						break;
					}

					/* snap function, if enabled */

					int ax = Fl::event_x();
					int fx = (ax - x()) * ae->zoom;
					if (ae->gridTool->isOn()) {
						ax = ae->gridTool->getSnapPoint(ax-x()) + x() -1;
						fx = ae->gridTool->getSnapFrame(ax-x());

						/* with snap=on an action can fall onto another */

						if (actionCollides(fx)) {
							ret = 1;
							break;
						}
					}

					geSampleAction *a = new geSampleAction(
							ax,                                   // x
							y()+4,                                // y
							h()-8,                                // h
							fx,																		// frame_a
							recorder::frames.size()-1,            // n. of actions recorded
							ch,                                   // pointer to SampleChannel
							true,                                 // record = true: record it!
							ae->getActionType());            // type of action
					add(a);
					G_MainWin->keyboard->setChannelWithActions((geSampleChannel*)ch->guiChannel); // mainWindow update
					redraw();
					ret = 1;
				}
				else {
					actionOriginalX = selected->x();
					actionOriginalW = selected->w();
					actionPickPoint = Fl::event_x() - selected->x();
					ret = 1;   // for dragging
				}
			}
			else
			if (Fl::event_button3()) {
				geSampleAction *a = getSelectedAction();
				if (a != nullptr) {
					a->delAction();
					remove(a);
					delete a;
					G_MainWin->keyboard->setChannelWithActions((geSampleChannel*)ae->chan->guiChannel);
					redraw();
					ret = 1;
				}
			}
			break;
		}
		case FL_RELEASE: {

			if (selected == nullptr) {
				ret = 1;
				break;
			}

			/* noChanges = true when you click on an action without doing anything
			 * (dragging or moving it). */

			bool noChanges = false;
			if (actionOriginalX == selected->x())
				noChanges = true;
			if (ch->mode == ChannelMode::SINGLE_PRESS &&
					actionOriginalX+actionOriginalW != selected->x()+selected->w())
				noChanges = false;

			if (noChanges) {
				ret = 1;
				selected = nullptr;
				break;
			}

			/* step 1: check if the action doesn't overlap with another one.
			 * In case of overlap the moved action returns to the original X
			 * value ("actionOriginalX"). */

			bool overlap = false;
			for (int i=0; i<children() && !overlap; i++) {

				/* never check against itself. */

				if ((geSampleAction*)child(i) == selected)
					continue;

				int action_x  = ((geSampleAction*)child(i))->x();
				int action_w  = ((geSampleAction*)child(i))->w();
				if (ch->mode == ChannelMode::SINGLE_PRESS) {

					/* when 2 segments overlap?
					 * start = the highest value between the two starting points
					 * end   = the lowest value between the two ending points
					 * if start < end then there's an overlap of end-start pixels. */

					 int start = action_x >= selected->x() ? action_x : selected->x();
					 int end   = action_x+action_w < selected->x()+selected->w() ? action_x+action_w : selected->x()+selected->w();
					 if (start < end) {
						selected->resize(actionOriginalX, selected->y(), actionOriginalW, selected->h());
						redraw();
						overlap = true;   // one overlap: stop checking
					}
				}
				else {
					if (selected->x() == action_x) {
						selected->position(actionOriginalX, selected->y());
						redraw();
						overlap = true;   // one overlap: stop checking
					}
				}
			}

			/* step 2: no overlap? then update the coordinates of the action, ie
			 * delete the previous rec and create a new one.
			 * Anyway the selected action becomes nullptr, because when you release
			 * the mouse button the dragging process ends. */

			if (!overlap) {
				if (ae->gridTool->isOn()) {
					int f = ae->gridTool->getSnapFrame(selected->absx());
					selected->moveAction(f);
				}
				else
					selected->moveAction();
			}
			selected = nullptr;
			ret = 1;
			break;
		}
	}
	return ret;
#endif

}


/* -------------------------------------------------------------------------- */


bool geSampleActionEditor::actionCollides(int frame)
{
	/* if SINGLE_PRESS we check that the tail (frame_b) of the action doesn't
	 * overlap the head (frame) of the new one. First the general case, yet. */

	bool collision = false;

	for (int i=0; i<children() && !collision; i++)
		if (((geSampleAction*) child(i))->frame_a == frame)
			collision = true;

	if (ch->mode == ChannelMode::SINGLE_PRESS) {
		for (int i=0; i<children() && !collision; i++) {
			geSampleAction *c = ((geSampleAction*) child(i));
			if (frame <= c->frame_b && frame >= c->frame_a)
				collision = true;
		}
	}

	return collision;
}
