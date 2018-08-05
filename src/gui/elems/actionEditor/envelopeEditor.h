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

	/* m_actionType
	What type of action this envelope editor is dealing with. */
	
	int m_actionType;

	void onAddAction()     override;
	void onDeleteAction()  override;
	void onMoveAction()    override;
	void onResizeAction()  override{}; // nothing to do here
	void onRefreshAction() override;

	int   frameToX(int frame) const;
	int   valueToY(float value) const;
	float yToValue(int pixel) const;

	bool isFirstPoint() const;
	bool isLastPoint()  const;

public:

	geEnvelopeEditor(int x, int y, int actionType, const char* l, SampleChannel* ch);
	~geEnvelopeEditor();

	void draw() override;

	void rebuild() override;
};


#endif
