//---------------------------------------------------------------------------

#ifndef IPsetH
#define IPsetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TIPSetForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label3;
        TLabel *Label8;
        TLabel *Label25;
        TEdit *IP;
        TEdit *IPMask;
        TEdit *Gataway;
        TButton *Button1;
        TButton *Button2;
        void __fastcall IPKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall TIPSetForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIPSetForm *IPSetForm;
//---------------------------------------------------------------------------
#endif
