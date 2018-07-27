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


#ifndef GE_ENVELOPE_EDITOR_H
#define GE_ENVELOPE_EDITOR_H


#include "baseActionEditor.h"


class SampleChannel;
class geEnvelopePoint;


class geEnvelopeEditor : public geBaseActionEditor
{
private:

	/* m_point
	Pointer to the selected point. Used when dragging things around. */

	geEnvelopePoint* m_point;

	/* m_actionType
	What type of action this envelope editor is dealing with. */
	
	int m_actionType;

	int calcPointX(int frame) const;
	int calcPointY(float value) const;

	geEnvelopePoint* getPointAtCursor() const;

	int onPush();
	int onDrag();
	int onRelease();

public:

	geEnvelopeEditor(int x, int y, int actionType, int range, const char* l, 
		SampleChannel* ch);

	void draw() override;
	int handle(int e) override;

	void rebuild() override;
};



#if 0
#include <vector>
#include "baseActionEditor.h"


class geEnvelopeEditor : public geBaseActionEditor
{
	const char *l;      // internal label
	int         type;   // type of action
	int         range;

	/* point
	 * a single dot in the graph. x = relative frame, y = relative value */

	struct point
	{
		int   frame;
		int   iValue;
		float fValue;
		int   x;
		int   y;
	};

	/* points
	 * array of points, filled by fillPoints() */

	std::vector<point> points;

	/* selectedPoint
	 * which point we are selecting? */

	int selectedPoint;

	/* draggedPoint
	 * which point we are dragging? */

	int draggedPoint;

	/* previousXPoint
	 * x coordinate of point at time t-1. Used to check effective shifts */

	int previousXPoint;

	void draw();

	int handle(int e);

	int getSelectedPoint();

	void sortPoints();

	/* verticalPoint
	 * check if two points form a vertical line. In that case the frame value
	 * would be the same and recorder would go crazy, so shift by a small value
	 * of frames to create a minimal fadein/fadeout level. Return 0: no
	 * vertical points; return 1: vertical with the next one, return -1: vertical
	 * with the previous one. */

	int verticalPoint(const point &p);

public:

	geEnvelopeEditor(int x, int y, int type, int range, const char* l);
	~geEnvelopeEditor();

	/* addPoint
	 * add a point made of frame+value to internal points[]. */

	void addPoint(int frame, int iValue=0, float fValue=0.0f, int x=-1, int y=-1);

	void rebuild();

	/* fill
	 * parse recorder's stack and fill the widget with points. It's up to
	 * the caller to call this method as initialization. */

	void fill();

	inline void clearPoints() { points.clear(); }
};

#endif


#endif
