#define app_settings 0
#define app_call 1
#define app_contacts 2
#define app_sms 3

new
	PlayerText: phone_apps_PTD[MAX_PLAYERS][max_apps_slots];

enum apps_enum
{
	apps_name[20],
	apps_model[30]
}
static const apps_data[][apps_enum] =
{
	{ "设置", "apps:settings" },
	{ "电话", "apps:call" },
	{ "通讯录", "apps:contacts" },
	{ "短信", "apps:sms" }
};

ShowPhoneApps(playerid)
{
	new
		app_id,
		str[37];

	for(new i; i < phone_data[playerid][phone_max_apps]; i ++)
	{
		if((app_id = phone_data[playerid][phone_apps][i]) == -1) continue;
		format(str, 37, "%s_%d", apps_data[app_id][apps_model], 1);

		PlayerTextDrawSetString(playerid, phone_apps_PTD[playerid][i], str);
		PlayerTextDrawColor(playerid, phone_apps_PTD[playerid][i], -1);

		PlayerTextDrawShow(playerid, phone_apps_PTD[playerid][i]);
	}
}
Phone_OpenApp(playerid, id)
{
	switch(phone_data[playerid][phone_apps][id])
	{
		case app_contacts: ShowPlayerContactMenu(playerid);
		case app_settings: ShowPlayerDialog(playerid, DIALOG_PHONE_SETTING, DIALOG_STYLE_TABLIST_HEADERS, ""#TEXT_WHITE"设定", ""#TEXT_WHITE"设定项目\n飞行模式\n切换静音\n删除所有短信", "选择", "返回");
		default: SendClientMessage(playerid, COLOR_WHITE, "错误: 未开发!");
	}
	return true;
}

DestroyPhoneAppsTextdraws(playerid)
{
	for(new i; i < phone_data[playerid][phone_max_apps]; i ++) PlayerTextDrawDestroy(playerid, phone_apps_PTD[playerid][i]);
}
CreatePhoneAppsTextdraws(playerid, x, y, model[])
{
	if(GetString(model, "iPhone"))
	{
		phone_apps_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 510.8668 + x, 171.5926 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 529.9625 + x, 171.5926 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 549.2578 + x, 171.5926 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 569.9528 + x, 171.5926 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][4] = CreatePlayerTextDraw(playerid, 510.8668 + x, 191.4939 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][5] = CreatePlayerTextDraw(playerid, 529.9625 + x, 191.4939 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][6] = CreatePlayerTextDraw(playerid, 549.2578 + x, 191.4939 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][7] = CreatePlayerTextDraw(playerid, 569.9528 + x, 191.4939 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][8] = CreatePlayerTextDraw(playerid, 510.8668 + x, 212.1951 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][9] = CreatePlayerTextDraw(playerid, 529.9625 + x, 212.1951 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][10] = CreatePlayerTextDraw(playerid, 549.2578 + x, 212.1951 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][11] = CreatePlayerTextDraw(playerid, 569.9528 + x, 212.1951 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][12] = CreatePlayerTextDraw(playerid, 510.8668 + x, 232.0963 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][13] = CreatePlayerTextDraw(playerid, 529.9625 + x, 232.0963 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][14] = CreatePlayerTextDraw(playerid, 549.2578 + x, 232.0963 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][15] = CreatePlayerTextDraw(playerid, 569.9528 + x, 232.0963 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][16] = CreatePlayerTextDraw(playerid, 510.8668 + x, 252.0976 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][17] = CreatePlayerTextDraw(playerid, 529.9625 + x, 252.0976 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][18] = CreatePlayerTextDraw(playerid, 549.2578 + x, 252.0976 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][19] = CreatePlayerTextDraw(playerid, 569.9528 + x, 252.0976 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][20] = CreatePlayerTextDraw(playerid, 510.8668 + x, 271.9988 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][21] = CreatePlayerTextDraw(playerid, 529.9625 + x, 271.9988 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][22] = CreatePlayerTextDraw(playerid, 549.2578 + x, 271.9988 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][23] = CreatePlayerTextDraw(playerid, 569.9528 + x, 271.9988 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][24] = CreatePlayerTextDraw(playerid, 510.8668 + x, 292.7000 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][25] = CreatePlayerTextDraw(playerid, 529.9625 + x, 292.7000 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][26] = CreatePlayerTextDraw(playerid, 549.2578 + x, 292.7000 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][27] = CreatePlayerTextDraw(playerid, 569.9528 + x, 292.7000 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][28] = CreatePlayerTextDraw(playerid, 510.8668 + x, 312.6012 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][29] = CreatePlayerTextDraw(playerid, 529.9625 + x, 312.6012 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][30] = CreatePlayerTextDraw(playerid, 549.2578 + x, 312.6012 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][31] = CreatePlayerTextDraw(playerid, 569.9528 + x, 312.6012 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][32] = CreatePlayerTextDraw(playerid, 510.8668 + x, 332.9025 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][33] = CreatePlayerTextDraw(playerid, 529.9625 + x, 332.9025 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][34] = CreatePlayerTextDraw(playerid, 549.2578 + x, 332.9025 + y, "mdl-5000:circle");
		phone_apps_PTD[playerid][35] = CreatePlayerTextDraw(playerid, 569.9528 + x, 332.9025 + y, "mdl-5000:circle");

		for(new i; i < 36; i ++)
		{
			PlayerTextDrawTextSize(playerid, phone_apps_PTD[playerid][i], 15.0000, 15.0000);
			PlayerTextDrawAlignment(playerid, phone_apps_PTD[playerid][i], 1);
			PlayerTextDrawColor(playerid, phone_apps_PTD[playerid][i], -1);
			PlayerTextDrawFont(playerid, phone_apps_PTD[playerid][i], 4);
			PlayerTextDrawSetProportional(playerid, phone_apps_PTD[playerid][i], 0);
			PlayerTextDrawSetShadow(playerid, phone_apps_PTD[playerid][i], 0);
			PlayerTextDrawSetSelectable(playerid, phone_apps_PTD[playerid][i], true);
		}
	}
}
