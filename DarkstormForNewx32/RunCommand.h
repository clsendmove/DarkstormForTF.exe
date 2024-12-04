#pragma once
#include "SDK.h"

typedef bool(__fastcall* RunCommand_t)(void*, void*, CBaseEntity*, CUserCmd*, CMoveHelper* MoveHelper);
inline RunCommand_t oRunCommand;
void __fastcall Hooked_RunCommand(PVOID Prediction, void* edx, CBaseEntity* pBaseEntity, CUserCmd* pCommand, CMoveHelper* MoveHelper);