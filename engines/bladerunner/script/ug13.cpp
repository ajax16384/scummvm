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

#include "bladerunner/script/script.h"

namespace BladeRunner {

void ScriptUG13::InitializeScene() {
	if (Game_Flag_Query(435)) {
		Setup_Scene_Information(-477.0f, 141.9f, -870.0f, 378);
	} else if (Game_Flag_Query(350)) {
		Setup_Scene_Information(39.0f, 52.94f, -528.0f, 600);
	} else {
		Setup_Scene_Information(-22.0f, 54.63f, -883.0f, 578);
		Actor_Set_Invisible(0, false);
	}
	if (!Game_Flag_Query(431)) {
		Scene_Exit_Add_2D_Exit(0, 394, 205, 464, 281, 0);
	}
	Scene_Exit_Add_2D_Exit(1, 560, 90, 639, 368, 1);
	Scene_Exit_Add_2D_Exit(2, 108, 85, 175, 210, 3);
	Ambient_Sounds_Add_Looping_Sound(331, 15, 0, 1);
	Ambient_Sounds_Add_Looping_Sound(332, 40, 0, 1);
	Ambient_Sounds_Add_Looping_Sound(333, 40, 0, 1);
	Ambient_Sounds_Add_Sound(401, 2, 120, 11, 12, -100, 100, -100, 100, 0, 0);
	Ambient_Sounds_Add_Sound(402, 2, 120, 11, 12, -100, 100, -100, 100, 0, 0);
	Ambient_Sounds_Add_Sound(369, 2, 120, 11, 12, -100, 100, -100, 100, 0, 0);
	Ambient_Sounds_Add_Sound(397, 2, 120, 11, 12, -100, 100, -100, 100, 0, 0);
	Ambient_Sounds_Add_Sound(398, 2, 120, 11, 12, -100, 100, -100, 100, 0, 0);
	if (Global_Variable_Query(1) == 4 && !Game_Flag_Query(169)) {
		Actor_Set_Goal_Number(12, 390);
	}
	if (Actor_Query_Goal_Number(12) == 599) {
		Actor_Change_Animation_Mode(12, 89);
	}
	if (Game_Flag_Query(429)) {
		Scene_Loop_Start_Special(0, 0, 0);
		Scene_Loop_Set_Default(1);
	} else if (Game_Flag_Query(431)) {
		Scene_Loop_Set_Default(4);
	} else {
		Scene_Loop_Set_Default(1);
	}
}

void ScriptUG13::SceneLoaded() {
	Obstacle_Object("BASKET", true);
	Obstacle_Object("BOLLARD", true);
	Unobstacle_Object("STAIR", true);
	Unobstacle_Object("BOX FOR ARCHWAY 02", true);
	Unobstacle_Object("STAIR_RAIL", true);
	Unobstacle_Object("DISC_LEFT", true);
	Clickable_Object("BASKET");
	Clickable_Object("BOLLARD");
	Unclickable_Object("BASKET");
	if (Global_Variable_Query(1) >= 3 && !Actor_Clue_Query(0, 128) && Game_Flag_Query(169) && (Actor_Clue_Query(0, 81) || Actor_Clue_Query(0, 80))) {
		Item_Add_To_World(111, 958, 85, -209.01f, 70.76f, -351.79f, 0, 16, 12, false, true, false, true);
	}
}

bool ScriptUG13::MouseClick(int x, int y) {
	return false;
}

bool ScriptUG13::ClickedOn3DObject(const char *objectName, bool a2) {

	if (Object_Query_Click("BOLLARD", objectName) && !Loop_Actor_Walk_To_XYZ(0, 7.0f, 44.0f, -695.0f, 0, 1, false, 0)) {
		Actor_Face_Object(0, "BOLLARD", true);
		if (Game_Flag_Query(431)) {
			Scene_Loop_Set_Default(1);
			Scene_Loop_Start_Special(2, 0, 0);
			Game_Flag_Reset(431);
			Game_Flag_Set(436);
			return true;
		} else {
			Scene_Loop_Set_Default(4);
			Scene_Loop_Start_Special(2, 3, 0);
			Game_Flag_Set(431);
			Scene_Exit_Remove(0);
			return true;
		}
	}
	return false;
}

bool ScriptUG13::ClickedOnActor(int actorId) {
	if (actorId == 12 && Global_Variable_Query(1) == 4 && !Loop_Actor_Walk_To_XYZ(0, -248.0f, 44.0f, -390.0f, 12, 1, false, 0)) {
		Actor_Face_Actor(0, 12, true);
		if (Actor_Query_Goal_Number(12) != 6 && Actor_Query_Goal_Number(12) != 599) {
			if (!Game_Flag_Query(554)) {
				sub_40223C();
			} else if (!Actor_Clue_Query(0, 122) || !Actor_Clue_Query(0, 123)) {
				sub_402AD4();
			} else {
				Actor_Set_Goal_Number(12, 391);
				if (Actor_Clue_Query(0, 131)) {
					sub_402AD4();
				} else {
					Actor_Face_Actor(0, 12, true);
					Actor_Says(0, 5600, 14);
					Actor_Says(12, 100, 53);
					Actor_Says(0, 5605, 18);
					Actor_Start_Speech_Sample(12, 110);
					Actor_Set_Goal_Number(12, 395);
				}
			}
		} else if (Random_Query(0, 1) == 1) {
			Actor_Says(0, 8590, 16);
		} else {
			Actor_Says(0, 8655, 15);
		}
	}
	return false;
}

bool ScriptUG13::ClickedOnItem(int itemId, bool a2) {
	if (itemId == 111 && !Loop_Actor_Walk_To_Item(0, 111, 36, 1, false)) {
		Actor_Face_Item(0, 111, true);
		Actor_Clue_Acquire(0, 128, 1, -1);
		Item_Remove_From_World(111);
		Item_Pickup_Spin_Effect(958, 426, 316);
		Actor_Voice_Over(3950, 99);
		Actor_Voice_Over(3960, 99);
		Actor_Voice_Over(3970, 99);
		Actor_Voice_Over(3980, 99);
		Actor_Voice_Over(3990, 99);
		Actor_Voice_Over(4000, 99);
		return true;
	}
	return false;
}

bool ScriptUG13::ClickedOnExit(int exitId) {

	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(0, -32.0f, 54.63f, -883.0f, 0, 1, false, 0)) {
			Player_Loses_Control();
			Game_Flag_Set(430);
			Game_Flag_Set(431);
			Set_Enter(81, 93);
			Scene_Loop_Start_Special(1, 3, 0);
		}
		return true;
	}
	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(0, 39.0f, 52.94f, -528.0f, 0, 1, false, 0)) {
			Game_Flag_Set(351);
			Set_Enter(87, 99);
		}
		return true;
	}
	if (exitId == 2) {
		if (!Loop_Actor_Walk_To_XYZ(0, -267.0f, 44.0f, -795.0f, 0, 1, false, 0)) {
			Actor_Face_Heading(0, 830, false);
			Footstep_Sound_Override_On(3);
			Loop_Actor_Travel_Stairs(0, 11, 1, 0);
			Footstep_Sound_Override_Off();
			if (!sub_402AD0()) {
				Loop_Actor_Walk_To_XYZ(0, -477.0f, 141.9f, -870.0f, 0, 0, false, 0);
				Game_Flag_Set(434);
				Set_Enter(89, 102);
				return true;
			}
			Actor_Face_Heading(0, 325, false);
			Loop_Actor_Travel_Stairs(0, 11, 0, 0);
		} else {
			return true;
		}
	}

	return false;
}

bool ScriptUG13::ClickedOn2DRegion(int region) {
	return false;
}

void ScriptUG13::SceneFrameAdvanced(int frame) {
	if (frame == 94) {
		Ambient_Sounds_Play_Sound(372, 90, 0, 0, 100);
	}
	if (Game_Flag_Query(436) && frame > 29 && frame < 91) {
		Scene_Exit_Add_2D_Exit(0, 394, 205, 464, 281, 0);
		Game_Flag_Reset(436);
		//return true;
		return;
	}
	if (Game_Flag_Query(429) && frame < 25) {
		Actor_Set_Invisible(0, true);
	} else if (Game_Flag_Query(430) && frame >= 94 && frame <= 120) {
		Actor_Set_Invisible(0, true);
	} else {
		Actor_Set_Invisible(0, false);
	}
	//return false;
	return;
}

void ScriptUG13::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void ScriptUG13::PlayerWalkedIn() {
	if (Game_Flag_Query(435)) {
		Loop_Actor_Walk_To_XYZ(0, -389.0f, 143.0f, -844.0f, 0, 0, false, 0);
		Actor_Face_Heading(0, 325, false);
		Footstep_Sound_Override_On(3);
		Loop_Actor_Travel_Stairs(0, 11, 0, 0);
		Footstep_Sound_Override_Off();
		Game_Flag_Reset(435);
	} else if (Game_Flag_Query(350)) {
		Loop_Actor_Walk_To_XYZ(0, -12.0f, 44.0f, -528.0f, 0, 0, false, 0);
		Game_Flag_Reset(350);
	} else {
		Loop_Actor_Walk_To_XYZ(0, -60.0f, 55.24f, -816.0f, 0, 0, false, 0);
		Game_Flag_Reset(429);
		Player_Gains_Control();
	}
	if (Actor_Query_Goal_Number(12) >= 390 && !Game_Flag_Query(169)) {
		if (Game_Flag_Query(553)) {
			if (Random_Query(1, 3) == 1) {
				Actor_Set_Goal_Number(12, 395);
			}
		} else {
			Game_Flag_Set(553);
			Actor_Says(12, 50, 3);
		}
	}
	//return false;
}

void ScriptUG13::PlayerWalkedOut() {
	Actor_Set_Invisible(0, false);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);
	if (Game_Flag_Query(430)) {
		Ambient_Sounds_Remove_Sound(401, false);
		Ambient_Sounds_Remove_Sound(402, false);
		Ambient_Sounds_Remove_Sound(369, false);
		Ambient_Sounds_Remove_Sound(397, false);
		Ambient_Sounds_Remove_Sound(398, false);
	} else {
		Ambient_Sounds_Remove_All_Non_Looping_Sounds(1);
	}
}

void ScriptUG13::DialogueQueueFlushed(int a1) {
}

void ScriptUG13::sub_40223C() {
	Actor_Face_Actor(0, 12, true);
	Game_Flag_Set(554);
	Actor_Says(0, 5560, 13);
	Actor_Says_With_Pause(0, 5565, 3.0f, 18);
	Actor_Says(12, 70, 31);
	Actor_Says(12, 80, 32);
	Actor_Says(0, 5570, 3);
	Actor_Says(12, 90, 32);
}

void ScriptUG13::sub_4023D8() {
	Actor_Face_Actor(0, 12, true);
	Actor_Clue_Acquire(0, 122, 0, 12);
	Actor_Modify_Friendliness_To_Other(12, 0, -5);
	Actor_Says(0, 5575, 16);
	Actor_Says(12, 120, 31);
	Actor_Says(0, 5610, 15);
	Actor_Says(12, 140, 32);
	Actor_Says(0, 5615, 18);
	Actor_Says(12, 160, 33);
	Actor_Says(0, 5620, 9);
	Actor_Says(12, 170, 30);
	Actor_Says(0, 5625, 12);
	Actor_Says(12, 180, 32);
	Actor_Says(0, 5630, 18);
	Actor_Says(12, 190, 32);
	Actor_Says(0, 5635, 15);
	Actor_Says(12, 200, 31);
}

void ScriptUG13::sub_4025E0() {
	Actor_Clue_Acquire(0, 123, 0, 12);
	Actor_Modify_Friendliness_To_Other(12, 0, -10);
	Actor_Says(12, 220, 30);
	Actor_Says(0, 5640, 19);
	Actor_Says(12, 230, 33);
	Actor_Says(0, 5645, 16);
	Actor_Says(12, 240, 30);
	Actor_Says(12, 250, 33);
	Actor_Says(0, 5650, 14);
	Actor_Says(12, 260, 32);
}

void ScriptUG13::sub_402960() {
	Actor_Says(0, 5670, 9);
	Actor_Says(12, 340, 31);
	Actor_Says(0, 5690, 19);
	Actor_Says(12, 350, 32);
	Actor_Says(0, 5695, 14);
	Actor_Says(12, 360, 33);
	Actor_Voice_Over(2710, 99);
	Actor_Voice_Over(2730, 99);
	Actor_Clue_Acquire(0, 124, 0, 12);
}

int ScriptUG13::sub_402AD0() {
	return 0;
}

void ScriptUG13::sub_402AD4() {
	Dialogue_Menu_Clear_List();
	DM_Add_To_List_Never_Repeat_Once_Selected(1320, 6, 3, 1);
	if (Actor_Clue_Query(0, 122)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1330, 5, 8, 5);
	}
	DM_Add_To_List_Never_Repeat_Once_Selected(1340, 2, 4, 6);
	if (Actor_Clue_Query(0, 131)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1350, 1, 3, 7);
	}
	Dialogue_Menu_Add_DONE_To_List(1360);
	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();
	switch (answer) {
	case 1360:
		return;
	case 1350:
		Actor_Clue_Acquire(12, 131, 0, 0);
		Actor_Says_With_Pause(0, 5595, 1.0f, 23);
		Item_Pickup_Spin_Effect(945, 193, 325);
		Actor_Says(12, 290, 33);
		Actor_Says(0, 5660, 13);
		Actor_Clue_Lose(0, 131);
		sub_402E24();
		break;
	case 1340:
		Actor_Modify_Friendliness_To_Other(12, 0, -10);
		Actor_Says(0, 5590, 15);
		Actor_Says(12, 270, 31);
		Actor_Says(0, 5655, 16);
		Actor_Says(12, 280, 32);
		break;
	case 1330:
		Actor_Says(0, 5585, 16);
		sub_4025E0();
		break;
	case 1320:
		sub_4023D8();
		break;
	default:
		Actor_Face_Actor(0, 12, true);
		Actor_Says(0, 5600, 14);
		Actor_Says(12, 100, 53);
		Actor_Says(0, 5605, 18);
		Actor_Start_Speech_Sample(12, 110);
		Actor_Set_Goal_Number(12, 395);
		break;
	}
}

void ScriptUG13::sub_402E24() {
	Actor_Set_Friendliness_To_Other(12, 0, 40);
	Dialogue_Menu_Clear_List();
	DM_Add_To_List_Never_Repeat_Once_Selected(1370, 1, 1, 8);
	DM_Add_To_List_Never_Repeat_Once_Selected(1380, 1, 8, 1);
	DM_Add_To_List_Never_Repeat_Once_Selected(1390, 8, 1, 1);
	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();
	if (answer == 1370) {
		Actor_Says(0, 5665, 16);
		Actor_Says(12, 300, 32);
		Actor_Says(0, 5680, 19);
		Actor_Says(12, 310, 33);
		Actor_Says(12, 330, 30);
		Actor_Start_Speech_Sample(12, 110);
		Actor_Set_Goal_Number(12, 395);
		Actor_Says(0, 5685, 18);
	} else if (answer == 1380) {
		if (Actor_Clue_Query(0, 123)) {
			sub_402960();
		} else {
			Actor_Says(0, 5700, 15);
			sub_4025E0();
		}
	} else if (answer == 1390) {
		Actor_Says(0, 5675, 9);
		Actor_Says(12, 370, 32);
		Actor_Says(0, 5705, 10);
	}
}

} // End of namespace BladeRunner
