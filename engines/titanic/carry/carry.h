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

#ifndef TITANIC_CARRY_H
#define TITANIC_CARRY_H

#include "titanic/core/game_object.h"
#include "titanic/messages/messages.h"
#include "titanic/messages/mouse_messages.h"

namespace Titanic {

class CCarry : public CGameObject {
	DECLARE_MESSAGE_MAP;
	bool MouseDragStartMsg(CMouseDragStartMsg *msg);
	bool MouseDragMoveMsg(CMouseDragMoveMsg *msg);
	bool MouseDragEndMsg(CMouseDragEndMsg *msg);
	bool UseWithCharMsg(CUseWithCharMsg *msg);
	bool LeaveViewMsg(CLeaveViewMsg *msg);
	bool UseWithOtherMsg(CUseWithOtherMsg *msg);
	bool VisibleMsg(CVisibleMsg *msg);
	bool MouseButtonDownMsg(CMouseButtonDownMsg *msg);
	bool RemoveFromGameMsg(CRemoveFromGameMsg *msg);
	bool MoveToStartPosMsg(CMoveToStartPosMsg *msg);
	bool EnterViewMsg(CEnterViewMsg *msg);
	bool PassOnDragStartMsg(CPassOnDragStartMsg *msg);
protected:
	int _fieldDC;
	CString _string3;
	CString _string4;
	Point _tempPos;
	int _field100;
	int _field104;
	int _field108;
	int _field10C;
	int _itemFrame;
	CString _string5;
	int _enterFrame;
	bool _enterFrameSet;
	int _visibleFrame;
public:
	CString _string1;
	bool _canTake;
	Point _origPos;
	CString _fullViewName;
public:
	CLASSDEF;
	CCarry();

	/**
	 * Save the data for the class to file
	 */
	virtual void save(SimpleFile *file, int indent);

	/**
	 * Load the data for the class from file
	 */
	virtual void load(SimpleFile *file);
};

} // End of namespace Titanic

#endif /* TITANIC_CARRY_H */
