/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef GLOBAL_HOTKEYS_IMPL_H
#define GLOBAL_HOTKEYS_IMPL_H

#include <windows.h>

#if defined (__cplusplus)
extern "C" {
#endif

typedef void (WINAPI *DLL_Function_Initialize) ();
typedef void (WINAPI *DLL_Function_Release) ();

typedef void (WINAPI *DLL_Function_InitGlobalHotkeysPlugin) ();
typedef void (WINAPI *DLL_Function_ReleaseGlobalHotkeysPlugin) ();

#if defined (__cplusplus)
}
#endif

#endif /* GLOBAL_HOTKEYS_IMPL_H */