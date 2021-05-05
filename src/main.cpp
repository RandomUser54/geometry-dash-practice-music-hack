#include <Windows.h>
#include <psapi.h>
HMODULE mods[500] = {};
BYTE buff[6] = { 0x90,0x90,0x90,0x90,0x90,0x90 };
BYTE buff1[2] = { 0x90,0x90 };
DWORD pid;
bool whyamistilldoingthis = false;
bool exists = false;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
	bool idkanymore = false;
	char str[20];
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hand = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	DWORD len = GetModuleBaseNameA(hand, NULL, str, sizeof(str));
	char gd[] = { 0x47, 0x65, 0x6f, 0x6d, 0x65, 0x74, 0x72, 0x79, 0x44, 0x61, 0x73, 0x68, 0x2e, 0x65, 0x78, 0x65 };
	for (int x = 0; x < 16; x++) {
		if (str[x] != gd[x]) {
			idkanymore = true;
                        CloseHandle(hand);
			return TRUE;
		}
	}
	if (idkanymore == false) {
		whyamistilldoingthis = true;
                CloseHandle(hand);
		return FALSE;
		
	}
}
int __stdcall WinMain(HINSTANCE hinst, HINSTANCE previns, LPSTR cmdline, int showcmd) {
	while(true){
		while (!whyamistilldoingthis) {
			EnumWindows(EnumWindowsProc, NULL);
			if (!whyamistilldoingthis) {
				exists = false;
			}
			Sleep(500);
		}
		if (!exists) {
			HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
			DWORD cbneeded;
			EnumProcessModules(proc, mods, sizeof(mods), &cbneeded);
			char name[50];
			DWORD prev;
			DWORD prev1;
			DWORD prev2;
			DWORD prev3;

			byte* ptr = (byte*)mods[0] + 0x20C925;
			byte* ptr1 = (byte*)mods[0] + 0x20D143;
			byte* ptr2 = (byte*)mods[0] + 0x20A563;
			byte* ptr3 = (byte*)mods[0] + 0x20A595;

			VirtualProtectEx(proc, ptr, sizeof(buff), PAGE_EXECUTE_READWRITE, &prev);
			VirtualProtectEx(proc, ptr1, sizeof(buff1), PAGE_EXECUTE_READWRITE, &prev1);
			VirtualProtectEx(proc, ptr2, sizeof(buff1), PAGE_EXECUTE_READWRITE, &prev2);
			VirtualProtectEx(proc, ptr3, sizeof(buff1), PAGE_EXECUTE_READWRITE, &prev3);

			WriteProcessMemory(proc, ptr, &buff, sizeof(buff), NULL);
			WriteProcessMemory(proc, ptr1, &buff1, sizeof(buff1), NULL);
			WriteProcessMemory(proc, ptr2, &buff1, sizeof(buff1), NULL);
			WriteProcessMemory(proc, ptr3, &buff1, sizeof(buff1), NULL);

			VirtualProtectEx(proc, ptr, sizeof(buff), prev, &prev);
			VirtualProtectEx(proc, ptr1, sizeof(buff1), prev1, &prev1);
			VirtualProtectEx(proc, ptr2, sizeof(buff1), prev2, &prev2);
			VirtualProtectEx(proc, ptr3, sizeof(buff1), prev3, &prev3);
			
                        CloseHandle(proc)
			exists = true;
		}
		whyamistilldoingthis = false;
	}
	return 0;
}
