//---------------------------------------------------------------------------

#ifndef srvH
#define srvH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSrvForm : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
private:	// User declarations
public:		// User declarations
        __fastcall TSrvForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSrvForm *SrvForm;
//---------------------------------------------------------------------------
#endif
