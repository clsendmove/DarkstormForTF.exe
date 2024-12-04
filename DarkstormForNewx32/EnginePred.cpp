#include "EnginePred.h"
#include "MD5.h"

#define TICK_INTERVAL		( gInts.Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )	( static_cast<int>( 0.5f + static_cast<float>(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )	( TICK_INTERVAL * ( t ) )

int CEnginePrediction::GetTickbase(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	static int nTick = 0;
	static CUserCmd* pLastCmd = nullptr;

	if (pCmd)
	{
		if (!pLastCmd || pLastCmd->hasbeenpredicted)
			nTick = pLocal->GetTickBase();

		else nTick++;

		pLastCmd = pCmd;
	}

	return nTick;
}
#undef max
void CEnginePrediction::Start(CUserCmd* pCmd)
{
	CBaseEntity* pLocal = GetBaseEntity(me);

	if (pLocal && pLocal->IsAlive() && gInts.MoveHelper) // && !Vars::DT::shifting)
	{
		pLocal->SetCurrentCmd(pCmd);

		*gInts.RandomSeed = MD5_PseudoRandom(pCmd->command_number) & std::numeric_limits<int>::max();

		m_fOldCurrentTime = gInts.Globals->curtime;
		m_fOldFrameTime = gInts.Globals->frametime;
		m_nOldTickCount = gInts.Globals->tickcount;

		const int nOldTickBase = pLocal->GetTickBase();
		const bool bOldIsFirstPrediction = gInts.Prediction->m_bFirstTimePredicted;
		const bool bOldInPrediction = gInts.Prediction->m_bInPrediction;

		gInts.Globals->curtime = TICKS_TO_TIME(GetTickbase(pCmd, pLocal));
		gInts.Globals->frametime = (gInts.Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL);
		gInts.Globals->tickcount = GetTickbase(pCmd, pLocal);

		gInts.Prediction->m_bFirstTimePredicted = false;
		gInts.Prediction->m_bInPrediction = true;
		gInts.Prediction->SetLocalViewAngles(pCmd->viewangles);
		gInts.MoveHelper->SetHost(pLocal);

		gInts.Prediction->SetupMove(pLocal, pCmd, gInts.MoveHelper, &m_MoveData);
		gInts.GameMovement->ProcessMovement(pLocal, &m_MoveData);
		gInts.Prediction->FinishMove(pLocal, pCmd, &m_MoveData);

		pLocal->SetTickBase(nOldTickBase);

		gInts.Prediction->m_bInPrediction = bOldInPrediction;
		gInts.Prediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
	}
}

void CEnginePrediction::End(CUserCmd* pCmd)
{
	CBaseEntity* pLocal = GetBaseEntity(me);

	if (pLocal && pLocal->IsAlive() && gInts.MoveHelper) // && !Vars::DT::shifting)
	{
		gInts.MoveHelper->SetHost(nullptr);

		gInts.Globals->curtime = m_fOldCurrentTime;
		gInts.Globals->frametime = m_fOldFrameTime;
		gInts.Globals->tickcount = m_nOldTickCount;

		pLocal->SetCurrentCmd(nullptr);

		*gInts.RandomSeed = -1;
	}
}
