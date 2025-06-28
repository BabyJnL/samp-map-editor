GetXYLeftOfPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x - (distance * floatsin(-A+90.0,degrees));
	y2 = y - (distance * floatcos(-A+90.0,degrees));
}

GetXYRightOfPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x - (distance * floatsin(-A-90.0,degrees));
	y2 = y - (distance * floatcos(-A-90.0,degrees));
}

GetXYInFrontOfPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x + (distance * floatsin(-A,degrees));
	y2 = y + (distance * floatcos(-A,degrees));
}

GetXYBehindPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x - (distance * floatsin(-A,degrees));
	y2 = y - (distance * floatcos(-A,degrees));
}

stock SendClientMessageEx(playerid, color, const text[], {Float, _}:...)
{
	static
	    args,
	    str[144];

	/*
     *  Custom function that uses #emit to format variables into a string.
     *  This code is very fragile; touching any code here will cause crashing!
	*/
	if ((args = numargs()) == 3)
	{
	    SendClientMessage(playerid, color, text);
	}
	else
	{
		while (--args >= 3)
		{
			#emit LCTRL 5
			#emit LOAD.alt args
			#emit SHL.C.alt 2
			#emit ADD.C 12
			#emit ADD
			#emit LOAD.I
			#emit PUSH.pri
		}
		#emit PUSH.S text
		#emit PUSH.C 144
		#emit PUSH.C str
		#emit PUSH.S 8
		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

		SendClientMessage(playerid, color, str);

		#emit RETN
	}
	return 1;
}

HexToRGBA(value, &a, &r, &g, &b)
{
    a = value & 0xFF;
    r = (value >> 24) & 0xFF;
    g = (value >> 16) & 0xFF;
    b = (value >> 8) & 0xFF;
}

stock ReplaceSlashNWithNewline(const input[], output[], size = sizeof(output))
{
    new i = 0, j = 0;

    // Hilangkan tanda kutip " di awal
    if (input[0] == '"') i++;

    while (input[i] != '\0' && j < size - 1)
    {
        if (input[i] == '"' && input[i + 1] == '\0')
            break;

        if (input[i] == '\\' && input[i + 1] == 'n')
        {
            output[j++] = '\n';
            i += 2;
        }
        else
        {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
    return 1;
}

stock EscapeNewlineForExport(const input[], output[], size = sizeof(output))
{
    new i = 0, j = 0;
    while (input[i] != '\0' && j < size - 2) 
    {
        if (input[i] == '\n')
        {
            output[j++] = '\\';
            output[j++] = 'n';
        }
        else
        {
            output[j++] = input[i];
        }
        i++;
    }
    output[j] = '\0';
}

