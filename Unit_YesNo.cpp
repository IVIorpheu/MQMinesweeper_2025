//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_YesNo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_YesNo *Form_YesNo;
//---------------------------------------------------------------------------
__fastcall TForm_YesNo::TForm_YesNo(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_YesNo::Panel_YesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    this->ModalResult = mrYes;
}
//---------------------------------------------------------------------------
void __fastcall TForm_YesNo::Panel_NoMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    this->ModalResult = mrNo;
}
//---------------------------------------------------------------------------
