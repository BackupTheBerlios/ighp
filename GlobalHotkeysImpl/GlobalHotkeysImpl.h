#ifndef GLOBAL_HOTKEYS_IMPL_H
#define GLOBAL_HOTKEYS_IMPL_H

#include <windows.h>

#if defined (__cplusplus)
extern "C" {
#endif

typedef void (WINAPI *DLL_Function_Initialize) ();
typedef void (WINAPI *DLL_Function_Release) ();

typedef void (WINAPI *DLL_Function_InitGlobalHotkeysPlugin) ();

#if defined (__cplusplus)
}
#endif

#endif /* GLOBAL_HOTKEYS_IMPL_H */