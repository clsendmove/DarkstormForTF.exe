#pragma once
#include "SDK.h"

typedef void(__thiscall* fn)(IEngineVGUI*, int);
inline fn oPaint;
void __stdcall Hooked_EngineVGUIPaint(int mode);