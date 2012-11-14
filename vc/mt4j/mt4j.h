// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem MT4J_EXPORTS-Symbol
// (in der Befehlszeile definiert) kompiliert. Dieses Symbol darf für kein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// MT4J_API-Funktionen als aus einer DLL importiert, während die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifdef MT4J_EXPORTS
#define MT4J_API __declspec(dllexport)
#else
#define MT4J_API __declspec(dllimport)
#endif

MT4J_API int __stdcall newInstance(char * className);
MT4J_API int __stdcall processTick(int jvmCtx, double bid, double ask);
MT4J_API void __stdcall addNewBar(int jvmCtx, double high, double low, double open, double close);
MT4J_API void __stdcall destroy(int jctx);
MT4J_API void __stdcall setIntProperty(int jvmCtx, char * propertyName, int value);
MT4J_API void __stdcall setBoolProperty(int jvmCtx, char * propertyName, int value);
MT4J_API void __stdcall setDoubleProperty(int jvmCtx, char * propertyName, double value);
MT4J_API void __stdcall setStringProperty(int jvmCtx, char * propertyName, char * value);
MT4J_API int __stdcall getBoolProperty(int jvmCtx, char * propertyName);
MT4J_API char * __stdcall getStringProperty(int jvmCtx, char * propertyName);
MT4J_API int __stdcall getIntProperty(int jvmCtx, char * propertyName);
MT4J_API double __stdcall getDoubleProperty(int jvmCtx, char * propertyName);
MT4J_API void __stdcall initObject(int jvmCtx);
MT4J_API void __stdcall deinitObject(int jvmCtx);
MT4J_API void __stdcall callNoArgsMethod(int jvmCtx, char * method);