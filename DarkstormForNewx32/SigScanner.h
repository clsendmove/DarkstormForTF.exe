#pragma once
#include "SDK.h"
class CSignature
{
public:
	DWORD dwFindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern);
	HMODULE GetModuleHandleSafe(LPCWSTR pszModuleName);
	DWORD GetClientSignature(const char* chPattern);
	DWORD GetEngineSignature(const char* chPattern);
	DWORD GetSignature(LPCWSTR dll, const char* chPattern);
};

inline CSignature gSignature;