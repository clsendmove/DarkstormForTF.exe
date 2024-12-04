#include "Hooked_EngineVGUIPaint.h"
#include "DrawManager.h"
#include "ESP.h"
enum EPaintMode
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
	PAINT_CURSOR = (1 << 2)
};

void __stdcall Hooked_EngineVGUIPaint(int mode)
{
	oPaint(gInts.EngineVGUI, mode);
	if (mode & PAINT_UIPANELS)
	{
		gInts.Surface->StartDrawing();
		{
			int iWidth, iHeight; //Resolution fix, so this can work in Fullscreen
			gInts.Engine->GetScreenSize(iWidth, iHeight);
			if (gScreenSize.Width != iWidth || gScreenSize.Height != iHeight)
				gInts.Engine->GetScreenSize(gScreenSize.Width, gScreenSize.Height);

			gDraw.DrawString(5, 5, COLORCODE(255,255,255,255), "Darkstorm For TF.exe(x32)");
			gESP.Run();
		}
		gInts.Surface->FinishDrawing();
	}
}