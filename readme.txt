SteamAPI Interface SDK
======================

Devloper: sk0r / Czybik
Version: v0.1 (Steam-API v015) 13th August, 2014
Contact: Czybik_Stylez@gmx.de

Description:
This is a SDK that lets your software interact with the Steam API. It's just a simple
interface using the steam_api.dll. With it you can for example access the Steam Friends
system. I have provided an example that changes the Steam Friends name pseudo-randomly
each defined milliseconds (approximately).

How it works:
The Steam API interface (implemented in steam_api.dll) is an interface that lets your
software interact with Steam. It connects with the Steam system and provides interfaces
that let you access Steam data and also modify Steam data (such as own Friends name, etc.).
To use Steam API properly you need a steam_appid.txt file in the same path of your loader executable.
It contains the Steam AppID of a Steam software known as games. For example you can use the
AppID 10 to let Steam think your app is an instance of Counter-Strike 1.6. After that you can load
the steam_api.dll, call exported function SteamAPI_Init() and, if returns true, you can then 
simply access the provided interfaces. For example if you want to access the Steam Friends system 
then just call the exported function SteamFriends() which returns the address of an instance
of ISteamFriends class. Note that those interface functions such as SteamFriends() will internally
access only a pointer that contains the address of the object instance. So you can just call the
function over and over, because it doesn't process any further stuff (like initializations, counting,
etc.). If you want to know how SteamAPI_Init() (and other functions) works please just have a look
at CSteamAPIContext::Init (steam_api.h) method. SteamAPI_Init just calls the method of the related
object instance. It therefore causes the initialization of the Steam API. You can see other typedefs
there, too. The Steam SDK sourcecode files describe the functionality very well.
Steps to take:
- Create a text file with name 'steam_appid.txt' and insert a Steam AppID. It must be placed inside the directory of the executable that is the host of the DLL
- Load steam_api.dll (can be loaded from any directory)
- Call SteamAPI_Init()
- Call your prefered interface function such as SteamFriends()
- Use the returned class instance
- Call SteamAPI_Shutdown when finished

How to use:
The SDK is provied as steamapi.cpp and steamapi.h and also the content of 'SDK\steam' (Steam SDK). The Steam SDK
provides definitions of the interfaces such as ISteamFriends etc.
Steps to take:
- Integrate the SteamSDK and the Steam API Interface SDK within your project
- Call 'CAccessSteamAPI::Initialize' and pass the full path of your application (where the steam_api.dll is located in)
- Call 'CAccessSteamAPI::InitResultToString' if you want to get a string related to the result value
- Call your prefered interface method (e.g. 'CAccessSteamAPI::SteamFriendsInterface') to access an interface
- Call 'CAccessSteamAPI::IsSteamRunning' to get to know if Steam is still running
- Call 'CAccessSteamAPI::Shutdown' when done

steam_appid.txt:
It must contain the Steam AppId of a Steam game. Note: If you use an ID of a game you do not own then you will
not me flagged as ingame in Steam Friends. If you use an ID of a game you do own than you will be flagged as
ingame (regardless if it's currently installed or not).

Note: I have provided a copy of the steam_api.dll of the current version.
