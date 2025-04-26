//---------------------------------------------------------------------------

#ifndef Unit_MainH
#define Unit_MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------

#include "MineSweeper3D.hpp"
#include <Vcl.MPlayer.hpp>

class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TTimer *Timer1;
    TMediaPlayer *MediaPlayer_FixBlock;
    TMediaPlayer *MediaPlayer_SetFlag;
    TMediaPlayer *MediaPlayer_RemoveFlag;
    TMediaPlayer *MediaPlayer_Help;
    TMediaPlayer *MediaPlayer_FinishBoom;
    TMediaPlayer *MediaPlayer_FinishWin;
    TMediaPlayer *MediaPlayer_BoomHeart;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
          
          
          

private:	// User declarations
public:		// User declarations

    TPanel                  * p_Panel_Viewport;
    T_MineSweeper3D         * p_MineSweeper3D;
    I_MinSwp_Displayer      * p_MinSwp_Displayer;
    I_MinSwp_AppWindow      * p_MinSwp_AppWindow;

    QM_VOID MinSwp_Display
        (
            QM_VOID
        );
    __fastcall TFormMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
