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
#include "../../../core/const.h"
#include "../../../core/sampleChannel.h"
#include "../../../glue/recorder.h"
#include "envelopePoint.h"
#include "envelopeEditor.h"


using std::vector;
using namespace giada;


geEnvelopeEditor::geEnvelopeEditor(int x, int y, int actionType, const char* l, 
	SampleChannel* ch)
:	geBaseActionEditor(x, y, 200, 40, ch),	
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
		if (m_action == nullptr)
			p->position(p->x(), calcPointY(p->a1.fValue));
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


void geEnvelopeEditor::rebuild()
{
	namespace mr = m::recorder;
	namespace cr = c::recorder;

	/* Remove all existing actions and set a new width, according to the current
	zoom level. */

	clear();
	size(m_base->fullWidth, h());

	vector<mr::action> actions = cr::getEnvelopeActions(m_ch, m_actionType);

	for (mr::action a : actions) {
		gu_log("[geEnvelopeEditor::rebuild] Action %d\n", a.frame);
		add(new geEnvelopePoint(calcPointX(a.frame), calcPointY(a.fValue), a)); 		
	}

	resizable(nullptr);

	redraw();
}


/* -------------------------------------------------------------------------- */


bool geEnvelopeEditor::isFirstPoint() const
{
	return find(m_action) == 0;
}


bool geEnvelopeEditor::isLastPoint() const
{
	return find(m_action) == children() - 1;
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


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::onAddAction()     
{
	Frame f = m_base->pixelToFrame(Fl::event_x() - x());
	float v = m_base->pixelToValue(Fl::event_y() - y(), h());
	c::recorder::recordEnvelopeAction(m_ch, m_actionType, f, v);
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::onDeleteAction()  
{
	c::recorder::deleteEnvelopeAction(m_ch, m_action->a1, /*moved=*/false);
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::onMoveAction()    
{
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

	m_action->position(ex, ey);
	redraw();
}


/* -------------------------------------------------------------------------- */


void geEnvelopeEditor::onRefreshAction() 
{
	Frame f = m_base->pixelToFrame((m_action->x() - x()) + geEnvelopePoint::SIDE / 2);
	float v = m_base->pixelToValue((m_action->y() - y()) + geEnvelopePoint::SIDE / 2, h());
	c::recorder::deleteEnvelopeAction(m_ch, m_action->a1, /*moved=*/true);
	c::recorder::recordEnvelopeAction(m_ch, m_actionType, f, v);
}