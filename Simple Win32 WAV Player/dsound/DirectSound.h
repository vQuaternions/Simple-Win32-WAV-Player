#pragma once
// Direct Sound example from windows sdk samples
// URL Link to Github: https://github.com/microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/directshow/dmo/dmodemo
// Manually add libraries
#pragma comment (lib, "Winmm.lib")
#pragma comment (lib, "msdmo.lib")
#pragma comment (lib, "dmoguids.lib")
#pragma comment (lib, "amstrmid.lib")
#pragma comment (lib, "dsound.lib")

#include <stdio.h>
#define STRICT

#include <windows.h>
#include <mmsystem.h>

#include <windowsx.h>
#include <TCHAR.h>
#include <objbase.h>
#include <commdlg.h>
#include <dsound.h>
#include "DSUtil.h"
#include "DXUtil.h"
#include <dmoreg.h>
#include <strsafe.h>
#include <string>
#include <sstream>
#include "appstream.h"

#include <codecvt>
// wstring convertor
//setup converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> wchar_converter;
//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
std::string str;

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define MAX_NUM 100
#define APPNAME TEXT("DMO Demo\0")

// struct holding DMOs registered as DMOCATEGORY_AUDIO_EFFECT
typedef struct tagDMOINFO {
	TCHAR szName[MAX_NUM];
	CLSID clsidDMO;
} DMOINFO;

DMOINFO             g_rgDmoInfo[MAX_PATH] = { 0 };
TCHAR               g_szInputFileName[MAX_PATH] = { 0 };
CSoundManager*      g_pSoundManager = NULL;
CSound*             g_pSound = NULL;
int iNumDmo = 0;

// Comparison callback used by qsort()
int CompareStrings(const void *str1, const void *str2)
{
	// Compare all of both strings
	return _tcsicmp((TCHAR *)str1, (TCHAR *)str2);
}

VOID ResetSoundBuffer()
{
	if (g_pSound)
	{
		g_pSound->Stop();
		g_pSound->Reset();
	}
}

HRESULT StreamData(HWND h, LPTSTR lpszInputFile, REFGUID rclsid)
{
	HRESULT         hr;
	BYTE            *pbOutData = 0;
	ULONG           uDataSize = 0;
	LPWAVEFORMATEX  pwfx = NULL;        // pointer to waveformatex structure.

	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		//MessageBox(hDlg, TEXT("Could not initialize COM library."),
		//	TEXT(DEMO_NAME), MB_OK | MB_ICONERROR);
		return hr;
	}

	CAppStream*  pStream = new CAppStream();
	if (!pStream)
		return E_OUTOFMEMORY;

	hr = pStream->StreamData(lpszInputFile,
		rclsid,
		h,
		&pbOutData,
		&uDataSize,
		&pwfx);
	if (FAILED(hr))
		return hr;

	// Free any previous sound, and make a new one
	SAFE_DELETE(g_pSound);

	// Load the data from memory into a DirectSound buffer
	hr = g_pSoundManager->CreateFromMemory(&g_pSound, pbOutData, uDataSize, pwfx, DSBCAPS_GLOBALFOCUS, GUID_NULL);
	if (FAILED(hr))
	{
		// Not a critical failure, so just update the status
		//MessageBox(hDlg, TEXT("Could not create sound buffer."),
		//	TEXT(DEMO_NAME), MB_OK | MB_ICONERROR);
		//SetDlgItemText(hDlg, IDC_FILENAME, TEXT("Could not create sound buffer."));
		return hr;
	}

	SAFE_DELETE_ARRAY(pbOutData);
	SAFE_DELETE(pStream);
	SafeGlobalFree(pwfx);

	CoUninitialize();
	return hr;
}

static LRESULT CALLBACK PrimaryDebug(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		PostMessage(hWnd, WM_CLOSE, 0, 0); // CLOSE WINDOW
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;		// message handled
}

class DirectSoundWavPlayer
{
public:
	CSoundManager*      g_pSoundManager = NULL;
	CSound*             g_pSound = NULL;
	HRESULT hr;

	HRESULT StreamWav(HWND h, LPTSTR lpszInputFile)
	{
		BYTE            *pbOutData = 0;
		ULONG           uDataSize = 0;
		LPWAVEFORMATEX  pwfx = NULL;	// pointer to waveformatex structure.

		this->hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(this->hr)) {
			printf("Could not initialize COM library.\n");
			return hr;
		}

		CAppStream*  pStream = new CAppStream();
		if (!pStream)
			return E_OUTOFMEMORY;

		this->hr = pStream->StreamData(lpszInputFile,
			g_rgDmoInfo[9].clsidDMO,
			h,
			&pbOutData,
			&uDataSize,
			&pwfx);
		if (FAILED(this->hr))
			return this->hr;

		// Free any previous sound, and make a new one
		SAFE_DELETE(this->g_pSound);

		// Load the data from memory into a DirectSound buffer
		this->hr = this->g_pSoundManager->CreateFromMemory(&this->g_pSound, pbOutData, uDataSize, pwfx, DSBCAPS_GLOBALFOCUS, GUID_NULL);
		if (FAILED(this->hr))
		{
			// Not a critical failure, so just update the status
			printf("Could not create sound buffer.\n");
			return this->hr;
		}

		SAFE_DELETE_ARRAY(pbOutData);
		SAFE_DELETE(pStream);
		SafeGlobalFree(pwfx);

		CoUninitialize();
		return this->hr;
	}

	DirectSoundWavPlayer(HWND h, LPTSTR f)
	{
		// Allocate memory for our CSoundManager
		this->g_pSoundManager = new CSoundManager();

		if (this->g_pSoundManager == NULL)
		{
			printf("Creating CSoundManager failed.\n");
		}

		hr = this->g_pSoundManager->Initialize(h, DSSCL_PRIORITY);
		if (FAILED(hr))
		{
			printf("Failed Initalizing Dsound...\n");
		}

		hr = this->g_pSoundManager->SetPrimaryBufferFormat(2, 22050, 16);
		if (FAILED(hr))
		{
			printf("Failed Initalizing Dsound...\n");
		}

		if (this->g_pSound)
		{
			this->g_pSound->Stop();
			this->g_pSound->Reset();
		}

		// Create memory buffer
		hr = this->StreamWav(h, (LPTSTR)f);
		if (FAILED(hr))
		{
			printf("Failed to stream .WAV file...\n");
		}
	}

	~DirectSoundWavPlayer() 
	{
		// Prevent memory leak free allocated memory
		SAFE_DELETE(this->g_pSoundManager);
		SAFE_DELETE(this->g_pSound);
	}

	HRESULT play()
	{
		this->hr = this->g_pSound->Play(0,     // lowest priority
			0);   // no flag is set
		return this->hr;
	}
	HRESULT stop()
	{
		this->hr = this->g_pSound->Stop();   // no flag is set
		return this->hr;
	}
	HRESULT rewind()
	{
		this->hr = this->g_pSound->Reset();   // no flag is set
		return this->hr;
	}
};

// Returns a result of S_OK if everything went properly
HRESULT initializeDSound()
{
	HRESULT hr;
	// Initialize Dsound
	HRESULT hrNext;
	IEnumDMO* pDMOEnum = NULL;
	WCHAR* wszDMOName = 0;
	DWORD ulNumInfoReturned = 0;
	CLSID clsidCurrentDMO;

	// Enumerate all the DMOs registered as DMOCATEGORY_AUDIO_EFFECT.
	hr = DMOEnum(DMOCATEGORY_AUDIO_EFFECT,
		DMO_ENUMF_INCLUDE_KEYED,
		0,        // Number of input partial media types
		NULL,
		0,        // Number of output partial media types
		NULL,
		&pDMOEnum);

	if (FAILED(hr))
		printf("Failed Initalizing Dsound...\n");

	do
	{
		// Get information about the next DMO in the enumeration
		hrNext = pDMOEnum->Next(1, &clsidCurrentDMO, &wszDMOName, &ulNumInfoReturned);
		if (FAILED(hrNext))
		{
			printf("Failed Initalizing DMO's...\n");
			SAFE_RELEASE(pDMOEnum);
			return hrNext;
		}

		if (S_OK == hrNext)
		{
			// Convert wstring to string
			str = wchar_converter.to_bytes(wszDMOName);
			// Copy this DMO name and CLSID into a global structure
			StringCchCopy(g_rgDmoInfo[iNumDmo].szName, MAX_NUM, str.c_str());
			g_rgDmoInfo[iNumDmo].szName[MAX_NUM - 1] = TEXT('\0'); // NULL-terminate
			//printf("%s \n", g_rgDmoInfo[iNumDmo].szName);
			g_rgDmoInfo[iNumDmo].clsidDMO = clsidCurrentDMO;
			CoTaskMemFree(wszDMOName);
			(iNumDmo)++;
		}

	} while ((S_OK == hrNext) && (iNumDmo < MAX_NUM));

	// Now that the global DMO list is completed, sort it alphabetically
	qsort(g_rgDmoInfo, iNumDmo, sizeof(DMOINFO), CompareStrings);

	// Release the DMO enumerator
	SAFE_RELEASE(pDMOEnum);
}
