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

#if 0
#ifndef GE_MUTE_TOOL_H
#define GE_MUTE_TOOL_H


#include <vector>
#include "baseActionEditor.h"


class gdActionEditor;


class geMuteEditor : public geBaseActionEditor
{
private:

	/* point
	 * a single dot in the graph. */

	struct point
	{
		int  frame;
		char type;
		int  x;
	};

	/* points
	 * array of on/off points, in frames */

	std::vector<point> points;

	/* draggedPoint
	 * which point we are dragging? */

	int draggedPoint;

	/* selectedPoint
	 * which point we are selecting? */

	int selectedPoint;

	/* previousXPoint
	 * x coordinate of point at time t-1. Used to check effective shifts */

	int previousXPoint;

	/* extractPoints
	 * va a leggere l'array di azioni di Recorder ed estrae tutti i punti
	 * interessanti mute_on o mute_off. Li mette poi nel vector points. */
	void extractPoints();

	/* getSelectedPoint
	 * ritorna l'indice di points[] in base al punto selezionato (quello
	 * con il mouse hover). Ritorna -1 se non trova niente. */
	int getSelectedPoint();

	/* pointCollides
	 * true if a point collides with another. Used while adding new points
	 * with snap active.*/

	bool pointCollides(int frame);

public:

	geMuteEditor(int x, int y);
	void draw();
	int  handle(int e);

	/* updateActions
	 * calculates new points affected by the zoom. Call this one after
	 * each zoom update. */

	void rebuild();
};

#endif
#endif
