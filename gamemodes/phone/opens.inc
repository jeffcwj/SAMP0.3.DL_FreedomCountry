Phone_OpenSettings(playerid)
{
	ShowPlayerDialog(playerid, DIALOG_PHONE_SETTING, DIALOG_STYLE_TABLIST_HEADERS, ""#TEXT_WHITE"设定", ""#TEXT_WHITE"设定项目\n飞行模式\n切换静音\n删除所有短信", "选择", "返回");
	return true;
}

Phone_OpenContacts(playerid)
{
	ShowPlayerContactMenu(playerid);
	return true;
}

Phone_OpenLoading(playerid, type, time = 70)
{
	if(phone_load_enable[playerid])return false;

	for(new i; i < 9; i ++) PlayerTextDrawShow(playerid, phone_load_PTD[playerid][i]);

	phone_load_count[playerid] =
	phone_load_list[playerid] = 0;

	phone_load_enable[playerid] = true;
	phone_load_timer[playerid] = SetTimerEx("@_loading_phone", time, true, "dd", playerid, type);

	phone_opened[playerid] = phone_type_load;
	return true;
}

Phone_OpenHome(playerid, type = 0)
{
	if(phone_opened[playerid]) Phone_Close(playerid, 1);

	phone_opened[playerid] = phone_type_home;
	if(phone_data[playerid][phone_status])
	{
		PlayerTextDrawColor(playerid, phone_PTD[playerid][0], -1);
		PlayerTextDrawSetSelectable(playerid, phone_PTD[playerid][0], false);
		PlayerTextDrawSetString(playerid, phone_PTD[playerid][0], "mdl-5004:wallpaper_1");
		PlayerTextDrawShow(playerid, phone_PTD[playerid][0]);
		if(type)
		{
			static const phone_month_name[][] = { "-", "1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月" };

			new
				day,
				month,
				hour,
				minute,

				str[20];

			getdate(_, month, day);
			gettime(hour, minute);

			format(str, 10, "%d:%d", hour, minute);
			PlayerTextDrawSetString(playerid, phone_block_PTD[playerid][2], str);

			format(str, 20, "%s%d号", phone_month_name[month], day);
			PlayerTextDrawSetString(playerid, phone_block_PTD[playerid][3], str);

			PlayerTextDrawShow(playerid, phone_block_PTD[playerid][0]);
			PlayerTextDrawShow(playerid, phone_block_PTD[playerid][1]);
			PlayerTextDrawShow(playerid, phone_block_PTD[playerid][2]);
			PlayerTextDrawShow(playerid, phone_block_PTD[playerid][3]);

			PlayerTextDrawSetSelectable(playerid, phone_PTD[playerid][0], true);
			phone_opened[playerid] = phone_type_block;
		}
		else
		{
			ShowPhoneApps(playerid);
			ShowStatusBar(playerid);
		}
	}
	else
	{
		PlayerTextDrawSetString(playerid, phone_PTD[playerid][0], "ld_spac:white");

		PlayerTextDrawColor(playerid, phone_PTD[playerid][0], 0x222222FF);
		PlayerTextDrawSetSelectable(playerid, phone_PTD[playerid][0], true);
	}

	PlayerTextDrawShow(playerid, phone_PTD[playerid][0]);
	PlayerTextDrawShow(playerid, phone_PTD[playerid][2]);

	SelectTextDraw(playerid, 0x4695CEFF);
	return true;
}
