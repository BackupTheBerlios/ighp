;======================================================
; Include
 
!include "MUI2.nsh"

;======================================================
; Installer Information
 
Name "iTunes Global Hotkeys Plugin v0.0.3"
OutFile "ighp_0.0.3.exe"
InstallDir "C:\Program Files\iTunes\Plug-ins"

;======================================================
; Modern Interface Configuration
 
!define MUI_ABORTWARNING
!define MUI_FINISHPAGE
!define MUI_FINISHPAGE_TEXT "Thank you for installing iTunes Global Hotkeys Plugin. You need to restart iTunes before using the plugin"

;======================================================
; Pages
 
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE License.txt
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;======================================================
; Languages
 
!insertmacro MUI_LANGUAGE "English"

;======================================================
; Sections

Section "Plugin" pluginSection
  ; Version checking logic
  ; TODO: for the next version

  SetOutPath $INSTDIR
  
  File "release\GlobalHotkeysPlugin.dll"
  File "release\GlobalHotkeysImpl.dll"
  File "/oname=Global Hotkeys License.txt" "License.txt"
 
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\UninstallGlobalHotkeys.exe"

SectionEnd

Section "Uninstall"

  ;Delete plugin files
  Delete "$INSTDIR\GlobalHotkeysPlugin.dll"
  Delete "$INSTDIR\GlobalHotkeysImpl.dll"
  Delete "$INSTDIR\Global Hotkeys License.txt"
  Delete "$INSTDIR\UninstallGlobalHotkeys.exe"

SectionEnd

