new
	PlayerText: phone_block_PTD[MAX_PLAYERS][4],
	PlayerText: phone_home_PTD[MAX_PLAYERS][4];

ShowStatusBar(playerid)
{
	new
		hour,
		minute,

		str[20];

	gettime(hour, minute);

	format(str, 10, "%d:%d", hour, minute);
	PlayerTextDrawSetString(playerid, phone_home_PTD[playerid][0], str);

	PlayerTextDrawShow(playerid, phone_home_PTD[playerid][3]);

	PlayerTextDrawShow(playerid, phone_home_PTD[playerid][0]);
	PlayerTextDrawShow(playerid, phone_home_PTD[playerid][1]);
	PlayerTextDrawShow(playerid, phone_home_PTD[playerid][2]);
}

DestroyHomeTextdraws(playerid)
{
	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][0]);
	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][1]);
	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][2]);
	PlayerTextDrawDestroy(playerid, phone_block_PTD[playerid][3]);

	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][0]);
	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][1]);
	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][2]);
	PlayerTextDrawDestroy(playerid, phone_home_PTD[playerid][3]);
}
CreateHomeTextdraws(playerid, x, y, model[])
{
	// Block
	phone_block_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 539.0000 + x, 290.0000 + y, "mdl-5003:block");
	PlayerTextDrawTextSize(playerid, phone_block_PTD[playerid][0], 17.0000, 17.0000);

	phone_block_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 548.0000 + x, 312.0000 + y, "µ¥»÷ÆÁÄ»~n~½âËø");
	PlayerTextDrawLetterSize(playerid, phone_block_PTD[playerid][1], 0.1459, 0.9197);

	phone_block_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 549.0001 + x, 188.9036 + y, "_");
	PlayerTextDrawLetterSize(playerid, phone_block_PTD[playerid][2], 0.3413, 2.0687);

	phone_block_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 549.0001 + x, 205.9046 + y, "_");
	PlayerTextDrawLetterSize(playerid, phone_block_PTD[playerid][3], 0.2046, 1.2639);

		// status bar
	phone_home_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 514.6668 + x, 153.9109 + y, "18:40");
	PlayerTextDrawLetterSize(playerid, phone_home_PTD[playerid][0], 0.1260, 0.7494);

	phone_home_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 572.0994 + x, 153.1259 + y, "mdl-5006:connection_off_1");
	PlayerTextDrawTextSize(playerid, phone_home_PTD[playerid][1], 7.0000, 7.0000);

	phone_home_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 580.3972 + x, 153.1259 + y, "mdl-5006:wifi_off_1");
	PlayerTextDrawTextSize(playerid, phone_home_PTD[playerid][2], 7.0000, 7.0000);

	phone_home_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 530.9332 + x, 356.6001 + y, "mdl-5003:button_1");
	PlayerTextDrawTextSize(playerid, phone_home_PTD[playerid][3], 34.0000, 5.0000);
	PlayerTextDrawAlignment(playerid, phone_home_PTD[playerid][3], 1);
	PlayerTextDrawColor(playerid, phone_home_PTD[playerid][3], 2004318207);
	PlayerTextDrawFont(playerid, phone_home_PTD[playerid][3], 4);
	PlayerTextDrawSetProportional(playerid, phone_home_PTD[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, phone_home_PTD[playerid][3], 0);
	PlayerTextDrawSetSelectable(playerid, phone_home_PTD[playerid][3], true);

	// Block
	PlayerTextDrawAlignment(playerid, phone_block_PTD[playerid][0], 1);
	PlayerTextDrawColor(playerid, phone_block_PTD[playerid][0], -116);
	PlayerTextDrawFont(playerid, phone_block_PTD[playerid][0], 4);
	PlayerTextDrawSetProportional(playerid, phone_block_PTD[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, phone_block_PTD[playerid][0], 0);

	PlayerTextDrawAlignment(playerid, phone_block_PTD[playerid][1], 2);
	PlayerTextDrawColor(playerid, phone_block_PTD[playerid][1], -116);
	PlayerTextDrawBackgroundColor(playerid, phone_block_PTD[playerid][1], 255);
	PlayerTextDrawFont(playerid, phone_block_PTD[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, phone_block_PTD[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, phone_block_PTD[playerid][1], 0);

	PlayerTextDrawAlignment(playerid, phone_block_PTD[playerid][2], 2);
	PlayerTextDrawColor(playerid, phone_block_PTD[playerid][2], -1);
	PlayerTextDrawFont(playerid, phone_block_PTD[playerid][2], 2);
	PlayerTextDrawSetProportional(playerid, phone_block_PTD[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, phone_block_PTD[playerid][2], 0);

	PlayerTextDrawAlignment(playerid, phone_block_PTD[playerid][3], 2);
	PlayerTextDrawColor(playerid, phone_block_PTD[playerid][3], -1);
	PlayerTextDrawFont(playerid, phone_block_PTD[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, phone_block_PTD[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, phone_block_PTD[playerid][3], 0);

	// status bar ¶î
	PlayerTextDrawAlignment(playerid, phone_home_PTD[playerid][0], 2);
	PlayerTextDrawColor(playerid, phone_home_PTD[playerid][0], 0x2B2B2BFF);
	PlayerTextDrawFont(playerid, phone_home_PTD[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, phone_home_PTD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, phone_home_PTD[playerid][0], 0);

	PlayerTextDrawAlignment(playerid, phone_home_PTD[playerid][1], 1);
	PlayerTextDrawColor(playerid, phone_home_PTD[playerid][1], -1);
	PlayerTextDrawFont(playerid, phone_home_PTD[playerid][1], 4);
	PlayerTextDrawSetProportional(playerid, phone_home_PTD[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, phone_home_PTD[playerid][1], 0);

	PlayerTextDrawAlignment(playerid, phone_home_PTD[playerid][2], 1);
	PlayerTextDrawColor(playerid, phone_home_PTD[playerid][2], -1);
	PlayerTextDrawFont(playerid, phone_home_PTD[playerid][2], 4);
	PlayerTextDrawSetProportional(playerid, phone_home_PTD[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, phone_home_PTD[playerid][2], 0);
}
