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
bool SetAppPath(char * lpszOut);
void ConsolePrint(char * lpszFmt, ...);
//======================================================================

//======================================================================
class CNameChanger { //Changes the Steam Friends name each defined msecs permanently
private:
	bool bStatus;

	DWORD dwCurTimer;
	DWORD dwLastTimer;

	char szCurRndName[100];

	ISteamFriends *pSteamFriends;

	char * GetCurrentName();
	char * GetName(char * game);
	bool IsProcessListed(char * game);

	char * oldName = NULL;
	char * lastGame = "";

	std::vector<std::pair<std::string, std::string>> namecombinations;

public:
	CNameChanger() { bStatus = FALSE; pSteamFriends = NULL; }
	~CNameChanger() { }

	void SetInterface(ISteamFriends* pSteamFriends);
	void ReadConfig();
	void ReadNames();
	bool SetStatus(bool bEnable);


	VOID Think(VOID);
};
//======================================================================

#endif