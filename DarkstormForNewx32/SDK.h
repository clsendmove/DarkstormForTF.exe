#pragma once
#include "Vector.h"
#include "Vector2.h"
#include "dt_common2.h"
#include "dt_recv2.h"
#include "Memory.h"
#include <windows.h>
#include <math.h>
#include <xstring>
#include <string>
#include <cstdio>
#include "CNetVars.h"

#define NETVAR(_name, type, table, name) inline type &_name() \
{ \
	static int offset = gNetVars.GetNetVar(table, name); \
	return *reinterpret_cast<type *>(reinterpret_cast<DWORD>(this) + offset); \
}
#define OFFSETThing( name, type, offset ) inline type& name( ) { \
	return *reinterpret_cast< type* >( reinterpret_cast< uint32_t >( this ) + offset ); \
}
#pragma warning(disable : 4002) // SHUSH NIGGER LOL
#define me gInts.Engine->GetLocalPlayer()
#define GetBaseEntity gInts.EntList->GetClientEntity
#define MASK_AIMBOT 0x200400B
#define	FL_ONGROUND (1<<0)
#define FL_DUCKING (1<<1)
#define CONTENTS_HITBOX 0x40000000
#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define BLU_TEAM 3
#define RED_TEAM 2
typedef float matrix3x4[3][4];

typedef struct player_info_s
{
	char			name[32];
	int				userID;
	char			guid[33];
	unsigned long	friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	unsigned long	customFiles[4];
	unsigned char	filesDownloaded;
} player_info_t;

class ClientClass
{
private:
	BYTE _chPadding[8];
public:
	char* chName;
	RecvTable* Table;
	ClientClass* pNextClass;
	int iClassID;
};

class CHLClient
{
public:
	ClientClass* GetAllClasses(void)
	{
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return Memory::CallVirtual<OriginalFn>(this, 8)(this); //Return the pointer to the head CClientClass.
	}
	/*
	bool WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand)
	{
		return Memory::CallVirtual< bool(__thiscall*)(CHLClient*, bf_write*, int, int, bool) >(this, 23)(this, buf, from, to, isnewcommand);
	}
	*/ // We do not need this. CL_SendMove Doesnt exist on tf.exe anymore.
};

class CGlobals
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
};

class CUserCmd
{
public:
	virtual ~CUserCmd() {}; //Destructor 0
	int command_number; //4
	int tick_count; //8
	Vector viewangles; //C
	float forwardmove; //18
	float sidemove; //1C
	float upmove; //20
	int	buttons; //24
	unsigned char impulse; //28
	int weaponselect; //2C
	int weaponsubtype; //30
	int random_seed; //34
	short mousedx; //38
	short mousedy; //3A
	bool hasbeenpredicted; //3C;
};

class CBaseCombatWeapon;
class CBaseEntity
{
public:
	Vector& GetAbsOrigin()
	{
		typedef Vector& (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 9)(this);
	}
	Vector& GetAbsAngles()
	{
		typedef Vector& (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 10)(this);
	}
	void GetWorldSpaceCenter(Vector& vWorldSpaceCenter)
	{
		Vector vMin, vMax;
		this->GetRenderBounds(vMin, vMax);
		vWorldSpaceCenter = this->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	}
	DWORD* GetModel()
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef DWORD* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(pRenderable, 9)(pRenderable);
	}
	bool SetupBones(matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef bool(__thiscall* OriginalFn)(PVOID, matrix3x4*, int, int, float);
		return Memory::CallVirtual<OriginalFn>(pRenderable, 16)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
	ClientClass* GetClientClass()
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(pNetworkable, 2)(pNetworkable);
	}
	bool IsDormant()
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(pNetworkable, 8)(pNetworkable);
	}
	int GetIndex()
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(pNetworkable, 9)(pNetworkable);
	}
	void GetRenderBounds(Vector& mins, Vector& maxs)
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef void(__thiscall* OriginalFn)(PVOID, Vector&, Vector&);
		Memory::CallVirtual<OriginalFn>(pRenderable, 20)(pRenderable, mins, maxs);
	}
	void SetCurrentCmd(CUserCmd* cmd)
	{
		DYNVAR_SET(CUserCmd*, (this - 0x4), cmd, "DT_BasePlayer", "localdata", "m_hConstraintEntity");
	}
	int GetHealth();
	int GetTeamNum();
	int& GetFlags();
	BYTE GetLifeState();
	int GetClassNum();
	const char* szGetClass();
	int GetCond();
	CBaseCombatWeapon* GetActiveWeapon();
	Vector GetCollideableMins();
	Vector GetCollideableMaxs();
	Vector GetEyePosition(); //yeah i know its actually view offset, but nobody will notice since you guys are just here to paste
	Vector GetAbsEyePosition(); //Without origin added, although you could just subtract origin, tbh idk why i have this
	//Vector GetVelocity();
	Vector GetEyeAngles();
	void SetEyeAngles(Vector eye_angle);
	bool IsAlive();
	void ForceTauntCam(bool t); // From Dumpster fire
	void SetTickBase(int tickbase);
	Vector m_vecOrigin();
	Vector m_vecVelocity();
	Vector m_vecBaseVelocity();
	Vector m_vecViewOffset();
	int m_hGroundEntity();
	int& GetTickBase();
	Vector GetHitboxPosition(int iHitbox);
	int GetHitboxSet();
	int GetNumOfHitboxes();
};
class CBaseCombatWeapon : public CBaseEntity
{
public:
	int GetMaxClip1()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 318)(this);
	}

	int GetMaxClip2()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 319)(this);
	}

	int GetSlot()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 330)(this);
	}

	char* GetName()
	{
		typedef char* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 329)(this);
	}

	char* GetPrintName()
	{
		typedef char* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 330)(this);
	}
	bool calc_is_attack_critical() {
		typedef bool(__thiscall* calc_is_attack_critical_fn)(CBaseCombatWeapon*);
		return Memory::CallVirtual< calc_is_attack_critical_fn >(this, 396)(this);
	}

	bool can_fire_random_critical_shot(float crit_chance) {
		typedef bool(__thiscall* can_fire_random_critical_shot_fn)(CBaseCombatWeapon*, float);
		return Memory::CallVirtual< can_fire_random_critical_shot_fn >(this, 426)(this, crit_chance);
	}
	/* int GetWeaponID() {
		return reinterpret_cast<int(__thiscall *)(void *)>(Memory::GetVFunc(this, 381))(this);
	}*/
	int GetWeaponID()
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 381)(this);
	}
	int GetItemDefinitionIndex()
	{
		DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}
	/* "DT_BaseCombatWeapon", "m_iClip1" */
	int m_iClip1()
	{
		DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "m_iClip1");
	}
	NETVAR(Clip1, int, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip1")
		/* M_DYNVARGET(Clip1, int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip1")*/
		NETVAR(Clip2, int, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip2")
		/* 	M_DYNVARGET(ChargeBeginTime, float, this, _("DT_WeaponPipebombLauncher"), _("PipebombLauncherLocalData"), _("m_flChargeBeginTime")) */
		float GetChargeBeginTime()
	{
		DYNVAR_RETURN(float, this, "DT_WeaponPipebombLauncher", "PipebombLauncherLocalData", "m_flChargeBeginTime");
	}
	bool is_reloading()
	{
		// "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack"
		static auto next_primary_attack_offset = gNetVars.get_offset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
		bool in_reload = *reinterpret_cast<bool*>(this + (next_primary_attack_offset + 0xc));
		int reload_mode = *reinterpret_cast<int*>(this + 0xb28);
		return (in_reload || reload_mode != 0);
	}
	float m_next_primary_attack()
	{
		DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
	}
};
class EngineClient
{
public:
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int&, int&);
		return Memory::CallVirtual<OriginalFn>(this, 5)(this, width, height);
	}
	bool GetPlayerInfo(int ent_num, player_info_t* pinfo)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, int, player_info_t*);
		return Memory::CallVirtual<OriginalFn>(this, 8)(this, ent_num, pinfo);
	}
	bool Con_IsVisible(void)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 11)(this);
	}
	int GetLocalPlayer(void)
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 12)(this);
	}
	float Time(void)
	{
		typedef float(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 14)(this);
	}
	void GetViewAngles(Vector& va)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, Vector& va);
		return Memory::CallVirtual<OriginalFn>(this, 19)(this, va);
	}
	void SetViewAngles(Vector& va)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, Vector& va);
		return Memory::CallVirtual<OriginalFn>(this, 20)(this, va);
	}
	int GetMaxClients(void)
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 21)(this);
	}
	bool IsInGame(void)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 26)(this);
	}
	bool IsConnected(void)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 27)(this);
	}
	bool IsDrawingLoadingImage(void)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 28)(this);
	}
	const matrix3x4& WorldToScreenMatrix(void)
	{
		typedef const matrix3x4& (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 36)(this);
	}
	bool IsTakingScreenshot(void)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 85)(this);
	}
	DWORD* GetNetChannelInfo(void)
	{
		typedef DWORD* (__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 72)(this);
	}
	void ClientCmd_Unrestricted(const char* chCommandString)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const char*);
		return Memory::CallVirtual<OriginalFn>(this, 106)(this, chCommandString);
	}
};

class IPanel
{
public:
	const char* GetName(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall* OriginalFn)(PVOID, unsigned int);
		return Memory::CallVirtual<OriginalFn>(this, 36)(this, vguiPanel);
	}
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Memory::CallVirtual<OriginalFn>(this, 11)(this, r, g, b, a);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Memory::CallVirtual<OriginalFn>(this, 12)(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Memory::CallVirtual<OriginalFn>(this, 14)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		Memory::CallVirtual<OriginalFn>(this, 17)(this, font);
	}
	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		Memory::CallVirtual<OriginalFn>(this, 19)(this, r, g, b, a);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		Memory::CallVirtual<OriginalFn>(this, 20)(this, x, y);
	}
	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
		return Memory::CallVirtual<OriginalFn>(this, 22)(this, text, textLen, 0);
	}
	unsigned long CreateFont()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 66)(this);
	}
	void SetFontGlyphSet(unsigned long& font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		Memory::CallVirtual<OriginalFn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t*, int&, int&);
		Memory::CallVirtual<OriginalFn>(this, 75)(this, font, text, wide, tall);
	}
	void StartDrawing()
	{
		typedef void(__thiscall* start_drawing_fn)(ISurface*);
		static auto start_drawing_ = Memory::find_signature< start_drawing_fn >("vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14");
		return start_drawing_(this);
	}

	void FinishDrawing()
	{
		typedef void(__thiscall* finish_drawing_fn)(ISurface*);
		static auto finish_drawing_ = Memory::find_signature< finish_drawing_fn >("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00");
		return finish_drawing_(this);
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* DrawLineFn)(void*, int, int, int, int);
		Memory::CallVirtual<DrawLineFn>(this, 15)(this, x0, y0, x1, y1);
	}
};

class CEntList
{
public:
	CBaseEntity* GetClientEntity(int entnum)
	{
		typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
		return Memory::CallVirtual<OriginalFn>(this, 3)(this, entnum);
	}
	CBaseEntity* GetClientEntityFromHandle(int hEnt)
	{
		typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
		return Memory::CallVirtual<OriginalFn>(this, 4)(this, hEnt);
	}
	int GetHighestEntityIndex(void)
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return Memory::CallVirtual<OriginalFn>(this, 6)(this);
	}
};

enum playercontrols
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),	// Used by client.dll for when scoreboard is held down
	IN_SPEED = (1 << 17),	// Player is holding the speed key
	IN_WALK = (1 << 18),	// Player holding walk key
	IN_ZOOM = (1 << 19),	// Zoom key for HUD zoom
	IN_WEAPON1 = (1 << 20),	// weapon defines these bits
	IN_WEAPON2 = (1 << 21),	// weapon defines these bits
	IN_BULLRUSH = (1 << 22),
};

enum tf_cond
{
	TFCond_Slowed = (1 << 0), //Toggled when a player is slowed down. 
	TFCond_Zoomed = (1 << 1), //Toggled when a player is zoomed. 
	TFCond_Disguising = (1 << 2), //Toggled when a Spy is disguising.  
	TFCond_Disguised = (1 << 3), //Toggled when a Spy is disguised. 
	TFCond_Cloaked = (1 << 4), //Toggled when a Spy is invisible. 
	TFCond_Ubercharged = (1 << 5), //Toggled when a player is �berCharged. 
	TFCond_TeleportedGlow = (1 << 6), //Toggled when someone leaves a teleporter and has glow beneath their feet. 
	TFCond_Taunting = (1 << 7), //Toggled when a player is taunting. 
	TFCond_UberchargeFading = (1 << 8), //Toggled when the �berCharge is fading. 
	TFCond_CloakFlicker = (1 << 9), //Toggled when a Spy is visible during cloak. 
	TFCond_Teleporting = (1 << 10), //Only activates for a brief second when the player is being teleported; not very useful. 
	TFCond_Kritzkrieged = (1 << 11), //Toggled when a player is being crit buffed by the KritzKrieg. 
	TFCond_TmpDamageBonus = (1 << 12), //Unknown what this is for. Name taken from the AlliedModders SDK. 
	TFCond_DeadRingered = (1 << 13), //Toggled when a player is taking reduced damage from the Deadringer. 
	TFCond_Bonked = (1 << 14), //Toggled when a player is under the effects of The Bonk! Atomic Punch. 
	TFCond_Stunned = (1 << 15), //Toggled when a player's speed is reduced from airblast or a Sandman ball. 
	TFCond_Buffed = (1 << 16), //Toggled when a player is within range of an activated Buff Banner. 
	TFCond_Charging = (1 << 17), //Toggled when a Demoman charges with the shield. 
	TFCond_DemoBuff = (1 << 18), //Toggled when a Demoman has heads from the Eyelander. 
	TFCond_CritCola = (1 << 19), //Toggled when the player is under the effect of The Crit-a-Cola. 
	TFCond_InHealRadius = (1 << 20), //Unused condition, name taken from AlliedModders SDK. 
	TFCond_Healing = (1 << 21), //Toggled when someone is being healed by a medic or a dispenser. 
	TFCond_OnFire = (1 << 22), //Toggled when a player is on fire. 
	TFCond_Overhealed = (1 << 23), //Toggled when a player has >100% health. 
	TFCond_Jarated = (1 << 24), //Toggled when a player is hit with a Sniper's Jarate. 
	TFCond_Bleeding = (1 << 25), //Toggled when a player is taking bleeding damage. 
	TFCond_DefenseBuffed = (1 << 26), //Toggled when a player is within range of an activated Battalion's Backup. 
	TFCond_Milked = (1 << 27), //Player was hit with a jar of Mad Milk. 
	TFCond_MegaHeal = (1 << 28), //Player is under the effect of Quick-Fix charge. 
	TFCond_RegenBuffed = (1 << 29), //Toggled when a player is within a Concheror's range. 
	TFCond_MarkedForDeath = (1 << 30), //Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated. 
	TFCond_NoHealingDamageBuff = (1 << 31), //Unknown what this is used for.

	TFCondEx_SpeedBuffAlly = (1 << 0), //Toggled when a player gets hit with the disciplinary action. 
	TFCondEx_HalloweenCritCandy = (1 << 1), //Only for Scream Fortress event maps that drop crit candy. 
	TFCondEx_CritCanteen = (1 << 2), //Player is getting a crit boost from a MVM canteen.
	TFCondEx_CritDemoCharge = (1 << 3), //From demo's shield
	TFCondEx_CritHype = (1 << 4), //Soda Popper crits. 
	TFCondEx_CritOnFirstBlood = (1 << 5), //Arena first blood crit buff. 
	TFCondEx_CritOnWin = (1 << 6), //End of round crits. 
	TFCondEx_CritOnFlagCapture = (1 << 7), //CTF intelligence capture crits. 
	TFCondEx_CritOnKill = (1 << 8), //Unknown what this is for. 
	TFCondEx_RestrictToMelee = (1 << 9), //Unknown what this is for. 
	TFCondEx_DefenseBuffNoCritBlock = (1 << 10), //MvM Buff.
	TFCondEx_Reprogrammed = (1 << 11), //MvM Bot has been reprogrammed.
	TFCondEx_PyroCrits = (1 << 12), //Player is getting crits from the Mmmph charge. 
	TFCondEx_PyroHeal = (1 << 13), //Player is being healed from the Mmmph charge. 
	TFCondEx_FocusBuff = (1 << 14), //Player is getting a focus buff.
	TFCondEx_DisguisedRemoved = (1 << 15), //Disguised remove from a bot.
	TFCondEx_MarkedForDeathSilent = (1 << 16), //Player is under the effects of the Escape Plan/Equalizer or GRU.
	TFCondEx_DisguisedAsDispenser = (1 << 17), //Bot is disguised as dispenser.
	TFCondEx_Sapped = (1 << 18), //MvM bot is being sapped.
	TFCondEx_UberchargedHidden = (1 << 19), //MvM Related
	TFCondEx_UberchargedCanteen = (1 << 20), //Player is receiving �berCharge from a canteen.
	TFCondEx_HalloweenBombHead = (1 << 21), //Player has a bomb on their head from Merasmus.
	TFCondEx_HalloweenThriller = (1 << 22), //Players are forced to dance from Merasmus.
	TFCondEx_BulletCharge = (1 << 26), //Player is receiving 75% reduced damage from bullets.
	TFCondEx_ExplosiveCharge = (1 << 27), //Player is receiving 75% reduced damage from explosives.
	TFCondEx_FireCharge = (1 << 28), //Player is receiving 75% reduced damage from fire.
	TFCondEx_BulletResistance = (1 << 29), //Player is receiving 10% reduced damage from bullets.
	TFCondEx_ExplosiveResistance = (1 << 30), //Player is receiving 10% reduced damage from explosives.
	TFCondEx_FireResistance = (1 << 31), //Player is receiving 10% reduced damage from fire.

	TFCondEx2_Stealthed = (1 << 0),
	TFCondEx2_MedigunDebuff = (1 << 1),
	TFCondEx2_StealthedUserBuffFade = (1 << 2),
	TFCondEx2_BulletImmune = (1 << 3),
	TFCondEx2_BlastImmune = (1 << 4),
	TFCondEx2_FireImmune = (1 << 5),
	TFCondEx2_PreventDeath = (1 << 6),
	TFCondEx2_MVMBotRadiowave = (1 << 7),
	TFCondEx2_HalloweenSpeedBoost = (1 << 8), //Wheel has granted player speed boost.
	TFCondEx2_HalloweenQuickHeal = (1 << 9), //Wheel has granted player quick heal.
	TFCondEx2_HalloweenGiant = (1 << 10), //Wheel has granted player giant mode.
	TFCondEx2_HalloweenTiny = (1 << 11), //Wheel has granted player tiny mode.
	TFCondEx2_HalloweenInHell = (1 << 12), //Wheel has granted player in hell mode.
	TFCondEx2_HalloweenGhostMode = (1 << 13), //Wheel has granted player ghost mode.
	TFCondEx2_Parachute = (1 << 16), //Player has deployed the BASE Jumper.
	TFCondEx2_BlastJumping = (1 << 17), //Player has sticky or rocket jumped.

	TFCond_MiniCrits = (TFCond_Buffed | TFCond_CritCola),
	TFCond_IgnoreStates = (TFCond_Ubercharged | TFCond_Bonked),
	TFCondEx_IgnoreStates = (TFCondEx_PyroHeal)
};

enum tf_classes
{
	TF2_Scout = 1,
	TF2_Soldier = 3,
	TF2_Pyro = 7,
	TF2_Demoman = 4,
	TF2_Heavy = 6,
	TF2_Engineer = 9,
	TF2_Medic = 5,
	TF2_Sniper = 2,
	TF2_Spy = 8,
};

enum source_lifestates
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

class ClientModeShared
{
public:
	bool IsChatPanelOutOfFocus(void)
	{
		typedef PVOID(__thiscall* OriginalFn)(PVOID);
		PVOID CHudChat = Memory::CallVirtual<OriginalFn>(this, 19)(this);
		if (CHudChat)
		{
			return *(PFLOAT)((DWORD)CHudChat + 0xFC) == 0;
		}
		return false;
	}
};

class IPrediction
{
public:
public:
	virtual		 ~IPrediction(void) {};
	virtual void Init(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void Update
	(
		int startframe,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	) = 0;

	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void PostEntityPacketReceived(void) = 0;
	virtual void PostNetworkDataReceived(int commands_acknowledged) = 0;
	virtual void OnReceivedUncompressedPacket(void) = 0;
	virtual void GetViewOrigin(Vector& org) = 0;
	virtual void SetViewOrigin(Vector& org) = 0;
	virtual void GetViewAngles(Vector& ang) = 0;
	virtual void SetViewAngles(Vector& ang) = 0;
	virtual void GetLocalViewAngles(Vector& ang) = 0;
	virtual void SetLocalViewAngles(Vector& ang) = 0;
};
//using entity_handle_t = c_base_handle;
#include "CBaseHandle.h"
typedef CBaseHandle EntityHandle_t;
class CMoveData
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;

	EntityHandle_t m_nPlayerHandle; // edict index on server, client entity handle on client

	int    m_nImpulseCommand; // Impulse command issued.
	Vector m_vecViewAngles; // Command view angles (local space)
	Vector m_vecAbsViewAngles; // Command view angles (world space)
	int    m_nButtons; // Attack buttons.
	int    m_nOldButtons; // From host_client->oldbuttons;
	float  m_flForwardMove;
	float  m_flSideMove;
	float  m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector m_vecVelocity; // edict::velocity		// Current movement direction.
	Vector m_vecAngles; // edict::angles
	Vector m_vecOldAngles;

	// Output only
	float  m_outStepHeight; // how much you climbed this move
	Vector m_outWishVel; // This is where you tried
	Vector m_outJumpVel; // This is your jump velocity

					   // Movement constraints	(radius 0 means no constraint)
	Vector m_vecConstraintCenter;
	float  m_flConstraintRadius;
	float  m_flConstraintWidth;
	float  m_flConstraintSpeedFactor;

	void        SetAbsOrigin(const Vector& vec);
	const Vector& GetAbsOrigin() const;

	// private:
	Vector m_vecAbsOrigin; // edict::origin
};


class CMoveHelper
{
public:
	void SetHost(CBaseEntity* host)
	{
		typedef void(__thiscall* FN)(PVOID, CBaseEntity*);
		Memory::CallVirtual<FN>(this, 0)(this, host);
	}
};

class C_Prediction : public IPrediction
{
public:
	virtual		 ~C_Prediction();
	virtual void Init();
	virtual void Shutdown();
	virtual void Update
	(
		int startframe,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	);

	virtual void OnReceivedUncompressedPacket();
	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet);
	virtual void PostEntityPacketReceived();
	virtual void PostNetworkDataReceived(int commands_acknowledged);
	virtual bool InPrediction();
	virtual bool IsFirstTimePredicted();
	virtual int	 GetIncomingPacketNumber();
	virtual void GetViewOrigin(Vector& org);
	virtual void SetViewOrigin(Vector& org);
	virtual void GetViewAngles(Vector& ang);
	virtual void SetViewAngles(Vector& ang);
	virtual void GetLocalViewAngles(Vector& ang);
	virtual void SetLocalViewAngles(Vector& ang);
	virtual void RunCommand(CBaseEntity* player, CUserCmd* ucmd, CMoveHelper* moveHelper);
	virtual void SetupMove(CBaseEntity* player, CUserCmd* ucmd, CMoveHelper* pHelper, CMoveData* move);
	virtual void FinishMove(CBaseEntity* player, CUserCmd* ucmd, CMoveData* move);
	virtual void SetIdealPitch(CBaseEntity* player, const Vector& origin, const Vector& angles, const Vector& viewheight);
	virtual void _Update
	(
		bool received_new_world_update,
		bool validframe,
		int incoming_acknowledged,
		int outgoing_command
	);

	//CHandle< C_BaseEntity > m_hLastGround;
	uint32_t m_hLastGround;
	bool	 m_bInPrediction;
	bool	 m_bFirstTimePredicted;
	bool	 m_bOldCLPredictValue;
	bool	 m_bEnginePaused;
	int		 m_nPreviousStartFrame;
	int		 m_nCommandsPredicted;
	int		 m_nServerCommandsAcknowledged;
	int		 m_bPreviousAckHadErrors;
	int		 m_nIncomingPacketNumber;
	float	 m_flIdealPitch;
};


class C_GameMovement
{
public:
	virtual			~C_GameMovement(void) {}

	// Process the current movement command
	virtual void	ProcessMovement(CBaseEntity* pPlayer, CMoveData* pMove) = 0;
	virtual void	StartTrackPredictionErrors(CBaseEntity* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(CBaseEntity* pPlayer) = 0;
	virtual void	DiffPrint(char const* fmt, ...) = 0;

	// Allows other parts of the engine to find out the normal and ducked player bbox sizes
	virtual Vector GetPlayerMins(bool ducked) const = 0;
	virtual Vector GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector GetPlayerViewOffset(bool ducked) const = 0;
};

enum EVGuiPanel
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS
};
class IEngineVGUI
{
public:
	virtual					~IEngineVGUI(void) { }
	virtual unsigned int	GetPanel(EVGuiPanel type) = 0;
	virtual bool			IsGameUIVisible() = 0;
};

class __declspec(align(16))VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};

	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

	explicit VectorAligned(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};
struct Ray_t
{
	VectorAligned   m_Start;
	VectorAligned   m_Delta;
	VectorAligned   m_StartOffset;
	VectorAligned   m_Extents;

	bool    m_IsRay;
	bool    m_IsSwept;

	void Init(Vector& start, Vector& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();
		m_IsRay = true;

		m_StartOffset.Init();
		m_Start = start;
	}

	void Init(Vector& start, Vector& end, Vector& mins, Vector& maxs)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		m_StartOffset = mins + maxs;
		m_StartOffset *= 0.5f;
		m_Start = start - m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};


struct cplane_t
{
	Vector normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[2];
};

struct csurface_t
{
	const char* name;
	short			surfaceProps;
	unsigned short	flags;
};

enum SurfaceFlags_t
{
	DISPSURF_FLAG_SURFACE = (1 << 0),
	DISPSURF_FLAG_WALKABLE = (1 << 1),
	DISPSURF_FLAG_BUILDABLE = (1 << 2),
	DISPSURF_FLAG_SURFPROP1 = (1 << 3),
	DISPSURF_FLAG_SURFPROP2 = (1 << 4),
};


class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}

private:
	CBaseTrace(const CBaseTrace& vOther);
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const
	{
		return fraction < 1 || allsolid || startsolid;
	}

public:
	float			fractionleftsolid;
	csurface_t		surface;

	int				hitgroup;

	short			physicsbone;

	CBaseEntity* m_pEnt;
	int				hitbox;

	CGameTrace() {}
	CGameTrace(const CGameTrace& vOther);
};
typedef CGameTrace trace_t;

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};
class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		CBaseEntity* pEntity = (CBaseEntity*)pEntityHandle;

		switch (pEntity->GetClientClass()->iClassID)
		{
		case 55: // Portal Window
		case 64: // Spawn Door visualizers
		case 117: // Sniper Dots
		case 225: // Medigun Shield
			return false;
			break;
		}

		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

class IEngineTrace
{
public:	  //We really only need this I guess...
	void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)//5
	{
		typedef void(__thiscall* TraceRayFn)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		return Memory::CallVirtual<TraceRayFn>(this, 4)(this, ray, fMask, pTraceFilter, pTrace);
	}
};
#include "studio.h"
class IVModelInfo
{
public:
	DWORD* GetModel(int index)
	{
		typedef DWORD* (__thiscall* GetModelFn)(void*, int);
		return Memory::CallVirtual<GetModelFn>(this, 1)(this, index);
	}

	int	GetModelIndex(const char* name)
	{
		typedef int(__thiscall* GetModelIndexFn)(void*, const char*);
		return Memory::CallVirtual< GetModelIndexFn >(this, 2)(this, name);
	}

	const char* GetModelName(const DWORD* model)
	{
		typedef const char* (__thiscall* GetModelNameFn)(void*, const DWORD*);
		return Memory::CallVirtual< GetModelNameFn >(this, 3)(this, model);
	}

	studiohdr_t* GetStudiomodel(const DWORD* mod)
	{
		typedef studiohdr_t* (__thiscall* GetStudiomodelFn)(void*, const DWORD*);
		return Memory::CallVirtual< GetStudiomodelFn >(this, 28)(this, mod);
	}
};

class CInterfaces
{
public:
	CEntList* EntList;
	EngineClient* Engine;
	IPanel* Panels;
	ISurface* Surface;
	ClientModeShared* ClientMode;
	CHLClient* Client;
	CGlobals* Globals;
	CMoveHelper* MoveHelper = nullptr;
	C_Prediction* Prediction = nullptr;
	C_GameMovement* GameMovement = nullptr;
	int* RandomSeed = nullptr;
	IEngineVGUI* EngineVGUI = nullptr;
	IVModelInfo* ModelInfo = nullptr;
	IEngineTrace* EngineTrace = nullptr;
};

inline CInterfaces gInts;