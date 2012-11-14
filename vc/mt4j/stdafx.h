// stdafx.h : Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.
// Windows-Headerdateien:
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h> 
#include <stdarg.h>
#include <time.h>


// TODO: Hier auf zusätzliche Header, die das Programm erfordert, verweisen.
#include <jni.h>

bool createJavaVm();
JNIEnv* prepareEnv();
void destroyVM();
void log(char * format,...);
bool loadDynModule();
void attachCurrentThread();
void detachCurrentThread();