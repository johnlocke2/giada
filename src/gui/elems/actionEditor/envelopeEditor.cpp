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
#include "../../../utils/log.h"
#include "../../../core/recorder.h"
#include "../../../core/const.h"
#include "../../../glue/recorder.h"
#include "../../dialogs/actionEditor/baseActionEditor.h"
#include "envelopePoint.h"
#include "envelopeEditor.h"


using std::vector;
using namespace giada;


geEnvelopeEditor::geEnvelopeEditor(int x, int y, int actionType, const char* l, 
	SampleChannel* ch)
:	geBaseActionEditor(x, y, 200, 40, ch),	
  m_point           (nullptr),
  m_actionType      (actionType)
{
	copy_label(l);
	rebuild();
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::draw() 
{
	baseDraw();

	/* Print label. */

	fl_color(G_COLOR_GREY_4);
	fl_font(FL_HELVETICA, G_GUI_FONT_SIZE_BASE);
	fl_draw(label(), x()+4, y(), w(), h(), (Fl_Align) (FL_ALIGN_LEFT));

	if (children() == 0)
		return;

	Pixel side = geEnvelopePoint::SIDE / 2;

	int x1 = child(0)->x() + side;
	int y1 = child(0)->y() + side;
	int x2 = 0;
	int y2 = 0;

	/* For each point: 
		- paint the connecting line with the next one;
		- reposition it on the y axis, only if there's no point selected (dragged
	    around). */

	for (int i=0; i<children(); i++) {
		geEnvelopePoint* p = static_cast<geEnvelopePoint*>(child(i));
		if (m_point == nullptr)
			p->position(p->x(), calcPointY(p->action->fValue));
		if (i > 0) {
			x2 = p->x() + side;
			y2 = p->y() + side;
			fl_line(x1, y1, x2, y2);
			x1 = x2;
			y1 = y2;
		}
	}

	draw_children();
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::onPush()
{
	m_point = getPointAtCursor();

	if (Fl::event_button1()) {    // Left button
		if (m_point == nullptr) {   // No action under cursor: add a new one
			if (Fl::event_x() >= m_base->loopWidth) // Avoid click on grey area
				return 0;
			Frame f = m_base->pixelToFrame(Fl::event_x() - x());
			float v = m_base->pixelToValue(Fl::event_y() - y(), h());
			c::recorder::recordEnvelopeAction(m_ch, m_actionType, f, v);
			rebuild();
		}
	}
	else
	if (Fl::event_button3()) {    // Right button
		if (m_point != nullptr) {
			c::recorder::deleteEnvelopeAction(m_ch, m_point->action, /*moved=*/false);
			m_point = nullptr;
			rebuild();	
		}
	}
	return 1;
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::onDrag()
{
	if (m_point == nullptr)
		return 0;

	Pixel side = geEnvelopePoint::SIDE / 2;
	Pixel ex   = Fl::event_x() - side;
	Pixel ey   = Fl::event_y() - side;

	Pixel x1 = x() - side;
	Pixel x2 = m_base->loopWidth + x() - side;
	Pixel y1 = y() - side;
	Pixel y2 = y() + h() - side;

	/* x-axis constraints. */
	if      (isFirstPoint() || ex < x1) ex = x1; 
	else if (isLastPoint()  || ex > x2) ex = x2;

	/* y-axis constraints. */
	if (ey < y1) ey = y1; else if (ey > y2) ey = y2;

	m_point->position(ex, ey);
	redraw();

	return 1;
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::onRelease()
{
	if (m_point == nullptr)
		return 0;

	/* TODO - do this only if the action has been really altered */

	Frame f = m_base->pixelToFrame((m_point->x() - x()) + geEnvelopePoint::SIDE / 2);
	float v = m_base->pixelToValue((m_point->y() - y()) + geEnvelopePoint::SIDE / 2, h());
	c::recorder::deleteEnvelopeAction(m_ch, m_point->action, /*moved=*/true);
	c::recorder::recordEnvelopeAction(m_ch, m_actionType, f, v);

	m_point = nullptr;

	rebuild();

	return 1;
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::handle(int e)
{
	switch (e) {
		case FL_PUSH:
			return onPush();
		case FL_DRAG:
			return onDrag();
		case FL_RELEASE:
			return onRelease();
		default:
			return Fl_Group::handle(e);
	}
}


/* -------------------------------------------------------------------------- */


geEnvelopePoint* geEnvelopeEditor::getPointAtCursor() const
{
	for (int i=0; i<children(); i++) {
		geEnvelopePoint* p = static_cast<geEnvelopePoint*>(child(i));
		if (p->hovered)
			return p;
	}
	return nullptr;
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::rebuild()
{
	namespace mr = m::recorder;
	namespace cr = c::recorder;

	/* Remove all existing actions and set a new width, according to the current
	zoom level. */

	clear();
	size(m_base->fullWidth, h());

	vector<const mr::action*> actions = cr::getEnvelopeActions(m_ch, m_actionType);

	for (const mr::action* a : actions) {
		gu_log("[geEnvelopeEditor::rebuild] Action %d\n", a->frame);
		add(new geEnvelopePoint(calcPointX(a->frame), calcPointY(a->fValue), a)); 		
	}

	resizable(nullptr);

	redraw();
}


/* -------------------------------------------------------------------------- */


bool geEnvelopeEditor::isFirstPoint() const
{
	return find(m_point) == 0;
}


bool geEnvelopeEditor::isLastPoint() const
{
	return find(m_point) == children() - 1;
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::calcPointX(int frame) const
{
	return x() + m_base->frameToPixel(frame) - (geEnvelopePoint::SIDE / 2);
}


int geEnvelopeEditor::calcPointY(float value) const
{
	return y() + m_base->valueToPixel(value, h()) - (geEnvelopePoint::SIDE / 2);
}


#if 0
extern gdMainWindow *G_MainWin;


using namespace giada::m;


geEnvelopeEditor::geEnvelopeEditor(int x, int y, int type, int range, const char* l)
	:	geBaseActionEditor(x, y, 200, 80),
		l                 (l),
		type              (type),
		range             (range),
		selectedPoint     (-1),
		draggedPoint      (-1)
{
	size(static_cast<gdActionEditor*>(window())->totalWidth, h());
}


/* -------------------------------------------------------------------------- */


geEnvelopeEditor::~geEnvelopeEditor() 
{
	clearPoints();
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::addPoint(int frame, int iValue, float fValue, int px, int py) 
{
	point p;
	p.frame  = frame;
	p.iValue = iValue;
	p.fValue = fValue;
	p.x = px;
	p.y = py;
	points.push_back(p);
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::rebuild() 
{
	for (unsigned i=0; i<points.size(); i++)
		points.at(i).x = points.at(i).frame / static_cast<gdActionEditor*>(window())->zoom;
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::draw() 
{
	baseDraw();

	/* print label */

	fl_color(G_COLOR_GREY_4);
	fl_font(FL_HELVETICA, 12);
	fl_draw(l, x()+4, y(), 80, h(), (Fl_Align) (FL_ALIGN_LEFT));

	int pxOld = x()-3;
	int pyOld = y()+1;
	int pxNew = 0;
	int pyNew = 0;

	fl_color(G_COLOR_LIGHT_1);

	for (unsigned i=0; i<points.size(); i++) {

		pxNew = points.at(i).x+x()-3;
		pyNew = points.at(i).y+y();

		if (selectedPoint == (int) i) {
			fl_color(G_COLOR_LIGHT_1);
			fl_rectf(pxNew, pyNew, 7, 7);
			fl_color(G_COLOR_LIGHT_1);
		}
		else
			fl_rectf(pxNew, pyNew, 7, 7);

		if (i > 0)
			fl_line(pxOld+3, pyOld+3, pxNew+3, pyNew+3);

		pxOld = pxNew;
		pyOld = pyNew;
	}
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::handle(int e) 
{
	gdActionEditor* ae = static_cast<gdActionEditor*>(window());

	/* Adding an action: no further checks required, just record it on frame
	 * mx*ae->zoom. Deleting action is trickier: find the active
	 * point and derive from it the corresponding frame. */

	int ret = 0;
	int mx  = Fl::event_x()-x();  // mouse x
	int my  = Fl::event_y()-y();  // mouse y

	switch (e) {

		case FL_ENTER: {
			ret = 1;
			break;
		}

		case FL_MOVE: {
			selectedPoint = getSelectedPoint();
			redraw();
			ret = 1;
			break;
		}

		case FL_LEAVE: {
			draggedPoint  = -1;
			selectedPoint = -1;
			redraw();
			ret = 1;
			break;
		}

		case FL_PUSH: {

			/* left click on point: drag
			 * right click on point: delete
			 * left click on void: add */

			if (Fl::event_button1()) {

				if (selectedPoint != -1) {
					draggedPoint = selectedPoint;
				}
				else {

					/* top & border fix */

					if (my > h()-8) my = h()-8;
					if (mx > ae->coverX-x()) mx = ae->coverX-x();

					if (range == G_RANGE_FLOAT) {

						/* if this is the first point ever, add other two points at the beginning
						 * and the end of the range */

						if (points.size() == 0) {
							addPoint(0, 0, 1.0f, 0, 1);
							recorder::rec(ae->chan->index, type, 0, 0, 1.0f);
							addPoint(clock::getFramesInLoop(), 0, 1.0f, ae->coverX, 1);
							recorder::rec(ae->chan->index, type, clock::getFramesInLoop(), 0, 1.0f);
              ae->chan->hasActions = true;
						}

						/* line between 2 points y = (x-a) / (b-a); a = h() - 8; b = 1 */

						int frame   = mx * ae->zoom;
						float value = (my - h() + 8) / (float) (1 - h() + 8);
						addPoint(frame, 0, value, mx, my);
						recorder::rec(ae->chan->index, type, frame, 0, value);
            ae->chan->hasActions = true;
						recorder::sortActions();
						sortPoints();
					}
					else {
						/// TODO
					}
					G_MainWin->keyboard->setChannelWithActions((geSampleChannel*)ae->chan->guiChannel); // update mainWindow
					redraw();
				}
			}
			else {

				/* right click on point 0 or point size-1 deletes the entire envelope */

				if (selectedPoint != -1) {
					if (selectedPoint == 0 || (unsigned) selectedPoint == points.size()-1) {
						recorder::clearAction(ae->chan->index, type);
            ae->chan->hasActions = recorder::hasActions(ae->chan->index);
						points.clear();
					}
					else {
						recorder::deleteAction(ae->chan->index,
              points.at(selectedPoint).frame, type, false, &mixer::mutex);
            ae->chan->hasActions = recorder::hasActions(ae->chan->index);
            recorder::sortActions();
						points.erase(points.begin() + selectedPoint);
					}
					G_MainWin->keyboard->setChannelWithActions((geSampleChannel*)ae->chan->guiChannel); // update mainWindow
					redraw();
				}
			}

			ret = 1;
			break;
		}

		case FL_RELEASE: {
			if (draggedPoint != -1) {

				if (points.at(draggedPoint).x == previousXPoint) {
					//gu_log("nothing to do\n");
				}
				else {
					int newFrame = points.at(draggedPoint).x * ae->zoom;

					/* x edge correction */

					if (newFrame < 0)
						newFrame = 0;
					else if (newFrame > clock::getFramesInLoop())
						newFrame = clock::getFramesInLoop();

					/* vertical line check */

					int vp = verticalPoint(points.at(draggedPoint));
					if (vp == 1) 			 newFrame -= 256;
					else if (vp == -1) newFrame += 256;

					/*  delete previous point and record a new one */

					recorder::deleteAction(ae->chan->index,
            points.at(draggedPoint).frame, type, false, &mixer::mutex);
          ae->chan->hasActions = recorder::hasActions(ae->chan->index);

					if (range == G_RANGE_FLOAT) {
						float value = (points.at(draggedPoint).y - h() + 8) / (float) (1 - h() + 8);
						recorder::rec(ae->chan->index, type, newFrame, 0, value);
            ae->chan->hasActions = true;
					}
					else {
						/// TODO
					}

					recorder::sortActions();
					points.at(draggedPoint).frame = newFrame;
					draggedPoint  = -1;
					selectedPoint = -1;
				}
			}
			ret = 1;
			break;
		}

		case FL_DRAG: {

			if (draggedPoint != -1) {

				/* y constraint */

				if (my > h()-8)
					points.at(draggedPoint).y = h()-8;
				else
				if (my < 1)
					points.at(draggedPoint).y = 1;
				else
					points.at(draggedPoint).y = my;

				/* x constraint
				 * constrain the point between two ends (leftBorder-point, point-point,
				 * point-rightBorder). First & last points cannot be shifted on x */

				if (draggedPoint == 0)
					points.at(draggedPoint).x = x()-8;
				else
				if ((unsigned) draggedPoint == points.size()-1)
					points.at(draggedPoint).x = ae->coverX;
				else {
					int prevPoint = points.at(draggedPoint-1).x;
					int nextPoint = points.at(draggedPoint+1).x;
					if (mx <= prevPoint)
						points.at(draggedPoint).x = prevPoint;
					else
					if (mx >= nextPoint)
						points.at(draggedPoint).x = nextPoint;
					//else
					//	points.at(draggedPoint).x = mx;
					else {
						if (ae->gridTool->isOn())
							points.at(draggedPoint).x = ae->gridTool->getSnapPoint(mx)-1;
						else
							points.at(draggedPoint).x = mx;
					}
				}
				redraw();
			}

			ret = 1;
			break;
		}
	}

	return ret;
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::verticalPoint(const point &p) 
{
	for (unsigned i=0; i<points.size(); i++) {
		if (&p == &points.at(i)) {
			if (i == 0 || i == points.size()-1)  // first or last point
				return 0;
			else {
				if (points.at(i-1).x == p.x)    // vertical with point[i-1]
					return -1;
				else
				if (points.at(i+1).x == p.x)    // vertical with point[i+1]
					return 1;
			}
			break;
		}
	}
	return 0;
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::sortPoints() 
{
	for (unsigned i=0; i<points.size(); i++)
		for (unsigned j=0; j<points.size(); j++)
			if (points.at(j).x > points.at(i).x)
				std::swap(points.at(j), points.at(i));
}


/* -------------------------------------------------------------------------- */


int geEnvelopeEditor::getSelectedPoint() 
{
	/* point is a 7x7 dot */

	for (unsigned i=0; i<points.size(); i++) {
		if (Fl::event_x() >= points.at(i).x+x()-4  &&
				Fl::event_x() <= points.at(i).x+x()+4  &&
				Fl::event_y() >= points.at(i).y+y()    &&
				Fl::event_y() <= points.at(i).y+y()+7)
		return i;
	}
	return -1;
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::fill() 
{
	gdActionEditor* ae = static_cast<gdActionEditor*>(window());

	points.clear();
	for (unsigned i=0; i<recorder::global.size(); i++)
		for (unsigned j=0; j<recorder::global.at(i).size(); j++) {
			recorder::action *a = recorder::global.at(i).at(j);
			if (a->type == type && a->chan == ae->chan->index) {
				if (range == G_RANGE_FLOAT)
					addPoint(
						a->frame,                      // frame
						0,                             // int value (unused)
						a->fValue,                     // float value
						a->frame / ae->zoom,       // x
						((1-h()+8)*a->fValue)+h()-8);  // y = (b-a)x + a (line between two points)
				// else: TODO
			}
		}

}

#endif