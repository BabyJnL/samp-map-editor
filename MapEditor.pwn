// ============= [Server Settings] =============
#define	SERVER_NAME			"Map Editor by BabyJnL"
#define SERVER_MAP			"San Andreas"
#define SERVER_GAMEMODE		"Map Editor V1.0"
#define SERVER_LANGUAGE		"Bahasa Indonesia"

#define DATABASE 			"internal/main.db"

// ============= [Define Enum] =============
enum 
{
	DIALOG_SERVER_INFO
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
	format(str, sizeof(str), "Welcome to "YELLOW"%s"WHITE"!\n"LIGHTBLUE"This gamemode is based on a script by "RED"tianmetal"LIGHTBLUE" and improved by "RED"BabyJnL"LIGHTBLUE".\nFeel free to use it, but please keep the credits intact.", SERVER_GAMEMODE);
	ShowPlayerDialog(playerid, DIALOG_SERVER_INFO, DIALOG_STYLE_MSGBOX, "Server Information", str, "Spawn", "");
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	SetPlayerPosFindZ(playerid, fX, fY, fZ);
	SetPlayerPos(playerid, fX, fY, fZ);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) 
{
	switch(dialogid) 
	{
		case DIALOG_SERVER_INFO:
		{
			LoadPlayerData(playerid);
			TogglePlayerSpectating(playerid, false);
			return 1;
		}
	}
	return 0;
}