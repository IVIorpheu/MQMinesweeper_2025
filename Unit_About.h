//---------------------------------------------------------------------------

#ifndef Unit_AboutH
#define Unit_AboutH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TForm_About : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TImage *Image1;
    TPanel *Panel1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TPanel *Panel2;
    TLabel *Label5;
    TPanel *Panel3;
    TPanel *Panel4;
    TImage *Image2;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TLabel *Label6;
    TPanel *Panel9;
    void __fastcall Label5Click(TObject *Sender);
    void __fastcall Panel3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm_About(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_About *Form_About;
//---------------------------------------------------------------------------
#endif
