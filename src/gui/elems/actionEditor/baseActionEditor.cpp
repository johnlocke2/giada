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
#include "../../../core/clock.h"
#include "gridTool.h"
#include "baseActionEditor.h"


using namespace giada;


geBaseActionEditor::geBaseActionEditor(int x, int y, int w, int h, Channel* ch)
	:	Fl_Group(x, y, w, h),
	  m_ch    (ch),
	  m_base  (static_cast<gdBaseActionEditor*>(window()))

{
}


/* -------------------------------------------------------------------------- */


void geBaseActionEditor::baseDraw(bool clear) const
{
	/* Clear the screen. */

	if (clear)
		fl_rectf(x(), y(), w(), h(), G_COLOR_GREY_1);

	/* Draw the outer container. */

	fl_color(G_COLOR_GREY_4);
	fl_rect(x(), y(), w(), h());

	/* Grid drawing. */

	fl_color(fl_rgb_color(54, 54, 54));
	fl_line_style(FL_DASH, 0, nullptr);

	for (Frame i=0; i<m::clock::getFramesInLoop(); i+=m_base->gridTool->getCellSize()) {
		Pixel p = m_base->frameToPixel(i) + x();
		fl_line(p, y()+1, p, y()+h()-2);
	}
	fl_line_style(0);

	/* Bars and beats drawing. */

	fl_color(G_COLOR_GREY_4);
/*	for (int i=0; i<(int) m_base->gridTool->beats.size(); i++) {
		int px = m_base->gridTool->beats.at(i)+x()-1;
		fl_line(px, y()+1, px, y()+h()-2);
	}

	fl_color(G_COLOR_LIGHT_1);
	for (int i=0; i<(int) m_base->gridTool->bars.size(); i++) {
		int px = m_base->gridTool->bars.at(i)+x()-1;
		fl_line(px, y()+1, px, y()+h()-2);
	}
*/
	/* Cover unused area. Avoid drawing cover if width == 0 (i.e. beats are 32). */

	int coverWidth = m_base->fullWidth - m_base->loopWidth;
	if (coverWidth != 0)
		fl_rectf(m_base->loopWidth+x(), y()+1, coverWidth, h()-2, G_COLOR_GREY_4);
}
