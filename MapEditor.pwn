// ============= [Server Settings] =============
#define	SERVER_NAME			"Map Editor by BabyJnL"
#define SERVER_MAP			"San Andreas"
#define SERVER_GAMEMODE		"Map Editor V1.2"
#define SERVER_LANGUAGE		"Bahasa Indonesia"

#define DATABASE 			"internal/main.db"

// ============= [Define Enum] =============
enum 
{
	DIALOG_SERVER_INFO,
	DIALOG_CREDITS,
    DIALOG_TELEPORT_OPTIONS,

	DIALOG_TEXT_MENU,
    DIALOG_TEXT_SET_MESSAGE,
    DIALOG_TEXT_SET_RESOLUTION,
    DIALOG_TEXT_SET_FONT,
    DIALOG_TEXT_SET_CUSTOM_FONT,
    DIALOG_TEXT_SET_FONT_SIZE,
    DIALOG_TEXT_SET_FONT_COLOR,
    DIALOG_TEXT_SET_BG_COLOR,
    DIALOG_TEXT_SET_ALLIGNMENT,

    DIALOG_MAP_LIST,
    DIALOG_LOAD_MAP_CONFIRMATION
}

// ============= [Macros] =============
#define strToLower(%0) \
    for(new i; %0[i] != EOS; ++i) \
        %0[i] = ('A' <= %0[i] <= 'Z') ? (%0[i] += 'a' - 'A') : (%0[i])  // strToLower 				by 	RyDeR`

// ============= [Import Libraries] =============
#include <a_samp>

#include <sscanf2>
#include <streamer> 

#include <filemanager> 

#include <YSI_Core\y_utils>
#include <YSI_Data\y_iterate>
#include <YSI_Server\y_colours>
#include <YSI_Visual\y_commands>

// Include modules
#include "modules/Utils.inc"
#include "modules/ObjectEditor.inc"

main() 
{
    print(" __       __   ______   _______         ________  _______   ______  ________  ______   _______  ");
    print("/  \\     /  | /      \\ /       \\       /        |/       \\ /      |/        |/      \\ /       \\ ");
    print("$$  \\   /$$ |/$$$$$$  |$$$$$$$  |      $$$$$$$$/ $$$$$$$  |$$$$$$/ $$$$$$$$//$$$$$$  |$$$$$$$  |");
    print("$$$  \\ /$$$ |$$ |__$$ |$$ |__$$ |      $$ |__    $$ |  $$ |  $$ |     $$ |  $$ |  $$ |$$ |__$$ |");
    print("$$$$  /$$$$ |$$    $$ |$$    $$/       $$    |   $$ |  $$ |  $$ |     $$ |  $$ |  $$ |$$    $$< ");
    print("$$ $$ $$/$$ |$$$$$$$$ |$$$$$$$/        $$$$$/    $$ |  $$ |  $$ |     $$ |  $$ |  $$ |$$$$$$$  |");
    print("$$ |$$$/ $$ |$$ |  $$ |$$ |            $$ |_____ $$ |__$$ | _$$ |_    $$ |  $$ \\__$$ |$$ |  $$ |");
    print("$$ | $/  $$ |$$ |  $$ |$$ |            $$       |$$    $$/ / $$   |   $$ |  $$    $$/ $$ |  $$ |");
    print("$$/      $$/ $$/   $$/ $$/             $$$$$$$$/ $$$$$$$/  $$$$$$/    $$/    $$$$$$/  $$/   $$/ ");
    print("                                                                                                 ");
    print(" __                        _______             __                     _____            __       ");
    print("/  |                      /       \\           /  |                   /     |          /  |      ");
    print("$$ |____   __    __       $$$$$$$  |  ______  $$ |____   __    __    $$$$$ | _______  $$ |      ");
    print("$$      \\ /  |  /  |      $$ |__$$ | /      \\ $$      \\ /  |  /  |      $$ |/       \\ $$ |      ");
    print("$$$$$$$  |$$ |  $$ |      $$    $$<  $$$$$$  |$$$$$$$  |$$ |  $$ | __   $$ |$$$$$$$  |$$ |      ");
    print("$$ |  $$ |$$ |  $$ |      $$$$$$$  | /    $$ |$$ |  $$ |$$ |  $$ |/  |  $$ |$$ |  $$ |$$ |      ");
    print("$$ |__$$ |$$ \\__$$ |      $$ |__$$ |/$$$$$$$ |$$ |__$$ |$$ \\__$$ |$$ \\__$$ |$$ |  $$ |$$ |_____ ");
    print("$$    $$/ $$    $$ |      $$    $$/ $$    $$ |$$    $$/ $$    $$ |$$    $$/ $$ |  $$ |$$       |");
    print("$$$$$$$/   $$$$$$$ |      $$$$$$$/   $$$$$$$/ $$$$$$$/   $$$$$$$ | $$$$$$/  $$/   $$/ $$$$$$$$/ ");
    print("          /  \\__$$ |                                    /  \\__$$ |                               ");
    print("          $$    $$/                                     $$    $$/                                ");
    print("           $$$$$$/                                       $$$$$$/                                 \n");  


	CheckRequiredDirectory();
	printf("[SERVER] %s successfully initialized", SERVER_GAMEMODE);
}

CheckRequiredDirectory()
{
	if(!dir_exists("map"))
	{
		dir_create("map");
		printf("[SERVER] Map directory hasn't been created, server is creating it for you!");
	}
	else printf("[SERVER] Map directory founded!");

	return 1;
}

CreateDatabase()
{
	new DB:database = db_open(DATABASE);

    db_query(database, "CREATE TABLE IF NOT EXISTS `users`(`name` VARCHAR(24), `skin` INT, `pos_x` FLOAT, `pos_y` FLOAT, `pos_z` FLOAT, `rot_z` FLOAT)");
    db_close(database);
	return 1;
}

LoadPlayerData(playerid) 
{
	if(GetPVarType(playerid, "Spawned") == 0)
	{
	    new 
			string[256],
			playername[MAX_PLAYER_NAME]
		;

		playername = ReturnPlayerName(playerid);
		strToLower(playername);

	    format(string,sizeof(string),"SELECT * FROM `users` WHERE `name`='%s'", playername);

		new DB:database = db_open(DATABASE);
		new DBResult:result = db_query(database, string);

		if(db_num_rows(result) > 0)
		{
		    new data[32],skin,Float:spawnPos[4];
			db_get_field(result, 1, data, sizeof(data)); skin = strval(data);
			db_get_field(result, 2, data, sizeof(data)); spawnPos[0] = floatstr(data);
			db_get_field(result, 3, data, sizeof(data)); spawnPos[1] = floatstr(data);
			db_get_field(result, 4, data, sizeof(data)); spawnPos[2] = floatstr(data);
			db_get_field(result, 5, data, sizeof(data)); spawnPos[3] = floatstr(data);
			db_free_result(result);

			SetSpawnInfo(playerid,-1, skin, spawnPos[0], spawnPos[1], spawnPos[2], spawnPos[3], 0, 0, 0, 0, 0, 0);
			SpawnPlayer(playerid);
		}
		else
		{
		    db_free_result(result);
			format(string,sizeof(string),"INSERT INTO `users` VALUES('%s', '0', '1958.33', '1343.12', '15.36', '269.15')", playername);
			db_query(database,string);
		}
		db_close(database);
		SetPVarInt(playerid, "Spawned", 1);
	}
	return 1;
}

SavePlayerData(playerid) 
{
	new 
		playername[MAX_PLAYER_NAME],
		Float:cPos[4],
		string[256]
	;

	playername = ReturnPlayerName(playerid);
	GetPlayerPos(playerid, cPos[0], cPos[1], cPos[2]);
	GetPlayerFacingAngle(playerid, cPos[3]);
	
	strToLower(playername);
	format(string, sizeof(string), "UPDATE `users` SET `skin`='%d',`pos_x`='%f',`pos_y`='%f',`pos_z`='%f',`rot_z`='%f' WHERE `name`='%s'", GetPlayerSkin(playerid), cPos[0], cPos[1], cPos[2], cPos[3], playername);

	new DB:database = db_open(DATABASE);
	db_query(database, string);
	db_close(database);

	print("[SERVER] Player data successfully saved");
	return 1;
}

public OnGameModeInit()
{
	new str[144];

	format(str, sizeof(str), "hostname %s", SERVER_NAME);
	SendRconCommand(str);

	format(str, sizeof(str), "gamemodetext %s", SERVER_GAMEMODE);
	SendRconCommand(str);

	format(str, sizeof(str), "mapname %s", SERVER_MAP);
	SendRconCommand(str);

	format(str, sizeof(str), "language %s", SERVER_LANGUAGE);
	SendRconCommand(str);

	CreateDatabase();
	return 1;
}

public OnPlayerConnect(playerid) 
{
	return 1;
}

public OnPlayerDisconnect(playerid) 
{
	SavePlayerData(playerid);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetSpawnInfo(playerid, 0, 0, 1958.33, 1343.12, 15.36, 269.15, 0, 0, 0, 0, 0, 0);
	TogglePlayerSpectating(playerid, true);

	new str[256];
	format(str, sizeof(str), "Welcome to "YELLOW"%s"WHITE"!\n\n"LIGHTBLUE"This gamemode is based on a script by "RED"tianmetal"LIGHTBLUE" and improved by "RED"BabyJnL"LIGHTBLUE".\nFeel free to use it, but please keep the credits intact.", SERVER_GAMEMODE);
	ShowPlayerDialog(playerid, DIALOG_SERVER_INFO, DIALOG_STYLE_MSGBOX, "Server Information", str, "Spawn", "");
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	SetPlayerPosFindZ(playerid, fX, fY, fZ);
	SetPlayerPos(playerid, fX, fY, fZ);
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_SERVER_INFO) 
	{
		if(response) 
		{
			LoadPlayerData(playerid);
			TogglePlayerSpectating(playerid, false);
		}
		return 1;
	}
    else if(dialogid == DIALOG_TEXT_MENU)
    {
        if(response) 
        {
            switch(listitem) 
            {
                case 0: 
                {
                    ShowPlayerDialog(playerid,DIALOG_TEXT_SET_MESSAGE,DIALOG_STYLE_INPUT, "Material Text: Set Text","Input text: (max length = 255 characters)","Set","Back");
                    return 1;
                }
                case 1:
                {
                    ShowPlayerDialog(playerid,DIALOG_TEXT_SET_RESOLUTION,DIALOG_STYLE_LIST,"Material Text: Set Resolution","32x32\n64x32\n64x64\n128x32\n128x64\n128x128\n256x32\n256x64\n256x128\n256x256\n512x64\n512x128\n512x256\n512x512","Set","Back");
				    return 1;
                }
                case 2: 
                {
                    new fonts[256];
                    Loop(i,sizeof(WinFonts))
                    {
                        strcat(fonts,WinFonts[i],sizeof(fonts));
                        strcat(fonts,"\n",sizeof(fonts));
                    }
                    strcat(fonts,"Custom",sizeof(fonts));
                    ShowPlayerDialog(playerid,DIALOG_TEXT_SET_FONT,DIALOG_STYLE_LIST,"Material Text: Set Font",fonts,"Set","Back");
                    return 1;
                }
                case 3: 
                {
                    ShowPlayerDialog(playerid,DIALOG_TEXT_SET_FONT_SIZE,DIALOG_STYLE_INPUT,"Material Text: Set Font Size","Input font size: (1-255)","Set","Back");
				    return 1;
                }
                case 4: 
                {
                    new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
                    new 
                        slot = GetPVarInt(playerid,"EditingObject"),
                        index = GetPVarInt(playerid,"EditingIndex")
                    ;
                    GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
                    SetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,((bold == 1) ? 0 : 1),fcolor,bcolor,alignment);
                    ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
                    return 1;
                }
                case 5:
                {
                    ShowPlayerDialog(playerid,DIALOG_TEXT_SET_FONT_COLOR,DIALOG_STYLE_INPUT,"Material Text: Set Font Color","Input color in RGBA format: (ex 255 0 0 255 = yellow)","Set","Back");
                    return 1;
                }
                case 6:
                {
                    ShowPlayerDialog(playerid,DIALOG_TEXT_SET_BG_COLOR,DIALOG_STYLE_INPUT,"Material Text: Set Background Color","Input color in RGBA format: (ex 255 0 0 255 = yellow)","Set","Back");
                    return 1;
                }
                case 7:
                {
                    ShowPlayerDialog(playerid,DIALOG_TEXT_SET_ALLIGNMENT,DIALOG_STYLE_LIST,"Material Text: Set Alignment","Left\nCenter\nRight","Set","Back");
                    return 1;
                }
                case 8:
                {
                    new 
                        Float:cPos[3],
                        Float:cRot[3],
                        slot = GetPVarInt(playerid,"EditingObject"),
                        index = GetPVarInt(playerid,"EditingIndex")
                    ;
                    GetDynamicObjectPos(Object[slot],cPos[0],cPos[1],cPos[2]);
                    GetDynamicObjectRot(Object[slot],cRot[0],cRot[1],cRot[2]);
                    new temp = CreateDynamicObject(Streamer_GetIntData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_MODEL_ID),cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
                    ObjectMaterial[slot][index] = 0;
                    Loop(i,MAX_OBJECT_MATERIAL_SLOT)
                    {
                        if(ObjectMaterial[slot][i] == MATERIAL_TYPE_TEXTURE)
                        {
                            new modelid,txdname[32],texturename[32],color;
                            GetDynamicObjectMaterial(Object[slot],i,modelid,txdname,texturename,color);
                            SetDynamicObjectMaterial(temp,i,modelid,txdname,texturename,color);
                        }
                        else if(ObjectMaterial[slot][i] == MATERIAL_TYPE_MESSAGE)
                        {
                            new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
                            GetDynamicObjectMaterialText(Object[slot],i,text,size,font,fsize,bold,fcolor,bcolor,alignment);
                            SetDynamicObjectMaterialText(temp,i,text,size,font,fsize,bold,fcolor,bcolor,alignment);
                        }
                    }
                    DestroyDynamicObject(Object[slot]);
                    Object[slot] = temp;
                    Streamer_Update(playerid,STREAMER_TYPE_OBJECT);
                }
            }
        }
        DeletePVar(playerid, "EditingObject");
	    DeletePVar(playerid, "EditingIndex");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_MESSAGE) 
    {
        if(response)
        {
            new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
            new 
                slot = GetPVarInt(playerid,"EditingObject"),
                index = GetPVarInt(playerid,"EditingIndex"),
                convertedText[128]
            ;

            GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
            if(IsNull(inputtext)) strcopy(convertedText, text);
            else ReplaceSlashNWithNewline(inputtext, convertedText);
            SetDynamicObjectMaterialText(Object[slot],index,convertedText,size,font,fsize,bold,fcolor,bcolor,alignment);
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_RESOLUTION)
    {
        if(response) 
        {
            new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
            new 
                slot = GetPVarInt(playerid,"EditingObject"),
                index = GetPVarInt(playerid,"EditingIndex")
            ;
            GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
            size = ((listitem+1)*10);
            SetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_FONT) 
    {
        if(response)
        {
            if(listitem < sizeof(WinFonts))
            {
                new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
                new 
                    slot = GetPVarInt(playerid,"EditingObject"),
                    index = GetPVarInt(playerid,"EditingIndex")
                ;
                GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
                SetDynamicObjectMaterialText(Object[slot],index,text,size,WinFonts[listitem],fsize,bold,fcolor,bcolor,alignment);
            }
            else
            {
                ShowPlayerDialog(playerid,DIALOG_TEXT_SET_CUSTOM_FONT,DIALOG_STYLE_INPUT,"Material Text: Set Custom Font","Input font name:","Input","Back");
            }
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_CUSTOM_FONT) 
    {
        if(response)
        {
            new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
            new 
                slot = GetPVarInt(playerid,"EditingObject"),
                index = GetPVarInt(playerid,"EditingIndex")
            ;
            GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
            SetDynamicObjectMaterialText(Object[slot],index,text,size,((IsNull(inputtext)) ? (text) : (inputtext)),fsize,bold,fcolor,bcolor,alignment);
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_FONT_SIZE)
    {
        if(response)
        {
            new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
            new 
                slot = GetPVarInt(playerid,"EditingObject"),
                index = GetPVarInt(playerid,"EditingIndex")
            ;
            GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
            SetDynamicObjectMaterialText(Object[slot],index,text,size,font,((IsNull(inputtext)) ? fsize : strval(inputtext)),bold,fcolor,bcolor,alignment);
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_FONT_COLOR) 
    {
        if(response)
        {
            new alpha,red,green,blue;
            if(!sscanf(inputtext,"dddD(255)",red,green,blue,alpha))
            {
                new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
                new 
                    slot = GetPVarInt(playerid,"EditingObject"),
                    index = GetPVarInt(playerid,"EditingIndex")
                ;
                GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
                fcolor = RGBAToHex(alpha,red,green,blue);
                SetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
            }
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_BG_COLOR) 
    {
        if(response)
        {
            new alpha,red,green,blue;
            if(!sscanf(inputtext,"dddD(255)",red,green,blue,alpha))
            {
                new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
                new 
                    slot = GetPVarInt(playerid,"EditingObject"),
                    index = GetPVarInt(playerid,"EditingIndex")
                ;
                GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
                bcolor = RGBAToHex(alpha,red,green,blue);
                SetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
            }
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TEXT_SET_ALLIGNMENT)
    {
        if(response)
        {
            new text[256],size,font[32],fsize,bold,fcolor,bcolor,alignment;
            new 
                slot = GetPVarInt(playerid,"EditingObject"),
                index = GetPVarInt(playerid,"EditingIndex")
            ;
            GetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,alignment);
            SetDynamicObjectMaterialText(Object[slot],index,text,size,font,fsize,bold,fcolor,bcolor,listitem);
        }
        ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        return 1;
    }
    else if(dialogid == DIALOG_TELEPORT_OPTIONS)
    {
        if(response) 
        {
            SetPlayerInterior(playerid,TeleportList[listitem][Interior]);
            SetPlayerPos(playerid,TeleportList[listitem][Pos_X],TeleportList[listitem][Pos_Y],TeleportList[listitem][Pos_Z]);
        }
        return 1;
    }
    else if(dialogid == DIALOG_MAP_LIST) 
    {
        if(response) 
        {
            new string[256];
            format(string, sizeof(string), ""YELLOW"WARNING!!!"WHITE":\n\nYou are about to load map "CYAN"%s"WHITE" to server\n"RED"This mean all of created object will be cleared"WHITE".\n\nAre you sure?", inputtext);
            SetPVarString(playerid, "SelectedMap", inputtext);
            ShowPlayerDialog(playerid, DIALOG_LOAD_MAP_CONFIRMATION, DIALOG_STYLE_MSGBOX, "Map Load Confirmation", string, "Confirm", "Cancel");
        }
        return 1;
    }
    else if(dialogid == DIALOG_LOAD_MAP_CONFIRMATION)
    {
        if(response) 
        {
            new 
                string[256],
                targetfile[256],
                filename[128]
            ;
            GetPVarString(playerid, "SelectedMap", filename);
            format(targetfile, sizeof(targetfile), "map/%s.txt", filename);

            if(!file_exists(targetfile))
                return SendErrorMessage(playerid, "Failed to load %s, no such file", targetfile);

            DestroyAllObjects();

            new 
                File:file = f_open(targetfile, "r"),
                model,
                Float:pos[3],
                Float:rot[3],
                object_counter = 0,
                pos1,
                pos2,
                last_slot
            ;

            while(f_read(file, string, sizeof(string)) > 0)
            {
                pos1 = strfind(string, "(", false);
                pos2 = strfind(string, ")", false);

                if(pos1 != -1 && pos1 != -2)
                {
                    if(strfind(string, "CreateObject", false) != -1 || strfind(string, "CreateDynamicObject", false) != -1)
                    {
                        strmid(string, string, (pos1+1), pos2);
                        if(!unformat(string, "p<,>iffffff", model, pos[0], pos[1], pos[2], rot[0], rot[1], rot[2]))
                        {
                            last_slot = CreateObjectEx(model, pos[0], pos[1], pos[2], rot[0], rot[1], rot[2]);
                            object_counter++;
                        }
                    }
                    else if(strfind(string, "SetObjectMaterialText", false) != -1)
                    {
                        new 
                            index,
                            text[128],
                            resolution,
                            font[32],
                            fontsize,
                            bold,
                            fontcolor,
                            color,
                            alignment,
                            convertedText[128]
                        ;
                        strmid(string, string,(pos1+1), pos2);
                        if(!unformat(string,"p<,>{s[32]}s[128]iis[32]I(24)I(1)X(0xFFFFFF)X(0x000000)I(1)", text, index, resolution, font, fontsize, bold, fontcolor, color, alignment))
                        {
                            if(MAX_OBJECT_MATERIAL_SLOT > index >= 0)
                            {
                                ReplaceSlashNWithNewline(text, convertedText);
                                SetDynamicObjectMaterialText(Object[last_slot], index, convertedText, resolution, font, fontsize, bold, fontcolor, color, alignment);
                                ObjectMaterial[last_slot][index] = MATERIAL_TYPE_MESSAGE;
                            }
                        }
                    }
                    else if(strfind(string, "SetObjectMaterial", false) != -1 || strfind(string, "SetDynamicObjectMaterial", false) != -1)
                    {
                        new 
                            index,
                            txdname[32],
                            texture[32],
                            color
                        ;

                        strmid(string, string, (pos1+1), pos2); 
                        if(!unformat(string,"p<,>{s[32]}dds[32]s[32]X(0x000000)", index, model, txdname, texture, color))
                        {
                            if(MAX_OBJECT_MATERIAL_SLOT > index >= 0)
                            {
                                SetDynamicObjectMaterial(Object[last_slot],index,model,txdname,texture,color);
                                ObjectMaterial[last_slot][index] = MATERIAL_TYPE_TEXTURE;
                            }
                        }
                    }
                }
            }
            f_close(file);
            SendObjectMessage(playerid, ""WHITE"Map "CYAN"%s"WHITE" successfully loaded, total objects: "YELLOW"%i", inputtext, object_counter);
        }
    }
    return 0;
}