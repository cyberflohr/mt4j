// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"

extern char MT4_BASE_DIR[255];
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	GetModuleFileName((HINSTANCE)&__ImageBase, MT4_BASE_DIR, sizeof(MT4_BASE_DIR));
	*(strrchr(MT4_BASE_DIR, '\\')) = 0;
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		log("DLL_PROCESS_ATTACH: %s", MT4_BASE_DIR);
		break;
	case DLL_THREAD_ATTACH:
		// don't call attach here because it will prevent debugger connect.
		// attachCurrentThread();
		break;
	case DLL_THREAD_DETACH:
		detachCurrentThread();
		break;
	case DLL_PROCESS_DETACH:
		log("DLL_PROCESS_DETACH"); 
		break;
	}
	return TRUE;
}

