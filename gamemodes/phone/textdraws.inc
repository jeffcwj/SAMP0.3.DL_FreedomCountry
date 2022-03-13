new PlayerText: phone_PTD[MAX_PLAYERS][4];

DestroyPhoneTextdraws(playerid)
{
	PlayerTextDrawDestroy(playerid, phone_PTD[playerid][0]);
	PlayerTextDrawDestroy(playerid, phone_PTD[playerid][1]);
	PlayerTextDrawDestroy(playerid, phone_PTD[playerid][2]);
	PlayerTextDrawDestroy(playerid, phone_PTD[playerid][3]);

	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][0]);
	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][1]);
	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][2]);
	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][3]);

	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][0]);
	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][1]);
	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][2]);
	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][3]);

	DestroyLoadingTextdraws(playerid);
	DestroyHomeTextdraws(playerid);
	DestroyPhoneAppsTextdraws(playerid);
	return true;
}
CreatePhoneTextdraws(playerid, model[])
{
	if(phone_opened[playerid])return false;

	new
		x = phone_data[playerid][phone_pos][0],
		y = phone_data[playerid][phone_pos][1];

	phone_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 501.0000 + x, 150.4370 + y, "_"); // §á§å§ã§ä§à
	PlayerTextDrawTextSize(playerid, phone_PTD[playerid][0], 92.0000, 213.0000);

	phone_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 501.0000 + x, 150.4369 + y, "ld_spac:white"); // §á§å§ã§ä§à
	PlayerTextDrawTextSize(playerid, phone_PTD[playerid][1], 91.0000, 28.0000);

	phone_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 475.6667 + x, 135.0888 + y, "mdl-5003:iphone_xr_blue"); // §á§å§ã§ä§à
	PlayerTextDrawTextSize(playerid, phone_PTD[playerid][2], 145.0000, 245.0000);

	phone_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 548.0000 + x, 165.1110 + y, "App"); // §á§å§ã§ä§à
	PlayerTextDrawLetterSize(playerid, phone_PTD[playerid][3], 0.1960, 0.9819);
	PlayerTextDrawTextSize(playerid, phone_PTD[playerid][3], 0.0000, 85.0000);
	PlayerTextDrawAlignment(playerid, phone_PTD[playerid][3], 2);
	PlayerTextDrawColor(playerid, phone_PTD[playerid][3], 724249599);
	PlayerTextDrawFont(playerid, phone_PTD[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, phone_PTD[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, phone_PTD[playerid][3], 0);

	PlayerTextDrawAlignment(playerid, phone_PTD[playerid][0], 1);
	PlayerTextDrawFont(playerid, phone_PTD[playerid][0], 4);
	PlayerTextDrawSetProportional(playerid, phone_PTD[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, phone_PTD[playerid][0], 0);

	PlayerTextDrawAlignment(playerid, phone_PTD[playerid][1], 1);
	PlayerTextDrawColor(playerid, phone_PTD[playerid][1], -1);
	PlayerTextDrawFont(playerid, phone_PTD[playerid][1], 4);
	PlayerTextDrawSetProportional(playerid, phone_PTD[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, phone_PTD[playerid][1], 0);

	PlayerTextDrawAlignment(playerid, phone_PTD[playerid][2], 1);
	PlayerTextDrawColor(playerid, phone_PTD[playerid][2], -1);
	PlayerTextDrawFont(playerid, phone_PTD[playerid][2], 4);
	PlayerTextDrawSetProportional(playerid, phone_PTD[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, phone_PTD[playerid][2], 0);

	CreateLoadingTextdraws(playerid, x, y, model);
	CreateHomeTextdraws(playerid, x, y, model);
	CreatePhoneAppsTextdraws(playerid, x, y, model);
	return true;
}
