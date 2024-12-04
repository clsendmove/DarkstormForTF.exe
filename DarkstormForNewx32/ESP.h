#pragma once
#include "SDK.h"

class CESP
{
public:
	void Run();
private:
	void ESP(CBaseEntity* pEntity);
};

inline CESP gESP;