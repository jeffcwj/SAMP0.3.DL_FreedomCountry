/*
	Include: n_ini
	Brief: Simple, fast, easy INI-File manager. Generally used for saving stuff!
	Update: v1 (29/11/14)
	Creator: Neufox

    Credits:
		* Thanks to Neufox        -   Creating this script
		* Thanks to [DRuG]Slick   -   Base SII include
		* Thanks to SAMP Team     -   You know why i stated this!

	Converts:
	    * DINI
	    * DUDB
	    * SFM(Southclaw's File Manager)
	    * FINI
	    * SII
	    * SF(Simple File)
	    * dFile
	- If you want to have more conversions, then post down your suggestion in main topic page!

	NOTE: Checkout line (715 - 716) if you want to edit UDB Settings.
		  Line (60 - 65), some configurations!

*/
#include <a_samp>

/*
native INI_Create(const filename[]);
native INI_Remove(const filename[]);
native INI_Open(const filename[]);
native INI_Close();
native INI_Save();
native INI_Rename(const newname[]);
native INI_Copy(const copyname[]);
native INI_WriteString(const key[], const value[]);
native INI_WriteInt(const key[], value);
native INI_WriteFloat(const key[], Float: value);
native INI_WriteBool(const key[], bool:value);
native INI_ReadString(const key[]);
native INI_ReadInt(const key[]);
native Float:INI_ReadFloat(const key[]);
native bool:INI_ReadBool(const key[]);
native INI_MultiWrite(format[], {Float,_}:...);
native INI_MultiRead(format[],{Float,_}:...);
native INI_RemoveKey(const key[]);
native INI_KeyExist(const key[]);
native INI_ReturnFile();
native INI_ReturnFilename();
*/

#pragma dynamic 45000 // Change accordingly!!

#if defined MAX_STRING
	#define INI_MAX_VALUE MAX_STRING
#else
	#define INI_MAX_VALUE 256
#endif

//Some configs
#define INI_MAX_KEY 		24//maximum number of keys/entries in a file
#define INI_MAX_LINE 		(INI_MAX_KEY + INI_MAX_VALUE + 3)
#define INI_MAX_FILENAME 	256//the max file name string can be!
#define INI_MAX_LINES 		256
#define INI_AUTO_SAVE       true//true = Automatically saves and write cache to file on closing
#define INI_CONVERT         true//true = Converts all other ini/file systems to n_ini automatically
//

enum E_CACHE
{
	E_KEY[INI_MAX_KEY],
	E_VALUE[INI_MAX_VALUE]
}

enum E_FILE
{
	E_FILENAME[INI_MAX_FILENAME],
	bool:E_OPEN
}

static gCache[INI_MAX_LINES][E_CACHE];
static gEmptyCache[E_CACHE];
static gFile[E_FILE];
static gNoFile[E_FILE];

#define INI_Exist fexist

stock INI_Create(const filename[], bool:open = false)
{
    if(!filename[0]) return false;
    if(INI_Exist(filename)) return false;

    if(open == false)
    {
		new File: h;
		h = fopen(filename, io_readwrite);
		if(h)
		{
			fclose(h);
			return true;
		}
	}
	else INI_Open(filename);
    return false;
}

stock INI_Remove(const filename[])
{
	if (!filename[0]) return false;
	if ((gFile[E_OPEN]) && (!strcmp(gFile[E_FILENAME], filename, true))) gFile = gNoFile;
	return fremove(filename);
}

stock INI_Open(const filename[])
{
	if (!filename[0]) return false;
	if (gFile[E_OPEN])
	{
		if (!strcmp(gFile[E_FILENAME], filename, true)) return true;
	 	INI_Close();
	}
	new File: h;
	h = fopen(filename, io_readwrite);
	if (h)
	{
		INI_strcpy(gFile[E_FILENAME], filename, INI_MAX_FILENAME, INI_MAX_FILENAME);
		new line[INI_MAX_LINE];
		new ln = -1;
		new separator;
		while (((ln + 1) < INI_MAX_LINES) && (fread(h, line)))
		{
			ln ++;
			INI_StripLine(line);
			separator = strfind(line, "=", false);
			if ((line[0] == ';') || (line[0] == '=') || (separator == -1) || (separator > INI_MAX_KEY))
			{
				if (line[0] == ';')
				{
					INI_strcpy(gCache[ln][E_VALUE], line, INI_MAX_VALUE, INI_MAX_VALUE);
				}
				else
				{
					INI_strcpy(gCache[ln][E_VALUE][1], line, INI_MAX_VALUE - 1, INI_MAX_VALUE - 1);
					gCache[ln][E_VALUE][0] = ';';
				}
				continue;
			}
			INI_strcpy(gCache[ln][E_KEY], line, separator, INI_MAX_KEY);
			INI_strcpy(gCache[ln][E_VALUE], line[separator + 1], INI_MAX_VALUE, INI_MAX_VALUE);
			if (!gCache[ln][E_VALUE][0]) gCache[ln][E_VALUE][0] = ' ';
		}
		fclose(h);
		gFile[E_OPEN] = true;
		return 1;
	}
	return 0;
}

stock INI_Close()
{
	if (!gFile[E_OPEN]) return false;
	#if(INI_AUTO_SAVE)
	    INI_Save();
	#endif
	for (new ln; ln < INI_MAX_LINES; ln++) gCache[ln] = gEmptyCache;
	gFile = gNoFile;
	return true;
}

stock INI_Save()
{
	if (!gFile[E_OPEN]) return false;
	new File: h;
	h = fopen(gFile[E_FILENAME], io_write);
	if (h)
	{
		new line[INI_MAX_LINE];
		new ln = -1;
		while (((ln + 1) < INI_MAX_LINES) && (gCache[ln + 1][E_VALUE][0]))
		{
			ln ++;
			if (gCache[ln][E_VALUE][0] == ';')
			{
				if (gCache[ln][E_VALUE][1])
				{
					format(line, sizeof(line), "%s\r\n", gCache[ln][E_VALUE]);
					fwrite(h, line);
					continue;
				}
				fwrite(h, "\r\n");
				continue;
			}
			format(line, sizeof(line), "%s=%s\r\n", gCache[ln][E_KEY], gCache[ln][E_VALUE]);
			fwrite(h, line);
		}
		fclose(h);
		return true;
	}
	return false;
}

stock INI_Rename(const newname[])
{
	if (!gFile[E_OPEN]) return false;

	INI_Create(newname, false);

	new File: h, File: g, string[INI_MAX_VALUE];
	h = fopen(gFile[E_FILENAME], io_read);
	g = fopen(newname, io_write);

	if (!h) return false;
	if (!g) return false;

	while(fread(h, string)) fwrite(g, string);
	fclose(h);
	fclose(g);
	fremove(gFile[E_FILENAME);
	return true;
}

stock INI_Copy(const copyname[])
{
	if (!gFile[E_OPEN]) return false;

	INI_Create(copyname, false);

	new File: h, File: g, string[INI_MAX_VALUE];
	h = fopen(gFile[E_FILENAME], io_read);
	g = fopen(copyname, io_write);

	if (!h) return false;
	if (!g) return false;

	while(fread(h, string)) fwrite(g, string);
	fclose(h);
	fclose(g);
	return true;
}

stock INI_WriteString(const key[], const value[])
{
	if ((!gFile[E_OPEN]) || (!key[0]) || (key[0] == ';') || (strfind(key, "=", false) != -1)) return false;
	new ln = -1;
	while (((ln + 1) < INI_MAX_LINES) && (gCache[ln + 1][E_VALUE][0]))
	{
		ln ++;
		if (gCache[ln][E_VALUE][0] == ';') continue;
		if (!strcmp(gCache[ln][E_KEY], key, false))
		{
			INI_strcpy(gCache[ln][E_VALUE], value, INI_MAX_VALUE, INI_MAX_VALUE);
			return true;
		}
	}
	ln ++;
	if (ln < INI_MAX_LINES)
	{
		INI_strcpy(gCache[ln][E_KEY], key, INI_MAX_KEY, INI_MAX_KEY);
		INI_strcpy(gCache[ln][E_VALUE], value, INI_MAX_VALUE, INI_MAX_VALUE);
		return true;
	}
	return false;
}

stock INI_WriteInt(const key[], value)
{
	new dest[12];
	format(dest, sizeof(dest), "%i", value);
	return INI_WriteString(key, dest);
}

stock INI_WriteFloat(const key[], Float: value)
{
	new dest[12];
	format(dest, sizeof(dest), "%0.4f", value);
	return INI_WriteString(key, dest);
}

stock INI_WriteBool(const key[], bool:value)
{
	new dest[12];
	if(value)
	{
	    format(dest, sizeof(dest), "1");
 	}
 	else format(dest, sizeof(dest), "0");
	return INI_WriteString(key, dest);
}

stock INI_ReadString(const key[])
{
	new dest[INI_MAX_VALUE];

	if ((!gFile[E_OPEN]) || (!key[0])) return dest;
	new ln = -1;
	while (((ln + 1) < INI_MAX_LINES) && (gCache[ln + 1][E_VALUE][0]))
	{
		ln ++;
		if (gCache[ln][E_VALUE][0] == ';') continue;
		if (!strcmp(gCache[ln][E_KEY], key, false))
		{
			INI_strcpy(dest, gCache[ln][E_VALUE], INI_MAX_VALUE, INI_MAX_VALUE);
			return dest;
		}
	}
	return dest;
}

stock INI_ReadInt(const key[])
{
	return strval(INI_ReadString(key));
}

stock Float:INI_ReadFloat(const key[])
{
	return floatstr(INI_ReadString(key));
}

stock bool:INI_ReadBool(const key[])
{
	if(strval(INI_ReadString(key)))
	{
		return true;
	}
	else return false;
}

stock INI_MultiWrite(format[], {Float,_}:...)
{
	new E_i = 0x-1;

    while(++E_i < strlen(format))
    {
        new E_l = 0x-1,
            E_loop = -1,
            E_string[INI_MAX_VALUE],
            E_paramtar[0x80];

        while(++E_l < 0x80)
		{
			E_paramtar[E_l] = getarg(1 + (E_i * 2), E_l);
        }
		switch(format[E_i])
        {
			case 'b': 		INI_WriteBool(E_paramtar, bool:getarg(2 + (E_i * 2)));
			case 'd', 'i': 	INI_WriteInt(E_paramtar, getarg(2 + (E_i * 2)));
			case 'f': 		INI_WriteFloat(E_paramtar, Float:(getarg(2 + (E_i * 2))));
			case 's':
			{
				while(++E_loop != 0x100)
				{
					E_string[E_loop] = getarg(2 + (E_i * 2), E_loop);
				}
				INI_WriteString(E_paramtar, E_string);
 			}
		}
	}
	return true;
}

stock INI_MultiRead(format[], {Float,_}:...)
{
    new E_i = 0x-1;

    while(++E_i < strlen(format))
    {
        new E_l = 0x-1,
			E_loop = 0x-1,
			E_parameter[0x80],
			E_string[INI_MAX_VALUE];

        while(++E_l < 0x80)
		{
			E_parameter[E_l] = getarg(1 + (E_i * 2), E_l);
        }
		switch(format[E_i])
	    {
	        case 'b': 		setarg(2 + (E_i * 2), 0, INI_ReadBool(E_parameter));
			case 'd', 'i': 	setarg(2 + (E_i * 2), 0, INI_ReadInt(E_parameter));
			case 'f': 		setarg(2 + (E_i * 2), 0, _:INI_ReadFloat(E_parameter));
			case 's':
			{
				E_string = INI_ReadString(E_parameter);

				while(strlen(E_string) != E_loop++)
				{
					setarg(2 + (E_i * 2), E_loop, E_string[E_loop]);
  				}
  			}
		}
    }
	return true;
}

stock INI_RemoveKey(const key[])
{
	if ((!gFile[E_OPEN]) || (!key[0]) || (strfind(key, "=", false) != -1)) return false;
	new ln = -1;
	while (((ln + 1) < INI_MAX_LINES) && (gCache[ln + 1][E_VALUE][0]))
	{
		ln ++;
		if (gCache[ln][E_VALUE][0] == ';') continue;
		if (!strcmp(gCache[ln][E_KEY], key, false))
		{
			for (; ln < (INI_MAX_LINES - 1); ln ++)
			{
				INI_strcpy(gCache[ln][E_KEY], gCache[ln + 1][E_KEY], INI_MAX_KEY, INI_MAX_KEY);
				INI_strcpy(gCache[ln][E_VALUE], gCache[ln + 1][E_VALUE], INI_MAX_VALUE, INI_MAX_VALUE);
			}
			return true;
		}
	}
	return false;
}

stock INI_KeyExist(const key[])
{
    if ((!gFile[E_OPEN]) || (!key[0]) || (strfind(key, "=", false) != -1)) return false;
	new ln = -1;
	while (((ln + 1) < INI_MAX_LINES) && (gCache[ln + 1][E_VALUE][0]))
	{
		ln ++;
		if (gCache[ln][E_VALUE][0] == ';') continue;
		if (!strcmp(gCache[ln][E_KEY], key, false))
		{
			return true;
		}
	}
	return false;
}

stock INI_ReturnFile()
{
    if ((!gFile[E_OPEN])) return false;
    else return gFile[E_OPEN];
}

stock INI_ReturnFilename()
{
    if ((!gFile[E_OPEN])) return gFile[E_FILENAME];
    else return gFile[E_FILENAME];
}

//system functions
stock INI_strcpy(dest[], const source[], numcells = sizeof(source), maxlength = sizeof(dest))
{
	new i;
	while ((source[i]) && (i < numcells) && (i < maxlength))
	{
		dest[i] = source[i];
		i ++;
	}
	dest[(i == maxlength) ? (i - 1) : (i)] = '\0';
}

stock INI_StripLine(string[])
{
	new l;
	l = strlen(string);
	if (string[l - 2] == '\r') string[l - 2] = '\0';
	if (string[l - 1] == '\n') string[l - 1] = '\0';
}

//converting system
#if(INI_CONVERT)

	//  -> converting DINI(one of the best ini code!)
	#if defined dini_Exists
	    #undef dini_Exists
	#endif
	#define dini_Exists 	fexist

	#if defined dini_Remove
	    #undef dini_Remove
	#endif
	#define dini_Remove 	INI_Remove

	#if defined dini_Create
	    #undef dini_Create
	#endif
	#define dini_Create 	INI_Create

	#if defined dini_Close
	    #undef dini_Close
	#endif
	#define dini_Close 	    INI_Close

	stock dini_Set(filename[],key[],value[])
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_WriteString(key, value);
	}

	stock dini_IntSet(filename[],key[],value)
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_WriteInt(key, value);
	}

	stock dini_FloatSet(filename[],key[],Float:value)
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_WriteFloat(key, value);
	}

	stock dini_BoolSet(filename[],key[],value)
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		if(value)
		{
			return INI_WriteBool(key, true);
		}
		return INI_WriteBool(key, false);
	}

	stock dini_Get(filename[],key[])
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_ReadString(key);
	}

	stock dini_Int(filename[],key[])
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_ReadInt(key);
	}

	stock Float:dini_Float(filename[],key[])
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_ReadFloat(key);
	}

	stock dini_Bool(filename[],key[])
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		if(INI_ReadBool(key))
		{
		    return 1;
		}
		else return 0;
	}

	stock dini_UnSet(filename[],key[])
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_RemoveKey(key);
	}

	stock dini_Isset(filename[],key[])
	{
	    if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_KeyExist(key);
	}

	#if defined DINI_StripNewLine
	    #undef DINI_StripNewLine
	#endif
	#define DINI_StripNewLine INI_StripLine

	stock DINI_fcopytextfile(filename[],copyname[])
	{
		if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_Copy(copyname);
	}

	#if defined dini_Copy
	    #undef dini_Copy
	#endif
	#define dini_Copy 		DINI_fcopytextfile

	stock dini_Rename(filename[],newname[])
	{
		if(! INI_Exist(filename))
	    {
	        INI_Create(filename);
	    }
		INI_Open(filename);
		return INI_Rename(newname);
	}

	//  -> converting SMF(Southclaw file manager)
	#if defined file_Create
	    #undef file_Create
	#endif
	#define file_Create 	INI_Create

	#if defined file_Delete
	    #undef file_Delete
	#endif
	#define file_Delete 	INI_Remove

	#if defined file_Open
	    #undef file_Open
	#endif
	#define file_Open 		INI_Open

	#if defined file_Save
	    #undef file_Save
	#endif
	#define file_Save 		INI_Save

	#if defined file_Close
	    #undef file_Close
	#endif
	#define file_Close 		INI_Close

	#if defined file_SetStr
	    #undef file_SetStr
	#endif
	#define file_SetStr 	INI_WriteString

	#if defined file_SetVal
	    #undef file_SetVal
	#endif
	#define file_SetVal 	INI_WriteInt

	#if defined file_SetFloat
	    #undef file_SetFloat
	#endif
	#define file_SetFloat 	INI_WriteFloat

	#if defined file_RemoveKey
	    #undef file_RemoveKey
	#endif
	#define file_RemoveKey 	INI_RemoveKey

	#if defined file_IncVal
	    #undef file_IncVal
	#endif
	#define file_IncVal(%1,%2) INI_WriteInt(%1, INI_ReadInt(%1) + %2)

	#if defined file_GetStr
	    #undef file_GetStr
	#endif
	#define file_GetStr 	INI_ReadString

	#if defined file_GetVal
	    #undef file_GetVal
	#endif
	#define file_GetVal 	INI_ReadInt

	#if defined file_GetFloat
	    #undef file_GetFloat
	#endif
	#define file_GetFloat 	INI_ReadFloat

	#if defined file_IsKey
	    #undef file_IsKey
	#endif
	#define file_IsKey 		INI_KeyExist

	#if defined file_dGetStr
	    #undef file_dGetStr
	#endif
	#define file_dGetStr 	dini_Get

	#if defined file_dGetVal
	    #undef file_dGetVal
	#endif
	#define file_dGetVal 	dini_Int

	#if defined file_dGetFloat
	    #undef file_dGetFloat
	#endif
	#define file_dGetFloat 	dini_Float

 	//  -> converting DUDB
	#define UDB_ACCOUNT_PATH        "Accounts"//specify the path where the user files will be made!
	#define UDB_AUTOCLOSE_TIME 		5000//specify the time after which the opened file will be auto closed!

	#if defined dUserADD
	    #undef dUserADD
	#endif
    #define dUserADD(%1,%2) 		UDB_Create(%1,%2)

	#if defined dUserDELETE
	    #undef dUserDELETE
	#endif
    #define dUserDELETE(%1) 		UDB_Remove(%1)

	#if defined dUser
	    #undef dUser
	#endif
    #define dUser(%1).( 			UDB_ReadString(%1,

	#if defined dUserSet
	    #undef dUserSet
	#endif
	#define dUserSet(%1).( 			UDB_WriteString(%1,

	#if defined dUserINT
	    #undef dUserINT
	#endif
	#define dUserINT(%1).( 			UDB_ReadInt(%1,

	#if defined dUserSetINT
	    #undef dUserSetINT
	#endif
	#define dUserSetINT(%1).( 		UDB_WriteInt(%1,

	#if defined dUserFLOAT
	    #undef dUserFLOAT
	#endif
	#define dUserFLOAT(%1).( 		UDB_ReadFloat(%1,

	#if defined dUserSetFLOAT
	    #undef dUserSetFLOAT
	#endif
	#define dUserSetFLOAT(%1).( 	UDB_WriteFloat(%1,

	#if defined dUserBOOL
	    #undef dUserBOOL
	#endif
	#define dUserBOOL(%1).( 		UDB_ReadBool(%1,

	#if defined dUserSetBOOL
	    #undef dUserSetBOOL
	#endif
	#define dUserSetBOOL(%1).( 		UDB_WriteBool(%1,

	#if defined udb_Exists
	    #undef udb_Exists
	#endif
	#define udb_Exists 		        UDB_Exist

	#if defined udb_Remove
	    #undef udb_Remove
	#endif
	#define udb_Remove 		        UDB_Remove

	#if defined udb_Create
	    #undef udb_Create
	#endif
	#define udb_Create 		        UDB_Create

	#if defined udb_RenameUser
	    #undef udb_RenameUser
	#endif
	#define udb_RenameUser 		   	UDB_Rename

	#if defined udb_CopyUser
	    #undef udb_CopyUser
	#endif
	#define udb_CopyUser 		   	UDB_Copy

	#if defined udb_CheckLogin
	    #undef udb_CheckLogin
	#endif
	#define udb_CheckLogin 		   	UDB_CheckPassword

	new UDB_Timer = -1;

	stock UDB_Exist(username[])
	{
	  	new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	return fexist(string);
	}

	stock UDB_Create(username[], password[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	INI_Create(string, true);
	  	INI_WriteString("Password", password);
	  	INI_Close();
	}

	stock UDB_Remove(username[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	    UDB_Timer = -1;
	  	}
	  	return INI_Remove(string);
	}

	stock UDB_WriteInt(username[], key[], value)
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_IntSet(string, key, value);
	}

	stock UDB_WriteFloat(username[], key[], Float:value)
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_FloatSet(string, key, value);
	}

	stock UDB_WriteBool(username[], key[], bool:value)
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_BoolSet(string, key, value);
	}

	stock UDB_WriteString(username[], key[], value[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_Set(string, key, value);
	}

	stock UDB_ReadInt(username[], key[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_Int(string, key);
	}

	stock Float:UDB_ReadFloat(username[], key[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_Float(string, key);
	}

	stock UDB_ReadBool(username[], key[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_Bool(string, key);
	}

	stock UDB_ReadString(username[], key[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_Get(string, key);
	}

	stock UDB_Rename(username[], newname[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_Rename(string, newname);
	}

	stock UDB_Copy(username[], copyname[])
	{
        new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	}
	  	UDB_Timer = SetTimer("UDB_AutoClose", UDB_AUTOCLOSE_TIME, false);
	  	return dini_Copy(string, copyname);
	}

	stock UDB_CheckPassword(username[], password[])
	{
		new string[INI_MAX_VALUE];
	  	format(string, sizeof(string),""UDB_ACCOUNT_PATH"/%s.udb",username);
	  	INI_Open(string);

	 	if(! strcmp(password, INI_ReadString("Password")))
		{
		 	INI_Close();
			return true;
	 	}
 		else
 		{
		 	INI_Close();
		 	return false;
		}
	}

	stock UDB_Close()
	{
		if(UDB_Timer != -1)
	  	{
	  	    KillTimer(UDB_Timer);
	  	    UDB_Timer = -1;
	  	}
	    #if(!INI_AUTO_SAVE)
	    	INI_Save();
        #endif
		return INI_Close();
	}

	forward UDB_AutoClose();
	public UDB_AutoClose()
	{
	    UDB_Close();
	    return true;
	}

 	//  -> converting FINI
	//   INI_WriteString("key", "valor") //already defined in n_ini
	//   INI_WriteFloat("key", valor) //already defined in n_ini
	//   INI_WriteInt("key", valor) //already defined in n_ini
	//   INI_ReadString("key") //already defined in n_ini
	//   INI_ReadInt("key") //already defined in n_ini
	//   INI_ReadFloat("key") //already defined in n_ini
	//   INI_Create() //already defined in n_ini
	//   INI_Open("arquivo") //already defined in n_ini

	#if defined INI_Isset
	    #undef INI_Isset
	#endif
	#define INI_Isset 			INI_KeyExist

	//   INI_Close() //already defined in n_ini
	//   INI_Save() //already defined in n_ini

	//  -> Converting SF - Simple File
	#if defined FileCreate
	    #undef FileCreate
	#endif
	#define FileCreate(%1,%2) 			INI_Create(%1)

	#if defined FileDelete
	    #undef FileDelete
	#endif
	#define FileDelete 					INI_Remove

	#if defined FileExist
	    #undef FileExist
	#endif
	#define FileExist 					INI_Exist

	#if defined SaveFileString
	    #undef SaveFileString
	#endif
	#define SaveFileString(%1,%2,%3) 	dini_Set(%1,%2,%3)

	#if defined SaveFileInteger
	    #undef SaveFileInteger
	#endif
	#define SaveFileInteger(%1,%2,%3) 	dini_IntSet(%1,%2,%3)

	#if defined SaveFileFloat
	    #undef SaveFileFloat
	#endif
	#define SaveFileFloat(%1,%2,%3) 	dini_FloatSet(%1,%2,%3)

	#if defined GetFileString
	    #undef GetFileString
	#endif
	#define GetFileString(%1,%2) 		dini_Get(%1,%2)

	#if defined GetFileInteger
	    #undef GetFileInteger
	#endif
	#define GetFileInteger(%1,%2) 		dini_IntGet(%1,%2)

	#if defined GetFileFloat
	    #undef GetFileFloat
	#endif
	#define GetFileFloat(%1,%2) 		dini_FloatGet(%1,%2)

	#if defined EditFileName
	    #undef EditFileName
	#endif
	#define EditFileName(%1,%2) 		dini_Rename(%1,%2)

	//  -> Converting dFile
	#if defined dfile_Create
	    #undef dfile_Create
	#endif
	#define dfile_Create 				INI_Create

	#if defined dfile_Delete
	    #undef dfile_Delete
	#endif
	#define dfile_Delete 				INI_Remove

	#if defined dfile_Open
	    #undef dfile_Open
	#endif
	#define dfile_Open 					INI_Open

	#if defined dfile_WriteString
	    #undef dfile_WriteString
	#endif
	#define dfile_WriteString 			INI_WriteString

	#if defined dfile_WriteInt
	    #undef dfile_WriteInt
	#endif
	#define dfile_WriteInt 				INI_WriteInt

	#if defined dfile_WriteBool
	    #undef dfile_WriteBool
	#endif
	#define dfile_WriteBool 			INI_WriteBool

	#if defined dfile_WriteFloat
	    #undef dfile_WriteFloat
	#endif
	#define dfile_WriteFloat 			INI_WriteFloat

	#if defined dfile_ReadString
	    #undef dfile_ReadString
	#endif
	#define dfile_ReadString  			INI_ReadString

	#if defined dfile_ReadInt
	    #undef dfile_ReadInt
	#endif
	#define dfile_ReadInt  				INI_ReadInt

	#if defined dfile_ReadFloat
	    #undef dfile_ReadFloat
	#endif
	#define dfile_ReadFloat  			INI_ReadFloat

	#if defined dfile_ReadBool
	    #undef dfile_ReadBool
	#endif
	#define dfile_ReadBool  			INI_ReadBool

	#if defined dfile_MoveFile
	    #undef dfile_MoveFile
	#endif
	#define dfile_MoveFile  			INI_Rename

	#if defined dfile_RenameFile
	    #undef dfile_RenameFile
	#endif
	#define dfile_RenameFile   			INI_Rename

	#if defined dfile_SaveFile
	    #undef dfile_SaveFile
	#endif
	#define dfile_SaveFile   			INI_Save

	#if defined dfile_FileExists
	    #undef dfile_FileExists
	#endif
	#define dfile_FileExists   			fexist

	#if defined dfile_ParseString
	    #undef dfile_ParseString
	#endif
	#define dfile_ParseString   		dini_Get

	#if defined dfile_ParseInt
	    #undef dfile_ParseInt
	#endif
	#define dfile_ParseInt    			dini_Int

	#if defined dfile_ParseFloat
	    #undef dfile_ParseFloat
	#endif
	#define dfile_ParseFloat    		dini_Float

	#if defined dfile_ParseBool
	    #undef dfile_ParseBool
	#endif
	#define dfile_ParseBool     		dini_Bool

	#if defined dfile_IsSet
	    #undef dfile_IsSet
	#endif
	#define dfile_IsSet     		    INI_KeyExist

	#if defined dfile_UnSet
	    #undef dfile_UnSet
	#endif
	#define dfile_UnSet      		    INI_RemoveKey

	#if defined dfile_MultiSet
	    #undef dfile_MultiSet
	#endif
	#define dfile_MultiSet      		INI_MultiWrite

	#if defined dfile_MultiGet
	    #undef dfile_MultiGet
	#endif
	#define dfile_MultiGet      		INI_MultiRead

#endif
