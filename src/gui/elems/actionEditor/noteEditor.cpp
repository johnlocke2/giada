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
#include "../../../core/conf.h"
#include "../../../core/midiChannel.h"
#include "../../../utils/log.h"
#include "pianoItem.h"
#include "pianoRoll.h"
#include "noteEditor.h"


using namespace giada;


geNoteEditor::geNoteEditor(int x, int y, gdMidiActionEditor* base)
: Fl_Scroll(x, y, 200, 422),
	m_base   (base)
{
	m_pianoRoll = new gePianoRoll(x, y, m_base->fullWidth, static_cast<MidiChannel*>(m_base->ch));

	rebuild();
	
	size(m_base->fullWidth, m::conf::pianoRollH);
	
	//type(0); // Don't show scrollbars
}


/* -------------------------------------------------------------------------- */


geNoteEditor::~geNoteEditor()
{
	m::conf::pianoRollH = h();
	m::conf::pianoRollY = m_pianoRoll->y();
}


/* -------------------------------------------------------------------------- */


void geNoteEditor::rebuild()
{
	size(m_base->fullWidth, h());
	m_pianoRoll->rebuild();
}