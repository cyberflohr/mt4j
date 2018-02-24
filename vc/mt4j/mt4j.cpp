// mt4j.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "mt4j.h"

typedef jint (WINAPI* JNI_CREATEJAVAVM)(JavaVM **pvm, void ** penv, void *args); 
typedef jint (WINAPI* JNI_GETCREATEDJAVAVMS)(JavaVM **pvm, jsize bufLen, jsize *nVMs); 

#define MAX_JVM_CLIENTS 10

FILE * logHandle;
JavaVM *jvm = NULL;
int eaCounter=0;
jobject jvmClients[MAX_JVM_CLIENTS];
__declspec( thread ) JNIEnv *jvmEnv = NULL;
char MT4_BASE_DIR[255];

JNI_GETCREATEDJAVAVMS GetCreatedJavaVMs;
JNI_CREATEJAVAVM jni_createJavaVm;

// BEGIN global configuration
char jvmDll[MAX_PATH];
JavaVMOption jvmOptions[50];
JavaVMInitArgs vm_args;
// END global configuration

jint JNICALL jvmOutputFctn(FILE *fp, const char *format, va_list args)
{
	if (logHandle == NULL) {
		char filename[255];
		sprintf(filename, "%s\\mt4j.log", MT4_BASE_DIR);
		logHandle = fopen(filename,"a");
	}

	time_t ltime;
	char buf[26];
	time( &ltime );

    ctime_s( buf, 26, &ltime );

	fprintf(logHandle, _T("%.24s T%04d "), buf, GetCurrentThreadId());
    vfprintf( logHandle, format, args );
	fprintf(logHandle, _T("\n"));

	fflush(logHandle);
    return 0;
}

void log(char * format, ...) {

	va_list args;
    va_start( args, format );
	jvmOutputFctn(logHandle, format, args);
    va_end( args );
} 

/**
 *  Main 
 *  Erzeugt eine neue JMV Instanz, gibt dabei den ermittelten Pfad zur DLL
 *  und den "status" beim Erzeugen aus 
 */ 
JNIEnv* prepareEnv() {

	if (jvm == NULL) {

		JavaVM *vmBuf[1];
		jsize nVMs;
		if (loadDynModule() == FALSE) {
			log("Module loading failed");
			return NULL;
		}

		log("Checking for JVM instances");
		if ((GetCreatedJavaVMs != NULL) && GetCreatedJavaVMs(vmBuf, 1, &nVMs) == 0) {
			log("%d existing JVM found", nVMs);
			if (nVMs == 1) {
				jvm = vmBuf[0];
				attachCurrentThread();
				log("Attached to existing JVM");
				return jvmEnv;
			}
		}

		log("No JVM exists -> create new JVM");
		if (createJavaVm() == false) {
			return NULL;
		}
		log("Attached to JVM");
	}

	attachCurrentThread();
	return jvmEnv;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Erzeugt eine neue Java Virtual Maschine
 */ 
bool createJavaVm() {


	jint  res = (*jni_createJavaVm)(&jvm,(void**)&jvmEnv,&vm_args);
	if (res == JNI_ERR) {
		log("createJavaVm failed: errno=%d", GetLastError());
		return false;
	}
	
	//return res;
	log("JVM instance created");
	return true;
}

/**
 * Destroy JVM instance.
 * Currently do nothing because a VM couldn't be destroyed.
 */
void destroyVM() {
	log("destroy JVM finished.");
}

/**
 * Read the mt4j property file and set global variables.
 */
bool readPropertiesFile() {

	char filename[255];
	sprintf(filename, "%s\\mt4j.properties", MT4_BASE_DIR);
	FILE *propFile = fopen(filename, "rt");
	if (propFile == NULL) {
		log("mt4j.properties file not found in MT4 libraries directory");
		return false;
	}

	char line[4096];
	char key[96];
	char value[4000];
	
	while(fgets(line, sizeof(line), propFile) != NULL) {
		
		if ((line[0] == '#') || (line[0] == '\r')) {
			continue;
		}

		int pos = strcspn(line, "=");
		if (pos > 0) {
			key[0] = 0;
			value[0] = 0;
			strncat(key, line, pos); 
			strncat(value, &line[pos+1], strlen(line)-pos-2);

			log("Parameter: %s = %s", key, value);
			if (strcmp(key, "jvmDll") == 0) {
				strcpy(jvmDll, value);

			} else if (strcmp(key, "jvmOptions") == 0) {
			    vm_args.version = JNI_VERSION_1_6;
				vm_args.ignoreUnrecognized = JNI_FALSE;  	
				vm_args.nOptions = atoi(value)+1;
				vm_args.options = jvmOptions;
				jvmOptions[0].optionString = (char*)malloc(9);
				strcpy(jvmOptions[0].optionString, "vfprintf");
				jvmOptions[0].extraInfo=jvmOutputFctn;

			} else if (strncmp(key, "jvmOption.", 10) == 0) {
				jvmOptions[atoi(&key[10])].optionString = (char*)malloc(strlen(value)+1);
				strcpy(jvmOptions[atoi(&key[10])].optionString, value);

//				log ("option:: %d -> %s", atoi(&key[10]), value);
			}
		}
	}
	fclose(propFile);  /* close the file prior to exiting the routine */

	return true;
}

/**
 * Load JVM Dll and find function entry points
 */
bool loadDynModule() {
	static HMODULE hLib = NULL;

	// modules alread loaded -> exit
	if (hLib != NULL) {
		return true;
	}

	if (readPropertiesFile() == false) {
		log("Error during reading mt4.properties files");
		return FALSE;
	}

	// dynamisches linken gegen die dll
	hLib = LoadLibrary(jvmDll);
	if(hLib == NULL)
	{
		log("Unable to open %s: errno=%d", jvmDll,GetLastError());
		return FALSE;
	}
	
	// erzeugen der Instanz mithilfe der DLL
	jni_createJavaVm = (JNI_CREATEJAVAVM) GetProcAddress(hLib, 
                                                       "JNI_CreateJavaVM");	
	
	if(jni_createJavaVm == NULL) {
		log("Unable to find proc address of %s in %s: errno=%d", "JNI_CreateJavaVM",  jvmDll,GetLastError());
		return FALSE;    	     
	}

	GetCreatedJavaVMs = (JNI_GETCREATEDJAVAVMS) GetProcAddress(hLib, 
                                                       "JNI_GetCreatedJavaVMs");
	if(GetCreatedJavaVMs == NULL) {
		log("Unable to find proc address of %s in %s: errno=%d", "JNI_GetCreatedJavaVMs",  jvmDll,GetLastError());
		return FALSE;    	     
	}
	
	return TRUE;
}

/**
 * Attach current thread to JVM.
 */
void attachCurrentThread() {
	if ((jvm != NULL) && (jvmEnv == NULL)) {
		jvm->AttachCurrentThread((void**)&jvmEnv, NULL);
	}
}

/**
 * Detaches current thread from JVM.
 */
void detachCurrentThread() {
	if (jvm != NULL) {
		jvm->DetachCurrentThread();
	}
}

jmethodID findProperyMethod(int jvmCtx, char * prefix, LPWSTR propertyName, char *signature) {

	char methodName[255];
	sprintf(methodName, "%s%ls", prefix, propertyName);

	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jclass cls = env->GetObjectClass(jvmClients[jvmCtx]);
		jmethodID processMethod = env->GetMethodID(cls, methodName, signature);

		if (processMethod == NULL && (strcmp(prefix, "is") == 0)) {
			sprintf(methodName, "get%ls", propertyName);
			processMethod = env->GetMethodID(cls, methodName, signature);
		}

		env->DeleteLocalRef(cls);
		if (processMethod != NULL) {
			return processMethod;
		}
	}

	log("findProperyMethod(): method not found: %s", methodName);
	return NULL;
}
// +++++++++++++++++++++ PUBLIC API starts here ++++++++++++++++++++

MT4J_API int __stdcall newInstance(LPWSTR lpClassName)
{
	int jvmCtx=-1;
	if (eaCounter == 0) {
		for (int i=0; i < MAX_JVM_CLIENTS; i++) {
			jvmClients[i] = NULL;
		}
		jvmCtx=0;
	} else {
		for (int i=0; i < MAX_JVM_CLIENTS; i++) {
			if (jvmClients[i] == NULL) {
				jvmCtx=i;
				break;
			}
		}
	}

	if (jvmCtx == -1) {
		log("initialize() failed, no more client slots available"); 
		return -1;
	}

	char *className = (char*)malloc(wcslen(lpClassName)+1);
	wcstombs(className, lpClassName, wcslen(lpClassName));
	className[wcslen(lpClassName)] = 0;

	eaCounter++;
	log("initialize(%s) : %d", className, jvmCtx);
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		log("loading EA class: %s", className);
		jclass eaClass = env->FindClass(className);
		jmethodID constructor = env->GetMethodID(eaClass, "<init>", "()V");
		jobject o = env->NewObject(eaClass, constructor);
		
		jvmClients[jvmCtx] = env->NewGlobalRef(o);

		env->DeleteLocalRef(eaClass);
		env->DeleteLocalRef(o);
	}
	free(className);
	return jvmCtx;
}

MT4J_API int __stdcall processTick(int jvmCtx, double bid, double ask)
{
//	log("process(%d)", jvmCtx); 
	jint rVal=0;

	JNIEnv *env = prepareEnv();
	jclass cls = env->GetObjectClass(jvmClients[jvmCtx]);
	jmethodID processMethod = env->GetMethodID(cls, "processTick", "(DD)I");
	env->DeleteLocalRef(cls);

	if (processMethod == NULL) {
		log("processTick() method not found");
		return 0;
	}
	rVal = env->CallIntMethod(jvmClients[jvmCtx],processMethod,bid,ask);
		
	return rVal;
}

MT4J_API void __stdcall addNewBar(int jvmCtx, double high, double low, double open, double close)
{
//	log("addNewBar(%d)", jvmCtx); 

	JNIEnv *env = prepareEnv();
	jclass cls = env->GetObjectClass(jvmClients[jvmCtx]);
	jmethodID processMethod = env->GetMethodID(cls, "addNewBar", "(DDDD)V");
	env->DeleteLocalRef(cls);

	if (processMethod == NULL) {
		log("addNewBar() method not found");
		return;
	}
	env->CallVoidMethod(jvmClients[jvmCtx],processMethod, high,low, open,close);
}

MT4J_API void __stdcall destroy(int jctx)
{
	log("deinitialize(%d)", jctx); 
	JNIEnv *env = prepareEnv();
	env->DeleteGlobalRef(jvmClients[jctx]);
	jvmClients[jctx]=NULL;

	if (--eaCounter == 0) {
		destroyVM();
	}
	return;
}

MT4J_API void __stdcall setIntProperty(int jvmCtx, LPWSTR propertyName, int value)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "set", propertyName, "(I)V");
		if (processMethod == NULL) {
			return;
		}

		env->CallVoidMethod(jvmClients[jvmCtx],processMethod,value);
	}
	return;
}

MT4J_API int __stdcall getIntProperty(int jvmCtx, LPWSTR propertyName)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "get", propertyName, "()I");
		if (processMethod == NULL) {
			return 0;
		}

		return env->CallIntMethod(jvmClients[jvmCtx],processMethod);
	}
	return 0;
}

MT4J_API void __stdcall setDoubleProperty(int jvmCtx, LPWSTR propertyName, double value)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "set", propertyName, "(D)V");
		if (processMethod == NULL) {
			return;
		}

		env->CallVoidMethod(jvmClients[jvmCtx],processMethod,value);
	}
	return;
}

MT4J_API double __stdcall getDoubleProperty(int jvmCtx, LPWSTR propertyName)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "get", propertyName, "()D");
		if (processMethod == NULL) {
			return 0;
		}

		return env->CallDoubleMethod(jvmClients[jvmCtx],processMethod);
	}
	return 0;
}

MT4J_API void __stdcall setStringProperty(int jvmCtx, LPWSTR propertyName, LPWSTR lpValue)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "set", propertyName, "(Ljava/lang/String;)V");
		if (processMethod == NULL) {
			return;
		}

		jstring str = env->NewString((jchar*)lpValue, wcslen(lpValue));
		env->CallVoidMethod(jvmClients[jvmCtx], processMethod, str);
		env->DeleteLocalRef(str);
	}
	return;
}

MT4J_API LPWSTR __stdcall getStringProperty(int jvmCtx, LPWSTR propertyName)
{
	JNIEnv *env = prepareEnv();
	wchar_t * retStr = NULL;
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "get", propertyName, "()Ljava/lang/String;");
		if (processMethod == NULL) {
			return retStr;
		}

		jstring str = (jstring)env->CallObjectMethod(jvmClients[jvmCtx],processMethod);
		if (str != NULL) {
			const jchar *cstr = env->GetStringChars(str, NULL);

			if (cstr != NULL) {
				retStr = (wchar_t*)malloc(env->GetStringLength(str)+1);
				wcscpy(retStr, (wchar_t*)cstr);
				env->ReleaseStringChars(str, cstr);
			}

			env->DeleteLocalRef(str);
		}
	}
	return retStr;
}

MT4J_API void __stdcall setBoolProperty(int jvmCtx, LPWSTR propertyName, int value)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "set", propertyName, "(Z)V");
		if (processMethod == NULL) {
			return;
		}

		env->CallVoidMethod(jvmClients[jvmCtx],processMethod,value);
	}
	return;
}

MT4J_API int __stdcall getBoolProperty(int jvmCtx, LPWSTR propertyName)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "is", propertyName, "()Z");
		if (processMethod == NULL) {
			return false;
		}

		return env->CallBooleanMethod(jvmClients[jvmCtx],processMethod);
	}
	return false;
}

MT4J_API void __stdcall initObject(int jvmCtx)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "", L"init", "()V");
		if (processMethod == NULL) {
			return;
		}

		env->CallVoidMethod(jvmClients[jvmCtx],processMethod);
	}
	return;
}

MT4J_API void __stdcall deinitObject(int jvmCtx)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "", L"deinit", "()V");
		if (processMethod == NULL) {
			return;
		}

		env->CallVoidMethod(jvmClients[jvmCtx],processMethod);
	}
	return;
}

MT4J_API void __stdcall callNoArgsMethod(int jvmCtx, LPWSTR method)
{
	JNIEnv *env = prepareEnv();
	if (env != NULL) {
		jmethodID processMethod = findProperyMethod(jvmCtx, "", method, "()V");
		if (processMethod == NULL) {
			return;
		}

		env->CallVoidMethod(jvmClients[jvmCtx],processMethod);
	}
	return;
}