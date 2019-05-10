//---------------------------------------------------------------------------

#ifndef SwitchDevH
#define SwitchDevH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"    
#include "ioctrl.h"
//---------------------------------------------------------------------------
class TSwitchDevForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label60;
        TLabel *Label35;
        TCSpinEdit *ChnNum;
        TButton *Button1;
        TButton *Button2;
        TComboBox *Type;
        TLabel *Label1;
        TCSpinEdit *Id;
private:	// User declarations
public:		// User declarations
        __fastcall TSwitchDevForm(TComponent* Owner);
        int GetPara(SWITCH_DEV *para);
        int SetPara(SWITCH_DEV *para);
};
//---------------------------------------------------------------------------
extern PACKAGE TSwitchDevForm *SwitchDevForm;
//---------------------------------------------------------------------------
#endif
