//---------------------------------------------------------------------------

#ifndef Unit_YesNoH
#define Unit_YesNoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_YesNo : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel_Yes;
    TPanel *Panel_No;
    TLabel *Label_Text;
    void __fastcall Panel_YesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Panel_NoMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TForm_YesNo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_YesNo *Form_YesNo;
//---------------------------------------------------------------------------
#endif
