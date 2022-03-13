#include <a_samp>
#include <streamer>
#define MAX_ADDOBJECTS      		100000
#define MAX_ADD3DTEXTS              500
#define MAX_3DTEXTS_STRING          512
#define MAX_REMOVEOBJ 				800
#define MAX_TIPS                    500
#define MAX_MESSAGE_COUNT           50
#define MAX_TELEPORT                500
#define ADDOBJECTS_FileLocation     "addobjects/addobjects.txt"
#define MAX_ADDTXT                  50
#define MAX_MOVEOBJECTS             MAX_ADDTXT
enum removeobjtype
{
	ModelID,
	Float:X,
	Float:Y,
	Float:Z,
	Float:R
};
enum add3dtextstype
{
	String[MAX_3DTEXTS_STRING],
	Float:X,
	Float:Y,
	Float:Z,
	Text3D:Tag3DText
};
enum addobjectstype
{
    OBJID,
	Float:X,
	Float:Y,
	Float:Z,
	Float:RX,
	Float:RY,
	Float:RZ,
	Float:Range,
	TagID
};
enum moveobjecttype
{
	move_id,
	Float:move_x,
	Float:move_y,
	Float:move_z,
	Float:move_speed,
	Float:move_rx,
	Float:move_ry,
	Float:move_rz,
	Float:move_range,
	move_use
};
enum teleporttype
{
	// o = outside, i = inside
	Float:oX,
	Float:oY,
	Float:oZ,
	oInterior,
	oText[MAX_PLAYER_NAME],
	Float:iX,
	Float:iY,
	Float:iZ,
	iInterior,
	iText[MAX_PLAYER_NAME],
	TagPickupID[2],
	Text3D:TagSDTextID[2]
};
enum addtxttype
{
	txt_id,
	txt_name[MAX_PLAYER_NAME]
};
new MoveObjects[MAX_MOVEOBJECTS][moveobjecttype],AddTeleport[MAX_TELEPORT][teleporttype];
new AddObjects[MAX_ADDOBJECTS][addobjectstype],Add3DTexts[MAX_ADD3DTEXTS][add3dtextstype],p_RemoveObj[MAX_REMOVEOBJ][removeobjtype],addtxt[MAX_ADDTXT][addtxttype];
new g_objs=0,g_3dtexts=0,g_removeobj=0,g_addtxt = 0,g_material = 0,g_moveobj = 0,g_materialtext = 0,g_tips = 0,g_teleport = 0;
new obj_distance;
public OnFilterScriptInit()
{
    AddobjectsBegin(1);
	return 1;
}
public OnFilterScriptExit()
{
	return 1;
}
public OnPlayerConnect(playerid)
{
	for(new id = 0; id < g_removeobj; id++)
	{
		if(p_RemoveObj[id][ModelID] != 0)
		{
			RemoveBuildingForPlayer(playerid, p_RemoveObj[id][ModelID], p_RemoveObj[id][X], p_RemoveObj[id][Y], p_RemoveObj[id][Z], p_RemoveObj[id][R]);
		}
	}
	return 1;
}
forward Teleport_Ms(playerid);
public Teleport_Ms(playerid)
{
	TogglePlayerControllable(playerid,1);
	PlayerPlaySound(playerid,1057,0.0,0.0,0.0);
	return 1;
}
forward NearestTeleport(playerid, type);
public NearestTeleport(playerid, type)
{
	new id = -1;
	for(new i = 0; i < g_teleport; i++)
	{
        if(type == 0 && IsPlayerInRangeOfPoint(playerid, 1.5, AddTeleport[i][oX], AddTeleport[i][oY], AddTeleport[i][oZ]))
        {
            TogglePlayerControllable(playerid, 0);
            SetPlayerInterior(playerid, AddTeleport[i][iInterior]);
            SetPlayerPos(playerid, AddTeleport[i][iX], AddTeleport[i][iY], AddTeleport[i][iZ]);
            SetTimerEx("Teleport_Ms", 3000, false, "i", playerid);
			i = g_teleport + 1;
		}
		if(type == 1 && IsPlayerInRangeOfPoint(playerid, 1.5, AddTeleport[i][iX], AddTeleport[i][iY], AddTeleport[i][iZ]))
        {
            SetPlayerInterior(playerid, AddTeleport[i][oInterior]);
			SetPlayerPos(playerid, AddTeleport[i][oX], AddTeleport[i][oY], AddTeleport[i][oZ]);
			i = g_teleport + 1;
		}
	}
	return id;
}
forward AutomaticDoor(playerid, oid, level);
public AutomaticDoor(playerid, oid, level)
{
	new id;
	for(new i = 0; i < g_moveobj; i++)
	{
		if(MoveObjects[i][move_id] != 0 && IsPlayerInRangeOfPoint(playerid, MoveObjects[i][move_range], AddObjects[MoveObjects[i][move_id]][X], AddObjects[MoveObjects[i][move_id]][Y], AddObjects[MoveObjects[i][move_id]][Z]))
		{
			id = MoveObjects[i][move_id];
			if(MoveObjects[i][move_use] == 0)
			{
				MoveDynamicObject(AddObjects[id][TagID], MoveObjects[i][move_x], MoveObjects[i][move_y], MoveObjects[i][move_z], MoveObjects[i][move_speed], MoveObjects[i][move_rx], MoveObjects[i][move_ry], MoveObjects[i][move_rz]);
           	    MoveObjects[i][move_use] = 1;
			}
			else if(MoveObjects[i][move_use] == 1)
			{
			    MoveDynamicObject(AddObjects[id][TagID], AddObjects[id][X], AddObjects[id][Y], AddObjects[id][Z], MoveObjects[i][move_speed], AddObjects[id][RX], AddObjects[id][RY], AddObjects[id][RZ]);
		   	 	MoveObjects[i][move_use] = 0;
			}
		}
	}
	return 1;
}
stock a_ReplaceChar(string[],ch = '*',ch2 = '_')
{
	new t_string[1024];
	for(new i = 0; i < strlen(string); i++)
	{
		if(string[i] == ch) string[i] = '\n';
		if(string[i] == ch2) string[i] = ' ';
	}
	format(t_string, sizeof t_string, string);
	return t_string;
}
stock AddRemoveObj(value[])
{
	new id = -1;
	for(new i = 0; i < MAX_REMOVEOBJ; i++)
	{
		if(p_RemoveObj[i][ModelID] == 0)
		{
			id = i;
			i = MAX_REMOVEOBJ + 1;
		}
	}
	if(id==-1) return ErrorPrintf("addobjects.pwn >> 删除OBJ数量到达上限,请及时修改MAX_REMOVEOBJ");
	new idx = 0;
	strval(strtok(value,idx));
	p_RemoveObj[id][ModelID] = strval(strtok(value,idx));
	p_RemoveObj[id][X] = floatstr(strtok(value,idx));
	p_RemoveObj[id][Y] = floatstr(strtok(value,idx));
	p_RemoveObj[id][Z] = floatstr(strtok(value,idx));
	p_RemoveObj[id][R] = floatstr(strtok(value,idx));
	g_removeobj ++;
	return 1;
}
stock Add_3DTexts(info[])
{
	new x = -1;
	for(new i = 0; i < MAX_ADD3DTEXTS; i++)
	{
		if(IsValidDynamic3DTextLabel(Add3DTexts[i][Tag3DText]) == 0)
		{
			x = i;
			i = MAX_ADD3DTEXTS + 1;
		}
	}
	if(x == -1) return ErrorPrintf("addobjects.pwn >> 3DText数量达到上限,请及时修改MAX_ADD3DTEXTS");
	new idx = 0,msg[128];
	msg = strtok(info,idx);
	format(Add3DTexts[x][String],MAX_3DTEXTS_STRING,"%s",a_ReplaceChar(strtok(info,idx)));
	msg = strtok(info,idx);
	Add3DTexts[x][X] = floatstr(msg);
	msg = strtok(info,idx);
	Add3DTexts[x][Y] = floatstr(msg);
	msg = strtok(info,idx);
	Add3DTexts[x][Z] = floatstr(msg);
	Add3DTexts[x][Tag3DText] = CreateDynamic3DTextLabel(Add3DTexts[x][String], 0xFFFFFFFF, Add3DTexts[x][X], Add3DTexts[x][Y], Add3DTexts[x][Z], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
    g_3dtexts ++;
	return 1;
}
stock Add_Objects(info[])
{
	new x = -1;
	for(new i = 0; i < MAX_ADDOBJECTS; i++)
	{
		if(AddObjects[i][OBJID] == 0)
		{
			x = i;
			i = MAX_ADDOBJECTS + 1;
		}
	}
	if(x == -1) return ErrorPrintf("addobjects.pwn >> Objects数量达到上限,请及时修改MAX_ADDOBJECTS");
	new idx = 0,msg[128];
	msg = strtok(info,idx);
	msg = strtok(info,idx);
	AddObjects[x][OBJID] = strval(msg);
	msg = strtok(info,idx);
	AddObjects[x][X] = floatstr(msg);
	msg = strtok(info,idx);
	AddObjects[x][Y] = floatstr(msg);
	msg = strtok(info,idx);
	AddObjects[x][Z] = floatstr(msg);
	msg = strtok(info,idx);
	AddObjects[x][RX] = floatstr(msg);
	msg = strtok(info,idx);
	AddObjects[x][RY] = floatstr(msg);
	msg = strtok(info,idx);
	AddObjects[x][RZ] = floatstr(msg);
	msg = strtok(info,idx);
	AddObjects[x][Range] = floatstr(msg);
	if(AddObjects[x][Range] < 5) AddObjects[x][Range] = 300.0;
	if(obj_distance != 0)
	{
		AddObjects[x][TagID] = CreateDynamicObjectEx(AddObjects[x][OBJID],AddObjects[x][X],AddObjects[x][Y],AddObjects[x][Z],AddObjects[x][RX],AddObjects[x][RY],AddObjects[x][RZ],obj_distance,obj_distance);
	}
	else
	{
        AddObjects[x][TagID] = CreateDynamicObjectEx(AddObjects[x][OBJID],AddObjects[x][X],AddObjects[x][Y],AddObjects[x][Z],AddObjects[x][RX],AddObjects[x][RY],AddObjects[x][RZ],AddObjects[x][Range],AddObjects[x][Range]);
	}
	g_objs ++;
	return 1;
}
stock Add_MoveObj(info[])
{
	new x = -1, id = -1;
	for(new i = 0; i < MAX_ADDOBJECTS; i++)
	{
		if(AddObjects[i][OBJID] == 0)
		{
			x = i-1;
			i = MAX_ADDOBJECTS + 1;
		}
	}
	for(new i = 0; i < MAX_MOVEOBJECTS; i++)
	{
		if(MoveObjects[i][move_id] == 0)
		{
			id = i;
			i = MAX_MOVEOBJECTS + 1;
		}
	}
	if(x == -1 || id == -1) return ErrorPrintf("addobjects.pwn >> Objects数量达到上限,请及时修改MAX_ADDOBJECTS");
	new idx = 0,msg[128];
	msg = strtok(info, idx);
	MoveObjects[id][move_id] = x;
	msg = strtok(info, idx);
	MoveObjects[id][move_x] = floatstr(msg);
	msg = strtok(info, idx);
	MoveObjects[id][move_y] = floatstr(msg);
	msg = strtok(info, idx);
	MoveObjects[id][move_z] = floatstr(msg);
	msg = strtok(info, idx);
	MoveObjects[id][move_speed] = floatstr(msg);
	msg = strtok(info, idx);
	MoveObjects[id][move_rx] = floatstr(msg);
	msg = strtok(info, idx);
	MoveObjects[id][move_ry] = floatstr(msg);
	msg = strtok(info, idx);
	MoveObjects[id][move_rz] = floatstr(msg);
	msg = strtok(info, idx);
	MoveObjects[id][move_range] = floatstr(msg);
	if(MoveObjects[id][move_range] < 2) MoveObjects[id][move_range] = 2;
	MoveObjects[id][move_use] = 0;
	g_moveobj ++;
	return 1;
}
stock Add_MaterialText(info[])
{
	new x = -1;
	for(new i = 0; i < MAX_ADDOBJECTS; i++)
	{
		if(AddObjects[i][OBJID] == 0)
		{
			x = i-1;
			i = MAX_ADDOBJECTS + 1;
		}
	}
	if(x == -1) return ErrorPrintf("addobjects.pwn >> Objects数量达到上限,请及时修改MAX_ADDOBJECTS");
	new idx = 0, msg[128], text_index, text_text[MAX_PLAYER_NAME], text_size, text_fontface[MAX_PLAYER_NAME], text_fontsize, text_bold, text_fontcolor, text_backcolor, text_alignment;
	msg = strtok(info, idx);
	msg = strtok(info, idx);
	text_index = strval(msg);
	format(text_text, MAX_PLAYER_NAME, strtok(info, idx));
	msg = strtok(info,idx);
	text_size = strval(msg);
	format(text_fontface, MAX_PLAYER_NAME, strtok(info, idx));
	msg = strtok(info,idx);
	text_fontsize = strval(msg);
 	msg = strtok(info,idx);
    text_bold = strval(msg);
    msg = strtok(info,idx);
    text_fontcolor = strval(msg);
    msg = strtok(info,idx);
    text_backcolor = strval(msg);
    msg = strtok(info,idx);
    text_alignment = strval(msg);
	SetDynamicObjectMaterialText(AddObjects[x][TagID], text_index, text_text,  text_size, text_fontface, text_fontsize, text_bold, text_fontcolor, text_backcolor, text_alignment);
	g_materialtext ++;
	return 1;
}
stock Add_Material(info[])
{
	new x = -1;
	for(new i = 0; i < MAX_ADDOBJECTS; i++)
	{
		if(AddObjects[i][OBJID] == 0)
		{
			x = i-1;
			i = MAX_ADDOBJECTS + 1;
		}
	}
	if(x == -1) return ErrorPrintf("addobjects.pwn >> Objects数量达到上限,请及时修改MAX_ADDOBJECTS");
	new idx = 0,msg[128],index,modelid,txtname[MAX_PLAYER_NAME],texturename[MAX_PLAYER_NAME];
	msg = strtok(info,idx);
	msg = strtok(info,idx);
	index = strval(msg);
	msg = strtok(info,idx);
	modelid = strval(msg);
	format(txtname,MAX_PLAYER_NAME,strtok(info,idx));
	format(texturename,MAX_PLAYER_NAME,strtok(info,idx));
	msg = strtok(info,idx);
	SetDynamicObjectMaterial(AddObjects[x][TagID], index, modelid, txtname, texturename, strval(msg));
	g_material ++;
	return 1;
}
stock Add_Teleport(info[])
{
	new id = -1;
	for(new i = 0; i < MAX_TELEPORT; i++)
	{
		if(IsValidDynamicPickup(AddTeleport[i][TagPickupID][0]) == 0)
		{
			id = i;
			i = MAX_TELEPORT + 1;
		}
	}
	if(id == -1) return ErrorPrintf("addobjects.pwn >> Teleport数量达到上限,请及时修改MAX_TELEPORT");
	new idx = 0, msg[128];
	msg = strtok(info, idx);
	msg = strtok(info, idx);
	AddTeleport[id][oX] = floatstr(msg);
	msg = strtok(info, idx);
	AddTeleport[id][oY] = floatstr(msg);
	msg = strtok(info, idx);
	AddTeleport[id][oZ] = floatstr(msg);
	msg = strtok(info, idx);
	AddTeleport[id][oInterior] = strval(msg);
	format(AddTeleport[id][oText], MAX_PLAYER_NAME, strtok(info, idx));
	msg = strtok(info, idx);
	AddTeleport[id][iX] = floatstr(msg);
	msg = strtok(info, idx);
	AddTeleport[id][iY] = floatstr(msg);
	msg = strtok(info, idx);
	AddTeleport[id][iZ] = floatstr(msg);
	msg = strtok(info, idx);
	AddTeleport[id][iInterior] = strval(msg);
	format(AddTeleport[id][iText], MAX_PLAYER_NAME, strtok(info, idx));
	AddTeleport[id][TagPickupID][0] = CreateDynamicPickupEx(1239, 1, AddTeleport[id][oX], AddTeleport[id][oY], AddTeleport[id][oZ], 15.0);
	AddTeleport[id][TagPickupID][1] = CreateDynamicPickupEx(1239, 1, AddTeleport[id][iX],AddTeleport[id][iY], AddTeleport[id][iZ], 15.0);
	format(msg, sizeof msg, "%s\n{E5E5E5}/enter", AddTeleport[id][oText]);
	AddTeleport[id][TagSDTextID][0] = CreateDynamic3DTextLabel(msg, 0xFFFFFFFF, AddTeleport[id][oX], AddTeleport[id][oY], AddTeleport[id][oZ], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	format(msg, sizeof msg, "%s\n{E5E5E5}/exit", AddTeleport[id][iText]);
	AddTeleport[id][TagSDTextID][1] = CreateDynamic3DTextLabel(msg, 0xFFFFFFFF, AddTeleport[id][iX], AddTeleport[id][iY], AddTeleport[id][iZ], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	g_teleport ++;
	return 1;
}
stock Initialization_Addobjects()
{
    printf("addobjects.pwn >> 初始化中.");
	for(new i = 0; i <= g_objs; i++)
	{
		if(AddObjects[i][OBJID] != 0)
		{
			if(IsValidDynamicObject(AddObjects[i][TagID]) == 1)
			{
            	DestroyDynamicObject(AddObjects[i][TagID]);
			}
			AddObjects[i][OBJID] = 0;
		}
	}
	for(new i = 0; i <= g_3dtexts; i++)
	{
        if(IsValidDynamic3DTextLabel(Add3DTexts[i][Tag3DText]) == 1)
		{
		    DestroyDynamic3DTextLabel(Add3DTexts[i][Tag3DText]);
		}
	}
	for(new id = 0; id <= g_removeobj; id++)
	{
		p_RemoveObj[id][ModelID] = 0;
	}
	for(new i = 0; i < MAX_ADDTXT; i++)
	{
		addtxt[i][txt_id] = 0;
	}
	for(new i = 0; i < MAX_MOVEOBJECTS; i++)
	{
        MoveObjects[i][move_id] = 0;
	}
	for(new i = 0; i <= g_teleport; i++)
	{
	    for(new c = 0; c < 2; c++)
	    {
			if(IsValidDynamic3DTextLabel(AddTeleport[i][TagSDTextID][c]) == 1)
			{
			    DestroyDynamic3DTextLabel(AddTeleport[i][TagSDTextID][c]);
			}
			if(IsValidDynamicPickup(AddTeleport[i][TagPickupID][c]) == 1)
			{
				DestroyDynamicPickup(AddTeleport[i][TagPickupID][c]);
			}
		}
	}
	g_tips = 0;
	g_removeobj = 0;
	g_objs = 0;
	g_3dtexts = 0;
	g_addtxt = 0;
	g_material = 0;
	g_moveobj = 0;
	g_materialtext = 0;
	g_teleport = 0;
	printf("addobjects.pwn >> 初始化完毕.");
}
forward AddobjectsBegin(dylan);
public AddobjectsBegin(dylan)
{
    Initialization_Addobjects();
    new msg[128];
	if(fexist(ADDOBJECTS_FileLocation)==1)
	{
		new File:m=fopen(ADDOBJECTS_FileLocation,io_read),info[128],idx=0,x=-1;
  		while(fread(m,info))
    	{
			msg = strtok(info,idx);
			if(mk_strcmp(msg,"addtxt")==0)
			{
			    x = -1;
				for(new i = 0; i < MAX_ADDTXT; i++)
				{
					if(addtxt[i][txt_id] == 0)
					{
						x = i;
						i = MAX_ADDTXT + 1;
					}
				}
				if(x == -1) return ErrorPrintf("addobjects.pwn >> 文档数量到达上限,请及时修改MAX_ADDTXT");
				addtxt[x][txt_id] = x + 1;
				format(addtxt[x][txt_name],MAX_PLAYER_NAME,strtok(info,idx));
				g_addtxt ++;
			}
			idx = 0;
		}
		fclose(m);
	}
	for(new i = 0; i < MAX_ADDTXT; i++)
	{
	    if(addtxt[i][txt_id] != 0)
	    {
	        obj_distance = 0;
	        format(msg,sizeof(msg),"addobjects/txt/%s.txt",addtxt[i][txt_name]);
			if(fexist(msg)==1)
			{
				new File:m = fopen(msg,io_read),info[512],idx=0;
	  			while(fread(m,info))
	    		{
					msg = strtok(info,idx);
					if(mk_strcmp(msg,"distance")==0)
					{
						msg = strtok(info ,idx);
						obj_distance = strval(msg);
					}
					if(mk_strcmp(msg,"obj")==0) Add_Objects(info);
					if(mk_strcmp(msg,"3dtext")==0) Add_3DTexts(info);
					if(mk_strcmp(msg,"removeobj")==0) AddRemoveObj(info);
					if(mk_strcmp(msg,"material")==0) Add_Material(info);
					if(mk_strcmp(msg,"moveobj")==0) Add_MoveObj(info);
					if(mk_strcmp(msg,"materialtext")==0) Add_MaterialText(info);
					if(mk_strcmp(msg,"teleport")==0) Add_Teleport(info);
					idx = 0;
				}
				fclose(m);
			}
			else
			{
			    format(msg,sizeof(msg),"addobjects.pwn >> 错误没有找到文档[%s],或[%s]文档数量有两个以上.",addtxt[i][txt_name],addtxt[i][txt_name]);
            	ErrorPrintf(msg);
            	return 1;
			}
		}
	}
	printf("addobjects.pwn >> 读取文档[%d]个,剩余可用[%d]个",g_addtxt,MAX_ADDTXT-g_addtxt);
	printf("addobjects.pwn >> 读取OBJ[%d]个,剩余可用[%d]个",g_objs,MAX_ADDOBJECTS-g_objs);
	printf("addobjects.pwn >> 读取纹理[%d]个,剩余可用[无上限].",g_material);
	printf("addobjects.pwn >> 读取3DText[%d]个,剩余可用[%d]个",g_3dtexts,MAX_ADD3DTEXTS-g_3dtexts);
	printf("addobjects.pwn >> 读取删除OBJ[%d]个,剩余可用[%d]个",g_removeobj,MAX_REMOVEOBJ-g_removeobj);
	printf("addobjects.pwn >> 读取移动OBJ[%d]个,剩余可用[%d]个",g_moveobj,MAX_MOVEOBJECTS-g_moveobj);
	printf("addobjects.pwn >> 读取纹理文本[%d]个,剩余可用[无上限]",g_materialtext);
	printf("addobjects.pwn >> 读取提示[%d]个,剩余可用[%d]个",g_tips,MAX_TIPS-g_tips);
	printf("addobjects.pwn >> 读取传送[%d]个,剩余可用[%d]个",g_teleport,MAX_TELEPORT-g_teleport);
	return 1;
}
stock ErrorPrintf(txt[])
{
	for(new i = 0; i < 50000; i++)
	{
		printf(txt);
	}
	return 1;
}
stock mk_strcmp(const string1[], const string2[], bool:ignorecase=false, length=cellmax)
{
	new mbcs;
 	for( new i=0; i<length; i++ )
    {
		new c1=string1[i], c2=string2[i];
 		if( c1 < 0 ) c1+=256;
		if( c2 < 0 ) c2+=256;
        if( ignorecase && c1 <= 0x7F && c2 <= 0x7F && mbcs==0 )
		{
			c1 = tolower(c1);
			c2 = tolower(c2);
		}
		if(mbcs==1) mbcs=0;
  		else if( c1 > 0x7F || c2 > 0x7F ) mbcs=1;
  		if( c1 != c2 || (c1==0 && c2==0) ) return c1-c2;
	}
	return 0;
}
stock dystrtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	new offset = index;
	new result[128];
	while ((index < length)  && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
stock strtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	new offset = index;
	new result[128];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
