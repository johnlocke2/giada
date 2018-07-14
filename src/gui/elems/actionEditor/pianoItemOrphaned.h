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


#ifndef GE_PIANO_ITEM_ORPHANED_H
#define GE_PIANO_ITEM_ORPHANED_H


#include "../../../core/recorder.h"
#include "basePianoItem.h"


class gdActionEditor;


class gePianoItemOrphaned : public geBasePianoItem
{
private:

	int note;
	int frame;
	int event;

public:

  static const int WIDTH = 12;

  gePianoItemOrphaned(int x, int y, int xRel, int yRel,
    struct giada::m::recorder::action action);

  void draw() override;
  int handle(int e) override;

  void reposition(int pianoRollX) override;

  void remove();
};


#endif
