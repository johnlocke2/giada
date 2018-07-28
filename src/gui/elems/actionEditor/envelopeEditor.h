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

	geEnvelopePoint* getPointAtCursor() const;

	int calcPointX(int frame) const;
	int calcPointY(float value) const;
	bool isFirstPoint() const;
	bool isLastPoint() const;

	int onPush();
	int onDrag();
	int onRelease();

public:

	geEnvelopeEditor(int x, int y, int actionType, const char* l, SampleChannel* ch);

	void draw() override;
	int handle(int e) override;

	void rebuild() override;
};


#endif
