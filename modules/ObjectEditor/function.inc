CreateObjectEx(model, Float:x, Float:y, Float:z, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, group = 0)
{
    new slot = Iter_Free(Objects);
    if(slot == cellmin) 
        return -1;

    Object[slot] = CreateDynamicObject(model, x, y, z, rx, ry, rz);
    ObjectGroup[slot] = group;
    ObjectEditor[slot] = INVALID_PLAYER_ID;

    Iter_Add(Objects, slot);
    CreateObjectLabel(slot, model, x, y, z, group);
    return slot;
}

IsValidObjectEx(slot) 
{
    return Iter_Contains(Objects, slot);
}

CreateObjectLabel(slot, model, Float:x, Float:y, Float:z, group = 0) 
{
    if(!IsValidObjectEx(slot))
        return 0;

    new label[128];
    format(label, sizeof(label), "Slot "YELLOW"%i\n"WHITE"Model "CYAN"%i\n"WHITE"Group "GREEN"%i", slot, model, group);
    ObjectLabel[slot] = CreateDynamic3DTextLabel(label, -1, x, y, z, 15.0);

    return 1;
}

UpdateObjectLabel(slot) 
{
    if(!IsValidObjectEx(slot))
        return 0;

    new 
        model,
        Float:pos[3]
    ;

    model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_MODEL_ID);
    GetDynamicObjectPos(Object[slot], pos[0], pos[1], pos[2]);

    DestroyObjectLabel(slot);
    CreateObjectLabel(slot, model, pos[0], pos[1], pos[2], ObjectGroup[slot]);

    return 1;
}

DestroyObjectLabel(slot) 
{
    if(!IsValidObjectEx(slot))
        return 0;

    DestroyDynamic3DTextLabel(ObjectLabel[slot]);
    return 1;
}

DestroyObjectEx(slot) 
{
    if(!IsValidObjectEx(slot))
        return 0;
    

    DestroyObjectLabel(slot);
    RemoveAllObjectMaterialEx(slot);
    DestroyDynamicObject(Object[slot]);

    Iter_Remove(Objects, slot);
    return 1;
}

DestroyAllObjects()
{
    foreach(new slot : Objects)
        DestroyObjectEx(slot);

    Iter_Clear(Objects);
    return 1;
}

SetObjectMaterialEx(slot, index, model, const txdname[], const texture[], alpha, red, green, blue) 
{
    if(!IsValidObjectEx(slot))
        return 0;

    if(model == 0) 
        model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, Object[slot], E_STREAMER_MODEL_ID);

    SetDynamicObjectMaterial(Object[slot], index, model, txdname, texture, RGBAToHex(alpha, red, green, blue));
    ObjectMaterial[slot][index] = MATERIAL_TYPE_TEXTURE;
    return 1;
}

// RemoveObjectMaterialEx(slot, index) 
// {
//     if(!IsValidObjectEx(slot))
//         return 0;

//     ObjectMaterial[slot][index] = MATERIAL_TYPE_NONE;
//     RemoveDynamicObjectMaterial(Object[slot], i);
//     return 1;
// }

RemoveAllObjectMaterialEx(slot) 
{
    if(!IsValidObjectEx(slot))
        return 0;

    Loop(i, MAX_OBJECT_MATERIAL_SLOT)
        ObjectMaterial[slot][i] = MATERIAL_TYPE_NONE;
    return 1;
}

MoveDynamicObjectEx(slot, Float:x, Float:y, Float:z, Float:speed)
{
    if(!IsValidObjectEx(slot))
        return 0;

	if(IsDynamicObjectMoving(Object[slot])) 
        StopDynamicObject(Object[slot]);

	return MoveDynamicObject(Object[slot], x, y, z, speed);
}