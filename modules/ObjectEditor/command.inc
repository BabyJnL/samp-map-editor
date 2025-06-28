SSCANF:objectmenu(string[])
{
	if(!strcmp(string,"create",true)) return 1;
	else if(!strcmp(string,"add",true)) return 1;

	else if(!strcmp(string,"delete",true)) return 2;
	else if(!strcmp(string,"destroy",true)) return 2;
	else if(!strcmp(string,"remove",true)) return 2;

	else if(!strcmp(string,"clear",true)) return 3;
	else if(!strcmp(string,"reset",true)) return 3;

	else if(!strcmp(string,"copy",true)) return 4;
	else if(!strcmp(string,"clone",true)) return 4;
	else if(!strcmp(string,"duplicate",true)) return 4;

	else if(!strcmp(string,"move",true)) return 5;

	else if(!strcmp(string,"rot",true)) return 6;
	else if(!strcmp(string,"rotate",true)) return 6;

	else if(!strcmp(string,"select",true)) return 7;
	else if(!strcmp(string,"control",true)) return 7;

	else if(!strcmp(string,"tele",true)) return 8;
	else if(!strcmp(string,"goto",true)) return 8;

	else if(!strcmp(string,"material",true)) return 9;

    else if(!strcmp(string,"resetmaterial",true)) return 10;
	else if(!strcmp(string,"clearmaterial",true))	return 10;

	else if(!strcmp(string,"textprop",true)) return 11;

    else if(!strcmp(string,"paintbrush",true)) return 12;

	else if(!strcmp(string,"model",true)) return 13;

	else if(!strcmp(string,"export",true)) return 14;
    else if(!strcmp(string,"save",true)) return 14;

	else if(!strcmp(string,"import",true)) return 15;
	else if(!strcmp(string,"load",true)) return 15;

    else if(!strcmp(string,"group",true)) return 16;

    else if(!strcmp(string,"gcopy",true)) return 17;
    else if(!strcmp(string,"gclone",true)) return 17;

    else if(!strcmp(string,"gmove",true)) return 18;

    else if(!strcmp(string,"grot",true)) return 19;
    else if(!strcmp(string,"grotate",true)) return 19;

    else if(!strcmp(string,"gdelete",true)) return 20;
    else if(!strcmp(string,"gdestroy",true)) return 20;
    else if(!strcmp(string,"gremove",true)) return 20;

	return 0;
}

CMD:object(playerid, params[]) 
{
    new string[256],item,subparam[96];
	sscanf(params,"K<objectmenu>(0)S()[96]",item,subparam);
    switch(item) 
    {
        case 1: 
        {
            // Create an Object

            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object create [model id]");

            new model = strval(subparam);
            if(!model) 
                return SendErrorMessage(playerid, "Invalid object model");

            new Float:pos[3];
            GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

            new slot = CreateObjectEx(model, pos[0], pos[1], pos[2]);
            if(slot == -1) 
                return SendErrorMessage(playerid, "Full slot!");
            
            Streamer_Update(playerid);
            SendObjectMessage(playerid, "Object created with "YELLOW"slot %i"WHITE", total objects: "GREEN"%i", slot, Iter_Count(Objects));
        }
        case 2: 
        {
            // Remove an Object

            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object delete [slot]");

            new slot = strval(subparam);
            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            DestroyObjectEx(slot);
            SendObjectMessage(playerid, "Object with "YELLOW"slot %i "WHITE"has been deleted, total objects: "GREEN"%i", slot, Iter_Count(Objects));
        }
        case 3:
        {
            // Clear Created Object

            if(IsNull(subparam) || strcmp(subparam, "confirm", true))
                return SendCommandMessage(playerid, "Are you sure you want to clear all the objects? ("YELLOW"/object clear confirm"WHITE")");

            new objectcount = Iter_Count(Objects);
            if(objectcount == 0)
                return SendErrorMessage(playerid, "No objects created");

            DestroyAllObjects();
            SendObjectMessage(playerid, ""YELLOW"%i objects "WHITE"have been cleared", objectcount);
        }
        case 4:
        {
            // Copy an Object

            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object copy [slot]");

            new slot = strval(subparam);
            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            new
                Float:pos[3],
                Float:rot[3],
                model,
                txdname[32],
                texture[32],
                materialColour,
                rgbaColor[4],
                text[255],
                resolution,
                font[20],
                size,
                bold, 
                fcolor,
                bcolor,
                alignment
            ;

            new objectmodel = Streamer_GetIntData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_MODEL_ID);
            GetDynamicObjectPos(Object[slot], pos[0], pos[1], pos[2]);
            GetDynamicObjectRot(Object[slot], rot[0], rot[1], rot[2]);

            new copyslot = CreateObjectEx(objectmodel, pos[0], pos[1], pos[2], rot[0], rot[1], rot[2], ObjectGroup[slot]);
            Loop(i, MAX_OBJECT_MATERIAL_SLOT)
            {
                if(ObjectMaterial[slot][i] == MATERIAL_TYPE_TEXTURE)
                {
                    GetDynamicObjectMaterial(Object[slot], i, model, txdname, texture, materialColour);
                    HexToRGBA(materialColour, rgbaColor[0], rgbaColor[1], rgbaColor[2], rgbaColor[3]);
                    SetObjectMaterialEx(copyslot, i, model, txdname, texture, rgbaColor[0], rgbaColor[1], rgbaColor[2], rgbaColor[3]);
                    ObjectMaterial[copyslot][i] = MATERIAL_TYPE_TEXTURE;
                }
                else if(ObjectMaterial[slot][i] == MATERIAL_TYPE_MESSAGE)
                {
                    GetDynamicObjectMaterialText(Object[slot],i,text,resolution,font,size,bold,fcolor,bcolor,alignment);
                    SetDynamicObjectMaterialText(Object[copyslot],i,text,resolution,font,size,bold,fcolor,bcolor,alignment);
                    ObjectMaterial[copyslot][i] = MATERIAL_TYPE_MESSAGE;
                }
            }
            Streamer_Update(playerid);
            SendCommandMessage(playerid, ""WHITE"Duplicated object with "YELLOW"slot %i "WHITE" to "YELLOW"slot %i"WHITE", total objects: "GREEN"%d", slot, copyslot, Iter_Count(Objects));
        }
        case 5: 
        {
            // Moving Object

            new 
                slot, 
                direction, 
                Float:amount, 
                Float:speed
            ;
            if(sscanf(subparam, "icfF(5.0)", slot, direction, amount, speed))
                return SendCommandMessage(playerid, "/object move [object id] [directoion (N/S/E/W/U(p)/D(own))] [amount] [opt:speed = 5.0]");

            if(floatround(speed) <= 0) 
                return SendErrorMessage(playerid, "Speed cannot go below 1!");
            
            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            new Float:cPos[4];
            GetDynamicObjectPos(Object[slot],cPos[0],cPos[1],cPos[2]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_R_Z,cPos[3]);
            switch(direction)
            {
                case 'N','n': cPos[1] += amount;
                case 'S','s': cPos[1] -= amount;
                case 'E','e': cPos[0] += amount;
                case 'W','w': cPos[0] -= amount;
                case 'U','u': cPos[2] += amount;
                case 'D','d': cPos[2] -= amount;
                case 'L','l': GetXYLeftOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                case 'R','r': GetXYRightOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                case 'F','f': GetXYInFrontOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                case 'B','b': GetXYBehindPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                default: return SendErrorMessage(playerid,"Invalid direction!");
            }
            MoveDynamicObjectEx(slot, cPos[0] ,cPos[1], cPos[2], speed);
        }
        case 6: 
        {
            // Rotating Object

            new 
                slot, 
                Float:rot[3]
            ;
            if(sscanf(subparam, "ifff", slot, rot[0], rot[1], rot[2]))
                return SendCommandMessage(playerid, "/object rotate [slot] [x] [y] [z]");

            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            SetDynamicObjectRot(Object[slot], rot[0], rot[1], rot[2]);
        }
        case 7: 
        {
            // Editing Object

            if(GetPVarType(playerid, "EditingObject") > 0)
                return SendErrorMessage(playerid, "You must first release your current object!");
            
            if(IsNull(subparam))
                return SelectObject(playerid);

            new slot = strval(subparam);
            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            if(ObjectEditor[slot] != INVALID_PLAYER_ID)
            {
                new editor = ObjectEditor[slot];
                if(GetPVarType(editor, "EditingObject") == slot)
                    return SendErrorMessage(playerid, ""YELLOW"%s"WHITE" is currently editing this object!", ReturnPlayerName(editor));
            }

            SetPVarInt(playerid, "EditingObject", slot);
            EditDynamicObject(playerid, Object[slot]);
            SendObjectMessage(playerid, "Selected object "YELLOW"slot %i", slot);
        }
        case 8: 
        {
            // Teleport to object slot

            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object tele [slot]");

            new slot = strval(subparam); 
            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            new Float:pos[3];
            GetDynamicObjectPos(Object[slot], pos[0], pos[1], pos[2]);
            SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
            SendObjectMessage(playerid, "Teleported to object "YELLOW"slot %i", slot);
        }
        case 9:
        {
            // Set Object Material

            new
                slot, 
                index,
                model,
                txdname[32],
                texture[32],
                color[4]
            ;
            if(sscanf(subparam, "iiis[32]s[32]I(0)I(0)I(0)I(0)", slot, index, model, txdname, texture, color[0], color[1], color[2], color[3]))
                return SendCommandMessage(playerid, "/object material [slot] [index] [model] [txdname] [texture] [opt: alpha] [opt: red]  [opt: green]  [opt: blue]");

            if(index < 0 || index > MAX_OBJECT_MATERIAL_SLOT)
                return SendErrorMessage(playerid, "index cannot go below 0 or over 9!");

            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            if(model == 0)
                model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_MODEL_ID);

            SetObjectMaterialEx(slot, index, model, txdname, texture, color[0], color[1], color[2], color[3]);
        }
        case 10: 
        {
            // Reset Object Material

            if(IsNull(subparam)) 
                return SendCommandMessage(playerid, "/object resetmaterial [slot]");

            new slot = strval(subparam);
            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            RemoveAllObjectMaterialEx(slot);

            new
                model,
                Float:pos[3],
                Float:rot[3]
            ;

            model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_MODEL_ID);
            GetDynamicObjectPos(Object[slot], pos[0], pos[1], pos[2]);
            GetDynamicObjectRot(Object[slot], rot[0], rot[1], rot[2]);

            DestroyDynamicObject(Object[slot]);
            DestroyDynamic3DTextLabel(ObjectLabel[slot]);

            CreateObjectLabel(slot, model, pos[0], pos[1], pos[2]);
            Object[slot] = CreateDynamicObject(model, pos[0], pos[1], pos[2], rot[0], rot[1], rot[2]);

            Streamer_Update(playerid);
        }
        case 11: 
        {
            // Set Object Text Prop

            new 
                slot,
                index
            ;
            if(sscanf(subparam, "iI(0)", slot, index))
                return SendCommandMessage(playerid, "/object textprop [slot] [index (default 0)]");

            if((index >= MAX_OBJECT_MATERIAL_SLOT) || (index < 0)) 
                return SendErrorMessage(playerid,"Index cannot go below 0 or over 9!");

            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            if(ObjectMaterial[slot][index] != MATERIAL_TYPE_MESSAGE)
            {
                SetDynamicObjectMaterialText(Object[slot],index,"Text Here",OBJECT_MATERIAL_SIZE_256x128,"Arial",24,1,0xFFFFFFFF,0xFF000000);
                ObjectMaterial[slot][index] = MATERIAL_TYPE_MESSAGE;
            }
            SetPVarInt(playerid,"EditingObject",slot);
            SetPVarInt(playerid,"EditingIndex",index);
            ShowPlayerDialog(playerid,DIALOG_TEXT_MENU,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
        }
        case 12: 
        {
            // Paintbrush

            new 
                index,
                model,
                txd_name[32],
                texture[32],
                color[4]
            ;

            if(sscanf(subparam, "iis[32]s[32]I(0)I(0)I(0)I(0)", index, model, txd_name, texture, color[0], color[1], color[2], color[3]))
                return SendCommandMessage(playerid, "/object paintbrush [material index] [model] [txdname] [texture] [opt: alpha] [opt: red]  [opt: green]  [opt: blue]");

            if(index >= MAX_OBJECT_MATERIAL_SLOT || index < 0)
                return SendErrorMessage(playerid, "Material index cannot go below 0 or over 9!");

            format(string, sizeof(string), "%i|%i|%s|%s|%x", index, model, txd_name, texture, RGBAToHex(color[0],color[1],color[2],color[3]));
            SetPVarString(playerid, "PaintParam", string);
            SendObjectMessage(playerid, "You can now set object material by shot on targeted object");
        }
        case 13: 
        {
            // Change Object Model
            new  
                slot, 
                model
            ;

            if(sscanf(subparam, "ii", slot, model))
                return SendCommandMessage(playerid, "/object model [slot] [model]");

            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            new Float:pos[3];
            GetDynamicObjectPos(Object[slot], pos[0], pos[1], pos[2]);

            RemoveAllObjectMaterialEx(slot);

            DestroyDynamicObject(Object[slot]);
            DestroyDynamic3DTextLabel(ObjectLabel[slot]);

            CreateObjectLabel(slot, model, pos[0], pos[1], pos[2]);
            Object[slot] = CreateDynamicObject(model, pos[0], pos[1], pos[2], 0, 0, 0);

            Streamer_Update(playerid);
        }
        case 14:
        {
            // Export map

            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object export [map name]");

            new totalobject = Iter_Count(Objects);
            if(totalobject == 0)
                return SendErrorMessage(playerid, "There aren't object to be exported");

            new name[64];
            format(name, sizeof(name), "map/%s.txt", subparam);

            new 
                File:file = f_open(name, "w"),
                model,
                Float:cPos[3],
                Float:cRot[3]
            ;

            if(!file)
                return SendErrorMessage(playerid, "Failed to open %s.txt", name);

            foreach(new slot : Objects)
            {
                new 
                    obString[512],
                    material_model_id,
                    txd_name[32],
                    texture[32],
                    material_colour,
                    text[256],
                    size,
                    font[32],
                    fsize,
                    bold,
                    fcolor,
                    bcolor,
                    alignment
                ;

                model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_MODEL_ID);

                Streamer_GetFloatData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_X, cPos[0]);
                Streamer_GetFloatData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_Y, cPos[1]);
                Streamer_GetFloatData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_Z, cPos[2]);
                Streamer_GetFloatData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_R_X, cRot[0]);
                Streamer_GetFloatData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_R_Y, cRot[1]);
                Streamer_GetFloatData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_R_Z, cRot[2]);

                format(string, sizeof(string), "obj = CreateObject(%i,%f,%f,%f,%f,%f,%f);\n", model, cPos[0], cPos[1], cPos[2], cRot[0], cRot[1], cRot[2]);
                strcat(obString, string);

                for(new i = 0; i < MAX_OBJECT_MATERIAL_SLOT; i++)
                {
                    if(ObjectMaterial[slot][i] == MATERIAL_TYPE_TEXTURE) 
                    {
                        GetDynamicObjectMaterial(Object[slot], i, material_model_id, txd_name, texture, material_colour);

                        format(string, sizeof(string), "SetObjectMaterial(obj,%i,%i,%s,%s,%x);\n", i, material_model_id, txd_name, texture, material_colour);
                        strcat(obString, string);
                    }
                    else if(ObjectMaterial[slot][i] == MATERIAL_TYPE_MESSAGE)
                    {
                        GetDynamicObjectMaterialText(Object[slot], i, text, size, font, fsize, bold, fcolor, bcolor, alignment);

                        format(string, sizeof(string), "SetObjectMaterialText(obj,%s,%i,%i,%s,%i,%i,0x%x,0x%x,%i);\n", text, i, size, font, fsize, bold, fcolor, bcolor, alignment);
                        strcat(obString, string);
                    }
                }
                f_write(file, obString);
            }
            f_close(file);
            SendObjectMessage(playerid, ""YELLOW"%i"WHITE" objects saved to file "CYAN"%s", totalobject, name);
        }
        case 15: 
        {
            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object load [map name]");
            
            new name[64];
            format(name, sizeof(name), "map/%s.txt", subparam);

            if(!file_exists(name))
                return SendErrorMessage(playerid, "Failed to load %s, no such file", name);

            if(Iter_Count(Objects) != 0)
                return SendErrorMessage(playerid, "You need to clear current object before load a map");

            new 
                File:file = f_open(name, "r"),
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
                        // if(!unformat(str,"p<,>{i}s[128]iis[32]I(24)I(1)I(-1)I(-16777216)I(1)", text, index, resolution, font, fontsize, bold, fontcolor, color, alignment))
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
                        if(!unformat(string,"p<,>{s[32]}dds[32]s[32]D(-1)", index, model, txdname, texture, color))
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
            SendObjectMessage(playerid, ""WHITE"Map "CYAN"%s"WHITE" successfully loaded, total objects: "YELLOW"%i", subparam, object_counter);
        }
        case 16:
        {
            new 
                slot, 
                group
            ;
            if(sscanf(subparam, "ii", slot, group))
                return SendCommandMessage(playerid, "/object group [slot] [group id]");

            if(!IsValidObjectEx(slot))
                return SendErrorMessage(playerid, "Object slot "YELLOW"%i "WHITE"doesn't exists!", slot);

            ObjectGroup[slot] = group;
            UpdateObjectLabel(slot);

            Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
        }
        case 17: 
        {
            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object gclone [group id]");

            new
                group = strval(subparam),
                objectlist[MAX_EDITING_OBJECT],
                objectcount,
                Float:pos[3],
                Float:rot[3],
                model,
                txdname[32],
                texture[32],
                materialColour,
                rgbaColor[4],
                text[255],
                resolution,
                font[20],
                size,
                bold, 
                fcolor,
                bcolor,
                alignment
            ;

            foreach(new slot : Objects) 
            {
                if(ObjectGroup[slot] == group)
                    objectlist[objectcount++] = slot;
            }

            Loop(idx, objectcount) 
            {
                new 
                    slot = objectlist[idx],
                    newgroup = ObjectGroup[slot] + 1
                ;

                new objectmodel = Streamer_GetIntData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_MODEL_ID);
                GetDynamicObjectPos(Object[slot], pos[0], pos[1], pos[2]);
                GetDynamicObjectRot(Object[slot], rot[0], rot[1], rot[2]);

                new copyslot = CreateObjectEx(objectmodel, pos[0], pos[1], pos[2], rot[0], rot[1], rot[2], newgroup);
                Loop(i, MAX_OBJECT_MATERIAL_SLOT)
                {
                    if(ObjectMaterial[slot][i] == MATERIAL_TYPE_TEXTURE)
                    {
                        GetDynamicObjectMaterial(Object[slot], i, model, txdname, texture, materialColour);
                        HexToRGBA(materialColour, rgbaColor[0], rgbaColor[1], rgbaColor[2], rgbaColor[3]);
                        SetObjectMaterialEx(copyslot, i, model, txdname, texture, rgbaColor[0], rgbaColor[1], rgbaColor[2], rgbaColor[3]);
                        ObjectMaterial[copyslot][i] = MATERIAL_TYPE_TEXTURE;
                    }
                    else if(ObjectMaterial[slot][i] == MATERIAL_TYPE_MESSAGE)
                    {
                        GetDynamicObjectMaterialText(Object[slot],i,text,resolution,font,size,bold,fcolor,bcolor,alignment);
                        SetDynamicObjectMaterialText(Object[copyslot],i,text,resolution,font,size,bold,fcolor,bcolor,alignment);
                        ObjectMaterial[copyslot][i] = MATERIAL_TYPE_MESSAGE;
                    }
                }
            }
            Streamer_Update(playerid);
            SendObjectMessage(playerid, ""WHITE"Duplicated object with "YELLOW"group %i"WHITE" to "YELLOW"group %i"WHITE", total objects: "GREEN"%d", group, Iter_Count(Objects));
        }
        case 18: 
        {
            new 
                group, 
                direction, 
                Float:amount, 
                Float:speed
            ;
            if(sscanf(subparam, "icfF(5.0)", group, direction, amount, speed))
                return SendCommandMessage(playerid, "/object gmove [group id] [direction (N/S/E/W/U(p)/D(own))] [amount] [opt:speed = 5.0]");

            if(floatround(speed) <= 0) 
                return SendErrorMessage(playerid, "Speed cannot go below 1!");

            foreach(new slot : Objects) 
            {
                if(ObjectGroup[slot] != group)
                    continue;

                new Float:cPos[4];
                GetDynamicObjectPos(Object[slot],cPos[0],cPos[1],cPos[2]);
                Streamer_GetFloatData(STREAMER_TYPE_OBJECT,Object[slot],E_STREAMER_R_Z,cPos[3]);
                switch(direction)
                {
                    case 'N','n': cPos[1] += amount;
                    case 'S','s': cPos[1] -= amount;
                    case 'E','e': cPos[0] += amount;
                    case 'W','w': cPos[0] -= amount;
                    case 'U','u': cPos[2] += amount;
                    case 'D','d': cPos[2] -= amount;
                    case 'L','l': GetXYLeftOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                    case 'R','r': GetXYRightOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                    case 'F','f': GetXYInFrontOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                    case 'B','b': GetXYBehindPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                    default: return SendErrorMessage(playerid,"Invalid direction!");
                }
                MoveDynamicObjectEx(slot, cPos[0] ,cPos[1], cPos[2], speed);   
            }
        }
        case 19: 
        {
            new
                group,
                Float:rot[3]
            ;
            if(sscanf(subparam, "ifff", group, rot[0], rot[1], rot[2]))
                return SendCommandMessage(playerid, "/object grotate [group id] [rot X] [rot Y] [rot Z]");

            foreach(new slot : Objects) 
            {
                if(ObjectGroup[slot] != group) 
                    continue;

                SetDynamicObjectRot(Object[slot], rot[0], rot[1], rot[2]);
            }
        }
        case 20: 
        {
            if(IsNull(subparam))
                return SendCommandMessage(playerid, "/object gremove [group id]");

            new 
                group = strval(subparam),
                objectcounter = 0
            ;
            foreach(new slot : Objects) 
            {
                if(ObjectGroup[slot] != group) 
                    continue;

                DestroyObjectEx(slot);
                objectcounter++;
            }
            SendObjectMessage(playerid, ""YELLOW"%i "WHITE"object(s) has been deleted, total objects: "GREEN"%i", objectcounter, Iter_Count(Objects));
        }
        default:
        {
            SendCommandMessage(playerid, "/object [options]");
            SendCommandMessage(playerid, "create, remove, clear, copy, move, rotate, select, tele");
            SendCommandMessage(playerid, "save, model, material, load, textprop, resetmaterial", "paintbrush");
            SendCommandMessage(playerid, "group, gcopy, gmove, grotate, gremove");
        }
    }
    return 1;
}
