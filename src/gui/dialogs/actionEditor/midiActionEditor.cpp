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


#include "../../../core/const.h"
#include "../../../core/graphics.h"
#include "../../../core/midiChannel.h"
#include "../../../glue/recorder.h"
#include "../../../utils/gui.h"
#include "../../../utils/string.h"
#include "../../elems/basics/scroll.h"
#include "../../elems/basics/button.h"
#include "../../elems/basics/resizerBar.h"
#include "../../elems/basics/choice.h"
#include "../../elems/basics/box.h"
#include "../../elems/actionEditor/sampleActionEditor.h"
#include "../../elems/actionEditor/envelopeEditor.h"
#include "../../elems/actionEditor/muteEditor.h"
#include "../../elems/actionEditor/noteEditor.h"
#include "../../elems/actionEditor/gridTool.h"
#include "midiActionEditor.h"


using std::string;


namespace giada
{
gdMidiActionEditor::gdMidiActionEditor(MidiChannel* ch)
	: gdBaseActionEditor(ch)
{
	computeWidth();

	Fl_Group* upperArea = new Fl_Group(8, 8, w()-16, 20);

	upperArea->begin();

		gridTool = new geGridTool(8, 8);

		geBox *b1  = new geBox(gridTool->x()+gridTool->w()+4, 8, 300, 20);    // padding actionType - zoomButtons
		zoomInBtn  = new geButton(w()-8-40-4, 8, 20, 20, "", zoomInOff_xpm, zoomInOn_xpm);
		zoomOutBtn = new geButton(w()-8-20,   8, 20, 20, "", zoomOutOff_xpm, zoomOutOn_xpm);
	
	upperArea->end();
	upperArea->resizable(b1);

	zoomInBtn->callback(cb_zoomIn, (void*)this);
	zoomOutBtn->callback(cb_zoomOut, (void*)this);

	/* Main viewport: contains all widgets. */

	viewport = new geScroll(8, 36, w()-16, h()-44);

	pr = new geNoteEditor(viewport->x(), upperArea->y()+upperArea->h()+8, this);
	viewport->add(pr);
	viewport->add(new geResizerBar(pr->x(), pr->y()+pr->h(), viewport->w(), 30, 8));

	end();
	prepareWindow();
}


/* -------------------------------------------------------------------------- */


void gdMidiActionEditor::rebuild()
{
	pr->rebuild();
}
} // giada::
