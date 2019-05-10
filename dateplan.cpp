//---------------------------------------------------------------------------


#pragma hdrstop

#include "dateplan.h"
#include "stdio.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TDatePlan::TDatePlan(TPaintBox *Box, int w, int h)
{
    PaintBox = Box;
    Box->Width = PLAN_HEAD_WIDTH+24*w+1;
    Box->Height = PLAN_HEAD_HEIGHT+7*h+1;
    PaintBox->OnPaint = OnPaint;
    PaintBox->OnMouseDown = OnMouseDown;
    PaintBox->OnMouseUp = OnMouseUp;
    PaintBox->OnMouseMove = OnMouseMove;
    W = w;
    H = h;
    MouseSt = 0;
    SelectSt = 0;
}

TDatePlan::~TDatePlan()
{
    delete PaintBox;
}

void __fastcall TDatePlan::OnPaint(TObject *Sender)
{
    DrawState();
}

void __fastcall TDatePlan::DrawState()
{
    int i, x, y;
    char tmp[8], *WeekDay[7]=
    {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};

    PaintBox->Canvas->Pen->Color = clBlack;
    PaintBox->Canvas->Brush->Style = bsClear;
    for(y = 0; y < 7; y++)
    {
        PaintBox->Canvas->TextOut(0, y*H+PLAN_HEAD_HEIGHT+2, WeekDay[y]);
    }
    y = PLAN_HEAD_HEIGHT-12;
    for(x = 0; x < 24; x++)
    {
        sprintf(tmp, "%d", x);
        PaintBox->Canvas->TextOut(x*W+PLAN_HEAD_WIDTH, y, tmp);
    }
    y = 2;
    x = PLAN_HEAD_WIDTH+156;
    PaintBox->Canvas->TextOut(x, y, "选中");
    x += 68;
    PaintBox->Canvas->TextOut(x, y, "取消");

    y = 0;
    x = PLAN_HEAD_WIDTH+136;
    PaintBox->Canvas->Brush->Color = PLAN_SELECTED_COLOR;
    PaintBox->Canvas->Rectangle(x, y, x+W, y+H);
    PaintBox->Canvas->Brush->Color = PLAN_NORMAL_COLOR;
    x += 68;
    PaintBox->Canvas->Rectangle(x, y, x+W, y+H);

    for(i = 0; i < 168; i++)
    {
        if(State[i])
        {
            PaintBox->Canvas->Brush->Color = PLAN_SELECTED_COLOR;
        }
        else
        {
            PaintBox->Canvas->Brush->Color = PLAN_NORMAL_COLOR;
        }
        x = i%24*W+PLAN_HEAD_WIDTH;
        y = i/24*H+PLAN_HEAD_HEIGHT;
        PaintBox->Canvas->Rectangle(x, y, x+W+1, y+H+1);
    }
}

void BuildRect(TRect &rt, int X0, int Y0, int X, int Y)
{
    if(X0 < X)
    {
        rt.left = X0;
        rt.right = X;
    }
    else
    {
        rt.left = X;
        rt.right = X0;
    }

    if(Y0 < Y)
    {
        rt.top = Y0;
        rt.bottom = Y;
    }
    else
    {
        rt.top = Y;
        rt.bottom = Y0;
    }
}

void __fastcall TDatePlan::OnMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if((X > PLAN_HEAD_WIDTH)&&(Y > PLAN_HEAD_HEIGHT))
    {
        MouseSt = 1;
        SelectSt = 1;
        X0 = X;
        Y0 = Y;
        X1 = X0;
        Y1 = Y0;
    }
}

void __fastcall TDatePlan::OnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{            
    TRect rt;
    int n, n0, x, y;

    if(MouseSt)
    {
        BuildRect(rt, X0, Y0, X1, Y1);
        rt.left -= PLAN_HEAD_WIDTH;
        rt.top -= PLAN_HEAD_HEIGHT;
        rt.right -= PLAN_HEAD_WIDTH;
        rt.bottom -= PLAN_HEAD_HEIGHT;
        rt.top -= (rt.top%H);
        rt.left -= (rt.left%W);
        if(rt.bottom > H*7)
            rt.bottom = H*7;
        if(rt.right > W*24)
            rt. right = W*24;
        n0 = rt.top/H*24+rt.left/W;
        n0 = !State[n0];
        for(y = rt.top; y < rt.bottom; y += H)
        {
            for(x = rt.left; x < rt.right; x += W)
            {
                n = y/H*24+x/W;
                State[n] = n0;//!State[n];
            }
        }
    }
    DrawState();
    MouseSt = 0;
}

void __fastcall TDatePlan::OnMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TRect rt;

    if(MouseSt)
    {
        BuildRect(rt, X0, Y0, X1, Y1);
        PaintBox->Canvas->DrawFocusRect(rt);
        if(X < PLAN_HEAD_WIDTH)
            X = PLAN_HEAD_WIDTH;
        if(Y < PLAN_HEAD_HEIGHT)
            Y = PLAN_HEAD_HEIGHT;
        if(X > PaintBox->Width)
            X = PaintBox->Width-1;
        if(Y > PaintBox->Height)
            Y = PaintBox->Height-1;
        BuildRect(rt, X0, Y0, X, Y);
        PaintBox->Canvas->DrawFocusRect(rt);
        X1 = X;
        Y1 = Y;
    }
}

void TDatePlan::SetPlan(char *Plan)
{
    for(int i = 0; i < 168; i++)
    {
        State[i] = (Plan[i] == '1') ?  1:0;
    }
}

void TDatePlan::GetPlan(char *Plan)
{
    for(int i = 0; i < 168; i++)
    {
        Plan[i] = State[i] ? '1':'0';
    }
}

void TDatePlan::SetPlan(unsigned int *plan)
{
    int i, j;

    for(i = 0; i < 7; i++)
    for(j = 0; j < 24; j++)
    {
        State[i*24+j] = (plan[i] & (1<<j)) ?  1:0;
    }

    DrawState();
}

void TDatePlan::GetPlan(unsigned int *plan)
{
    memset((char *)plan, 0, 7*4);
    for(int i = 0; i < 168; i++)
    {
        if(State[i])
        {
            plan[i/24] |= 1 << (i%24);
        }
    }
}

void TDatePlan::Clear()
{
    for(int i = 0; i < 168; i++)
    {
        State[i] = 0;
    }
}
