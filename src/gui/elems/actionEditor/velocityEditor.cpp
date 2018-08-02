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
#include "../../../utils/math.h"
#include "../../../core/const.h"
#include "../../../core/clock.h"
#include "../../../core/midiChannel.h"
#include "../../../glue/recorder.h"
#include "envelopePoint.h"
#include "velocityEditor.h"


using std::vector;
using namespace giada;


geVelocityEditor::geVelocityEditor(int x, int y, MidiChannel* ch)
:	geBaseActionEditor(x, y, 200, 40, ch)
{
	rebuild();
}


/* -------------------------------------------------------------------------- */


void geVelocityEditor::draw() 
{
	baseDraw();

	/* Print label. */

	fl_color(G_COLOR_GREY_4);
	fl_font(FL_HELVETICA, G_GUI_FONT_SIZE_BASE);
	fl_draw("Velocity", x()+4, y(), w(), h(), (Fl_Align) (FL_ALIGN_LEFT));

	if (children() == 0)
		return;

	Pixel side = geEnvelopePoint::SIDE / 2;

	for (int i=0; i<children(); i++) {
		geEnvelopePoint* p = static_cast<geEnvelopePoint*>(child(i));
		if (m_action == nullptr)
			p->position(p->x(), calcPointY(m::MidiEvent(p->a1.iValue).getVelocity()));
		Pixel x1 = p->x() + side;
		Pixel y1 = p->y();
		Pixel y2 = y() + h();
		fl_line(x1, y1, x1, y2);
	}

	draw_children();
}


/* -------------------------------------------------------------------------- */


int geVelocityEditor::calcPointY(int value) const
{
	float v = u::math::map<int, float>(value, 0, G_MAX_VELOCITY, 0.0, 1.0);
	return y() + m_base->valueToPixel(v, h()) - (geEnvelopePoint::SIDE / 2);
}


/* -------------------------------------------------------------------------- */


void geVelocityEditor::rebuild()
{
	namespace mr = m::recorder;
	namespace cr = c::recorder;

	/* Remove all existing actions and set a new width, according to the current
	zoom level. */

	clear();
	size(m_base->fullWidth, h());

	int limit = m::clock::getFramesInLoop();

	vector<mr::Composite> actions = cr::getMidiActions(m_ch->index, limit); 
	for (mr::Composite comp : actions)
	{
		gu_log("[geVelocityEditor::rebuild] f=%d\n", comp.a1.frame);

		Pixel px = x() + m_base->frameToPixel(comp.a1.frame);
		Pixel py = y() + calcPointY(m::MidiEvent(comp.a1.iValue).getVelocity());

		add(new geEnvelopePoint(px, py, comp.a1));
	}

	resizable(nullptr);

	redraw();
}


/* -------------------------------------------------------------------------- */


void geVelocityEditor::onMoveAction()    
{
	Pixel side = geEnvelopePoint::SIDE / 2;
	Pixel ey   = Fl::event_y() - side;

	Pixel y1 = y() - side;
	Pixel y2 = y() + h() - side;

	if (ey < y1) ey = y1; else if (ey > y2) ey = y2;

	m_action->position(m_action->x(), ey);
	redraw();
}


/* -------------------------------------------------------------------------- */


void geVelocityEditor::onRefreshAction() 
{

	float fv = m_base->pixelToValue((m_action->y() - y()) + geEnvelopePoint::SIDE / 2, h());
	int   iv = u::math::map<float, int>(fv, 0.0, 1.0, 0, G_MAX_VELOCITY);
	
	printf("%d\n", iv);
#if 0
	Frame f = m_base->pixelToFrame((m_action->x() - x()) + geEnvelopePoint::SIDE / 2);
	float v = m_base->pixelToValue((m_action->y() - y()) + geEnvelopePoint::SIDE / 2, h());
	c::recorder::deleteEnvelopeAction(m_ch, m_action->a1, /*moved=*/true);
	c::recorder::recordEnvelopeAction(m_ch, m_actionType, f, v);
#endif
}