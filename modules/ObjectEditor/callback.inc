#include <YSI_Coding\y_hooks>

hook OnGameModeExit() 
{
    DestroyAllObjects();
}

hook OnPlayerDisconnect(playerid, reason) 
{
    if(GetPVarType(playerid,"EditingObject") > 0)
	{
	    new slot = GetPVarInt(playerid,"EditingObject");
		DeletePVar(playerid,"EditingObject");
	    ObjectEditor[slot] = INVALID_PLAYER_ID;
	}
}

hook OnDynamicObjectMoved(objectid)
{
    foreach(new slot : Objects)
    {
        if(Object[slot] != objectid)
            continue;

        new Float:pos[3];
        GetDynamicObjectPos(objectid, pos[0], pos[1], pos[2]);

        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ObjectLabel[slot], E_STREAMER_X, pos[0]);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ObjectLabel[slot], E_STREAMER_Y, pos[1]);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ObjectLabel[slot], E_STREAMER_Z, pos[2]);
        break;
    }
    return 1;
}

hook OnPlayerSelectDynamicObj(playerid, objectid, modelid, Float:x, Float:y, Float:z)
{
	foreach(new slot : Objects)
	{
	    if(Object[slot] == objectid)
	    {
            if((ObjectEditor[slot] != INVALID_PLAYER_ID) && (GetPVarInt(ObjectEditor[slot],"EditingObject") == slot))
            {
                new editor = ObjectEditor[slot];
                return SendErrorMessage(playerid, ""YELLOW"%s "WHITE"is currently editing this object!", ReturnPlayerName(editor));
            }

            ObjectEditor[slot] = playerid;
            SetPVarInt(playerid,"EditingObject",slot);
            EditDynamicObject(playerid,objectid);
            SendObjectMessage(playerid, "Selected object with "YELLOW"slot %i",slot);
        }
    }
    return 1;
}

hook OnPlayerEditDynamicObj(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    if(response == EDIT_RESPONSE_FINAL)
	{
		if(GetPVarType(playerid,"EditingObject") > 0)
		{
		    new slot = GetPVarInt(playerid,"EditingObject");
		    DeletePVar(playerid,"EditingObject");
		    ObjectEditor[slot] = INVALID_PLAYER_ID;

            Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ObjectLabel[slot], E_STREAMER_X, x);
            Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ObjectLabel[slot], E_STREAMER_Y, y);
            Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, ObjectLabel[slot], E_STREAMER_Z, z);

		    SetDynamicObjectPos(objectid,x,y,z);
            SetDynamicObjectRot(objectid,rx,ry,rz);
		}
	}
	else if(response == EDIT_RESPONSE_CANCEL)
	{
	    if(GetPVarType(playerid,"EditingObject") > 0)
		{
		    new slot = GetPVarInt(playerid,"EditingObject");
		    DeletePVar(playerid,"EditingObject");
		    ObjectEditor[slot] = INVALID_PLAYER_ID;

		    new Float:cPos[3],Float:cRot[3];
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_X,cPos[0]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_Y,cPos[1]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_Z,cPos[2]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_R_X,cRot[0]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_R_Y,cRot[1]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_R_Z,cRot[2]);

            SetDynamicObjectPos(objectid,cPos[0],cPos[1],cPos[2]);
            SetDynamicObjectRot(objectid,cRot[0],cRot[1],cRot[2]);
		}
	}
    return 1;
}

hook OnPlayerShootDynamicObj(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
{
    foreach(new idx : Objects)
    {
        if(Object[idx] != objectid)
            continue;

        if(GetPVarType(playerid, "PaintParam") == PLAYER_VARTYPE_STRING)
        {
            new 
                var[256],
                index,
                model,
                txd_name[32],
                texture[32],
                colour
            ;

            GetPVarString(playerid, "PaintParam", var);
            unformat(var, "p<|>iis[32]s[32]i", index, model, txd_name, texture, colour);
            SetDynamicObjectMaterial(Object[idx], index, model, txd_name, texture, colour); 
            ObjectMaterial[idx][index] = MATERIAL_TYPE_TEXTURE;
            break;
        }
    }
    return 1;
}