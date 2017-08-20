/* -----------------------------------------------------------------------------
 *
 * Giada - Your Hardcore Loopmachine
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2017 Giovanni A. Zuliani | Monocasual
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


#include <cassert>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Button.H>
#include "../../../../core/sampleChannel.h"
#include "../../../../core/midiChannel.h"
#include "../../../../glue/channel.h"
#include "../../../../utils/log.h"
#include "../../../../utils/fs.h"
#include "../../../../utils/string.h"
#include "../../../dialogs/gd_warnings.h"
#include "../../../elems/basics/boxtypes.h"
#include "../../../elems/basics/resizerBar.h"
#include "keyboard.h"
#include "sampleChannel.h"
#include "midiChannel.h"
#include "column.h"


using std::vector;
using std::string;


geColumn::geColumn(int X, int Y, int W, int H, int index, geKeyboard* parent)
	: Fl_Group(X, Y, W, H), 
	  parent  (parent), 
	  index   (index)
{
  /* geColumn does a bit of a mess: we pass a pointer to its parent (geKeyboard) and
  the geColumn itself deals with the creation of another widget, outside geColumn
  and inside geKeyboard, which handles the vertical resize bar (geResizerBar).
  The resizer cannot stay inside geColumn: it needs a broader view on the other
  side widgets. The view can be obtained from geKeyboard only (the upper level).
  Unfortunately, parent() can be nullptr: at this point (i.e the constructor)
  geColumn is still detached from any parent. We use a custom geKeyboard *parent
  instead. */

	begin();
	addChannelBtn = new geButton(x(), y(), w(), G_GUI_CHANNEL_H_1, "Add new channel");
	end();

  resizer = new geResizerBar(x()+w(), y(), G_GUI_OUTER_MARGIN * 2, h(), 
  	G_MIN_COLUMN_WIDTH, geResizerBar::HORIZONTAL);
  parent->add(resizer);

	addChannelBtn->callback(cb_addChannel, (void*)this);
}


/* -------------------------------------------------------------------------- */


geColumn::~geColumn()
{
  /* FIXME - this could actually cause a memory leak. resizer is
  just removed, not deleted. But we cannot delete it right now. */

  parent->remove(resizer);
}


/* -------------------------------------------------------------------------- */



int geColumn::handle(int e)
{
	switch (e) {
		case FL_RELEASE: {
			if (Fl::event_button() == FL_RIGHT_MOUSE) {
				__cb_addChannel();
				return 1;
			}
		}
		case FL_DND_ENTER:           	// return(1) for these events to 'accept' dnd
		case FL_DND_DRAG:
		case FL_DND_RELEASE: {
			return 1;
		}
		case FL_PASTE: {              // handle actual drop (paste) operation
			vector<string> paths;
			gu_split(Fl::event_text(), "\n", &paths);
			bool fails = false;
			int result = 0;
			for (string& path : paths) {
				gu_log("[geColumn::handle] loading %s...\n", path.c_str());
				SampleChannel *c = static_cast<SampleChannel*>(glue_addChannel(index, CHANNEL_SAMPLE));
				result = glue_loadChannel(c, gu_stripFileUrl(path));
				if (result != G_RES_OK) {
					deleteChannel(c->guiChannel);
					fails = true;
				}
			}
			if (fails) {
				if (paths.size() > 1)
					gdAlert("Some files were not loaded successfully.");
				else
					parent->printChannelMessage(result);
			}
			return 1;
		}
	}

	/* we return fl_Group::handle only if none of the cases above are fired. That
	is because we don't want to propagate a dnd drop to all the sub widgets. */

	return Fl_Group::handle(e);
}


/* -------------------------------------------------------------------------- */


void geColumn::resize(int X, int Y, int W, int H)
{
  /* Resize all children, including "add channel" button. */

  for (int i=0; i<children(); i++) {
    Fl_Widget* c = child(i);
    c->resize(X, Y + (i * (c->h() + G_GUI_INNER_MARGIN)), W, c->h());
  }

  /* Resize group itself. Must use internal functions, resize() would trigger
  infinite recursion. */

  x(X); y(Y); w(W); h(H);

  /* Resize resizerBar. */

  resizer->size(G_GUI_OUTER_MARGIN * 2, H);
}


/* -------------------------------------------------------------------------- */


void geColumn::refreshChannels()
{
	for (int i=1; i<children(); i++)
		static_cast<geChannel*>(child(i))->refresh();
}


/* -------------------------------------------------------------------------- */


void geColumn::draw()
{
	fl_color(G_COLOR_GREY_1_5);
	fl_rectf(x(), y(), w(), h());

  /* call draw and then redraw in order to avoid channel corruption when
  scrolling horizontally */

  for (int i=0; i<children(); i++) {
    child(i)->draw();
    child(i)->redraw();
  }
}


/* -------------------------------------------------------------------------- */


void geColumn::cb_addChannel(Fl_Widget* v, void* p) { ((geColumn*)p)->__cb_addChannel(); }


/* -------------------------------------------------------------------------- */


geChannel *geColumn::addChannel(Channel* ch)
{
	geChannel* gch = nullptr;

	/* All geChannels are added with y=0. That's not a problem, they will be 
	repositioned later on during geColumn::resize(). */

	if (ch->type == CHANNEL_SAMPLE)
		gch = new geSampleChannel(x(), 0, w(), G_GUI_CHANNEL_H_1, 
			static_cast<SampleChannel*>(ch));
	else
		gch = new geMidiChannel(x(), 0, w(), G_GUI_CHANNEL_H_1, 
			static_cast<MidiChannel*>(ch));

	add(gch);

  resize(x(), y(), w(), (children() * (G_GUI_CHANNEL_H_1 + G_GUI_INNER_MARGIN)) + 66); // evil space for drag n drop
	gch->redraw();    // fix corruption
	parent->redraw(); // redraw Keyboard
	return gch;
}


/* -------------------------------------------------------------------------- */


void geColumn::deleteChannel(geChannel* gch)
{
	gch->hide();
	remove(gch);
	delete gch;

	/* reposition all other channels and resize this group */
	/** TODO
	 * reposition is useless when called by geColumn::clear(). Add a new
	 * parameter to skip the operation */

	for (int i=0; i<children(); i++) {
		gch = static_cast<geChannel*>(child(i));
		gch->position(gch->x(), y()+(i*24));
	}
	size(w(), children() * 24 + 66);  // evil space for drag n drop
	redraw();
}


/* -------------------------------------------------------------------------- */


void geColumn::__cb_addChannel()
{
	gu_log("[geColumn::__cb_addChannel] index = %d\n", index);
	int type = openTypeMenu();
	if (type)
		glue_addChannel(index, type);
}


/* -------------------------------------------------------------------------- */


int geColumn::openTypeMenu()
{
	Fl_Menu_Item rclick_menu[] = {
		{"Sample channel"},
		{"MIDI channel"},
		{0}
	};

	Fl_Menu_Button* b = new Fl_Menu_Button(0, 0, 100, 50);
	b->box(G_CUSTOM_BORDER_BOX);
	b->textsize(G_GUI_FONT_SIZE_BASE);
	b->textcolor(G_COLOR_LIGHT_2);
	b->color(G_COLOR_GREY_2);

	const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, b);
	if (!m) return 0;

	if (strcmp(m->label(), "Sample channel") == 0)
		return CHANNEL_SAMPLE;
	if (strcmp(m->label(), "MIDI channel") == 0)
		return CHANNEL_MIDI;
	return 0;
}


/* -------------------------------------------------------------------------- */


void geColumn::clear(bool full)
{
	if (full)
		Fl_Group::clear();
	else {
		while (children() >= 2) {  // skip "add new channel" btn
			int i = children()-1;
			deleteChannel(static_cast<geChannel*>(child(i)));
		}
	}
}


/* -------------------------------------------------------------------------- */


Channel* geColumn::getChannel(int i)
{
	return static_cast<geChannel*>(child(i + 1))->ch;  // Skip "add channel"
}


/* -------------------------------------------------------------------------- */


int geColumn::getIndex()       { return index; }
void geColumn::setIndex(int i) { index = i; }
bool geColumn::isEmpty()       { return children() == 1; }
int geColumn::countChannels()  { return children() - 1; }