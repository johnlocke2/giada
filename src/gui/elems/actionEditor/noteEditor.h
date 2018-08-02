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


#ifndef GE_NOTE_EDITOR_H
#define GE_NOTE_EDITOR_H


#include "../../dialogs/actionEditor/midiActionEditor.h"
#include "../basics/scroll.h"


class gePianoRoll;


class geNoteEditor : public geScroll
{
private:

	giada::gdMidiActionEditor* m_base;

public:

	geNoteEditor(int x, int y, giada::gdMidiActionEditor* base);
	~geNoteEditor();

	void rebuild();
	void scroll();

	gePianoRoll* pianoRoll;
};


#endif
