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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef TSAGE_RINGWORLD2_SCENES2_H
#define TSAGE_RINGWORLD2_SCENES2_H

#include "common/scummsys.h"
#include "tsage/converse.h"
#include "tsage/events.h"
#include "tsage/core.h"
#include "tsage/scenes.h"
#include "tsage/globals.h"
#include "tsage/sound.h"
#include "tsage/ringworld2/ringworld2_logic.h"
#include "tsage/ringworld2/ringworld2_speakers.h"

namespace TsAGE {

namespace Ringworld2 {

using namespace TsAGE;

class Scene2000 : public SceneExt {
	class Action1 : public ActionExt {
	public:
		virtual void signal();
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
	class Exit2 : public SceneExit {
	public:
		virtual void changeScene();
	};
	class Exit3 : public SceneExit {
	public:
		virtual void changeScene();
	};
	class Exit4 : public SceneExit {
	public:
		virtual void changeScene();
	};
	class Exit5 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	bool _exitingFlag;
	int _mazePlayerMode;

	NamedHotspot _item1;
	SceneActor _object1;
	SceneActor _objList1[11];
	Exit1 _exit1;
	Exit2 _exit2;
	Exit3 _exit3;
	Exit4 _exit4;
	Exit5 _exit5;
	Action1 _action1, _action2, _action3, _action4, _action5;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
	virtual void process(Event &event);
	virtual void synchronize(Serializer &s);

	void initExits();
	void initPlayer();
};

class Scene2350 : public SceneExt {
	class Actor2 : public SceneActor {
		virtual bool startAction(CursorType action, Event &event);
	};
	class Actor3 : public SceneActor {
		virtual bool startAction(CursorType action, Event &event);
	};

	class ExitUp : public SceneExit {
		virtual void changeScene();
	};
	class ExitWest : public SceneExit {
		virtual void changeScene();
	};
public:

	SpeakerQuinn2350 _quinnSpeaker;
	SpeakerPharisha2350 _pharishaSpeaker;
	NamedHotspot _item1;
	SceneActor _actor1;
	Actor2 _actor2;
	Actor3 _actor3;
	Actor3 _actor4;
	ExitUp _exitUp;
	ExitWest _exitWest;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
	virtual void process(Event &event);
};

class Scene2400 : public SceneExt {
	class Exit1 : public SceneExit {
		virtual void changeScene();
	};
	class Exit2 : public SceneExit {
		virtual void changeScene();
	};
public:
	Exit1 _exit1;
	Exit2 _exit2;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void signal();
};

class Scene2425 : public SceneExt {
	class Item1 : public NamedHotspot {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};
	class Item2 : public NamedHotspot {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};
	class Item3 : public NamedHotspot {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};
	class Item4 : public NamedHotspot {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};

	class Actor1 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor2 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	Item1 _item1;
	Item2 _item2;
	Item3 _item3;
	Item4 _item4;
	Actor1 _actor1;
	Actor2 _actor2;
	Actor2 _actor3;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
};

class Scene2430 : public SceneExt {
	class Actor1 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor2 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor3 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	NamedHotspot _item1;
	NamedHotspot _item2;
	NamedHotspot _item3;
	NamedHotspot _item4;
	NamedHotspot _item5;
	NamedHotspot _item6;
	NamedHotspot _item7;
	NamedHotspot _item8;
	NamedHotspot _item9;
	NamedHotspot _item10;
	NamedHotspot _item11;
	NamedHotspot _item12;
	NamedHotspot _item13;
	Actor1 _actor1;
	Actor2 _actor2;
	Actor3 _actor3;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void signal();
};

class Scene2435 : public SceneExt {
	class Actor1 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor2 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	SpeakerQuinn2435 _quinnSpeaker;
	SpeakerSeeker2435 _seekerSpeaker;
	SpeakerPharisha2435 _pharishaSpeaker;
	NamedHotspot _item1;
	NamedHotspot _item2;
	NamedHotspot _item3;
	Actor1 _actor1;
	Actor2 _actor2;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
};

class Scene2440 : public SceneExt {
	class Actor1 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor2 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	NamedHotspot _item1;
	NamedHotspot _item2;
	NamedHotspot _item3;
	NamedHotspot _item4;
	NamedHotspot _item5;
	NamedHotspot _item6;
	NamedHotspot _item7;
	Actor1 _actor1;
	Actor2 _actor2;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
};

class Scene2445 : public SceneExt {
public:
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void signal();
};

class Scene2450 : public SceneExt {
	class Actor2 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor3 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	SpeakerQuinn2450 _quinnSpeaker;
	SpeakerSeeker2450 _seekerSpeaker;
	SpeakerCaretaker2450 _caretakerSpeaker;
	NamedHotspot _item1;
	NamedHotspot _item2;
	NamedHotspot _item3;
	SceneActor _actor1;
	Actor2 _actor2;
	Actor3 _actor3;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
};

class Scene2455 : public SceneExt {
	class Actor1 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor2 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor3 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	NamedHotspot _item1;
	Actor1 _actor1;
	Actor2 _actor2;
	Actor3 _actor3;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
};

class Scene2500 : public SceneExt {
	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	SpeakerQuinn2500 _quinnSpeaker;
	SpeakerSeeker2500 _seekerSpeaker;
	SpeakerMiranda2500 _mirandaSpeaker;
	SpeakerWebbster2500 _webbsterSpeaker;
	NamedHotspot _item1;
	SceneActor _actor1;
	SceneActor _actor2;
	SceneActor _actor3;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void signal();
};

class Scene2525 : public SceneExt {
	class Item5 : public NamedHotspot {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};

	class Actor3 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	NamedHotspot _item1;
	NamedHotspot _item2;
	NamedHotspot _item3;
	NamedHotspot _item4;
	Item5 _item5;
	SceneActor _actor1;
	SceneActor _actor2;
	Actor3 _actor3;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
};

class Scene2530 : public SceneExt {
	class Actor2 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};
	class Actor3 : public SceneActor {
	public:
		bool startAction(CursorType action, Event &event);
	};

	class Exit1 : public SceneExit {
	public:
		virtual void changeScene();
	};
public:
	NamedHotspot _item1;
	NamedHotspot _item2;
	NamedHotspot _item3;
	NamedHotspot _item4;
	NamedHotspot _item5;
	SceneActor _actor1;
	Actor2 _actor2;
	Actor3 _actor3;
	Exit1 _exit1;
	SequenceManager _sequenceManager;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void signal();
};
} // End of namespace Ringworld2
} // End of namespace TsAGE

#endif
