#ifndef _VARS_H
#define _VARS_H

/* 
	Steam API Interface

	Developer: sk0r / Czybik
	Version: v0.1 (Steam-API v015) 13th August, 2014
	Contact: Czybik_Stylez@gmx.de

	See readme.txt for more details

	File: vars.h: Global stuff interface*
*/

//======================================================================
#define PROGRAM_NAME "Steam Name Changer"
#define PROGRAM_VERSON "\'v1.0 (Steam-API v015) 13th August, 2014\'"
#define PROGRAM_AUTHOR "sk0r / Czybik, Bluscream, Rektinator"
#define PROGRAM_CONTACT "Czybik_Stylez@gmx.de"
#define PROGRAM_INFO PROGRAM_NAME " version " PROGRAM_VERSON " developed by " PROGRAM_AUTHOR " (" PROGRAM_CONTACT ")"
//======================================================================
#define ENABLED 1
#define METHOD "GAME"
#define NC_TIMETOWAIT 1000
//======================================================================
extern char g_szAppPath[MAX_PATH];
extern CAccessSteamAPI g_oSteamAPI;
extern CNameChanger g_oNameChanger;
//======================================================================

#endif