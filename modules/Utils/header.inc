#define SendCommandMessage(%0,%1) \
    SendClientMessageEx(%0, X11_GREEN, "CMD: "WHITE""%1)

#define SendObjectMessage(%0,%1) \
    SendClientMessageEx(%0, X11_GREEN, "OBJECT: "WHITE""%1)

#define SendErrorMessage(%0,%1) \
    SendClientMessageEx(%0, X11_RED, "ERROR: "WHITE""%1)

#define RGBAToHex(%0,%1,%2,%3) \
	((0x1000000 * (%0)) + (0x10000 * (%1)) + (0x100 * (%2)) + (%3))

#define Loop(%0,%1) for(new %0 = 0; %0 < %1; %0++)    

enum 
{
    DIALOG_CREDITS
}

new const core_contributors[][][] =
{
    {"tianmetal", "For Base Script"},
    {"BabyJnL", "Bug Fixes & Enhancements"}
};

new const third_party_credits[][][] = 
{
    {"Southclaws", "For sampctl and tooling"},
    {"Y_Less", "For YSI and sscanf"}, 
    {"Incognito", "For Streamer Plugin"},
    {"JaTochNietDan", "For FileManager"}
};