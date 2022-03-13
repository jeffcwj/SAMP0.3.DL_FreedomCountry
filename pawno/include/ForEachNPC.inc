#if defined _fen_included
	#endinput
#endif
#define _fen_included

#include <a_samp>

//------------------------------------------------------------------------------

#define ForEachNPC(%0) 		for(new i_%0=0, %0=ConnectedNPCList[0], j_%0=ConnectedNPCs; i_%0<j_%0; %0=ConnectedNPCList[++i_%0])
#define GetConnectedNPCs()   ConnectedNPCs

//------------------------------------------------------------------------------

new ConnectedNPCs;
new ConnectedNPCList[MAX_PLAYERS+1];//Loop would bug when server is full D: (Without +1)

//------------------------------------------------------------------------------

public OnPlayerConnect(playerid)
{
	if(IsPlayerNPC(playerid))
		AddNPC(playerid);
	#if defined FEN_OnPlayerConnect
	return FEN_OnPlayerConnect(playerid);
	#endif
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect FEN_OnPlayerConnect
forward FEN_OnPlayerConnect(playerid);

public OnPlayerDisconnect(playerid,reason)
{
    RemoveNPC(playerid);
	#if defined FEN_OnPlayerDisconnect
	return FEN_OnPlayerDisconnect(playerid, reason);
	#endif
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect FEN_OnPlayerDisconnect
forward FEN_OnPlayerDisconnect(playerid,reason);

//------------------------------------------------------------------------------

stock AddNPC(playerid)
{
	if(ConnectedNPCs>=MAX_PLAYERS || playerid<0 || playerid>=MAX_PLAYERS)
	    return 0;

	ConnectedNPCList[ConnectedNPCs++]=playerid;
	return 1;
}

stock RemoveNPC(playerid)
{
	for(new i=0;i<ConnectedNPCs;i++)
	{
	    if(ConnectedNPCList[i]==playerid)
	    {
	        ConnectedNPCs--;
	        ConnectedNPCList[i]=ConnectedNPCList[ConnectedNPCs];
	        return 1;
	    }
	}
	return 0;
}
