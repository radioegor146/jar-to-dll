#ifndef UTILS_H_
#define UTILS_H_

#include <windows.h>

extern HMODULE global_dll_instance;

void ShowMessage(const wchar_t* message);
void Error(const wchar_t* error);

#endif  //UTILS_H_
