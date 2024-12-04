#include "SDK.h"

int CBaseEntity::GetHealth()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iHealth");
}

int CBaseEntity::GetTeamNum()
{
	DYNVAR_RETURN(int, this, "DT_BaseEntity", "m_iTeamNum");
}

int& CBaseEntity::GetFlags()
{
	//DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_fFlags");
	static int offset = gNetVars.get_offset("DT_BasePlayer", "m_fFlags");
	return *(int*)(this + offset);
}


BYTE CBaseEntity::GetLifeState()
{
	DYNVAR_RETURN(BYTE, this, "DT_BasePlayer", "m_lifeState");
}

int CBaseEntity::GetClassNum()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_PlayerClass", "m_iClass");
}

Vector CBaseEntity::GetEyeAngles() {
	DYNVAR_RETURN(Vector, this, "DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
}

void CBaseEntity::SetEyeAngles(Vector eye_angle) {
	*(Vector*)(this + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]")) = eye_angle;
}
int CBaseEntity::GetCond()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
}
Vector CBaseEntity::GetCollideableMins()
{
	DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMins");
}

Vector CBaseEntity::GetCollideableMaxs()
{
	DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMaxs");
}

Vector CBaseEntity::GetEyePosition()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]") + this->GetAbsOrigin();
}

Vector CBaseEntity::GetAbsEyePosition()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
}
bool CBaseEntity::IsAlive()
{
	return this->GetLifeState() == LIFE_ALIVE;
}
void CBaseEntity::ForceTauntCam(bool t) // From Dumpster fire
{
	DYNVAR(n, bool, "DT_TFPlayer", "m_nForceTauntCam");
	return n.SetValue(this, t);
}
void CBaseEntity::SetTickBase(int tickbase)
{
	DYNVAR(n, int, "DT_BasePlayer", "localdata", "m_nTickBase");
	return n.SetValue(this, tickbase);
}
Vector CBaseEntity::m_vecOrigin()
{
	DYNVAR_RETURN(Vector, this, "DT_BaseEntity", "m_vecOrigin");
}
Vector CBaseEntity::m_vecVelocity()
{
	DYNVAR_RETURN(Vector, this, "DT_TFPlayer", "m_vecVelocity[0]");
}
Vector CBaseEntity::m_vecBaseVelocity()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "m_vecBaseVelocity");
}
Vector CBaseEntity::m_vecViewOffset()
{
	DYNVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
}
int CBaseEntity::m_hGroundEntity()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_hGroundEntity");
}
int& CBaseEntity::GetTickBase()
{
	//DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_nTickBase");
	static int offset = gNetVars.get_offset("DT_BasePlayer", "localdata", "m_nTickBase");
	return *(int*)(this + offset);
}
void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut)
{
	for (auto i = 0; i < 3; i++)
		vOut[i] = vSome.Dot((Vector&)vMatrix[i]) + vMatrix[i][3];
}

Vector CBaseEntity::GetHitboxPosition(int iHitbox)
{
	DWORD* model = this->GetModel();
	if (!model)
		return Vector();

	studiohdr_t* hdr = gInts.ModelInfo->GetStudiomodel(model);
	if (!hdr)
		return Vector();

	matrix3x4 matrix[128];
	if (!this->SetupBones(matrix, 128, 0x100, 0))
		return Vector();

	int HitboxSetIndex = *(int*)((DWORD)hdr + 0xB0);
	if (!HitboxSetIndex)
		return Vector();

	mstudiohitboxset_t* pSet = (mstudiohitboxset_t*)(((PBYTE)hdr) + HitboxSetIndex);

	mstudiobbox_t* box = pSet->pHitbox(iHitbox);
	if (!box)
		return Vector();

	Vector vCenter = (box->bbmin + box->bbmax) * 0.5f;

	Vector vHitbox;

	VectorTransform(vCenter, matrix[box->bone], vHitbox);

	return vHitbox;
}
int CBaseEntity::GetHitboxSet()
{
	static int m_nHitboxSet = gNetVars.get_offset("DT_BaseAnimating", "m_nHitboxSet");
	return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + m_nHitboxSet);
}
int CBaseEntity::GetNumOfHitboxes()
{
	//model_t* pModel = (model_t*)GetClientRenderable()->GetModel();
	DWORD* pModel = this->GetModel();
	if (!pModel)
		return 0;

	studiohdr_t* pHdr = (studiohdr_t*)gInts.ModelInfo->GetStudiomodel(pModel);
	if (!pHdr)
		return 0;
	int HitboxSetIndex = *(int*)((DWORD)pHdr + 0xB0);
	if (!HitboxSetIndex)
		return 0;

	mstudiohitboxset_t* pSet = (mstudiohitboxset_t*)(((PBYTE)pHdr) + HitboxSetIndex);
	if (!pSet)
		return 0;

	return pSet->numhitboxes;
}

CBaseCombatWeapon* CBaseEntity::GetActiveWeapon()
{
	DYNVAR(pHandle, DWORD, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	return (CBaseCombatWeapon*)gInts.EntList->GetClientEntityFromHandle(pHandle.GetValue(this));
}

const char* CBaseEntity::szGetClass()
{
	DYNVAR(iClass, int, "DT_TFPlayer", "m_PlayerClass", "m_iClass");

	switch (iClass.GetValue(this))
	{
	case TF2_Scout:
		return "Scout";
	case TF2_Soldier:
		return "Soldier";
	case TF2_Pyro:
		return "Pyro";
	case TF2_Demoman:
		return "Demoman";
	case TF2_Heavy:
		return "Heavy";
	case TF2_Engineer:
		return "Engineer";
	case TF2_Medic:
		return "Medic";
	case TF2_Sniper:
		return "Sniper";
	case TF2_Spy:
		return "Spy";
	default:
		return "Unknown class";
	}

	return "Unknown class"; //Just in case
}