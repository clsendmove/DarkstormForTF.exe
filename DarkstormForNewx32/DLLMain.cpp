#include "SDK.h"
#include <Windows.h>
#include <thread>
#include "SigScanner.h"
#include "CNetVars.h"
#include "DrawManager.h"
#include "MinHook/MinHook.h"
#include "CreateMove.h"
#include "RunCommand.h"
#include "Hooked_EngineVGUIPaint.h"
DWORD WINAPI dwMainThread(LPVOID lpArguments)
{
	Memory::GetInterface<EngineClient*>(gInts.Engine, "engine.dll", "VEngineClient013");
	Memory::GetInterface<CHLClient*>(gInts.Client, "client.dll", "VClient017");
	Memory::GetInterface<IPanel*>(gInts.Panels, "vgui2.dll", "VGUI_Panel009");
	Memory::GetInterface<ISurface*>(gInts.Surface, "vguimatsurface.dll", "VGUI_Surface030");
	Memory::GetInterface<CEntList*>(gInts.EntList, "client.dll", "VClientEntityList003");
	Memory::GetInterface<C_Prediction*>(gInts.Prediction, "client.dll", "VClientPrediction001");
	Memory::GetInterface<C_GameMovement*>(gInts.GameMovement, "client.dll", "GameMovement001");
	Memory::GetInterface<IEngineVGUI*>(gInts.EngineVGUI, "engine.dll", "VEngineVGui002");
	Memory::GetInterface<IEngineTrace*>(gInts.EngineTrace, "engine.dll", "EngineTraceClient003");
	Memory::GetInterface<IVModelInfo*>(gInts.ModelInfo, "engine.dll", "VModelInfoClient006");

	const PDWORD pdwClient = reinterpret_cast<PDWORD>(gInts.Client);
	const PDWORD pdwTable = *reinterpret_cast<PDWORD*>(pdwClient);

	gInts.Globals = *reinterpret_cast<CGlobals**>(gSignature.GetEngineSignature("68 ? ? ? ? 50 50 FF 12") + 0x1);
	gInts.RandomSeed = *reinterpret_cast<int32_t**>(gSignature.GetClientSignature("C7 05 ? ? ? ? ? ? ? ? 5D C3 8B 40 34") + 0x02);

	if (pdwClient && pdwTable)
	{
		gInts.ClientMode = **reinterpret_cast<ClientModeShared***>(static_cast<DWORD>(pdwTable[0xA]) + 0x05); // 0xA = 10, + 0x05
	}
	gNetVars.Initialize();
	gDraw.Initialize();

	MH_Initialize();
	{
		MH_CreateHook(Memory::GetVirtual(gInts.ClientMode, 21), &Hooked_CreateMove, reinterpret_cast<void**>(&oCreateMove));
		MH_CreateHook(Memory::GetVirtual(gInts.Prediction, 17), &Hooked_RunCommand, reinterpret_cast<void**>(&oRunCommand));
		MH_CreateHook(Memory::GetVirtual(gInts.EngineVGUI, 14), &Hooked_EngineVGUIPaint, reinterpret_cast<void**>(&oPaint));
	}
	MH_EnableHook(MH_ALL_HOOKS);

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpArguments), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		if (const HANDLE hMain = CreateThread(0, 0, dwMainThread, hInstance, 0, 0))
			CloseHandle(hMain);

	return true;
}