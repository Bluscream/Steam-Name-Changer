#include "StdAfx.h"
using namespace std;

/* 
	Steam API Interface

	Developer: sk0r / Czybik
	Version: v0.1 (Steam-API v015) 13th August, 2014
	Contact: Czybik_Stylez@gmx.de

	See readme.txt for more details

	File: utils.cpp: Utilitiy functions 
*/

//======================================================================
BOOL SetAppPath(LPSTR lpszOut)
{
	//Write application path to string

	if (!lpszOut)
		return FALSE;

	//Get full file name
	if (!GetModuleFileNameA(NULL, lpszOut, MAX_PATH))
		return FALSE;

	//Remove file name from string
	for (size_t i = strlen(lpszOut); i >= 3; i--) {
		if (lpszOut[i] == '\\')
			break;

		lpszOut[i] = 0;
	}

	return TRUE;
}
//======================================================================

//======================================================================
VOID ConsolePrint(LPCSTR lpszFmt, ...)
{
	//Print text to console

	char szFmtBuf[250];
	
	//Format text
	va_list vaList;
	va_start(vaList, lpszFmt);
	vsprintf_s(szFmtBuf, lpszFmt, vaList);
	va_end(vaList);

	//Pass to standard output
	std::cout << szFmtBuf << std::endl;
}
//======================================================================

//======================================================================
VOID CNameChanger::SetInterface(ISteamFriends* pSteamFriends)
{
	//Set pointer to Steam Friends interface

	this->pSteamFriends = pSteamFriends;
}
//======================================================================
VOID CNameChanger::ReadConfig()
{

}

//======================================================================
int get_file_size(FILE *p_file)
{
	fseek(p_file, 0, SEEK_END);
	int size = ftell(p_file);
	fseek(p_file, 0, SEEK_SET);
	return size;
}

//======================================================================

//======================================================================

std::vector<std::string> split(std::string text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

//======================================================================

//======================================================================

void convertToLower(char *str)
{
	for (int i = 0; i < strlen(str); i++)
		str[i] = tolower(str[i]);
}

//======================================================================

//======================================================================

VOID CNameChanger::ReadNames()
{
	FILE *fp = fopen("names.cfg", "rb");
	int len = get_file_size(fp);

	// Allocate buffer
	char *fileBuffer = new char[len];
	fread(fileBuffer, len, 1, fp);			// Read file data to buffer
	fclose(fp);								// Close file

	// Split buffer by lines
	std::vector<std::string> lines = split(std::string(fileBuffer), '\n');

	// Loop through the lines
	for (auto curline : lines)
	{
		if (curline[0] == '#' || !strnicmp(curline.c_str(), "//", 2))
			continue;

		std::vector<std::string> expressions = split(curline, '=');
		char *processName = strdup(expressions[0].c_str());
		convertToLower(processName);

		namecombinations.push_back({ processName, expressions[1] });
	}
}
//======================================================================

//======================================================================
BOOL CNameChanger::SetStatus(BOOL bEnable)
{
	//Set name change status

	if ((bEnable) && (!pSteamFriends))
		return FALSE;

	//Set flag
	bStatus = bEnable;

	//If enabled, initialize timers
	if (bStatus) {
		ConsolePrint("Current Steam Friends name: %s", pSteamFriends->GetPersonaName());

		dwCurTimer = dwLastTimer = GetTickCount();
	}

	return TRUE;
}
//======================================================================

//======================================================================

LPSTR CNameChanger::GetName(LPSTR game)
{
	if (!strcmp(game, "<unknown>"))
		return NULL;

	for (auto process : namecombinations)
	{
		if (!strcmp(process.first.c_str(), game))
			return strdup(process.second.c_str());
	}

	return "Failure";
}

//======================================================================

//======================================================================

BOOL CNameChanger::IsProcessListed(LPSTR game)
{
	for (auto process : namecombinations)
	{
		if (!strcmp(process.first.c_str(), game))
			return true;
	}

	return false;
}

//======================================================================

//======================================================================
LPSTR CNameChanger::GetCurrentName()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		// Find default name, because this failed
		return GetName("default");
	}

	for (int pid = 0; pid < i; pid++)
	{
		char *szProcessName = new char[MAX_PATH];
		strcpy(szProcessName, "<unknown>");

		// Get a handle to the process.
		try
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
				PROCESS_VM_READ,
				FALSE, aProcesses[pid]);

			// Get the process name.
			if (NULL != hProcess)
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
					&cbNeeded))
				{
					GetModuleBaseNameA(hProcess, hMod, szProcessName,
						MAX_PATH);

					// 0 terminate string early
					szProcessName[strlen(szProcessName) - 4] = '\0';
					convertToLower(szProcessName);
				}
			}

			if (IsProcessListed(szProcessName))
				return GetName(szProcessName);
		}
		catch (int e)
		{
		}
		
	}

	return GetName("default");
}
//======================================================================

//======================================================================
VOID CNameChanger::Think(VOID)
{
	//Process name change

	#define NC_TIMETOWAIT 1000

	if (!bStatus)
		return;

	//Update timer
	dwCurTimer = GetTickCount();

	if (dwCurTimer > dwLastTimer + NC_TIMETOWAIT) { //If determined time has elapsed
		//Update last timer
		dwLastTimer = GetTickCount();

		//Change name
		LPSTR pString = GetCurrentName();
		if (pString) {
			if (oldName != NULL && !strcmp(oldName, pString))
				return;

			// Set the old name
			oldName = strdup(pString);

			// Set the actual name
			printf("Setting personal name to %s.\n", pString);
			pSteamFriends->SetPersonaName(pString);
		}
	}
}
//======================================================================
