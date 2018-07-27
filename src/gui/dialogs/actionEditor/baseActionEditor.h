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


#ifndef GD_BASE_ACTION_EDITOR_H
#define GD_BASE_ACTION_EDITOR_H


#include <vector>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include "../../../core/types.h"
#include "../window.h"


class Channel;
class geChoice;
class geGridTool;
class geButton;
class geButton;
class geScroll;
class geSampleActionEditor;
class geMuteEditor;
class geEnvelopeEditor;
class geNoteEditor;


namespace giada
{
class gdBaseActionEditor : public gdWindow
{
protected:

	gdBaseActionEditor(Channel* ch);

	static void cb_zoomIn(Fl_Widget* w, void* p);
	static void cb_zoomOut(Fl_Widget* w, void* p);
	
	/* computeWidth
  Computes total width, in pixel. */

	void computeWidth();

public:

	virtual ~gdBaseActionEditor();

	/* rebuild
	Forces all internal widgets to rebuild themselves. Used when refreshing the
	whole Action Editor window. */
	
	virtual void rebuild() = 0;
	
	virtual void zoomIn();
	virtual void zoomOut();

	int handle(int e) override;

	Pixel frameToPixel(Frame f) const;
	Frame pixelToFrame(Pixel p) const;
	Pixel valueToPixel(float f, Pixel range) const;
	float pixelToValue(Pixel p, Pixel range) const;
	int getActionType() const;

	geChoice*   actionType;
	geGridTool* gridTool;
	geButton*   zoomInBtn;
	geButton*   zoomOutBtn;
	geScroll*   viewport;       // widget container

	geSampleActionEditor* ac;
	geMuteEditor*         mc;
	geEnvelopeEditor*     vc;
	geNoteEditor*         pr;

	Channel* ch;

	float ratio;
	Pixel fullWidth;   // Full widgets width, i.e. scaled-down full sequencer
	Pixel loopWidth; 	 // Loop width, i.e. scaled-down sequencer range
};
} // giada::


#endif
