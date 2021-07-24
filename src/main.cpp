#include <Windows.h>
#include <psapi.h>
HMODULE gmd;
BYTE buff[] = { 0x90,0x90,0x90,0x90,0x90,0x90 };
BYTE buff1[] = { 0x90,0x90 };
DWORD pid;
bool found = false;
bool exists = false;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
	char gd[] = "GeometryDash.exe";
	char str[sizeof(gd)];
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hand = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	GetModuleBaseNameA(hand, NULL, str, sizeof(str));
	for (int x = 0; x < (sizeof(gd) - 1); x++) {
		if (str[x] != gd[x]) {
			CloseHandle(hand);
			return TRUE;
		}
	}
	found = true;
	CloseHandle(hand);
	return FALSE;
}
int WinMain(HINSTANCE hinst, HINSTANCE previns, LPSTR cmdline, int showcmd) {
	while (true) {
		EnumWindows(EnumWindowsProc, NULL);
		if (!found && exists) {
			exists = false;
		}
		Sleep(500);
		if (!exists && found) {
			HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
			DWORD cbneeded;
			EnumProcessModules(proc, &gmd, sizeof(gmd), &cbneeded);
			DWORD prev;
			DWORD prev1;
			DWORD prev2;
			DWORD prev3;

			byte* ptr = (byte*)gmd + 0x20C925;
			byte* ptr1 = (byte*)gmd + 0x20D143;
			byte* ptr2 = (byte*)gmd + 0x20A563;
			byte* ptr3 = (byte*)gmd + 0x20A595;

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

			CloseHandle(proc);
			exists = true;
		}
		found = false;
	}
	return 0;
}
