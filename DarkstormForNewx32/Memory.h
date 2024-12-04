#pragma once
#include <Windows.h>
#include <vector>
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

namespace Memory
{
	template <typename T>
	inline void GetInterface(T& Interface, const char* szModuleName, const char* szInterfaceName)
	{
		if (HMODULE Module = GetModuleHandleA(szModuleName))
		{
			if (CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(Module, "CreateInterface"))
				Interface = reinterpret_cast<T>(CreateInterface(szInterfaceName, NULL));
			else
				MessageBoxA(NULL, szInterfaceName, "Failed to find interface!", NULL);

			return;
		}

		MessageBoxA(NULL, szInterfaceName, "Failed to find find module!", NULL);
	}

	inline void* GetVirtual(void* iClass, int iIndex)
	{
		return (void*)((DWORD)(*(PVOID**)iClass)[iIndex]);
	}

	template< typename Fn >
	inline Fn CallVirtual(void* iClass, size_t iIndex)
	{
		return reinterpret_cast<Fn>(GetVirtual(iClass, iIndex));
	}
	template < typename T, const std::uint16_t offset = 0u >
	inline T find_signature(const char* module_name, const char* signature) {
		if (const auto module_handle = GetModuleHandleA(module_name)) {
			static auto signature_to_byte = [](const char* signature) {
				auto bytes = std::vector< int >{ };
				auto start = const_cast<char*>(signature);
				auto end = const_cast<char*>(signature) + strlen(signature);

				for (auto current = start; current < end; ++current) {
					if (*current == '?') {
						++current;

						if (*current == '?')
							++current;

						bytes.push_back(-1);
					}

					else bytes.push_back(strtoul(current, &current, 16));
				}

				return bytes;
			};

			auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
			auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

			auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
			auto pattern_bytes = signature_to_byte(signature);
			auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

			auto s = pattern_bytes.size();
			auto d = pattern_bytes.data();

			for (auto i = 0ul; i < size_of_image - s; ++i) {
				bool found = true;

				for (auto j = 0ul; j < s; ++j) {
					if (scan_bytes[i + j] != d[j] && d[j] != -1) {
						found = false;
						break;
					}
				}

				if (found)
					return reinterpret_cast<T>(&scan_bytes[i] + offset);
			}
		}

		return 0;
	}

	
}