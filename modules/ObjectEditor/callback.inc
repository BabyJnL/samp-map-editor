#include <YSI_Coding\y_hooks>

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

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_TEXT_MENU)
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
    return 1;
}