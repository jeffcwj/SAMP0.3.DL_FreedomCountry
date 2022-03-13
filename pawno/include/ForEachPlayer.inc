#if defined _fep_included
	#endinput
#endif
#define _fep_included

#include <a_samp>

//------------------------------------------------------------------------------

#define ForEachPlayer(%0) 		for(new i_%0=0, %0=ConnectedPlayerList[0], j_%0=ConnectedPlayers; i_%0<j_%0; %0=ConnectedPlayerList[++i_%0])
#define GetConnectedPlayers()   ConnectedPlayers

//------------------------------------------------------------------------------

new ConnectedPlayers;
new ConnectedPlayerList[MAX_PLAYERS+1];//Loop would bug when server is full D: (Without +1)

//------------------------------------------------------------------------------

public OnPlayerConnect(playerid)
{
	if(!IsPlayerNPC(playerid))
		AddPlayer(playerid);
	#if defined FEP_OnPlayerConnect
	return FEP_OnPlayerConnect(playerid);
	#endif
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect FEP_OnPlayerConnect
forward FEP_OnPlayerConnect(playerid);

public OnPlayerDisconnect(playerid,reason)
{
    RemovePlayer(playerid);
	#if defined FEP_OnPlayerDisconnect
	return FEP_OnPlayerDisconnect(playerid, reason);
	#endif
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect FEP_OnPlayerDisconnect
forward FEP_OnPlayerDisconnect(playerid,reason);

//------------------------------------------------------------------------------

stock AddPlayer(playerid)
{
	if(ConnectedPlayers>=MAX_PLAYERS || playerid<0 || playerid>=MAX_PLAYERS)
	    return 0;

	ConnectedPlayerList[ConnectedPlayers++]=playerid;
	return 1;
}

stock RemovePlayer(playerid)
{
	for(new i=0;i<ConnectedPlayers;i++)
	{
	    if(ConnectedPlayerList[i]==playerid)
	    {
	        ConnectedPlayers--;
	        ConnectedPlayerList[i]=ConnectedPlayerList[ConnectedPlayers];
	        return 1;
	    }
	}
	return 0;
}
