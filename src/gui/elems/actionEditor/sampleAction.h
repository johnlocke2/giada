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


#ifndef GE_ACTION_H
#define GE_ACTION_H


#include <FL/Fl_Box.H>
#include "../../../core/recorder.h"


class gdActionEditor;
class SampleChannel;


class geSampleAction : public Fl_Box
{
private:

	const SampleChannel* ch;

public:

	static const int MIN_WIDTH    = 12;
	static const int HANDLE_WIDTH = 6;

	geSampleAction(int x, int y, int w, int h, const SampleChannel* ch,
		const giada::m::recorder::action a1, const giada::m::recorder::action a2);

	void draw() override;
	int  handle(int e) override;

	bool isOnEdges() const;

	bool  onRightEdge;
	bool  onLeftEdge;
	bool  hovered;
	int   pick;

	const giada::m::recorder::action a1;
	const giada::m::recorder::action a2;
};


#endif