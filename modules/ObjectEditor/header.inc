#define MAX_EDITING_OBJECT			1000
#define MAX_OBJECT_MATERIAL_SLOT	10

#define MATERIAL_TYPE_NONE		0
#define MATERIAL_TYPE_TEXTURE	1
#define MATERIAL_TYPE_MESSAGE	2

new 
    Iterator:Objects<MAX_EDITING_OBJECT>,
    Object[MAX_EDITING_OBJECT],
    ObjectGroup[MAX_EDITING_OBJECT],
    Text3D:ObjectLabel[MAX_EDITING_OBJECT],
    ObjectEditor[MAX_EDITING_OBJECT],
    ObjectMaterial[MAX_EDITING_OBJECT][MAX_OBJECT_MATERIAL_SLOT]
;

new const WinFonts[15][20] = {
    {"Arial"},
    {"Calibri"},
    {"Comic Sans MS"},
    {"Georgia"},
    {"Times New Roman"},
    {"Consolas"},
    {"Constantia"},
    {"Corbel"},
    {"Courier New"},
    {"Impact"},
    {"Lucida Console"},
    {"Palatino Linotype"},
    {"Tahoma"},
    {"Trebuchet MS"},
    {"Verdana"}
};

enum 
{
    DIALOG_TEXT_MENU,
    DIALOG_TEXT_SET_MESSAGE,
    DIALOG_TEXT_SET_RESOLUTION,
    DIALOG_TEXT_SET_FONT,
    DIALOG_TEXT_SET_CUSTOM_FONT,
    DIALOG_TEXT_SET_FONT_SIZE,
    DIALOG_TEXT_SET_FONT_COLOR,
    DIALOG_TEXT_SET_BG_COLOR,
    DIALOG_TEXT_SET_ALLIGNMENT
}