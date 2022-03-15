/*

------------------------------
------------------------------
-- Simple Win32 .WAV Player --
------------------------------
------------------------------
	
	This project is just a simple Win32 .WAV player in a console application that uses DirectSound to play an audio file. The purpose 
	of it was to make a simple .WAV player thats easy to setup, use and thats freely available to anyone. It also works for 
	x64 compilation if desired.

	The .WAV file that comes with the project is a personal file and requires my permission for usage in anything beyond
	this example project, please contact the email below for more info.

	Direct Sound example from Windows SDK Samples that I repurposed for easier use.
	URL Link to Github: https://github.com/microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/directshow/dmo/dmodemo
	--License for this project can be in the project directory under the file "LICENSE" and also in the dsound folder included with the project--

	If the DirectX libraries do not exist you may need to install in the Windows SDK to build!

	By the way I own a small gaming company called Major Gaming which has a few games available on the Google Play Store for Android. The 2 games are 
	Knife Roulette and The White Blood Cell the links are below please check them out and leave a review!

		- Knife Roulette : https://play.google.com/store/apps/details?id=com.Knife_Roulette_1_0_0
		- The White Blood Cell: https://play.google.com/store/apps/details?id=com.The_White_Blood_Cell

	Website & Email:
		Major Gaming Website: https://majorgamingllc.weebly.com/
		Major Gaming Email: majorgamingllc.owner@gmail.com

*/

// Header file that helps us initialize and inludes our class DirectSoundWavPlayer
#include "dsound/DirectSound.h"
int main() 
{
	// This function setups DirectSound for us
	HRESULT hr = initializeDSound();
	// Check for error and exit w/ code if error
	if (hr != S_OK) { printf("Failed to initailize Dsound...\n"); exit(0xEEEE); }

	// Create dummy window for dsound
	// Directsound requires a window to play sound
	// This is all basic Win32 Window creation code
	HINSTANCE handle = GetModuleHandle(0); // Get handle of our process to create window
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.style = CS_HREDRAW | CS_VREDRAW | BDR_OUTER | CS_OWNDC;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = handle;
	wc.lpszClassName = (LPCWSTR)"primary";
	wc.lpfnWndProc = (WNDPROC)PrimaryDebug;
	if (!RegisterClassW(&wc)) {
		MessageBox(0, "Class creation failed..", "Class creation failed..", 0);
	}
	// Create window (The window is set to not exist w/ width and height set to 0,0)
	HWND window = CreateWindowW(wc.lpszClassName, (LPCWSTR)"Direct Sound Example", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 0, 0, NULL, NULL, handle, NULL);
	ShowWindow(window,0); // Hide window

	// Our .WAV file
	const char* file = "Ball_Collide.wav";
	// Create an instance of our simple .WAV player pass our file to it
	DirectSoundWavPlayer audio(window, (LPTSTR)file);

	// Play sound
	hr = audio.play();
	// Check for error and exit w/ code if error
	if (hr != S_OK) { printf("Failed to play .WAV file...\n"); exit(0xEEEE); }

	// Setup loop to play the sample over and check for input
	bool Stop = false; // Prevent stop from being pressed repeatedly
	bool Rewind = false; // Prevent rewind from being pressed repeatedly
	printf("--DirectSound initialized succesfully--\n");
	printf("File load: %s\nPress \"P\" to play the file, press \"S\" to stop and press \"R\" to rewind! \n",file);
	while (1) { // Begin infinite loop
		if (GetAsyncKeyState('P') & 0x8000) {
			if (!Stop) {
				printf("P pressed playing audio\n");
				hr = audio.play(); // Play function
				if (hr != S_OK) { printf("Failed to play .WAV file...\n"); exit(0xEEEE); }
				Stop = true;
				Rewind = true;
			}
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			if (Stop) {
				printf("S pressed stopped audio\n");
				hr = audio.stop(); // Stop function
				if (hr != S_OK) { printf("Failed to stop .WAV file...\n"); exit(0xEEEE); }
				Stop = false;
				Rewind = false;
			}
		}
		if (GetAsyncKeyState('R') & 0x8000) {
			if (Rewind) {
				printf("R pressed rewind audio\n");
				hr = audio.rewind(); // Rewind function
				if (hr != S_OK) { printf("Failed to rewind .WAV file...\n"); exit(0xEEEE); }
				Stop = false;
				Rewind = false;
			}
		}
	}
	PostMessage(window, WM_CLOSE, 0, 0); // CLOSE WINDOW
	PostQuitMessage(0);
	DestroyWindow(window);
	return 0;
}