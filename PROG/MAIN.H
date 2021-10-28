
#define TOTALSHAPE 10
#define TOTALANIMS 10
#define TOTALOBJECTS 50

extern void ptext(void);
extern void Init_Mode(void);
extern void Close_Mode(void);
extern void helpscreen(void);
extern void Calc_Coords(void);
extern void Init_Screen(void);
extern void initarray(void);
extern void PutColor();
extern void BoxImage();
extern void BoxMask();
extern int Get_Mouse(int command);
extern void CutImage(void);
extern void PutImage(void);
extern void RestoreBack(void);
extern void CustomGrid(void);
extern void Check_Draw(void);
extern void BoxImage(void);
extern void BoxImage(void);
extern void PutImage(void);
extern void SaveLastImage(void);
extern void PutColor(void);
extern void DrawGrid(void);
extern void Check_Color(void);
extern int bioskey(int cmd);
extern void GetKeys(void);
extern void Colors(void);
extern void nextshape(int , int);
extern void drawshape(int , int);
extern void Draw_Data_to_Grid(int , int);
extern void clearshape(void);
extern void cleargrid(void);
extern void placeshape(int , int);
extern void draw_screen(int , int);
extern void layout_screen(int , int);
extern int filewrite(void);
extern int fileread(void);
extern void EraseObject(void);
extern void SaveLastImage(void);
extern void UndoLastChange(void);
extern void copyshape(void);
extern void flipshape(void);
extern void Animate(void);
extern void Debug(void);
extern void BigBox(int , int);
extern void Check_Select(void);
extern void draw_plus_minus(void);
extern void DrawPnt(void);
extern void SaveRow(void);
extern void DrawRow(int rw);
