#ifndef _UTILS_H
#define _UTILS_H

/* 
	Steam API Interface

	Developer: sk0r / Czybik
	Version: v0.1 (Steam-API v015) 13th August, 2014
	Contact: Czybik_Stylez@gmx.de

	See readme.txt for more details

	File: utils.h: Utility interface
*/

//======================================================================
BOOL SetAppPath(LPSTR lpszOut);
VOID ConsolePrint(LPCSTR lpszFmt, ...);
//======================================================================

//======================================================================
class CNameChanger { //Changes the Steam Friends name each defined msecs permanently
private:
	BOOL bStatus;

	DWORD dwCurTimer;
	DWORD dwLastTimer;

	char szCurRndName[100];

	ISteamFriends *pSteamFriends;

	LPSTR GetCurrentName();
	LPSTR GetName(LPSTR game);
	BOOL IsProcessListed(LPSTR game);

	LPSTR oldName = NULL;

	std::vector<std::pair<std::string, std::string>> namecombinations;

public:
	CNameChanger() { bStatus = FALSE; pSteamFriends = NULL; }
	~CNameChanger() { }

	VOID SetInterface(ISteamFriends* pSteamFriends);
	VOID ReadConfig();
	VOID ReadNames();
	BOOL SetStatus(BOOL bEnable);


	VOID Think(VOID);
};
//======================================================================

#endif