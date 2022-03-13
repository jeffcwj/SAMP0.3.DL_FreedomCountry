phone_OnPlayerDisconnect(playerid)
{
	KillTimer(phone_load_timer[playerid]);
	
	if(phone_opened[playerid]) Phone_Close(playerid);
}
phone_OnPlayerConnect(playerid)
{
	phone_opened[playerid] = 0;
	phone_timing[playerid] = gettime() + 5;

	KillTimer(phone_load_timer[playerid]);
	phone_load_enable[playerid] = false;
}
phone_OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(_:clickedid == INVALID_TEXT_DRAW)
	{
		switch(phone_opened[playerid])
		{
			case phone_type_home, phone_type_block: return Phone_Close(playerid);
			case phone_type_load: return SelectTextDraw(playerid, 0x4695CEFF);
			case phone_type_contacts, phone_type_settings: return Phone_OpenHome(playerid);
			case phone_type_contact_info, phone_type_contact_add: return Phone_OpenContacts(playerid);
		}
	}
	return true;
}
phone_OnPlayerClickPTextDraw(playerid, PlayerText:playertextid)
{
	if(phone_type_home == phone_opened[playerid])
	{
		for(new i; i < phone_data[playerid][phone_max_apps]; i ++)
		{
			if(playertextid == phone_apps_PTD[playerid][i])return Phone_OpenApp(playerid, i);
		}
	}

	if(phone_opened[playerid])
	{
		if(playertextid == phone_home_PTD[playerid][3] && phone_opened[playerid] != phone_type_home)return Phone_OpenHome(playerid);
		if(playertextid == phone_PTD[playerid][0])
		{
			if(!phone_data[playerid][phone_status])
			{
				Phone_OpenLoading(playerid, 1);

				PlayerTextDrawSetSelectable(playerid, phone_PTD[playerid][0], false);
				PlayerTextDrawShow(playerid, phone_PTD[playerid][0]);
			}
			else if(phone_type_block == phone_opened[playerid])
			{
				if(phone_data[playerid][phone_block])
				{

				}
				else Phone_OpenHome(playerid);
			}
			return true;
		}
	}
	return true;
}

phone_second_timer(playerid)
{
	if(phone_opened[playerid]
		&& phone_opened[playerid] != phone_type_block
		&& phone_opened[playerid] != phone_type_load)
	{
		new
			hour,
			minute,

			str[20];

		gettime(hour, minute);

		format(str, 10, "%d:%d", hour, minute);
		PlayerTextDrawSetString(playerid, phone_home_PTD[playerid][0], str);
	}
}
