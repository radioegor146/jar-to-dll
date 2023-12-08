#include <windows.h>

#include "injector.h"
#include "utils.h"

static DWORD RunInjectorThreadProxy(LPVOID) {
  RunInjector();
  return 1;
}

BOOL WINAPI DllMain(HINSTANCE dll_instance, DWORD reason, LPVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH) {
    ::global_dll_instance = dll_instance;
    CreateThread(nullptr, 0, &RunInjectorThreadProxy, nullptr, 0, nullptr);
  }
  return TRUE;
}
