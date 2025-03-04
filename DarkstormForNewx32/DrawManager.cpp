#include "DrawManager.h"
#pragma warning(disable : 4244) // SHUSH NIGGER LOL
#define ESP_HEIGHT 14
//===================================================================================
void CDrawManager::Initialize()
{
	if (gInts.Surface == NULL)
		return;

	gInts.Engine->GetScreenSize(gScreenSize.Width, gScreenSize.Height);
	m_Font = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(m_Font, "Arial", ESP_HEIGHT, 800, 0, 0, 0x200);
}
//===================================================================================
void CDrawManager::DrawString(int x, int y, DWORD dwColor, const wchar_t* pszText)
{
	if (pszText == NULL)
		return;

	gInts.Surface->DrawSetTextPos(x, y);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawPrintText(pszText, wcslen(pszText));
}

void CDrawManager::DrawStringAligned(int x, int y, DWORD dwColor, const short align, const wchar_t* pszText)
{
	if (pszText == NULL)
		return;

	if (align)
	{
		int w = 0, h = 0;
		gInts.Surface->GetTextSize(m_Font, pszText, w, h);

		if (align & TXT_LEFT)
			x -= w;

		if (align & TXT_TOP)
			y -= h;

		if (align & TXT_CENTERX)
			x -= (w / 2);

		if (align & TXT_CENTERY)
			y -= (h / 2);
	}

	gInts.Surface->DrawSetTextPos(x, y);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawPrintText(pszText, wcslen(pszText));
}
void CDrawManager::DrawStringAligned(int x, int y, DWORD dwColor, const short align, const char* pszText, ...)
{
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);

	if (align)
	{
		int w = 0, h = 0;
		gInts.Surface->GetTextSize(m_Font, szString, w, h);

		if (align & TXT_LEFT)
			x -= w;

		if (align & TXT_TOP)
			y -= h;

		if (align & TXT_CENTERX)
			x -= (w / 2);

		if (align & TXT_CENTERY)
			y -= (h / 2);
	}

	gInts.Surface->DrawSetTextPos(x, y);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawPrintText(szString, wcslen(szString));
}
//===================================================================================
void CDrawManager::DrawString(int x, int y, DWORD dwColor, const char* pszText, ...)
{
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);


	gInts.Surface->DrawSetTextPos(x, y);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawPrintText(szString, wcslen(szString));
}
//===================================================================================
int CDrawManager::GetESPHeight()
{
	return ESP_HEIGHT;
}
//===================================================================================
void CDrawManager::DrawRect(int x, int y, int w, int h, DWORD dwColor)
{
	gInts.Surface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawFilledRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::OutlineRect(int x, int y, int w, int h, DWORD dwColor)
{
	gInts.Surface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawOutlinedRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::DrawBox(Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius)
{
	Vector vScreen;

	if (!WorldToScreen(vOrigin, vScreen))
		return;

	int radius2 = radius << 1;

	OutlineRect(vScreen.x - radius + box_width, vScreen.y - radius + box_width, radius2 - box_width, radius2 - box_width, 0x000000FF);
	OutlineRect(vScreen.x - radius - 1, vScreen.y - radius - 1, radius2 + (box_width + 2), radius2 + (box_width + 2), 0x000000FF);
	DrawRect(vScreen.x - radius + box_width, vScreen.y - radius, radius2 - box_width, box_width, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x - radius, vScreen.y + radius, radius2, box_width, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x - radius, vScreen.y - radius, box_width, radius2, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x + radius, vScreen.y - radius, box_width, radius2 + box_width, COLORCODE(r, g, b, alpha));
}
//===================================================================================
bool CDrawManager::WorldToScreen(Vector& vOrigin, Vector& vScreen)
{
	const matrix3x4& worldToScreen = gInts.Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	vScreen.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		vScreen.x = (gScreenSize.Width / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.Width + 0.5); //Get the X dimension and push it in to the Vector.
		vScreen.y = (gScreenSize.Height / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.Height + 0.5); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}
void CDrawManager::DrawLine(int x, int y, int x1, int y1, uint32_t Color)
{
	gInts.Surface->DrawSetColor(RED(Color), GREEN(Color), BLUE(Color), ALPHA(Color));
	gInts.Surface->DrawLine(x, y, x1, y1);
}
