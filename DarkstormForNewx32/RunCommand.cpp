#include "RunCommand.h"

void __fastcall Hooked_RunCommand(PVOID Prediction, void* edx, CBaseEntity* pBaseEntity, CUserCmd* pCommand, CMoveHelper* MoveHelper)
{
	oRunCommand(Prediction, edx, pBaseEntity, pCommand, MoveHelper);
	if (MoveHelper && !gInts.MoveHelper)
		gInts.MoveHelper = MoveHelper;
}