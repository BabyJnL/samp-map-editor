CMD:credits(playerid, params[]) 
{
    new string[512];
    strcat(string, ""WHITE"This project wouldn't be possible without the help and support of the following:\n\n");
    Loop(i, sizeof third_party_credits) 
    {
        new temp[128];
        format(temp, sizeof(temp), ""CYAN"%s"WHITE" - %s\n", third_party_credits[i][0], third_party_credits[i][1]);
        strcat(string, temp);
    }
    strcat(string, "\nGamemode Developers:\n");
    Loop(i, sizeof core_contributors)
    {
        new temp[128];
        format(temp, sizeof(temp), ""RED"%s"WHITE" - %s\n", core_contributors[i][0], core_contributors[i][1]);
        strcat(string, temp); 
    }
    // format(string, sizeof(string), ""WHITE"This project wouldn't be possible without the help and support of the following:\n\n"CYAN"SA-MP Community"WHITE" - \n"CYAN"Y-Less"WHITE" - For YSI and sscanf\n"CYAN"Southclaws"WHITE" - For sampctl and tooling\nIncognito - For Streamer Plugin\nJaTochNietDan - For FileManager\nTianmetal - For Base Script");
    // format(string, sizeof(string), ""WHITE"This project wouldn't be possible without the help and support of the following:\n\n")
    ShowPlayerDialog(playerid, DIALOG_CREDITS, DIALOG_STYLE_MSGBOX, "Gamemode Credits", string, "Close", "");
    return 1;
}

CMD:jetpack(playerid, params[])
{
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
    return 1;
} 

CMD:lt(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYLeftOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}

CMD:rt(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYRightOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}

CMD:ft(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYInFrontOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}

CMD:bt(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYBehindPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}

CMD:up(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[3];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+cDistance));
	return 1;
}

CMD:dn(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[3];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]-cDistance));
	return 1;
}

CMD:skin(playerid, params[]) 
{
    new skinid;

    if(sscanf(params, "i", skinid))
        return SendCommandMessage(playerid, "/skin [skinid]");

    if(skinid < 0 || skinid > 311)
        return SendErrorMessage(playerid, "skin id can't go below 0 or over 311!");

    SetPlayerSkin(playerid, skinid);
    return 1;
}

CMD:land(playerid, params[])
{
    new Float:pos[3];

    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    SetPlayerPosFindZ(playerid, pos[0], pos[1], pos[2]);
    return 1;
}

CMD:weapon(playerid, params[])
{
    new weaponid;
    if(sscanf(params, "i", weaponid))
        return SendCommandMessage(playerid, "/weapon [weapon id]");

    if(0 > weaponid > 46)
        return SendErrorMessage(playerid, "Invalid weapon id!");

    GivePlayerWeapon(playerid, weaponid, 99999);
    return 1;
}