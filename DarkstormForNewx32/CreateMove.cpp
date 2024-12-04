#include "CreateMove.h"
#include "EnginePred.h"
bool __fastcall Hooked_CreateMove(void* ecx, void* edx, float input_sample_frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number || !pCmd)
		return oCreateMove(ecx, edx, input_sample_frametime, pCmd);

	CBaseEntity* pBaseEntity = GetBaseEntity(me);

	if (pBaseEntity == NULL)
		return oCreateMove(ecx, edx, input_sample_frametime, pCmd);

	gEnginePrediction.Start(pCmd);
	{

	}
	gEnginePrediction.End(pCmd);

	return false;
}