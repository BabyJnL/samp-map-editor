#include <YSI_Coding\y_va>

hook OnPlayerUpdate(playerid) 
{
    SetPlayerHealth(playerid, 400.0);
    return 1;
}