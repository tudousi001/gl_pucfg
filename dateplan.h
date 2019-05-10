//---------------------------------------------------------------------------

#ifndef dateplanH
#define dateplanH
//---------------------------------------------------------------------------
#include <Classes.hpp>  
#include <ComCtrls.hpp>

#define PLAN_HEAD_WIDTH 42
#define PLAN_HEAD_HEIGHT 32
#define PLAN_SELECTED_COLOR clLime
#define PLAN_NORMAL_COLOR  clSkyBlue

class TDatePlan
{
private:
    TPaintBox *PaintBox;                    
    char State[168];
    int W, H;
    int X0, Y0, X1, Y1;
    int MouseSt;
    int SelectSt;

    void __fastcall OnPaint(TObject *Sender);
    void __fastcall OnMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall OnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall OnMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y);
    void __fastcall DrawState();
public:
    TDatePlan(TPaintBox *Box, int w, int h);
    ~TDatePlan();
    void SetPlan(char *Plan);
    void GetPlan(char *Plan);
    void SetPlan(unsigned int *plan);
    void GetPlan(unsigned int *plan);
    void Clear();
};

#endif
