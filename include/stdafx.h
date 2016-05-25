#pragma once
#pragma warning(disable: 4482)

//게임 관련 공용헤더 include
#include "Character.h"
#include "Keyboard.h"
#include "Map.h"
#include "Effect.h"
#include "Camera.h"

//게임 프레임워크 관련 include
#include "GameState.h"
#include "GameRun.h"
#include "Result.h"

//UI 관련 include
#include "GUIBase.h"
#include "GameLoginWindow.h"
#include "GameLobbyWindow.h"
#include "GameRoomWindow.h"
#include "GameRunWindow.h"
#include "GameResultWindow.h"

#include <queue>
#include "..\server\Typedef_packet.h"

#include "SoundManager.h"
//#include "SoundMan.h"