new
	PlayerText: phone_load_PTD[MAX_PLAYERS][9],

	phone_load_timer[MAX_PLAYERS],
	phone_load_count[MAX_PLAYERS],
	bool: phone_load_enable[MAX_PLAYERS],
	phone_load_list[MAX_PLAYERS];

@_loading_phone(playerid, type);
@_loading_phone(playerid, type)
{
	if(!phone_load_enable[playerid])return false;
	
	if(phone_load_list[playerid] > 0 && phone_load_list[playerid] < 8)
	{
		PlayerTextDrawColor(playerid, phone_load_PTD[playerid][phone_load_list[playerid]-1], 0x333333FF);
		PlayerTextDrawShow(playerid, phone_load_PTD[playerid][phone_load_list[playerid]-1]);
	}
	else if(phone_load_list[playerid] == 0)
	{
		PlayerTextDrawColor(playerid, phone_load_PTD[playerid][7], 0x333333FF);
		PlayerTextDrawShow(playerid, phone_load_PTD[playerid][7]);
	}

	PlayerTextDrawColor(playerid, phone_load_PTD[playerid][phone_load_list[playerid]], COLOR_LIGHTBLUE);
	PlayerTextDrawShow(playerid, phone_load_PTD[playerid][phone_load_list[playerid]]);

	if(++ phone_load_list[playerid] == 8) phone_load_list[playerid] = 0;

	new str[5];

	format(str, 5, "%d%%", phone_load_count[playerid]);
	PlayerTextDrawSetString(playerid, phone_load_PTD[playerid][8], str);

	if(++ phone_load_count[playerid] >= 100)
	{
		for(new i; i < 9; i ++) PlayerTextDrawHide(playerid, phone_load_PTD[playerid][i]);

		KillTimer(phone_load_timer[playerid]);
		phone_load_enable[playerid] = false;

		switch(type)
		{
			case 1:
			{
				phone_data[playerid][phone_status] = 1;
				Phone_OpenHome(playerid, 1);
			}
		}
	}
	return true;
}

DestroyLoadingTextdraws(playerid)
{
	for(new i; i < 9; i ++) PlayerTextDrawDestroy(playerid, phone_load_PTD[playerid][i]);
}
CreateLoadingTextdraws(playerid, x, y, model[])
{
	phone_load_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 545.3334 + x, 243.0518 + y, "mdl-5000:circle");
	phone_load_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 551.3334 + x, 247.0518 + y, "mdl-5000:circle");
	phone_load_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 555.3334 + x, 254.4667 + y, "mdl-5000:circle");
	phone_load_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 552.3334 + x, 262.0517 + y, "mdl-5000:circle");
	phone_load_PTD[playerid][4] = CreatePlayerTextDraw(playerid, 545.3334 + x, 267.0517 + y, "mdl-5000:circle");
	phone_load_PTD[playerid][5] = CreatePlayerTextDraw(playerid, 537.3334 + x, 262.0517 + y, "mdl-5000:circle");
	phone_load_PTD[playerid][6] = CreatePlayerTextDraw(playerid, 534.3334 + x, 254.4667 + y, "mdl-5000:circle");
	phone_load_PTD[playerid][7] = CreatePlayerTextDraw(playerid, 537.6667 + x, 247.0519 + y, "mdl-5000:circle");

	for(new i; i < 8; i ++)
	{
		PlayerTextDrawTextSize(playerid, phone_load_PTD[playerid][i], 3.5000, 3.5000);
		PlayerTextDrawAlignment(playerid, phone_load_PTD[playerid][i], 1);
		PlayerTextDrawColor(playerid, phone_load_PTD[playerid][i], 0x333333FF);
		PlayerTextDrawFont(playerid, phone_load_PTD[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, phone_load_PTD[playerid][i], 0);
		PlayerTextDrawSetShadow(playerid, phone_load_PTD[playerid][i], 0);
	}

	phone_load_PTD[playerid][8] = CreatePlayerTextDraw(playerid, 547.3665 + x, 252.5370 + y, "_");
	PlayerTextDrawLetterSize(playerid, phone_load_PTD[playerid][8], 0.1466, 0.7994);
	PlayerTextDrawAlignment(playerid, phone_load_PTD[playerid][8], 2);
	PlayerTextDrawColor(playerid, phone_load_PTD[playerid][8], -1);
	PlayerTextDrawFont(playerid, phone_load_PTD[playerid][8], 1);
	PlayerTextDrawSetProportional(playerid, phone_load_PTD[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, phone_load_PTD[playerid][8], 0);
}
