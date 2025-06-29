CMD:help(playerid, params[]) 
{
    SendCommandMessage(playerid, "/help, /credits, /goto, /jetpack, /lt, /rt, /ft, /bt");
    SendCommandMessage(playerid, "/up, /dn, /skin, /land, /weapon, /teleport, /setloc, /setint");
    SendCommandMessage(playerid, "/gotols, /teleport, /object");
    return 1;
}

CMD:credits(playerid, params[]) 
{
    new string[512];
    strcat(string, ""WHITE"This project wouldn't be possible without the help and support of the following:\n\n");
    Loop(i, sizeof third_party_credits) 
    {
        new temp[128];
        format(temp, sizeof(temp), ""CYAN"%s"WHITE" %s\n", third_party_credits[i][0], third_party_credits[i][1]);
        strcat(string, temp);
    }
    strcat(string, "\n"LIGHTBLUE"Gamemode Developers:\n");
    Loop(i, sizeof core_contributors)
    {
        new temp[128];
        format(temp, sizeof(temp), ""RED"%s"WHITE" %s\n", core_contributors[i][0], core_contributors[i][1]);
        strcat(string, temp); 
    }
    ShowPlayerDialog(playerid, DIALOG_CREDITS, DIALOG_STYLE_MSGBOX, "Gamemode Credits", string, "Close", "");
    return 1;
}

CMD:goto(playerid, params[]) 
{
    new targetplayer;
    if(sscanf(params, "u", targetplayer))
        return SendCommandMessage(playerid, "/goto [playerid/name]");

    if(!IsPlayerConnected(targetplayer))
        return SendErrorMessage(playerid, "Target player is not connected!");

    new Float:pos[3];

    GetPlayerPos(targetplayer, pos[0], pos[1], pos[2]);
    SetPlayerPos(playerid, pos[0], pos[1], pos[2]);

    SendClientMessageEx(playerid, X11_GREEN, "TELEPORT: "WHITE"Teleported to "YELLOW"%s", ReturnPlayerName(targetplayer));
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

CMD:teleport(playerid,params[])
{
    new string[2744];
    format(string,sizeof(string),"Four Dragons' Managerial Suite\nGanton Gym\nBrothel\nBrothel2\nInside Track Betting\nBlastin' Fools Records\nThe Big Spread Ranch\nWarehouse 1\nWarehouse 2\nB Dup's Apartment\nB Dup's ");
    format(string,sizeof(string),"%sCrack Palace\nWheel Arch Angels\nOG Loc's House\nBarber Shop\nPlanning Department\nLas Venturas Police Department\nPro-Laps\nSex Shop\nLas Venturas Tattoo parlor\nLost San Fierro Tattoo ",string);
    format(string,sizeof(string),"%sparlor\n24/7 (version 1)\nDiner 1\nPizza Stack\nRusty Brown's Donuts\nAmmu-nation\nVictim\nLoco Low Co\nSan Fierro Police Department\n24/7 (version 2 - large)\nBelow The Belt Gym (Las ",string);
    format(string,sizeof(string),"%sVenturas)\nTransfenders\nWorld of Coq\nAmmu-nation (version 2)\nSubUrban\nDenise's Bedroom\nHelena's Barn\nBarbara's Love nest\nSan Fierro Garage\nOval Stadium\n8-Track Stadium\nThe Pig ",string);
    format(string,sizeof(string),"%sPen (strip club 2)\nFour Dragons\nLiberty City\nRyder's house\nSweet's House\nRC Battlefield\nThe Johnson House\nBurger shot\nCaligula's Casino\nKatie's Lovenest\nBarber Shop 2 ",string);
    format(string,sizeof(string),"%s(Reece's)\nAngel Pine Trailer\n24/7 (version 3)\nZip\nThe Pleasure Domes\nMadd Dogg's Mansion\nBig Smoke's Crack Palace\nBurning Desire Building\nWu-Zi Mu's\nAbandoned AC tower",string);
    format(string,sizeof(string),"%s\nWardrobe/Changing room\nDidier Sachs\nCasino (Redsands West)\nKickstart Stadium\nClub\nAtrium\nLos Santos Tattoo Parlor\nSafe House group 1\nSafe House group 2\nSafe House group 3\nSafe ",string);
    format(string,sizeof(string),"%sHouse group 4\nSherman Dam\n24/7 (version 4)\nJefferson Motel\nJet Interior\nThe Welcome Pump\nBurglary House X1\nBurglary House X2\nBurglary House X3\nBurglary House X4\nBinco",string);
    format(string,sizeof(string),"%s\n4 Burglary houses\nBlood Bowl Stadium\nBudget Inn Motel Room\nPalamino Bank\nPalamino Diner\nDillimore Gas Station\nLil' Probe Inn\nTorreno's Ranch\nZombotech - lobby area\nCrypt in LS ",string);
    format(string,sizeof(string),"%scemetery (temple)\nBlueberry Liquor Store\nPair of Burglary Houses\nCrack Den\nBurglary House X11\nBurglary House X12\nAmmu-nation (version 3)\nJay's Diner\n24/7 (version 5)\nWarehouse ",string);
    format(string,sizeof(string),"%s3\nMichelle's Love Nest*\nBurglary House X14\nSindacco Abatoir\nK.A.C.C. Military Fuels Depot\nArea 69\nBurglary House X13\nUnused Safe House\nMillie's Bedroom\nBarber Shop",string);
    format(string,sizeof(string),"%s\nDirtbike Stadium\nCobra Gym\nLos Santos Police Department\nLos Santos Airport\nBurglary House X15\nBurglary House X16\nBurglary House X17\nBike School\nFrancis International Airport",string);
    format(string,sizeof(string),"%s\nVice Stadium\nBurglary House X18\nBurglary House X19\nZero's RC Shop\nAmmu-nation (version 4)\nAmmu-nation (version 5)\nBurglary House X20\n24/7 (version 6)\nSecret Valley Diner\nRosenberg's Office in Caligulas\nFanny Batter's ",string);
    format(string,sizeof(string),"%sWhore House\nColonel Furhberger's\nCluckin' Bell\nThe Camel's Toe Safehouse\nCaligula's Roof\nOld Venturas Strip Casino\nDriving School\nVerdant Bluffs Safehouse\nBike School\nAndromada\nFour ",string);
    format(string,sizeof(string),"%sDragons' Janitor's Office\nBar\nBurglary House X21\nWillowfield Safehouse\nBurglary House X22\nBurglary House X23\nOtto's Autos\nWang Cars",string);
    ShowPlayerDialog(playerid,DIALOG_TELEPORT_OPTIONS,DIALOG_STYLE_LIST,"Interiors",string,"Teleport","Close");
    return 1;
}

CMD:setloc(playerid, params[]) 
{
    new Float:pos[3];
    if(sscanf(params, "fff", pos[0], pos[1], pos[2]))
        return SendCommandMessage(playerid, "/setloc [x] [y] [z]");

    SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    SendClientMessageEx(playerid, X11_GREEN, "TELEPORT: "WHITE"Teleported to coordinate "YELLOW"%.2f"WHITE" "YELLOW"%.2f"WHITE" "YELLOW"%.2f", pos[0], pos[1], pos[2]);
    return 1;
}

CMD:setint(playerid, params[])
{
    if(IsNull(params))
        return SendCommandMessage(playerid, "/setint [interior]");

    new interior = strval(params);

    SetPlayerInterior(playerid, interior);
    SendClientMessageEx(playerid, X11_GRAY, "interior %i", interior);
    return 1;
}

CMD:gotols(playerid, params[])
{
    SetPlayerPos(playerid, 1531.5206, -1672.3798, 13.3828);
    SendClientMessageEx(playerid, X11_GREEN, "TELEPORT: "WHITE"Teleported to "YELLOW"Los Santos");
    return 1;
}