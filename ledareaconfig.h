//---------------------------------------------------------------------------

#ifndef ledareaconfigH
#define ledareaconfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ledshowCtrl.h"
//---------------------------------------------------------------------------
class TareaConfig : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *xPos;
        TEdit *yPos;
        TEdit *areaW;
        TEdit *areaH;
        TLabel *Label6;
        TButton *Button1;
        TButton *Button2;
        TComboBox *CmbAreaNo;
        TComboBox *CmbTextRows;
private:	// User declarations
public:		// User declarations
        __fastcall TareaConfig(TComponent* Owner);
        int GetParam(STRAREAINFOS *parea);
        int SetParam(STRAREAINFOS *parea);
};
//---------------------------------------------------------------------------
extern PACKAGE TareaConfig *areaConfig;
//---------------------------------------------------------------------------
#endif
