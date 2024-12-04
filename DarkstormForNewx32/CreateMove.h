#pragma once
#include "SDK.h"
typedef bool(__fastcall* CreateMove_t)(void*, void*, float, CUserCmd*);
inline CreateMove_t oCreateMove;
bool __fastcall Hooked_CreateMove(void* ClientMode, void* edx, float flInputSampleFrametime, CUserCmd* pCmd);