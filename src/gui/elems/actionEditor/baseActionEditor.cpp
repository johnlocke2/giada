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
#include "../../dialogs/actionEditor/baseActionEditor.h"
#include "gridTool.h"
#include "baseActionEditor.h"


using namespace giada;


geBaseActionEditor::geBaseActionEditor(int x, int y, int w, int h)
	:	Fl_Group(x, y, w, h) 
{
}


/* -------------------------------------------------------------------------- */


void geBaseActionEditor::baseDraw(bool clear)
{
	gdBaseActionEditor* ae = static_cast<gdBaseActionEditor*>(window());

	/* Clear the screen. */

	if (clear)
		fl_rectf(x(), y(), w(), h(), G_COLOR_GREY_1);

	/* Draw the outer container. */

	fl_color(G_COLOR_GREY_4);
	fl_rect(x(), y(), w(), h());

	/* Grid drawing, if > 1. */

	if (ae->gridTool->getValue() > 1) {

		fl_color(fl_rgb_color(54, 54, 54));
		fl_line_style(FL_DASH, 0, nullptr);

		for (int i=0; i<(int) ae->gridTool->points.size(); i++) {
			int px = ae->gridTool->points.at(i)+x()-1;
			fl_line(px, y()+1, px, y()+h()-2);
		}
		fl_line_style(0);
	}

	/* Bars and beats drawing. */

	fl_color(G_COLOR_GREY_4);
	for (int i=0; i<(int) ae->gridTool->beats.size(); i++) {
		int px = ae->gridTool->beats.at(i)+x()-1;
		fl_line(px, y()+1, px, y()+h()-2);
	}

	fl_color(G_COLOR_LIGHT_1);
	for (int i=0; i<(int) ae->gridTool->bars.size(); i++) {
		int px = ae->gridTool->bars.at(i)+x()-1;
		fl_line(px, y()+1, px, y()+h()-2);
	}

	/* Cover unused area. Avoid drawing cover if width == 0 (i.e. beats are 32). */

	int coverWidth = ae->fullWidth - ae->loopWidth;
	if (coverWidth != 0)
		fl_rectf(ae->loopWidth+x(), y()+1, coverWidth, h()-2, G_COLOR_GREY_4);
}
