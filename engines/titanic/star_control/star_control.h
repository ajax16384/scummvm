/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef TITANIC_STAR_CONTROL_H
#define TITANIC_STAR_CONTROL_H

#include "titanic/core/game_object.h"
#include "titanic/star_control/star_field.h"
#include "titanic/star_control/star_view.h"

namespace Titanic {

class CStarControl : public CGameObject {
	DECLARE_MESSAGE_MAP;
	bool MouseButtonDownMsg(CMouseButtonDownMsg *msg);
	bool MouseMoveMsg(CMouseMoveMsg *msg);
	bool KeyCharMsg(CKeyCharMsg *msg);
	bool FrameMsg(CFrameMsg *msg);
private:
	int _fieldBC;
	CStarField _starField;
	CStarView _view;
	Rect _starRect;
#if 0
	int _field80B0;
#endif
private:
	/**
	 * Called for ever new game frame
	 */
	void newFrame();
public:
	CLASSDEF;
	CStarControl();
	virtual ~CStarControl();

	/**
	 * Save the data for the class to file
	 */
	virtual void save(SimpleFile *file, int indent);

	/**
	 * Load the data for the class from file
	 */
	virtual void load(SimpleFile *file);

	/**
	 * Allows the item to draw itself
	 */
	virtual void draw(CScreenManager *screenManager);

	void fn1(int action);
	bool fn4();

	/**
	 * Returns true if a star destination can be set
	 */
	bool canSetStarDestination() const;

	/**
	 * Called when a star destination is set
	 */
	void starDestinationSet();
};

} // End of namespace Titanic

#endif /* TITANIC_STAR_CONTROL_H */
