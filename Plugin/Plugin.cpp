/*
 * Copyright (c) 2008-2012 Stefan Cosma <stefan.cosma@gmail.com>
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

#include "stdafx.h"
#include "Plugin.h"
#include "GlobalHotkeys.h"

HANDLE treadhandle = 0;

extern "C" IGHP_API void InitPlugin()
{
	PluginWinApp* pPlugin = Plugin::GetInstance();
	pPlugin->Run();
}

extern "C" IGHP_API void ReleasePlugin()
{
	PluginWinApp* pPlugin = Plugin::GetInstance();
	SAFE_DEL(pPlugin);
}

extern "C"
{

DWORD WINAPI PluginThread(LPVOID lpParam)
{
	InitPlugin();
	return 0;
}

#include <iTunesVisualAPI.h>
#include <stdlib.h>

//########################################
//	typedef's, struct's, enum's, etc.
//########################################

#define	kTVisualPluginMajorVersion		1
#define	kTVisualPluginMinorVersion		0
#define	kTVisualPluginReleaseStage		0x80
#define	kTVisualPluginNonFinalRelease	0

struct VisualPluginData {
	void *				appCookie;
	ITAppProcPtr		appProc;

	HWND				destPort;

	Rect				destRect;
	OptionBits			destOptions;
	UInt32				destBitDepth;

	RenderVisualData	renderData;
	UInt32				renderTimeStampID;
	
	ITTrackInfo			trackInfo;
	ITStreamInfo		streamInfo;

	Boolean				playing;
	Boolean				padding[3];

//	Plugin-specific data
	UInt8				minLevel[kVisualMaxDataChannels];		// 0-128
	UInt8				maxLevel[kVisualMaxDataChannels];		// 0-128

	UInt8				min, max;
};
typedef struct VisualPluginData VisualPluginData;


//########################################
//	local ( static ) globals
//########################################

//########################################
//	exported function prototypes
//########################################

extern OSStatus iTunesPluginMainMachO( OSType inMessage, PluginMessageInfo *inMessageInfoPtr, void *refCon );

//########################################
//	MemClear
//########################################
static void MemClear( LogicalAddress dest, SInt32 length )
{
	register unsigned char	*ptr;

	ptr = ( unsigned char* ) dest;
	
	while ( length-- > 0 )
		*ptr++ = 0;
}	// MemClear

//########################################
//	static ( local ) functions
//########################################


//########################################
//	RenderVisualPort
//########################################
static void RenderVisualPort(VisualPluginData *visualPluginData, GRAPHICS_DEVICE destPort,const Rect *destRect,Boolean onlyUpdate)
{
	RECT	srcRect;
	HBRUSH	hBrush;
	HDC		hdc;

	if (destPort == nil)
		return;

	srcRect.left = destRect->left;
	srcRect.top = destRect->top;
	srcRect.right = destRect->right;
	srcRect.bottom = destRect->bottom;
	
	hdc = GetDC(destPort);		
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &srcRect, hBrush);
	
	DeleteObject(hBrush);
	ReleaseDC(destPort, hdc);
}


// ChangeVisualPort
//
static OSStatus ChangeVisualPort(VisualPluginData *visualPluginData,GRAPHICS_DEVICE destPort,const Rect *destRect)
{
	OSStatus		status;
	
	status = noErr;
			
	visualPluginData->destPort = destPort;
	if (destRect != nil)
		visualPluginData->destRect = *destRect;

	return status;
}

/*
	ResetRenderData
*/
static void ResetRenderData(VisualPluginData *visualPluginData)
{
	MemClear(&visualPluginData->renderData,sizeof(visualPluginData->renderData));

	visualPluginData->minLevel[0] = 
		visualPluginData->minLevel[1] =
		visualPluginData->maxLevel[0] =
		visualPluginData->maxLevel[1] = 0;
}

/*
	VisualPluginHandler
*/
static OSStatus VisualPluginHandler(OSType message,VisualPluginMessageInfo *messageInfo,void *refCon)
{
	OSStatus			status;
	VisualPluginData *	visualPluginData;

	visualPluginData = (VisualPluginData*) refCon;
	
	status = noErr;

	switch (message)
	{
		/*
			Sent when the visual plugin is registered.  The plugin should do minimal
			memory allocations here.  The resource fork of the plugin is still available.
		*/		
		case kVisualPluginInitMessage:
		{
			visualPluginData = (VisualPluginData*) calloc(1, sizeof(VisualPluginData));
			if (visualPluginData == nil)
			{
				status = memFullErr;
				break;
			}

			visualPluginData->appCookie	= messageInfo->u.initMessage.appCookie;
			visualPluginData->appProc	= messageInfo->u.initMessage.appProc;

			messageInfo->u.initMessage.refCon	= (void*) visualPluginData;

			treadhandle = CreateThread(NULL, NULL, PluginThread, NULL, NULL, NULL);

			break;
		}
			
		/*
			Sent when the visual plugin is unloaded
		*/		
		case kVisualPluginCleanupMessage:
			if (visualPluginData != nil)
				free(visualPluginData);
			
			Plugin::GetInstance()->Quit();

			WaitForSingleObject(treadhandle, 1000);
			CloseHandle(treadhandle);

			ReleasePlugin();

			break;
			
		/*
			Sent when the visual plugin is enabled.  iTunes currently enables all
			loaded visual plugins.  The plugin should not do anything here.
		*/
		case kVisualPluginEnableMessage:
		case kVisualPluginDisableMessage:
			break;

		/*
			Sent if the plugin requests idle messages.  Do this by setting the kVisualWantsIdleMessages
			option in the RegisterVisualMessage.options field.
		*/
		case kVisualPluginIdleMessage:
			if (false == visualPluginData->playing)
				RenderVisualPort(visualPluginData,visualPluginData->destPort,&visualPluginData->destRect,false);
			break;
					
		/*
			Sent when iTunes is going to show the visual plugin in a port.  At
			this point,the plugin should allocate any large buffers it needs.
		*/
		case kVisualPluginShowWindowMessage:
			visualPluginData->destOptions = messageInfo->u.showWindowMessage.options;

			status = ChangeVisualPort(	visualPluginData,
										#if TARGET_OS_WIN32
											messageInfo->u.setWindowMessage.window,
										#endif
										#if TARGET_OS_MAC
											messageInfo->u.setWindowMessage.port,
										#endif
										&messageInfo->u.showWindowMessage.drawRect);
			if (status == noErr)
				RenderVisualPort(visualPluginData,visualPluginData->destPort,&visualPluginData->destRect,true);
			break;
			
		/*
			Sent when iTunes is no longer displayed.
		*/
		case kVisualPluginHideWindowMessage:
			(void) ChangeVisualPort(visualPluginData,nil,nil);

			MemClear(&visualPluginData->trackInfo,sizeof(visualPluginData->trackInfo));
			MemClear(&visualPluginData->streamInfo,sizeof(visualPluginData->streamInfo));
			break;
		
		/*
			Sent when iTunes needs to change the port or rectangle of the currently
			displayed visual.
		*/
		case kVisualPluginSetWindowMessage:
			visualPluginData->destOptions = messageInfo->u.setWindowMessage.options;

			status = ChangeVisualPort(	visualPluginData,
										#if TARGET_OS_WIN32
											messageInfo->u.showWindowMessage.window,
										#endif
										#if TARGET_OS_MAC
											messageInfo->u.showWindowMessage.port,
										#endif
										&messageInfo->u.setWindowMessage.drawRect);

			if (status == noErr)
				RenderVisualPort(visualPluginData,visualPluginData->destPort,&visualPluginData->destRect,true);
			break;
		
		/*
			Sent for the visual plugin to render a frame.
		*/
		case kVisualPluginRenderMessage:
			visualPluginData->renderTimeStampID	= messageInfo->u.renderMessage.timeStampID;

			RenderVisualPort(visualPluginData,visualPluginData->destPort,&visualPluginData->destRect,false);
			break;
#if 0			
		/*
			Sent for the visual plugin to render directly into a port.  Not necessary for normal
			visual plugins.
		*/
		case kVisualPluginRenderToPortMessage:
			status = unimpErr;
			break;
#endif 0
		/*
			Sent in response to an update event.  The visual plugin should update
			into its remembered port.  This will only be sent if the plugin has been
			previously given a ShowWindow message.
		*/	
		case kVisualPluginUpdateMessage:
			RenderVisualPort(visualPluginData,visualPluginData->destPort,&visualPluginData->destRect,true);
			break;
		
		/*
			Sent when the player starts.
		*/
		case kVisualPluginPlayMessage:
			if (messageInfo->u.playMessage.trackInfo != nil)
				visualPluginData->trackInfo = *messageInfo->u.playMessage.trackInfoUnicode;
			else
				MemClear(&visualPluginData->trackInfo,sizeof(visualPluginData->trackInfo));

			if (messageInfo->u.playMessage.streamInfo != nil)
				visualPluginData->streamInfo = *messageInfo->u.playMessage.streamInfoUnicode;
			else
				MemClear(&visualPluginData->streamInfo,sizeof(visualPluginData->streamInfo));
		
			visualPluginData->playing = true;
			break;

		/*
			Sent when the player changes the current track information.  This
			is used when the information about a track changes,or when the CD
			moves onto the next track.  The visual plugin should update any displayed
			information about the currently playing song.
		*/
		case kVisualPluginChangeTrackMessage:
			if (messageInfo->u.changeTrackMessage.trackInfo != nil)
				visualPluginData->trackInfo = *messageInfo->u.changeTrackMessage.trackInfoUnicode;
			else
				MemClear(&visualPluginData->trackInfo,sizeof(visualPluginData->trackInfo));

			if (messageInfo->u.changeTrackMessage.streamInfo != nil)
				visualPluginData->streamInfo = *messageInfo->u.changeTrackMessage.streamInfoUnicode;
			else
				MemClear(&visualPluginData->streamInfo,sizeof(visualPluginData->streamInfo));
			break;

		/*
			Sent when the player stops.
		*/
		case kVisualPluginStopMessage:
			visualPluginData->playing = false;
			
			ResetRenderData(visualPluginData);

			RenderVisualPort(visualPluginData,visualPluginData->destPort,&visualPluginData->destRect,true);
			break;
		
		/*
			Sent when the player changes position.
		*/
		case kVisualPluginSetPositionMessage:
			break;

		/*
			Sent when the player pauses.  iTunes does not currently use pause or unpause.
			A pause in iTunes is handled by stopping and remembering the position.
		*/
		case kVisualPluginPauseMessage:
			visualPluginData->playing = false;

			ResetRenderData(visualPluginData);

			RenderVisualPort(visualPluginData,visualPluginData->destPort,&visualPluginData->destRect,true);
			break;
			
		/*
			Sent when the player unpauses.  iTunes does not currently use pause or unpause.
			A pause in iTunes is handled by stopping and remembering the position.
		*/
		case kVisualPluginUnpauseMessage:
			visualPluginData->playing = true;
			break;
		
		default:
			status = unimpErr;
			break;
	}
	return status;	
}

/*
	RegisterVisualPlugin
*/
static OSStatus RegisterVisualPlugin(PluginMessageInfo *messageInfo)
{
	OSStatus			status;
	PlayerMessageInfo	playerMessageInfo;
	Str255				pluginName = "Global Hotkeys - http://ighp.berlios.de";
		
	MemClear(&playerMessageInfo.u.registerVisualPluginMessage,sizeof(playerMessageInfo.u.registerVisualPluginMessage));
	
	// copy in name length byte first
	playerMessageInfo.u.registerVisualPluginMessage.name[0] = lstrlenA((LPCSTR)pluginName);
	// now copy in actual name
	//memcpy(&playerMessageInfo.u.registerVisualPluginMessage.name[0], &pluginName[0], pluginName[0]);
	memcpy(&playerMessageInfo.u.registerVisualPluginMessage.name[1], &pluginName, lstrlenA((LPCSTR)pluginName));

	SetNumVersion(&playerMessageInfo.u.registerVisualPluginMessage.pluginVersion,kTVisualPluginMajorVersion,kTVisualPluginMinorVersion,kTVisualPluginReleaseStage,kTVisualPluginNonFinalRelease);

	playerMessageInfo.u.registerVisualPluginMessage.options					= kVisualWantsIdleMessages;
	playerMessageInfo.u.registerVisualPluginMessage.handler					= (VisualPluginProcPtr)VisualPluginHandler;
	playerMessageInfo.u.registerVisualPluginMessage.registerRefCon			= 0;
	playerMessageInfo.u.registerVisualPluginMessage.creator					= 0;
	
	playerMessageInfo.u.registerVisualPluginMessage.timeBetweenDataInMS		= 0xFFFFFFFE; // 16 milliseconds = 1 Tick,0xFFFFFFFF = Often as possible.
	playerMessageInfo.u.registerVisualPluginMessage.numWaveformChannels		= 2;
	playerMessageInfo.u.registerVisualPluginMessage.numSpectrumChannels		= 2;
	
	playerMessageInfo.u.registerVisualPluginMessage.minWidth				= 64;
	playerMessageInfo.u.registerVisualPluginMessage.minHeight				= 64;
	playerMessageInfo.u.registerVisualPluginMessage.maxWidth				= 32767;
	playerMessageInfo.u.registerVisualPluginMessage.maxHeight				= 32767;
	playerMessageInfo.u.registerVisualPluginMessage.minFullScreenBitDepth	= 0;
	playerMessageInfo.u.registerVisualPluginMessage.maxFullScreenBitDepth	= 0;
	playerMessageInfo.u.registerVisualPluginMessage.windowAlignmentInBytes	= 0;
	
	status = PlayerRegisterVisualPlugin(messageInfo->u.initMessage.appCookie,messageInfo->u.initMessage.appProc,&playerMessageInfo);
		
	return status;
	
}

//########################################
//	main entrypoint
//########################################

IGHP_API OSStatus iTunesPluginMain(OSType message,PluginMessageInfo *messageInfo,void *refCon)
{
	OSStatus		status;
	
	(void) refCon;
	
	switch (message)
	{
		case kPluginInitMessage:
			status = RegisterVisualPlugin(messageInfo);
			break;
			
		case kPluginCleanupMessage:
			status = noErr;
			break;
			
		default:
			status = unimpErr;
			break;
	}
	
	return status;
}


}
