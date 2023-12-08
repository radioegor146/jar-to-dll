#ifndef JAR_TO_DLL_UTILS_H
#define JAR_TO_DLL_UTILS_H

#include <windows.h>

extern HMODULE global_dll_instance;

void ShowMessage(const wchar_t* message);
void Error(const wchar_t* error);

#endif  //JAR_TO_DLL_UTILS_H
