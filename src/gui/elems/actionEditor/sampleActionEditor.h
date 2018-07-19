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


#ifndef GE_SAMPLE_ACTION_EDITOR_H
#define GE_SAMPLE_ACTION_EDITOR_H


#include "baseActionEditor.h"


class geSampleAction;
class SampleChannel;


class geSampleActionEditor : public geBaseActionEditor
{
private:

	SampleChannel* ch;

	/* action
	Pointer to the selected action. Used when dragging action around. */

	geSampleAction* action;

	/* actionOriginalX, actionOriginalW
	 * x and w of the action, when moved. Useful for checking if the action
	 * overlaps another one: in that case the moved action returns to
	 * actionOriginalX (and to actionOriginalW if resized). */

	int actionOriginalX;
	int actionOriginalW;

	/* actionPickPoint
	 * the precise x point in which the action has been picked with the mouse,
	 * before a dragging action. */

	int actionPickPoint;

	void moveAction();
	void resizeAction();

	/* actionCollides
	 * true if an action collides with another. Used while adding new points
	 * with snap active.*/

	bool actionCollides(int frame);

	/* getActionAtCursor
	Returns the action under the mouse. nullptr if nothing found. */

	geSampleAction* getActionAtCursor();
	geSampleAction* getSelectedAction();

	int onPush();
	int onDrag();
	int onRelease();

public:

	geSampleActionEditor(int x, int y, SampleChannel* ch);

	void draw() override;
	int  handle(int e) override;

	void rebuild() override;
};


#endif
