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


#include <cmath>
#include "../../../utils/gui.h"
#include "../../../utils/string.h"
#include "../../../core/graphics.h"
#include "../../../core/conf.h"
#include "../../../core/const.h"
#include "../../../core/clock.h"
#include "../../../core/sampleChannel.h"
#include "../../elems/basics/scroll.h"
#include "../../elems/basics/button.h"
#include "../../elems/basics/resizerBar.h"
#include "../../elems/basics/choice.h"
#include "../../elems/basics/box.h"
#include "../../elems/actionEditor/actionEditor.h"
#include "../../elems/actionEditor/envelopeEditor.h"
#include "../../elems/actionEditor/muteEditor.h"
#include "../../elems/actionEditor/noteEditor.h"
#include "../../elems/actionEditor/gridTool.h"
#include "baseActionEditor.h"


using std::string;
using namespace giada::m;


namespace giada
{
gdBaseActionEditor::gdBaseActionEditor(Channel* ch)
	:	gdWindow (640, 284),
		ch       (ch),
		zoom     (100),
		usedWidth(0)
{
	if (conf::actionEditorW) {
		resize(conf::actionEditorX, conf::actionEditorY, conf::actionEditorW, conf::actionEditorH);
		zoom = conf::actionEditorZoom;
	}

	totalWidth = std::ceil(clock::getFramesInSeq() / (float) zoom);
}


/* -------------------------------------------------------------------------- */


gdBaseActionEditor::~gdBaseActionEditor()
{
	conf::actionEditorX = x();
	conf::actionEditorY = y();
	conf::actionEditorW = w();
	conf::actionEditorH = h();
	conf::actionEditorZoom = zoom;

	/** CHECKME - missing clear() ? */
}


/* -------------------------------------------------------------------------- */


void gdBaseActionEditor::cb_zoomIn(Fl_Widget *w, void *p)  { ((gdBaseActionEditor*)p)->cb_zoomIn(); }
void gdBaseActionEditor::cb_zoomOut(Fl_Widget *w, void *p) { ((gdBaseActionEditor*)p)->cb_zoomOut(); }


/* -------------------------------------------------------------------------- */


void gdBaseActionEditor::update()
{
	totalWidth = (int) ceilf(clock::getFramesInSeq() / (float) zoom);
	if (totalWidth < viewport->w()) {
		totalWidth = viewport->w();
		zoom = (int) ceilf(clock::getFramesInSeq() / (float) totalWidth);
		viewport->scroll_to(0, viewport->yposition());
	}
}


/* -------------------------------------------------------------------------- */


int gdBaseActionEditor::handle(int e)
{
	int ret = Fl_Group::handle(e);
	switch (e) {
		case FL_MOUSEWHEEL: {
			Fl::event_dy() == -1 ? cb_zoomIn() : cb_zoomOut();
			ret = 1;
			break;
		}
	}
	return ret;
}
} // giada::