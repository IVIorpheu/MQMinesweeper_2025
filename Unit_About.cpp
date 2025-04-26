//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_About *Form_About;
//---------------------------------------------------------------------------
__fastcall TForm_About::TForm_About(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_About::Label5Click(TObject *Sender)
{
    ShellExecuteW
        (
            NULL, /* Application->Handle, */
            L"open",
            L"https://github.com/Qadiany3D",
            NULL,
            NULL,
            SW_SHOWNORMAL
        );
}
//---------------------------------------------------------------------------

void __fastcall TForm_About::Panel3Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

