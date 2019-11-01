//Code generated at 1572584676

static void MORSTAIRS_MAP_EVENTS_F();
static void OVERWORLD_MAP_EVENTS_F();
static void DUNGEON_MAP_EVENTS_F();
#define MORSTAIRS_MAP_EVENTS MORSTAIRS_MAP_EVENTS_F();
#define OVERWORLD_MAP_EVENTS OVERWORLD_MAP_EVENTS_F();
#define DUNGEON_MAP_EVENTS DUNGEON_MAP_EVENTS_F();

GENERATE_FUNC_LEADING_TO_DESTINATION(MORSTAIRS_MAP_TO_OVERWORLD_MAP_0, 12, 9, MORSTAIRS_MAP_MAP_PATH, MORSTAIRS_MAP_EVENTS)
GENERATE_FUNC_LEADING_TO_DESTINATION(OVERWORLD_MAP_TO_MORSTAIRS_MAP_1, 17, 5, OVERWORLD_MAP_MAP_PATH, OVERWORLD_MAP_EVENTS)
GENERATE_FUNC_LEADING_TO_DESTINATION(DUNGEON_MAP_TO_MORSTAIRS_MAP_2, 16, 3, DUNGEON_MAP_MAP_PATH, DUNGEON_MAP_EVENTS)
GENERATE_FUNC_LEADING_TO_DESTINATION(MORSTAIRS_MAP_TO_DUNGEON_MAP_3, 29, 5, MORSTAIRS_MAP_MAP_PATH, MORSTAIRS_MAP_EVENTS)
GENERATE_FUNC_LEADING_TO_DESTINATION(DUNGEON_MAP_TO_OVERWORLD_MAP_6, 23, 20, DUNGEON_MAP_MAP_PATH, DUNGEON_MAP_EVENTS)
GENERATE_FUNC_LEADING_TO_DESTINATION(OVERWORLD_MAP_TO_DUNGEON_MAP_7, 19, 7, OVERWORLD_MAP_MAP_PATH, OVERWORLD_MAP_EVENTS)

static void MORSTAIRS_MAP_EVENTS_F() { 
	ADD_EVENT_AT_POSITION(11, 9, OVERWORLD_MAP_TO_MORSTAIRS_MAP_1)
	ADD_EVENT_AT_POSITION(29, 5, DUNGEON_MAP_TO_MORSTAIRS_MAP_2)
}

static void OVERWORLD_MAP_EVENTS_F() { 
	ADD_EVENT_AT_POSITION(17, 5, MORSTAIRS_MAP_TO_OVERWORLD_MAP_0)
	ADD_EVENT_AT_POSITION(19, 7, DUNGEON_MAP_TO_OVERWORLD_MAP_6)
}

static void DUNGEON_MAP_EVENTS_F() { 
	ADD_EVENT_AT_POSITION(16, 3, MORSTAIRS_MAP_TO_DUNGEON_MAP_3)
	ADD_EVENT_AT_POSITION(23, 20, OVERWORLD_MAP_TO_DUNGEON_MAP_7)
}