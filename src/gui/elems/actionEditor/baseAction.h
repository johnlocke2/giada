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


#ifndef GE_BASE_ACTION_H
#define GE_BASE_ACTION_H


#include <FL/Fl_Box.H>
#include "../../../core/recorder.h"


class geBaseAction : public Fl_Box
{
private:
	
	bool m_resizable;

public:

	static const int MIN_WIDTH    = 12;
	static const int HANDLE_WIDTH = 6;

	geBaseAction(int x, int y, int w, int h, bool resizable,
		giada::m::recorder::action a1, giada::m::recorder::action a2);

	int  handle(int e) override;

	bool isOnEdges() const;

	/* setLeftEdge/setRightEdge
	Set new left/right edges position, relative range. */

	void setLeftEdge(int p);
	void setRightEdge(int p);

	void setPosition(int p);

	bool onRightEdge;
	bool onLeftEdge;
	bool hovered;
	bool altered;
	int  pick;

	giada::m::recorder::action a1;
	giada::m::recorder::action a2;
};


#endif
