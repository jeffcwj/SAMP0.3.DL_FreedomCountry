#define max_apps_slots 36
#define max_phone_wallpapers 7

#define phone_type_home 1
#define phone_type_load 2
#define phone_type_block 3

#define phone_type_contacts 4
#define phone_type_contact_info 5
#define phone_type_contact_add 6

#define phone_type_settings 7

GetString(param_1[], param_2[])return !strcmp(param_1, param_2, false);

enum phone_enum
{
	phone_id,
	phone_status,

	phone_model[40],
	phone_number,
	phone_apps[max_apps_slots],
	phone_max_apps,

	phone_pos[2],
	phone_block,

	phone_avatar[25]
}
new
	phone_data[MAX_PLAYERS][phone_enum],
	
	phone_timing[MAX_PLAYERS];

#include "phone/Apps/apps"

#include "phone/loading"
#include "phone/home"
#include "phone/textdraws"

#include "phone/opens"
#include "phone/funcs"

Phone_Close(playerid, type = 0, timing = 1)
{
	if(!phone_opened[playerid])return false;

	switch(phone_opened[playerid])
	{
		case phone_type_home:
		{
			for(new i; i < phone_data[playerid][phone_max_apps]; i ++) PlayerTextDrawHide(playerid, phone_apps_PTD[playerid][i]);
		}
		case phone_type_block:
		{
			PlayerTextDrawHide(playerid, phone_block_PTD[playerid][0]);
			PlayerTextDrawHide(playerid, phone_block_PTD[playerid][1]);
			PlayerTextDrawHide(playerid, phone_block_PTD[playerid][2]);
			PlayerTextDrawHide(playerid, phone_block_PTD[playerid][3]);

			PlayerTextDrawSetSelectable(playerid, phone_PTD[playerid][0], false);
		}
	}

	phone_opened[playerid] = 0;
	if(!type)
	{
		DestroyPhoneTextdraws(playerid);
		DestroyPhoneAppsTextdraws(playerid);

		CancelSelectTextDraw(playerid);
		if(timing) phone_timing[playerid] = gettime() + 7;
	}
	return true;
}
Phone_Open(playerid, home = 1)
{
	if(phone_opened[playerid]
		|| phone_timing[playerid] > gettime())
	{
		SendClientMessage(playerid, COLOR_WHITE, "错误：手机已打开或重新打开的速度太快!");
		return false;
	}
	phone_opened[playerid] = 0;
	SetTimerEx("@_load_phone", 200, false, "dd", playerid, home);
	return true;
}

@_load_phone(playerid, home);
@_load_phone(playerid, home)
{
	CreatePhoneTextdraws(playerid, "iPhone");
	if(home) Phone_OpenHome(playerid, 1);
	ShowPlayerPhoneMenu(playerid);
	return true;
}
