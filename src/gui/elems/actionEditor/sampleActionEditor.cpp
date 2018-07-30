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
: geBaseActionEditor(x, y, 200, 40, ch)
{
	rebuild();
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::rebuild()
{
	namespace mr = m::recorder;
	namespace cr = c::recorder;

	SampleChannel* ch = static_cast<SampleChannel*>(m_ch);

	/* Remove all existing actions and set a new width, according to the current
	zoom level. */

	clear();
	size(m_base->fullWidth, h());

	vector<mr::Composite> comps = cr::getSampleActions(ch);

	for (mr::Composite comp : comps) {
		gu_log("[geSampleActionEditor::rebuild] Action [%d, %d)\n", 
			comp.a1.frame, comp.a2.frame);
		Pixel px = x() + m_base->frameToPixel(comp.a1.frame);
		Pixel py = y() + 4;
		Pixel pw = 0;
		Pixel ph = h() - 8;
		if (comp.a2.frame != -1)
				pw = m_base->frameToPixel(comp.a2.frame - comp.a1.frame);

		geSampleAction* a = new geSampleAction(px, py, pw, ph, ch, comp.a1, comp.a2);
		add(a);
		resizable(a);
	}

	/* If channel is LOOP_ANY, deactivate it: a loop mode channel cannot hold 
	keypress/keyrelease actions. */
	
	ch->isAnyLoopMode() ? deactivate() : activate();

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
	fl_font(FL_HELVETICA, G_GUI_FONT_SIZE_BASE);
	if (active())
		fl_draw("start/stop", x()+4, y(), w(), h(), (Fl_Align) (FL_ALIGN_LEFT | FL_ALIGN_CENTER));
	else
		fl_draw("start/stop (disabled)", x()+4, y(), w(), h(), (Fl_Align) (FL_ALIGN_LEFT | FL_ALIGN_CENTER));

	draw_children();
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::onAddAction()     
{
	Frame f = m_base->pixelToFrame(Fl::event_x() - x());
	c::recorder::recordSampleAction(static_cast<SampleChannel*>(m_ch), 
		m_base->getActionType(), f);
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::onDeleteAction()  
{
	c::recorder::deleteSampleAction(static_cast<SampleChannel*>(m_ch), m_action->a1, m_action->a2);
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::onMoveAction()    
{
	Pixel ex = Fl::event_x() - m_action->pick;
	if      (ex < x()) ex = x();
	else if (ex + m_action->w() > m_base->loopWidth + x()) ex = m_base->loopWidth + x() - m_action->w();

	m_action->setPosition(ex);
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::onResizeAction()  
{
	Pixel ex = Fl::event_x();
	if      (ex < x()) ex = x();
	else if (ex > m_base->loopWidth + x()) ex = m_base->loopWidth + x();

	if (m_action->onRightEdge) 
		m_action->setRightEdge(ex - m_action->x());
	else
		m_action->setLeftEdge(ex);
}


/* -------------------------------------------------------------------------- */


void geSampleActionEditor::onRefreshAction() 
{
	SampleChannel* ch = static_cast<SampleChannel*>(m_ch);

	Frame f1 = m_base->pixelToFrame(m_action->x() - x());
	Frame f2 = m_base->pixelToFrame(m_action->x() + m_action->w() - x());
	c::recorder::deleteSampleAction(ch, m_action->a1, m_action->a2);
	c::recorder::recordSampleAction(ch, m_base->getActionType(), f1, f2);
}
