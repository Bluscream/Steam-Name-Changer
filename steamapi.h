#ifndef _STEAM_API_H
#define _STEAM_API_H

/* 
	Steam API Interface

	Developer: sk0r / Czybik
	Version: v0.1 (Steam-API v015) 13th August, 2014
	Contact: Czybik_Stylez@gmx.de

	See readme.txt for more details

	File: steamapi.h: SteamAPI access interface
*/

/*
	//Implementation of exported function steam_api.dll::SteamFriends()
	60272F10 > A1 A4822860      MOV EAX,DWORD PTR DS:[602882A4] //Copy DWORD at given address (m_pSteamFriends) to register EAX as result value
	60272F15   C3               RETN //Restore return address
	This would apply for all other exported Steam API interfaces. So you can just call those functions whenever you want. They do not process futher
	stuff than just passing you the address of the specified interface object instance.
*/

//======================================================================
#define STEAMAPI_MODULE "steam_api.dll"
#define STEAMAPI_VERSON "v015"

#define STEAMAPPID_NAME "steam_appid.txt"
#define STEAMAPPID_DEFAULT 10

#define MAKE_INTTD(cls) typedef cls * (* Tpfn##cls)(void)
#define MAKE_INTM(cls) Tpfn##cls p##cls
//======================================================================

//======================================================================
//Access type definitions
typedef bool (*TpfnSteamAPI_Init)(void);
typedef bool (*TpfnSteamAPI_IsSteamRunning)(void);
typedef void (*TpfnSteamAPI_Shutdown)(void);

//interface type definitions
MAKE_INTTD(ISteamUser);
MAKE_INTTD(ISteamFriends);
MAKE_INTTD(ISteamUtils);
MAKE_INTTD(ISteamMatchmaking);
MAKE_INTTD(ISteamUserStats);
MAKE_INTTD(ISteamApps);
MAKE_INTTD(ISteamMatchmakingServers);
//======================================================================

//======================================================================
enum InitResult {
	IR_SUCCESS,
	IR_PARAM,
	IR_APPID,
	IR_LOADLIB,
	IR_SACC,
	IR_INIT,
	IR_SAPI,
	IR_NULL
};
//======================================================================

//======================================================================
struct steamapi_accesstable_s {
	TpfnSteamAPI_Init pfnSteamAPI_Init; //Initialize Steam API
	TpfnSteamAPI_IsSteamRunning pfnSteamAPI_IsSteamRunning; //Check if Steam is currently running
	TpfnSteamAPI_Shutdown pfnSteamAPI_Shutdown; //Shutdown Steam API
};

struct steamapi_table_s { //Provided interfaces from Steam API to application
	MAKE_INTM(ISteamUser);
	MAKE_INTM(ISteamFriends);
	MAKE_INTM(ISteamUtils);
	MAKE_INTM(ISteamMatchmaking);
	MAKE_INTM(ISteamUserStats);
	MAKE_INTM(ISteamApps);
	MAKE_INTM(ISteamMatchmakingServers);
};
//======================================================================

//======================================================================
class CAccessSteamAPI {
private:
	InitResult irStatus;

	char szAppPath[MAX_PATH];

	HMODULE hSteamAPIDll;

	steamapi_accesstable_s sAccTable;
	steamapi_table_s sApiTable;

	BOOL CreateSteamAppIdFile(LPCSTR lpszAppPath);
	BOOL FileExists(LPCSTR lpszFileName);
public:
	CAccessSteamAPI() { irStatus = IR_NULL; hSteamAPIDll = NULL; memset(&sAccTable, 0x00, sizeof(sAccTable)); memset(&sApiTable, 0x00, sizeof(sApiTable)); }
	~CAccessSteamAPI() { }

	InitResult Initialize(LPCSTR lpszAppPath);
	BOOL Shutdown(VOID);

	BOOL IsSteamRunning(VOID);

	ISteamUser*	GetSteamUserInterface(VOID);
	ISteamFriends* SteamFriendsInterface(VOID);
	ISteamUtils* SteamUtilsInterface(VOID);
	ISteamMatchmaking* SteamMatchmakingInterface(VOID);
	ISteamUserStats* SteamUserStatsInterface(VOID);
	ISteamApps* SteamAppsInterface(VOID);
	ISteamMatchmakingServers* SteamMatchmakingServers(VOID);

	LPCSTR InitResultToString(InitResult irValue);
};
//======================================================================

#endif
