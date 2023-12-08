#include "utils.h"

#include <windows.h>
#include <winuser.h>

HMODULE global_dll_instance = nullptr;

void ShowMessage(const wchar_t* message) {
  MessageBoxW(nullptr, message, L"JarToDllInjector", MB_OK | MB_ICONINFORMATION);
}

void Error(const wchar_t* error) {
  DWORD last_status_code = GetLastError();
  wchar_t second_message[256] = {};
  wsprintfW(second_message, L"Last error code: %u", last_status_code);
  MessageBoxW(nullptr, error, L"JarToDllInjector", MB_OK | MB_ICONEXCLAMATION);
  MessageBoxW(nullptr, second_message, L"JarToDllInjector", MB_OK | MB_ICONEXCLAMATION);
  FreeLibraryAndExitThread(global_dll_instance, 1);
}