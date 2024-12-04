#include "ESP.h"

void CESP::Run()
{
	if (!gInts.Engine->IsInGame())
		return;
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;
		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive())
			continue;

		ESP(pEntity);
	}
}
#include "DrawManager.h"
void CESP::ESP(CBaseEntity* pEntity)
{
	Vector vecWorld, vecScreen;
	pEntity->GetWorldSpaceCenter(vecWorld);

	player_info_t pInfo;
	if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
		return;
	if (gDraw.WorldToScreen(vecWorld, vecScreen))
	{
		gDraw.DrawString(vecScreen.x, vecScreen.y, COLORCODE(255,255,255,255), "%s", pInfo.name);
	}
}