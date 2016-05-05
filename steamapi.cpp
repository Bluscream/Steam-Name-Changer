#include "StdAfx.h"

/* 
	Steam API Interface

	Developer: sk0r / Czybik
	Version: v0.1 (Steam-API v015) 13th August, 2014
	Contact: Czybik_Stylez@gmx.de

	See readme.txt for more details

	File: steamapi.cpp: SteamAPI access implementation
*/

//======================================================================
InitResult CAccessSteamAPI::Initialize(LPCSTR lpszAppPath)
{
	//Initialize Steam API

	if (irStatus == IR_SUCCESS)
		return IR_SUCCESS;

	if (!lpszAppPath)
		return IR_PARAM;

	char szAPIModule[MAX_PATH];
	char szAppTxt[MAX_PATH];

	//Format strings
	sprintf_s(szAPIModule, "%s" STEAMAPI_MODULE, lpszAppPath);
	sprintf_s(szAppTxt, "%s" STEAMAPPID_NAME, lpszAppPath);

	//Check for a steam_appid.txt. Create the file with a default AppId if not exists
	if (!FileExists(szAppTxt)) {
		if (!CreateSteamAppIdFile(lpszAppPath)) {
			return IR_APPID;
		}
	}

	//Load module
	hSteamAPIDll = LoadLibraryA(szAPIModule);
	if (!hSteamAPIDll)
		return IR_LOADLIB;

	//Initialize access functions

	#define RETRIEVE_ACCADDRESS(fnc, n) sAccTable.pfn##fnc = (Tpfn##fnc)GetProcAddress(hSteamAPIDll, n); \
									 if (!sAccTable.pfn##fnc) { FreeLibrary(hSteamAPIDll); return IR_SACC; }

	RETRIEVE_ACCADDRESS(SteamAPI_Init, "SteamAPI_Init");
	RETRIEVE_ACCADDRESS(SteamAPI_IsSteamRunning, "SteamAPI_IsSteamRunning");
	RETRIEVE_ACCADDRESS(SteamAPI_Shutdown, "SteamAPI_Shutdown");

	//Call function to let SteamAPI initialize and connect with Steam
	if (!sAccTable.pfnSteamAPI_Init()) {
		FreeLibrary(hSteamAPIDll);
		return IR_INIT;
	}

	//Initialize API functions

	#define RETRIEVE_APIADDRESS(cls, n) sApiTable.p##cls = (Tpfn##cls)GetProcAddress(hSteamAPIDll, n); \
									 if (!sApiTable.p##cls) { FreeLibrary(hSteamAPIDll); return IR_SAPI; }

	RETRIEVE_APIADDRESS(ISteamUser, "SteamUser");
	RETRIEVE_APIADDRESS(ISteamFriends, "SteamFriends");
	RETRIEVE_APIADDRESS(ISteamUtils, "SteamUtils");
	RETRIEVE_APIADDRESS(ISteamMatchmaking, "SteamMatchmaking");
	RETRIEVE_APIADDRESS(ISteamUserStats, "SteamUserStats");
	RETRIEVE_APIADDRESS(ISteamApps, "SteamApps");
	RETRIEVE_APIADDRESS(ISteamMatchmakingServers, "SteamMatchmakingServers");

	//Success
	return irStatus = IR_SUCCESS;
}
//======================================================================

//======================================================================
BOOL CAccessSteamAPI::CreateSteamAppIdFile(LPCSTR lpszAppPath)
{
	//Create a valid steam_appid.txt

	char szFileName[MAX_PATH];
	char szAppId[25];
	DWORD dwBytesWritten;

	//Format strings
	sprintf_s(szFileName, "%s" STEAMAPPID_NAME, lpszAppPath);
	sprintf_s(szAppId, "%d", STEAMAPPID_DEFAULT);

	//Create new file
	HANDLE hFile = CreateFileA(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	//Write buffer to file
	if ((!WriteFile(hFile, szAppId, strlen(szAppId), &dwBytesWritten, NULL)) || (dwBytesWritten != strlen(szAppId))) {
		CloseHandle(hFile);
		return FALSE;
	}

	//Close handle
	CloseHandle(hFile);

	return TRUE;
}
//======================================================================

//======================================================================
BOOL CAccessSteamAPI::Shutdown(VOID)
{
	//Shutdown Steam API

	if (irStatus != IR_SUCCESS)
		return FALSE;

	//Call Steam API shutdown function
	sAccTable.pfnSteamAPI_Shutdown();

	//Clear variable
	irStatus = IR_NULL;

	//Free library
	return FreeLibrary(hSteamAPIDll);
}
//======================================================================

//======================================================================
BOOL CAccessSteamAPI::IsSteamRunning(VOID)
{
	//Check if Steam is currently running

	if (irStatus != IR_SUCCESS)
		return FALSE;

	return sAccTable.pfnSteamAPI_IsSteamRunning() == true;
}
//======================================================================

//======================================================================
ISteamUser*	CAccessSteamAPI::GetSteamUserInterface(VOID)
{
	//Return address of 'ISteamUser' class instance

	if (irStatus != IR_SUCCESS)
		return NULL;

	return sApiTable.pISteamUser();
}
//======================================================================

//======================================================================
ISteamFriends* CAccessSteamAPI::SteamFriendsInterface(VOID)
{
	//Return address of 'ISteamFriends' class instance

	if (irStatus != IR_SUCCESS)
		return NULL;

	return sApiTable.pISteamFriends();
}
//======================================================================

//======================================================================
ISteamUtils* CAccessSteamAPI::SteamUtilsInterface(VOID)
{
	//Return address of 'ISteamUtils' class instance

	if (irStatus != IR_SUCCESS)
		return NULL;

	return sApiTable.pISteamUtils();
}
//======================================================================

//======================================================================
ISteamMatchmaking* CAccessSteamAPI::SteamMatchmakingInterface(VOID)
{
	//Return address of 'ISteamMatchmaking' class instance

	if (irStatus != IR_SUCCESS)
		return NULL;

	return sApiTable.pISteamMatchmaking();
}
//======================================================================

//======================================================================
ISteamUserStats* CAccessSteamAPI::SteamUserStatsInterface(VOID)
{
	//Return address of 'ISteamUserStats' class instance

	if (irStatus != IR_SUCCESS)
		return NULL;

	return sApiTable.pISteamUserStats();
}
//======================================================================

//======================================================================
ISteamApps* CAccessSteamAPI::SteamAppsInterface(VOID)
{
	//Return address of 'ISteamApps' class instance

	if (irStatus != IR_SUCCESS)
		return NULL;

	return sApiTable.pISteamApps();
}
//======================================================================

//======================================================================
ISteamMatchmakingServers* CAccessSteamAPI::SteamMatchmakingServers(VOID)
{
	//Return address of 'ISteamMatchmakingServers' class instance

	if (irStatus != IR_SUCCESS)
		return NULL;

	return sApiTable.pISteamMatchmakingServers();
}
//======================================================================

//======================================================================
LPCSTR CAccessSteamAPI::InitResultToString(InitResult irValue)
{
	//Get description of InitResult value

	switch (irValue) {
	case IR_SUCCESS:
		return "IR_SUCCESS";
		break;
	case IR_PARAM:
		return "IR_PARAM";
		break;
	case IR_APPID:
		return "IR_APPID";
		break;
	case IR_LOADLIB:
		return "IR_LOADLIB";
		break;
	case IR_SACC:
		return "IR_SACC";
		break;
	case IR_INIT:
		return "IR_INIT";
		break;
	case IR_SAPI:
		return "IR_SAPI";
		break;
	default:
		return "<undefined>";
		break;
	}

	return "IR_NULL";
}
//======================================================================

//======================================================================
BOOL CAccessSteamAPI::FileExists(LPCSTR lpszFileName)
{
	//Check if a specified file exists

	HANDLE hFile = CreateFileA(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return TRUE;
	}
	
	return FALSE;
}
//======================================================================