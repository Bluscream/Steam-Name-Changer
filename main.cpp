#include "StdAfx.h"

/* 
	Steam API Interface

	Developer: sk0r / Czybik
	Version: v0.1 (Steam-API v015) 13th August, 2014
	Contact: Czybik_Stylez@gmx.de

	See readme.txt for more details

	File: main.cpp: Main functions
*/

//======================================================================
VOID OnShutdown(VOID);
//======================================================================

//======================================================================
BOOL WINAPI ConsoleControlHandler(DWORD dwCtrlType)
{
	//Console control handler

	if (dwCtrlType == CTRL_CLOSE_EVENT) { //Close console event
		OnShutdown();

		return TRUE;
	}

	return FALSE;
}
//======================================================================

//======================================================================
VOID OnShutdown(VOID)
{
	//Do stuff on shutdown

	ConsolePrint("Shutting down...");

	g_oNameChanger.SetStatus(FALSE);
	g_oSteamAPI.Shutdown();
	SetConsoleCtrlHandler(&ConsoleControlHandler, FALSE);
}
//======================================================================

//======================================================================
int main(int argc, char* argv[])
{
	//Entry point implementation

	//Set Pseudo-random seed
	srand(time(NULL));

	//Set console title
	SetConsoleTitleA(PROGRAM_NAME);

	//Print about message
	ConsolePrint(PROGRAM_INFO "\n");

	//Format application path
	if (!SetAppPath(g_szAppPath)) {
		ConsolePrint("[Formatting App Path] Failed: %d", GetLastError());
		return EXIT_FAILURE;
	}

	//Register console control handler
	if (!SetConsoleCtrlHandler(&ConsoleControlHandler, TRUE)) {
		ConsolePrint("[Console Control Handler] Failed to register: %d", GetLastError());
		return EXIT_FAILURE;
	}

	//Initialize Steam API

	InitResult irResult = g_oSteamAPI.Initialize(g_szAppPath);
	
	ConsolePrint("[SteamAPI] Initialized: %d (%s) -> %d\n", irResult, g_oSteamAPI.InitResultToString(irResult), GetLastError());

	if (irResult != IR_SUCCESS)
		return EXIT_FAILURE;

	//Initialize name changer

	ISteamFriends* pSteamFriends = g_oSteamAPI.SteamFriendsInterface();
	if (!pSteamFriends)
		ConsolePrint("[SteamAPI] SteamFriendsInterface failed to initialize!");

	g_oNameChanger.SetInterface(pSteamFriends);
	g_oNameChanger.SetStatus(TRUE);

	//Main loop

	ConsolePrint("Waiting for games, press [ESC] to exit.");
	
	g_oNameChanger.ReadNames();

	while(g_oSteamAPI.IsSteamRunning()) {
		if ((GetForegroundWindow() == FindWindowA(NULL, PROGRAM_NAME)) && (GetAsyncKeyState(VK_ESCAPE))) {
			break;
		}

		g_oNameChanger.Think();
	}

	//When done
	OnShutdown();

	return EXIT_SUCCESS;
}
//======================================================================
