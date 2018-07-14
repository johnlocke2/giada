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
class geGridTool_NEW;
class geButton;
class geButton;
class geScroll;
class geActionEditor;
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
	
	/* update
  Computes total width, in pixel. */

	void update();

public:

	virtual ~gdBaseActionEditor();

	virtual void cb_zoomIn()   = 0;
	virtual void cb_zoomOut()  = 0;

	int handle(int e) override;

	geChoice*   actionType;
	geGridTool_NEW* gridTool;
	geButton*   zoomIn;
	geButton*   zoomOut;
	geScroll*   viewport;       // widget container

	geActionEditor*   ac;
	geMuteEditor*     mc;
	geEnvelopeEditor* vc;
	geNoteEditor*     pr;

	Channel* ch;

	int zoom;
	Pixel totalWidth;  // full width of the widget
	Pixel usedWidth; 	 // width of the used area
};
} // giada::


#endif
