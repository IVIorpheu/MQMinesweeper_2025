/*===========================================================*/


#include <vcl.h>
#pragma hdrstop

#include "Unit_Main.h"
#include "fmath3d.h"

#include "minswp_game.h"
#include "minswp_life.h"
#include "minswp_trimesh.h"
#include "minswp_polymesh.h"
#include "MinSwp_Displayer_GDI.hpp"
#include "MineSweeper3D.hpp"

#include "Unit_YesNo.h"
#include "Unit_About.h"

#include <vector>


/*===========================================================*/

#pragma package(smart_init)
#pragma resource "*.dfm"

TFormMain *FormMain;

/*===========================================================*/

__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
}

/*===========================================================*/

QM_VOID TFormMain::MinSwp_Display(QM_VOID)
{
    if (p_MineSweeper3D)
        p_MineSweeper3D->Display();
};

/*===========================================================*/

class TPanel_Viewport: public TPanel
{
private:
    TFormMain * this__p_Parent;
public:
    __fastcall TPanel_Viewport(
        System::Classes::TComponent* AOwner,
        TFormMain * p_Parent) /* minswp_override */ :
            TPanel(AOwner)
    {
        this__p_Parent = p_Parent;
    };

    void __fastcall Paint() minswp_override
    {
        this__p_Parent->MinSwp_Display();
    };
};

/*===========================================================*/

class T_MinSwp_AppWindow_VCL: public I_MinSwp_AppWindow
{
private:
    TFormMain *         this__p_FormMain;

public:
    T_MinSwp_AppWindow_VCL(TFormMain * p_FormMain = QM_NULL)
    {
        this__p_FormMain =
            ( p_FormMain) ?
              p_FormMain :
                FormMain;
    };
    /*=======================================================*/
    virtual QM_VOID Close(QM_VOID) minswp_override
    {
        this__p_FormMain->Close();
    };
    /*=======================================================*/
    virtual HWND    GetWindowHandle(QM_VOID) minswp_override
    {
        return this__p_FormMain->p_Panel_Viewport->Handle;
    };
    /*=======================================================*/
    virtual QM_LBOOL    OpenYesNoDialog
        (
            const QM_WCHAR  * p_Text,
            const QM_WCHAR  * p_Caption
        ) minswp_override
    {
        Form_YesNo->Label_Text->Caption = p_Text;
        Form_YesNo->Caption = UnicodeString(L"MQMinesweeper_") + p_Caption;
        if (Form_YesNo->ShowModal() == mrYes)
            return QM_TRUE;
        else
            return QM_FALSE;
    };
    /*=======================================================*/
    virtual QM_VOID     ShowAboutDialog(QM_VOID) minswp_override
    {
        Form_About->ShowModal();
    };
    /*=======================================================*/
    virtual QM_VOID     Play_Sound
        (
            const MINSWP_SOUND Sound
        ) minswp_override
    {
        TMediaPlayer * p_MediaPlayer = NULL;
        switch (Sound)
        {
            case MINSWP_SOUND_FIXBLOCK  : p_MediaPlayer = this__p_FormMain->MediaPlayer_FixBlock;       break;
            case MINSWP_SOUND_SETFLAG   : p_MediaPlayer = this__p_FormMain->MediaPlayer_SetFlag;        break;
            case MINSWP_SOUND_REMOVEFLAG: p_MediaPlayer = this__p_FormMain->MediaPlayer_RemoveFlag;     break;
            case MINSWP_SOUND_HELP      : p_MediaPlayer = this__p_FormMain->MediaPlayer_Help;           break;
            case MINSWP_SOUND_BOOMHEART : p_MediaPlayer = this__p_FormMain->MediaPlayer_BoomHeart;      break;
            case MINSWP_SOUND_FINISHBOOM: p_MediaPlayer = this__p_FormMain->MediaPlayer_FinishBoom;     break;
            case MINSWP_SOUND_FINISHWIN : p_MediaPlayer = this__p_FormMain->MediaPlayer_FinishWin;      break;

            default:
                ;
        };

        if (
                (p_MediaPlayer) &&
                (p_MediaPlayer->FileName != L"")
            )
        {
            p_MediaPlayer->Position = 0;
            p_MediaPlayer->Play();
        };
    };
};

/*===========================================================*/

#define MINSWP_SOUND_PATH_FIXBLOCK      L"MinSwp_Sound_FixBlock.wav"
#define MINSWP_SOUND_PATH_SETFLAG       L"MinSwp_Sound_SetFlag.wav"
#define MINSWP_SOUND_PATH_REMOVEFLAG    L"MinSwp_Sound_RemoveFlag.wav"
#define MINSWP_SOUND_PATH_HELP          L"MinSwp_Sound_Help.wav"
#define MINSWP_SOUND_PATH_BOOMHEART     L"MinSwp_Sound_BoomHeart.wav"
#define MINSWP_SOUND_PATH_FINISHBOOM    L"MinSwp_Sound_FinishBoom.wav"
#define MINSWP_SOUND_PATH_FINISHWIN     L"MinSwp_Sound_FinishWin.wav"

QM_LBOOL LoadSoundToMediaPlayer
    (
              TMediaPlayer        * p_MediaPlayer,
        const QM_WCHAR            * p_FileName
    )
{
    UnicodeString s = p_FileName;
    if (FileExists(s)) { p_MediaPlayer->FileName = s; goto goto_return_QM_TRUE; }
    else
    {
        s = L"Sound\\" + s;
        if (FileExists(s)) { p_MediaPlayer->FileName = s; goto goto_return_QM_TRUE; }
        else
        {
            s = L"..\\" + s;
            if (FileExists(s)) { p_MediaPlayer->FileName = s; goto goto_return_QM_TRUE; }
            else
            {
                s = L"..\\" + s;
                if (FileExists(s)) { p_MediaPlayer->FileName = s; goto goto_return_QM_TRUE; }
                else
                {
                    s = L"..\\" + s;
                    if (FileExists(s)) { p_MediaPlayer->FileName = s; goto goto_return_QM_TRUE; }
                        p_MediaPlayer->FileName = L"";
                };
            };
        };
    };
    return QM_FALSE;

    goto_return_QM_TRUE:
        try
        {
            p_MediaPlayer->Open();
        }
        catch (...)
        {
            p_MediaPlayer->FileName = L"";
        };

        return QM_TRUE;

};

void __fastcall TFormMain::FormCreate(TObject *Sender)
{
    this->p_MineSweeper3D                       = QM_NULL;
    this->p_MinSwp_Displayer                    = QM_NULL;

    this->p_Panel_Viewport                      = new TPanel_Viewport(this, this);
    this->p_Panel_Viewport->ParentBackground    = false;
    this->p_Panel_Viewport->Color               = clBlack;
    this->p_Panel_Viewport->Align               = alClient;
    this->p_Panel_Viewport->BevelOuter          = bvNone;
    this->p_Panel_Viewport->Parent              = this;
    this->p_Panel_Viewport->OnMouseUp           = this->OnMouseUp;
    this->p_Panel_Viewport->OnMouseMove         = this->OnMouseMove;
    this->p_Panel_Viewport->OnMouseDown         = this->OnMouseDown;

    p_MinSwp_AppWindow      = new T_MinSwp_AppWindow_VCL(this);
    p_MineSweeper3D         = new T_MineSweeper3D(       this->p_MinSwp_AppWindow);
    p_MinSwp_Displayer      = new T_MinSwp_Displayer_GDI(this->p_MinSwp_AppWindow);
    p_MineSweeper3D->SetDisplayer(                       this->p_MinSwp_Displayer);

    LoadSoundToMediaPlayer(this->MediaPlayer_FixBlock       , MINSWP_SOUND_PATH_FIXBLOCK    );
    LoadSoundToMediaPlayer(this->MediaPlayer_SetFlag        , MINSWP_SOUND_PATH_SETFLAG     );
    LoadSoundToMediaPlayer(this->MediaPlayer_RemoveFlag     , MINSWP_SOUND_PATH_REMOVEFLAG  );
    LoadSoundToMediaPlayer(this->MediaPlayer_Help           , MINSWP_SOUND_PATH_HELP        );
    LoadSoundToMediaPlayer(this->MediaPlayer_BoomHeart      , MINSWP_SOUND_PATH_BOOMHEART   );
    LoadSoundToMediaPlayer(this->MediaPlayer_FinishBoom     , MINSWP_SOUND_PATH_FINISHBOOM  );
    LoadSoundToMediaPlayer(this->MediaPlayer_FinishWin      , MINSWP_SOUND_PATH_FINISHWIN   );

}

/*===========================================================*/

QM_ULONG Generate_InButton
    (
        QM_VOID
    )
{
    return
        (
            ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? MINSWP_INBUTTON_MOUSE_L    : 0) |
            ((GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? MINSWP_INBUTTON_MOUSE_M    : 0) |
            ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? MINSWP_INBUTTON_MOUSE_R    : 0) |
            ((GetAsyncKeyState(VK_MENU   ) & 0x8000) ? MINSWP_INBUTTON_MENU       : 0) |
            ((GetAsyncKeyState(VK_SHIFT  ) & 0x8000) ? MINSWP_INBUTTON_SHIFT      : 0) |
            ((GetAsyncKeyState(VK_CONTROL) & 0x8000) ? MINSWP_INBUTTON_CONTROL    : 0)
        );
};

/*===========================================================*/

void __fastcall TFormMain::FormMouseDown
    (
        TObject *       Sender,
        TMouseButton    Button,
        TShiftState     Shift,
        int             X,
        int             Y
    )
{
    this->FocusControl(NULL);
    p_MineSweeper3D->OnMouse_Down
        (
            X,
            Y,
            Generate_InButton()
        );
}

/*===========================================================*/

void __fastcall TFormMain::FormMouseMove
    (
        TObject *       Sender,
        TShiftState     Shift,
        int             X,
        int             Y
    )
{
    p_MineSweeper3D->OnMouse_Move
        (
            X,
            Y,
            Generate_InButton()
        );
}

/*===========================================================*/

void __fastcall TFormMain::FormMouseUp
    (
        TObject *       Sender,
        TMouseButton    Button,
        TShiftState     Shift,
        int             X,
        int             Y
    )
{
    p_MineSweeper3D->OnMouse_Up
        (
            X,
            Y,
            Generate_InButton()
        );
}

/*===========================================================*/

void __fastcall TFormMain::FormKeyPress(TObject *Sender, System::WideChar &Key)
          
{
    switch (Key)
    {
        case L'm':
        case L'M':
            {
                p_MineSweeper3D->OpenMenuDirectory
                    (
                        T_MineSweeper3D::MENUDIR_HOME
                    );
            }; break;
        case L'o':
        case L'O':
            {
                p_MineSweeper3D->OpenMenuDirectory
                    (
                        T_MineSweeper3D::MENUDIR_OPTIONS
                    );
            }; break;
        case L's':
        case L'S':
            {
                p_MineSweeper3D->OpenMenuDirectory
                    (
                        T_MineSweeper3D::MENUDIR_SKIN
                    );
            }; break;
        case L'g':
        case L'G':
            {
                p_MineSweeper3D->OpenMenuDirectory
                    (
                        T_MineSweeper3D::MENUDIR_SELECT_GAME
                    );
            }; break;
        case L'n':
        case L'N':
            {
                p_MineSweeper3D->GetStartNewGame();
            }; break;
        default:
            ;
    }
    MinSwp_Display();
}

/*===========================================================*/

void __fastcall TFormMain::Timer1Timer(TObject *Sender)
{
    static QM_SLL64 t0 = 0;
    if (
            (p_MineSweeper3D) &&
            (p_MineSweeper3D->GetGame())
        )
    {
        const QM_SLL64 t1 =
            p_MineSweeper3D->GetGame()->Timer_GetTimePerSec_i64();
        if (t0 != t1)
        {
            t0 = t1;
            MinSwp_Display();
        };
    };
}

/*===========================================================*/

void __fastcall TFormMain::FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
    switch (Key)
    {
        case VK_ESCAPE:
            {
                if (p_MineSweeper3D)
                {
                    switch (p_MineSweeper3D->GetStageState())
                    {
                        default:
                        case MINSWP_STAGESTATE_GAME:
                            {
                                p_MineSweeper3D->SetStageState
                                    (
                                        MINSWP_STAGESTATE_MENU
                                    );
                            }; break;
                        case MINSWP_STAGESTATE_MENU:
                            {
                                p_MineSweeper3D->SetStageState
                                    (
                                        MINSWP_STAGESTATE_GAME
                                    );
                            }; break;
                    };
                    MinSwp_Display();
                };
            }; break;
        default:
            ;
    };
}

/*===========================================================*/





void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    if (this->p_MineSweeper3D->GetExitAndClose() == QM_FALSE)
        Action = caNone;
}
//---------------------------------------------------------------------------

