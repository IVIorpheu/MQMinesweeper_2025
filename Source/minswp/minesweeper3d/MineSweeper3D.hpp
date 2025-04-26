/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : MineSweeper3D.hpp     (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINESWEEPER3D_HPP__DEFINED
#define __MINESWEEPER3D_HPP__DEFINED

/*===========================================================*/

#include "minswp_game.h"
#include "MinSwp_PolyMesh_FileInMemory.hpp"

/*
#include "minswp_displayer_gdi.hpp"
*/
#include <string>
#include <Windows.h>

/*===========================================================*/

#define MINSWP_INBUTTON_MOUSE_L     (1UL << 0)
#define MINSWP_INBUTTON_MOUSE_M     (1UL << 1)
#define MINSWP_INBUTTON_MOUSE_R     (1UL << 2)
#define MINSWP_INBUTTON_MOUSE_LMR   \
    (                               \
        MINSWP_INBUTTON_MOUSE_L |   \
        MINSWP_INBUTTON_MOUSE_M |   \
        MINSWP_INBUTTON_MOUSE_R |   \
    )

#define MINSWP_INBUTTON_CONTROL     (1UL << 3)
#define MINSWP_INBUTTON_SHIFT       (1UL << 4)
#define MINSWP_INBUTTON_MENU        (1UL << 5)

/*===========================================================*/

#undef minswp_override

#if defined(__clang__)
#   define minswp_override override
#else
#   define minswp_override
#endif /* defined(__clang__) */

/*===========================================================*/

class T_MinSwp_MenuItem;
class T_MineSweeper3D;

typedef enum _MINSWP_SOUND
{
    MINSWP_SOUND_FIXBLOCK,
    MINSWP_SOUND_SETFLAG,
    MINSWP_SOUND_REMOVEFLAG,
    MINSWP_SOUND_HELP,
    MINSWP_SOUND_BOOMHEART,
    MINSWP_SOUND_FINISHBOOM,
    MINSWP_SOUND_FINISHWIN

} MINSWP_SOUND;

struct I_MinSwp_AppWindow
{
    virtual QM_VOID     Close(QM_VOID)                  = 0;
    virtual HWND        GetWindowHandle(QM_VOID)        = 0;
    virtual QM_LBOOL    OpenYesNoDialog
        (
            const QM_WCHAR  * p_Text,
            const QM_WCHAR  * p_Caption
        ) = 0;
    virtual QM_VOID     ShowAboutDialog(QM_VOID)        = 0;

    virtual QM_VOID     Play_Sound(const MINSWP_SOUND Sound) = 0;
};

struct I_MinSwp_Displayer
{
    virtual QM_VOID Display
        (
                  T_MineSweeper3D     * p_MineSweeper3D
        ) = 0;
    virtual QM_ULONG    Skin_GetCount()             = 0;
    virtual QM_ULONG    Skin_GetCurrentIndex()      = 0;
    virtual QM_ULONG    Skin_SetCurrentIndex(const QM_ULONG SkinIndex)  = 0;
    virtual const
            QM_WCHAR *  Skin_GetName(        const QM_ULONG SkinIndex)  = 0;

};

/*===========================================================*/

typedef enum _MINSWP_MENUITEM_TYPE
{
    MINSWP_MENUITEM_T_FOLDER,
    MINSWP_MENUITEM_T_LABEL,
    MINSWP_MENUITEM_T_BUTTON,
    MINSWP_MENUITEM_T_SLIDER,
    MINSWP_MENUITEM_T_CHECK,
    MINSWP_MENUITEM_T_RADIOGROUP,
    MINSWP_MENUITEM_T_RADIOBUTTON
} MINSWP_MENUITEM_TYPE;

typedef enum _MINSWP_STAGE_STATE
{
    MINSWP_STAGESTATE_MENU,
    MINSWP_STAGESTATE_GAME
} MINSWP_STAGE_STATE;

/*===========================================================*/

class T_MinSwp_MenuItem
{
    static QM_LBOOL PointXY_IsIn_FRect
        (
            const QM_FLOAT      X,
            const QM_FLOAT      Y,
            const QM_FRECT    * p_fRect
        )
    {
        if (
                ( p_fRect->L <= X               ) &&
                (               X <= p_fRect->R ) &&
                ( p_fRect->T <= Y               ) &&
                (               Y <= p_fRect->B )
            )
            return QM_TRUE;
        else
            return QM_FALSE;
    };

    T_MinSwp_MenuItem(QM_VOID) {};
public:
    MINSWP_MENUITEM_TYPE    ItemType;
    std::wstring            Caption;

    /*=======================================================*/

    class T_Nodes
    {
    public:
        T_MinSwp_MenuItem         * p_BackButton;
        T_MinSwp_MenuItem         * p_Parent;
        T_MinSwp_MenuItem         * p_FirstChild;
        T_MinSwp_MenuItem         * p_Next;
    } Nodes[1];

    /*=======================================================*/

    class T_Slider
    {
        struct
        {
            QM_SLONG    Min,
                        Val,
                        Max;
        } Range[1];
    public:
        T_Slider()
        {
            this->Range->Min = 0;
            this->Range->Val = 50;
            this->Range->Max = 100;
        };
        QM_SLONG GetMin( QM_VOID ) { return this->Range->Min; };
        QM_SLONG GetVal( QM_VOID ) { return this->Range->Val; };
        QM_SLONG GetMax( QM_VOID ) { return this->Range->Max; };
        QM_SLONG SetMin
            (
                const QM_SLONG  X
            )
        {
            this->Range->Min =
                (   this->Range->Max < X) ?
                    this->Range->Max : X;
            if (    this->Range->Val < this->Range->Min)
                    this->Range->Val = this->Range->Min;
            return  this->Range->Min;
        };
        QM_SLONG SetVal
            (
                const QM_SLONG  X
            )
        {
            this->Range->Val =
                (X < this->Range->Min) ?
                     this->Range->Min :
            (
                (X > this->Range->Max) ?
                     this->Range->Max :
                 X
            );
            return this->Range->Val;
        };
        QM_SLONG SetMax
            (
                const QM_SLONG  X
            )
        {
            this->Range->Max =
                (   this->Range->Min > X) ?
                    this->Range->Min : X;
            if (    this->Range->Val > this->Range->Max)
                    this->Range->Val = this->Range->Max;
            return  this->Range->Max;
        };
        QM_FLOAT GetVal01(QM_VOID)
        {
            return
                ((QM_FLOAT)(this->Range->Val - this->Range->Min)) /
                ((QM_FLOAT)(this->Range->Max - this->Range->Min));
        };
        QM_FLOAT SetVal01
            (
                const QM_FLOAT  V01
            )
        {
            const QM_SLONG v = this->Range->Min +
                (QM_SLONG)
                    (
                        ((QM_FLOAT)(this->Range->Max - this->Range->Min)) * V01 + 0.5f
                    );
            this->SetVal(v);
            return this->GetVal01();
        };
    } Slider[1];

    struct
    {
        QM_ULONG        ItemIndex;
    } Radio[1];

    struct
    {
        QM_LBOOL        Enabled;
    } CheckBox[1];

    /*=======================================================*/

    T_MinSwp_MenuItem(T_MinSwp_MenuItem * p_BackButton)
    {
        this->Nodes->p_BackButton       = p_BackButton;
        this->Nodes->p_Parent           = QM_NULL;
        this->Nodes->p_FirstChild       = QM_NULL;
        this->Nodes->p_Next             = QM_NULL;
        this->ItemType                  = MINSWP_MENUITEM_T_FOLDER;
        this->Radio->ItemIndex          = 0;
        this->CheckBox->Enabled         = QM_FALSE;
        this->Caption                   = L"";
    };

    /*=======================================================*/

    ~T_MinSwp_MenuItem()
    {
        if (this->Nodes->p_Parent)
        {
            T_MinSwp_MenuItem * p_child = this->Nodes->p_Parent->Nodes->p_FirstChild;
            if (p_child == this)
            {
                this->Nodes->p_Parent->Nodes->p_FirstChild = this->Nodes->p_Next;
            }
            else
            {
                while (
                        (p_child) &&
                        (p_child->Nodes->p_Next != this)
                    )
                {
                    p_child = p_child->Nodes->p_Next;
                };
                if (p_child)
                    p_child->Nodes->p_Next = this->Nodes->p_Next;
            };
            this->Nodes->p_Next     = QM_NULL;
            this->Nodes->p_Parent   = QM_NULL;
        };

        this->Childs_ReleaseAll();
    };

    /*=======================================================*/

    QM_ULONG Childs_GetCount(QM_VOID)
    {
        QM_ULONG c = 0;
        T_MinSwp_MenuItem * p_child = this->Nodes->p_FirstChild;
        while (p_child)
        {
            c++;
            p_child = p_child->Nodes->p_Next;
        };

        return c + ((this->Nodes->p_BackButton) ? 1 : 0);
    };

    virtual
    QM_LBOOL Childs_GetRect
        (
            const QM_ULONG      Index,
                  QM_FRECT    * p_Out_Rect
        )
    {
        QM_ULONG c = 1; __qm_comment__('First Item Is BackButton')
        T_MinSwp_MenuItem * p_child = this->Nodes->p_FirstChild;
        while (p_child)
        {
            c++;
            p_child = p_child->Nodes->p_Next;
        };

        if (Index < c)
        {
            const QM_FLOAT t = 0.2f;
            const QM_FLOAT h = 1.0f - 2.0f * t;
            const QM_ULONG n = 10;

            const QM_FLOAT item_h = h / (QM_FLOAT)n;
            const QM_FLOAT all_items_h = c * item_h;

            const QM_FLOAT first_item_t = t /* + (h - all_items_h) * 0.5f */ ;
            p_Out_Rect->L = 1.0f / 4.0f;
            p_Out_Rect->R = 3.0f / 4.0f;
            p_Out_Rect->T = first_item_t + ((QM_FLOAT)Index) * item_h;
            p_Out_Rect->B = p_Out_Rect->T + item_h;

            return QM_TRUE;
        }
        else
            return QM_FALSE;
    };

    QM_LBOOL Childs_PushBack
        (
            T_MinSwp_MenuItem       * p_Item
        )
    {
        if (p_Item->Nodes->p_Parent)
            return QM_FALSE;
        else
        {
            T_MinSwp_MenuItem * p_child = this->Nodes->p_FirstChild;
            if (p_child)
            {
                while (p_child->Nodes->p_Next)
                {
                    p_child = p_child->Nodes->p_Next;
                };
                p_child->Nodes->p_Next = p_Item;
            }
            else
                this->Nodes->p_FirstChild = p_Item;

            p_Item->Nodes->p_Parent = this;
            return QM_TRUE;
        };
    };

    T_MinSwp_MenuItem * Childs_GetByIndex
        (
            const QM_ULONG  Index
        )
    {
        if (Index)
        {
            T_MinSwp_MenuItem * p_child = this->Nodes->p_FirstChild;
            if (p_child)
            {
                for (QM_ULONG i = 1; i < Index; i++)
                {
                    if (p_child)
                        p_child = p_child->Nodes->p_Next;
                    else
                        return QM_NULL;
                };
            };
            return p_child;
        }
        else
            return this->Nodes->p_BackButton;
    };

    QM_SLONG Childs_FindIndex
        (
            const T_MinSwp_MenuItem * p_Child
        )
    {
        if (this->Nodes->p_BackButton == p_Child)
            return 0;
        else
        {
            QM_SLONG c;
            T_MinSwp_MenuItem * p_node = this->Nodes->p_FirstChild;
            c = 1;
            while (p_node)
            {
                if (p_node == p_Child)
                    return c;
                else
                {
                    c++;
                    p_node = p_node->Nodes->p_Next;
                };
            };
            return -1;
        };
    };

    T_MinSwp_MenuItem * Childs_FindByXY
        (
            const QM_FLOAT      X,
            const QM_FLOAT      Y,
            const QM_FLOAT      W,
            const QM_FLOAT      H
        )
    {
        const QM_FLOAT x01 = X / W;
        const QM_FLOAT y01 = Y / H;
        const QM_ULONG c = this->Childs_GetCount();
        for (QM_ULONG i = 0; i < c; i++)
        {
            QM_FRECT fr;
            this->Childs_GetRect(i, &fr);

            if (
                    PointXY_IsIn_FRect
                        (
                            x01,
                            y01,
                            &fr
                        )
                )
                return this->Childs_GetByIndex(i);
        };
        return QM_NULL;
    };

    QM_VOID Childs_ReleaseAll
        (
            QM_VOID
        )
    {
        while (this->Nodes->p_FirstChild)
        {
            delete this->Nodes->p_FirstChild;
        };
    };
};

/*===========================================================*/

#define MINSWP_YESNODLG_STARTNEWGAME_TEXT       L"Are you sure you want to start a new game?"
#define MINSWP_YESNODLG_STARTNEWGAME_CAPT       L"Start_New_Game"

#define MINSWP_YESNODLG_EXITGAME_TEXT           L"Are you sure you want to exit the game?"
#define MINSWP_YESNODLG_EXITGAME_CAPT           L"Exit"


class T_MineSweeper3D
{
    MINSWP_ADJACENT_TYPE        this__Adjacent_Type;
    struct
    {
        T_MinSwp_Game *             p_Game;
        MINSWP_GAME_TYPE            GameType;
        QM_ULONG                    ClassicWSegs;
        QM_ULONG                    ClassicHSegs;
        QM_ULONG                    NumMines;
    } this__CurrentGame[1];

    HMINSWP_LIFE                this__h_Life;
    QM_SLONG                    this__Selected_Poly;
    QM_LBOOL                    this__Mouse_L_Down;
    QM_LBOOL                    this__Mouse_R_Down;
    MINSWP_STAGE_STATE          this__StageState;
    T_MinSwp_MenuItem         * this__p_CurrentMenuItem;
    I_MinSwp_AppWindow        * this__p_AppWindow;
    I_MinSwp_Displayer        * this__p_Displayer;
    HMINSWP_POLYMESH            this__h_CurrentPolyMesh;
    QM_SLONG                    this__Prev_Menu_p_SelectGame_Radio_ItemIndex;
    QM_LBOOL                    this__ClosedGame;
    QM_LBOOL                    this__Sound_Disable;

    HMINSWP_POLYMESH    this__CreateAndLoadPolyMeshFile
        (
            const MINSWP_GAME_TYPE      GameType
        )
    {
        const QM_VOID * p_memory =
            T_MinSwp_PolyMesh_FileInMemory::GetFile
                (
                    GameType
                );
        if (p_memory)
        {
            HMINSWP_POLYMESH h_pm =
                MinSwp_PolyMesh_Create_FromFileInMemory
                    (
                        p_memory
                    );
            if (h_pm)
                return h_pm;
            else
                MinSwp_ShowMessage_W_Force(L"Failed load from PolyMesh file!");
        };

        return QM_NULL;
    };

    QM_VOID this__PlaySound
        (
            const MINSWP_SOUND Sound
        )
    {
        if (!this__Sound_Disable)
        {
            this__p_AppWindow->Play_Sound(Sound);
        };
    };
public:
    T_MineSweeper3D
        (
            I_MinSwp_AppWindow    * p_AppWindow
        )
    {
        this__p_AppWindow               = p_AppWindow;
        this__Adjacent_Type             = MINSWP_ADJT_BYVERTS;
        this__h_Life                    = MinSwp_Life_Create(3);
        this__Selected_Poly             = -1;
        this__Mouse_L_Down              = QM_FALSE;
        this__Mouse_R_Down              = QM_FALSE;
        this__StageState                = MINSWP_STAGESTATE_MENU;
        this__p_CurrentMenuItem         = QM_NULL;
        this__p_Displayer               = QM_NULL;
        this__Prev_Menu_p_SelectGame_Radio_ItemIndex
                                        = 0;
        this__h_CurrentPolyMesh         = QM_NULL;
        this__ClosedGame                = QM_FALSE;
        this__Sound_Disable             = QM_FALSE;

        {
            const QM_ULONG c = sizeof(this->Menu[0]) / sizeof(this->Menu->Items__i__[0]);
            QM_ULONG i;

            this->Menu->p_BackButton = new T_MinSwp_MenuItem(QM_NULL);
            for (i = 1; i < c; i++)
            {
                this->Menu->Items__i__[i] = new T_MinSwp_MenuItem(this->Menu->p_BackButton);
            };

            this->Menu->p_Home->Childs_PushBack(        this->Menu->p_StartNewGame);
            this->Menu->p_Home->Childs_PushBack(        this->Menu->p_Options);
            this->Menu->p_Options->Childs_PushBack(         this->Menu->p_SelectGame);
            this->Menu->p_SelectGame->Childs_PushBack(          this->Menu->p_SelectGame_Classic);
            this->Menu->p_SelectGame->Childs_PushBack(          this->Menu->p_SelectGame_ClassicWave);
            this->Menu->p_SelectGame->Childs_PushBack(          this->Menu->p_SelectGame_Voronoi);
            this->Menu->p_SelectGame->Childs_PushBack(          this->Menu->p_SelectGame_Infinity);
            this->Menu->p_SelectGame->Childs_PushBack(          this->Menu->p_SelectGame_Delphi);
            this->Menu->p_SelectGame->Childs_PushBack(          this->Menu->p_SelectGame_CppBuilder);
            this->Menu->p_SelectGame->Childs_PushBack(          this->Menu->p_SelectGame_Custom);
            this->Menu->p_Options->Childs_PushBack(         this->Menu->p_Skin);
            this->Menu->p_Options->Childs_PushBack(         this->Menu->p_Sound);
            this->Menu->p_Options->Childs_PushBack(         this->Menu->p_Classic_Width);
            this->Menu->p_Options->Childs_PushBack(         this->Menu->p_Classic_Height);
            this->Menu->p_Options->Childs_PushBack(         this->Menu->p_NumMines);
            this->Menu->p_Home->Childs_PushBack(        this->Menu->p_About);
            this->Menu->p_Home->Childs_PushBack(        this->Menu->p_Exit);

            this->Menu->p_BackButton->Caption               =   L"Back";
            this->Menu->p_Home->Caption                     =   L"Home";
            this->Menu->p_StartNewGame->Caption             =       L"Start New Game";
            this->Menu->p_Options->Caption                  =       L"Options";
            this->Menu->p_SelectGame->Caption               =           L"Geometry" /* L"Select Game" */ ;
            this->Menu->p_SelectGame_Classic->Caption       =               L"Classic";
            this->Menu->p_SelectGame_ClassicWave->Caption   =               L"ClassicWave";
            this->Menu->p_SelectGame_Voronoi->Caption       =               L"Voronoi";
            this->Menu->p_SelectGame_Infinity->Caption      =               L"Infinity";
            this->Menu->p_SelectGame_Delphi->Caption        =                               L"RAS Studio "
                                                                            L"  Delphi  "   L"           ";
            this->Menu->p_SelectGame_CppBuilder->Caption    =                               L"RAS Studio "
                                                                            L"C++Builder"   L"           ";
            this->Menu->p_SelectGame_Custom->Caption        =               L"Custom ...";
            this->Menu->p_Skin->Caption                     =           L"Skin";
            this->Menu->p_Sound->Caption                    =           L"Sound";
            this->Menu->p_Classic_Width->Caption            =           L"Classic.Width";
            this->Menu->p_Classic_Height->Caption           =           L"Classic.Height";
            this->Menu->p_NumMines->Caption                 =           L"Num Mines";
            this->Menu->p_About->Caption                    =       L"About";
            this->Menu->p_Exit->Caption                     =       L"Exit";

            this->Menu->p_BackButton->ItemType              = MINSWP_MENUITEM_T_BUTTON;
            this->Menu->p_Home->ItemType                    = MINSWP_MENUITEM_T_FOLDER;
            this->Menu->p_StartNewGame->ItemType            = MINSWP_MENUITEM_T_BUTTON;
            this->Menu->p_Options->ItemType                 = MINSWP_MENUITEM_T_FOLDER;
            this->Menu->p_SelectGame->ItemType              = MINSWP_MENUITEM_T_RADIOGROUP;
            this->Menu->p_SelectGame_Classic->ItemType      = MINSWP_MENUITEM_T_RADIOBUTTON;
            this->Menu->p_SelectGame_ClassicWave->ItemType  = MINSWP_MENUITEM_T_RADIOBUTTON;
            this->Menu->p_SelectGame_Voronoi->ItemType      = MINSWP_MENUITEM_T_RADIOBUTTON;
            this->Menu->p_SelectGame_Infinity->ItemType     = MINSWP_MENUITEM_T_RADIOBUTTON;
            this->Menu->p_SelectGame_Delphi->ItemType       = MINSWP_MENUITEM_T_RADIOBUTTON;
            this->Menu->p_SelectGame_CppBuilder->ItemType   = MINSWP_MENUITEM_T_RADIOBUTTON;
            this->Menu->p_SelectGame_Custom->ItemType       = MINSWP_MENUITEM_T_RADIOBUTTON;
            this->Menu->p_Skin->ItemType                    = MINSWP_MENUITEM_T_RADIOGROUP;
            this->Menu->p_Sound->ItemType                   = MINSWP_MENUITEM_T_CHECK;
            this->Menu->p_Classic_Width->ItemType           = MINSWP_MENUITEM_T_SLIDER;
            this->Menu->p_Classic_Height->ItemType          = MINSWP_MENUITEM_T_SLIDER;
            this->Menu->p_NumMines->ItemType                = MINSWP_MENUITEM_T_SLIDER;
            this->Menu->p_About->ItemType                   = MINSWP_MENUITEM_T_BUTTON;
            this->Menu->p_Exit->ItemType                    = MINSWP_MENUITEM_T_BUTTON;

            this->Menu->p_SelectGame->Radio->ItemIndex      = 1;
            this->Menu->p_Skin->Radio->ItemIndex            = 1;
            this->Menu->p_Sound->CheckBox->Enabled          = QM_TRUE;

            this->Menu->p_Classic_Width->Slider->SetMin(8);
            this->Menu->p_Classic_Width->Slider->SetMax(30);
            this->Menu->p_Classic_Width->Slider->SetVal(25);
            this->Menu->p_Classic_Height->Slider->SetMin(5);
            this->Menu->p_Classic_Height->Slider->SetMax(25);
            this->Menu->p_Classic_Height->Slider->SetVal(15);
            this->Menu->p_NumMines->Slider->SetMin(5);
            this->Menu->p_NumMines->Slider->SetMax(300);
            this->Menu->p_NumMines->Slider->SetVal(50);
        };

        {
            this__CurrentGame->p_Game       = QM_NULL;
            this__CurrentGame->GameType     = MINSWP_GAMETYPE_CLASSIC;
            this__CurrentGame->NumMines     = this->Menu->p_NumMines->Slider->GetVal();
            this__CurrentGame->ClassicWSegs = this->Menu->p_Classic_Width->Slider->GetVal();
            this__CurrentGame->ClassicHSegs = this->Menu->p_Classic_Height->Slider->GetVal();
            this->ResetGame();
        };

        this->SetStageState(MINSWP_STAGESTATE_GAME);
    };

    /*=======================================================*/
private:
    QM_VOID               this__SetSelectedPolyIndex(   QM_SLONG I) {        this__Selected_Poly = I;      };
    QM_VOID               this__SetMouse_L_Down(        QM_LBOOL B) {        this__Mouse_L_Down = B;       };
    QM_VOID               this__SetMouse_R_Down(        QM_LBOOL B) {        this__Mouse_R_Down = B;       };
public:
    MINSWP_ADJACENT_TYPE    GetAdjacentType(            QM_VOID) { return this__Adjacent_Type;          };
    T_MinSwp_Game *         GetGame(                    QM_VOID) { return this__CurrentGame->p_Game;    };
    HMINSWP_LIFE            GetLife(                    QM_VOID) { return this__h_Life;                 };
    QM_SLONG                GetSelectedPolyIndex(       QM_VOID) { return this__Selected_Poly;          };
    QM_LBOOL                GetMouse_L_Down(            QM_VOID) { return this__Mouse_L_Down;           };
    QM_LBOOL                GetMouse_R_Down(            QM_VOID) { return this__Mouse_R_Down;           };
    QM_FLOAT                GetPanelTopHeightPercent(   QM_VOID) { return 0.06f; };
    /*=======================================================*/
public:
    QM_LBOOL GetStartNewGame
        (
            QM_VOID
        )
    {
        if (
                this__p_AppWindow->OpenYesNoDialog
                    (
                        MINSWP_YESNODLG_STARTNEWGAME_TEXT,
                        MINSWP_YESNODLG_STARTNEWGAME_CAPT
                    )
            )
        {
            this->ResetGame();
            return QM_TRUE;
        }
        else
            return QM_FALSE;
    };

    QM_LBOOL GetExitAndClose
        (
            QM_VOID
        )
    {
        if (this__ClosedGame)
            return QM_TRUE;
        else
        {
            if (
                    this__p_AppWindow->OpenYesNoDialog
                        (
                            MINSWP_YESNODLG_EXITGAME_TEXT,
                            MINSWP_YESNODLG_EXITGAME_CAPT
                        )
                )
            {
                this__ClosedGame = QM_TRUE;
                this__p_AppWindow->Close();
                return QM_TRUE;
            }
            else
                return QM_FALSE;
        };
    };

    QM_LBOOL ResetGame
        (
            QM_VOID
        )
    {
        const QM_SLONG num_mines        = this->Menu->p_NumMines->Slider->GetVal();
        const QM_SLONG classic_wsegs    = this->Menu->p_Classic_Width->Slider->GetVal();
        const QM_SLONG classic_hsegs    = this->Menu->p_Classic_Height->Slider->GetVal();

        const
        MINSWP_GAME_TYPE    game_type_prev = this__CurrentGame->GameType;
        MINSWP_GAME_TYPE    game_type;

        switch (this->Menu->p_SelectGame->Radio->ItemIndex)
        {
            default:
            case 1: game_type = MINSWP_GAMETYPE_CLASSIC     ; break;
            case 2: game_type = MINSWP_GAMETYPE_CLASSICWAVE ; break;
            case 3: game_type = MINSWP_GAMETYPE_VORONOI     ; break;
            case 4: game_type = MINSWP_GAMETYPE_INFINITY    ; break;
            case 5: game_type = MINSWP_GAMETYPE_DELPHI      ; break;
            case 6: game_type = MINSWP_GAMETYPE_CPPBUILDER  ; break;
            case 7: game_type = MINSWP_GAMETYPE_CUSTOM      ; break;
            /*
            case 1000: game_type = MINSWP_GAMETYPE_CUBE3D      ; break;
            */
        };

        if (
                (          this__CurrentGame->p_Game) ||
                ((QM_SLONG)this__CurrentGame->NumMines    != num_mines) ||
                (          this__CurrentGame->GameType    != game_type) ||
                (
                    (game_type == MINSWP_GAMETYPE_CLASSIC) &&
                    (
                        ((QM_SLONG)this__CurrentGame->ClassicWSegs    != classic_wsegs) ||
                        ((QM_SLONG)this__CurrentGame->ClassicHSegs    != classic_hsegs)
                    )
                )
            )
        {
            delete
            this->this__CurrentGame->p_Game;
            this->this__CurrentGame->p_Game =
            QM_NULL;
        };

        this__CurrentGame->NumMines     = num_mines;
        this__CurrentGame->GameType     = game_type;
        this__CurrentGame->ClassicWSegs = classic_wsegs;
        this__CurrentGame->ClassicHSegs = classic_hsegs;

        /*===================================================*/

        HMINSWP_POLYMESH    h_polymesh =
            this__h_CurrentPolyMesh;

        if (
                (game_type == MINSWP_GAMETYPE_CLASSIC) ||
                (game_type != game_type_prev)
            )
        {
            h_polymesh = QM_NULL;
        };

        if (h_polymesh == QM_NULL)
        {
            h_polymesh =
                (game_type == MINSWP_GAMETYPE_CLASSIC) ?
                    MinSwp_PolyMesh_Create_Plane
                        (
                            classic_wsegs,
                            classic_hsegs
                        ) :
                    this__CreateAndLoadPolyMeshFile
                        (
                            game_type
                        );
        };

        if (
                (h_polymesh == QM_NULL) &&
                (game_type  == MINSWP_GAMETYPE_CUSTOM)
            )
        {
            this__CurrentGame->GameType =
            game_type                   =
            game_type_prev;
            this->Menu->p_SelectGame->Radio->ItemIndex =
                this__Prev_Menu_p_SelectGame_Radio_ItemIndex;

            h_polymesh = this__h_CurrentPolyMesh;
        };

        if (
                (h_polymesh) &&
                (h_polymesh != this__h_CurrentPolyMesh)
            )
        {
            if (this__h_CurrentPolyMesh)
            {
                MinSwp_PolyMesh_Release(
                    this__h_CurrentPolyMesh);
                    this__h_CurrentPolyMesh =
                QM_NULL;
            };
            this__h_CurrentPolyMesh = h_polymesh;
        };

        this__SetSelectedPolyIndex(-1);
        this__SetMouse_L_Down(QM_FALSE);
        this__SetMouse_R_Down(QM_FALSE);
        if (h_polymesh)
        {
            this__CurrentGame->p_Game =
                T_MinSwp_Game::CreateNew
                    (
                        h_polymesh
                    );
            this__CurrentGame->p_Game->StartNewGame
                (
                    num_mines
                );
            MinSwp_Life_Reset
                (
                    this->GetLife()
                );

            return QM_TRUE;
        }
        else
            return QM_FALSE;
    };

    MINSWP_STAGE_STATE GetStageState
        (
            QM_VOID
        )
    {
        return this__StageState;
    };
    MINSWP_STAGE_STATE SetStageState
        (
            const MINSWP_STAGE_STATE StageState
        )
    {
        this__StageState = StageState;

        if (this__StageState == MINSWP_STAGESTATE_GAME)
            this->GetGame()->Timer_Play();
        else
            this->GetGame()->Timer_Pause();

        this__p_CurrentMenuItem =
            (this__StageState == MINSWP_STAGESTATE_MENU) ?
                this->Menu->p_Home :
                QM_NULL;

        this->Display();
        return this__StageState;
    };
    typedef enum _MENU_DIRECTORY
    {
        MENUDIR_HOME,
        MENUDIR_OPTIONS,
        MENUDIR_SKIN,
        MENUDIR_SELECT_GAME
    } MENU_DIRECTORY;
    QM_LBOOL OpenMenuDirectory
        (
            const MENU_DIRECTORY MenuDir
        )
    {
        if (
                this->SetStageState
                    (   MINSWP_STAGESTATE_MENU) ==
                        MINSWP_STAGESTATE_MENU
            )
        {
            switch (MenuDir)
            {
                case MENUDIR_HOME       : this__p_CurrentMenuItem = this->Menu->p_Home      ; break;
                case MENUDIR_OPTIONS    : this__p_CurrentMenuItem = this->Menu->p_Options   ; break;
                case MENUDIR_SKIN       : this__p_CurrentMenuItem = this->Menu->p_Skin      ; break;
                case MENUDIR_SELECT_GAME: this__p_CurrentMenuItem = this->Menu->p_SelectGame; break;
                default:
                    return QM_FALSE;
            };
        };
        this->Display();
        return QM_TRUE;
    };
    /*=======================================================*/
    RECT * GetWindowRect
        (
            RECT     * p_Out_Rect
        )
    {
        GetClientRect
            (
                this__p_AppWindow->GetWindowHandle(),
                p_Out_Rect
            );
        return p_Out_Rect;
    };
    /*=======================================================*/
    QM_ULONG ClassicGame_GetWidth(QM_VOID)
    {
        return (QM_ULONG)this->Menu->p_Classic_Width->Slider->GetVal();
    };
    QM_ULONG ClassicGame_GetHeight(QM_VOID)
    {
        return (QM_ULONG)this->Menu->p_Classic_Height->Slider->GetVal();
    };
    QM_ULONG GetNumMines(QM_VOID)
    {
        return (QM_ULONG)this->Menu->p_NumMines->Slider->GetVal();
    };
    QM_VOID __SetNumMines(const QM_ULONG C)
    {
        this->Menu->p_NumMines->Slider->SetVal(C);
    };
    /*=======================================================*/
    T_MinSwp_MenuItem * GetCurrentMenuItem(QM_VOID)
    {
        return this__p_CurrentMenuItem;
    };
    /*=======================================================*/

    struct
    {
        union
        {
            T_MinSwp_MenuItem     * Items__i__[1];
            T_MinSwp_MenuItem     * p_BackButton;
        };
            T_MinSwp_MenuItem     * p_Home;
            T_MinSwp_MenuItem     *     p_StartNewGame;
            T_MinSwp_MenuItem     *     p_Options;
            T_MinSwp_MenuItem     *         p_SelectGame;
            T_MinSwp_MenuItem     *             p_SelectGame_Classic;
            T_MinSwp_MenuItem     *             p_SelectGame_ClassicWave;
            T_MinSwp_MenuItem     *             p_SelectGame_Voronoi;
            T_MinSwp_MenuItem     *             p_SelectGame_Infinity;
            T_MinSwp_MenuItem     *             p_SelectGame_Delphi;
            T_MinSwp_MenuItem     *             p_SelectGame_CppBuilder;
            T_MinSwp_MenuItem     *             p_SelectGame_Custom;
            T_MinSwp_MenuItem     *         p_Skin; /* Radio */
            T_MinSwp_MenuItem     *         p_Sound; /* Check */
            T_MinSwp_MenuItem     *         p_Classic_Width;
            T_MinSwp_MenuItem     *         p_Classic_Height;
            T_MinSwp_MenuItem     *         p_NumMines;
            T_MinSwp_MenuItem     *     p_About;
            T_MinSwp_MenuItem     *     p_Exit;
    } Menu[1];

    QM_VOID Display(QM_VOID)
    {
        if (this__p_Displayer)
        {
            this__p_Displayer->Display(this);
        };
    };

    I_MinSwp_Displayer * GetDisplayer(QM_VOID)
    {
        return this__p_Displayer;
    };
    QM_VOID SetDisplayer(I_MinSwp_Displayer * p_Displayer)
    {
        if (this__p_Displayer != p_Displayer)
        {
            this__p_Displayer = p_Displayer;
            this->Menu->p_Skin->Childs_ReleaseAll();
            if (p_Displayer)
            {
                const QM_ULONG c = p_Displayer->Skin_GetCount();
                QM_ULONG i;
                for (i = 0; i < c; i++)
                {
                    T_MinSwp_MenuItem * p_item = new T_MinSwp_MenuItem(this->Menu->p_BackButton);
                    p_item->Caption = p_Displayer->Skin_GetName(i);
                    p_item->ItemType = MINSWP_MENUITEM_T_RADIOBUTTON;
                    this->Menu->p_Skin->Childs_PushBack(p_item);
                };
                this->Menu->p_Skin->Radio->ItemIndex = 1;
            };
        };
        this->Display();
    };

    /*=======================================================*/
private:
    QM_SLONG this__GetSelPolyIndex
        (
            const QM_SLONG      Mouse_X,
            const QM_SLONG      Mouse_Y
        )
    {
        RECT         client_rect;
        MINSWP_RAY3D ray;
        GetClientRect
            (
                this__p_AppWindow->GetWindowHandle(),
                &client_rect
            );

        const QM_SLONG W = client_rect.right  - client_rect.left;
        const QM_SLONG H = client_rect.bottom - client_rect.top;
        const QM_SLONG panel_top_H = (QM_SLONG)((QM_FLOAT)H * GetPanelTopHeightPercent());
        MinSwp_Vec3D_SetVal
            (
                &ray.Pos,
                (QM_FLOAT)Mouse_X / (QM_FLOAT)W,
                (
                     (QM_FLOAT)(Mouse_Y - panel_top_H) /
                    ((QM_FLOAT)H        - panel_top_H)
                ),
                1.0f
            );
        MinSwp_Vec3D_SetVal
            (
                &ray.Dir,
                0.0f,
                0.0f,
                -1.0f
            );
        return this->GetGame()->GetIntersectedPolyByRay(&ray);
    };

    QM_VOID this__OnMouse_Down__Game
        (
            const QM_SLONG      Mouse_X,
            const QM_SLONG      Mouse_Y,
            const QM_ULONG      InButton
        )
    {
        if (this->GetGame() == QM_NULL)
        {
            MinSwp_ShowMessage_W(L"p_Game == QM_NULL");
            return ;
        };

        QM_SLONG poly_index =
            this__GetSelPolyIndex
                (
                    Mouse_X,
                    Mouse_Y
                );

        RECT win_rect;
        this->GetWindowRect(&win_rect);
        const QM_FLOAT h = (QM_FLOAT)(win_rect.bottom - win_rect.top );
        const QM_FLOAT w = (QM_FLOAT)(win_rect.right  - win_rect.left);
        const QM_FLOAT panel_top_H = h * this->GetPanelTopHeightPercent();

        if (
                (Mouse_X < (QM_SLONG)panel_top_H)  &&
                (Mouse_Y < (QM_SLONG)panel_top_H)
            )
        {
            this->SetStageState(MINSWP_STAGESTATE_MENU);
            this__PlaySound(MINSWP_SOUND_HELP);
            this->Display();
            return ;
        };

        if ((this->GetGame()->GetGameState() & MINSWP_GS_ENDED) == MINSWP_GS_ENDED)
        {
            if (InButton & MINSWP_INBUTTON_MOUSE_L)
            {
                this__PlaySound(MINSWP_SOUND_HELP);
                this->GetStartNewGame();
                this->Display();
                return ;
            };
        };

        if (InButton & MINSWP_INBUTTON_MOUSE_L)
        {
            MINSWP_CHECKBLOCK_RESULT chkblk_res =
                this->GetGame()->CheckBlock
                    (
                        poly_index,
                        this->GetAdjacentType(),
                        this->GetLife()
                    );
            switch (chkblk_res)
            {
                case MINSWP_CHKBLKRES_OK_RELEASEBLOCKS:
                    {
                        this__PlaySound(MINSWP_SOUND_FIXBLOCK);
                    }; break;
                case MINSWP_CHKBLKRES_OK_BOOMSETHEART:
                    {
                        this__PlaySound(MINSWP_SOUND_BOOMHEART);
                    }; break;
                case MINSWP_CHKBLKRES_OK_FINISHLOSS:
                    {
                        this__PlaySound(MINSWP_SOUND_FINISHBOOM);
                    }; break;
                case MINSWP_CHKBLKRES_OK_FINISHWIN:
                    {
                        this__PlaySound(MINSWP_SOUND_FINISHWIN);
                    }; break;
                default:
                    ;
            }
        }
        else if (InButton & MINSWP_INBUTTON_MOUSE_R)
        {
            switch (this->GetGame()->SetFlag(poly_index))
            {
                case MINSWP_SETFLAGRES_OK_REMOVE    : this__PlaySound(MINSWP_SOUND_REMOVEFLAG); break;
                case MINSWP_SETFLAGRES_OK_INSTALL   : this__PlaySound(MINSWP_SOUND_SETFLAG);    break;
                default:
                    ;
            };
        };

        if ((InButton & MINSWP_INBUTTON_MOUSE_M) == 0)/* ((GetAsyncKeyState(VK_MBUTTON) & 0x8000) == 0) */
            poly_index = -1;
        else
            this__PlaySound(MINSWP_SOUND_HELP);

        this__SetSelectedPolyIndex(poly_index);
        this__SetMouse_L_Down( InButton & MINSWP_INBUTTON_MOUSE_L );
        this__SetMouse_R_Down( InButton & MINSWP_INBUTTON_MOUSE_R );
        this->Display();

        if (this->GetGame()->GetGameState() == MINSWP_GS_WIN)
        {
            MinSwp_ShowMessage_W(L"You Win! :)");
        };
    };

    QM_VOID this__OnMouse_Move__Game
        (
            const QM_SLONG      Mouse_X,
            const QM_SLONG      Mouse_Y,
            const QM_ULONG      InButton
        )
    {
        if (this->GetGame() == QM_NULL) return ;

        QM_SLONG sel_index = -1;
        if (
                (InButton & MINSWP_INBUTTON_MOUSE_M) || /* ((GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0) || */
                (InButton & MINSWP_INBUTTON_CONTROL) /* ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0) */
            )
            sel_index =
                this__GetSelPolyIndex
                    (
                        Mouse_X,
                        Mouse_Y
                    );

        if (sel_index != this->GetSelectedPolyIndex())
        {
            this__SetSelectedPolyIndex(sel_index);
            this__SetMouse_L_Down(QM_FALSE);
            this__SetMouse_R_Down(QM_FALSE);
            this->Display();

            this__PlaySound(MINSWP_SOUND_HELP);
        };
    };

    QM_VOID this__OnMouse_Up__Game
        (
            const QM_SLONG      Mouse_X,
            const QM_SLONG      Mouse_Y,
            const QM_ULONG      InButton
        )
    {
        if (this->GetGame() == QM_NULL) return ;

        int sel_index = -1;
        if (InButton & MINSWP_INBUTTON_MOUSE_M)
            sel_index = this->GetSelectedPolyIndex();

        this__SetSelectedPolyIndex(sel_index);
        this__SetMouse_L_Down(QM_FALSE);
        this__SetMouse_R_Down(QM_FALSE);
        this->Display();
    };

    /*=======================================================*/

    QM_VOID this__OnMouse_Down__Menu
        (
            const QM_SLONG      Mouse_X,
            const QM_SLONG      Mouse_Y,
            const QM_ULONG      InButton
        )
    {
        if ((InButton & MINSWP_INBUTTON_MOUSE_L) == 0)
            return ;

        T_MinSwp_MenuItem * p_menu_item =
            this->GetCurrentMenuItem();
        RECT wr;
        GetClientRect
            (
                this__p_AppWindow->GetWindowHandle(),
                &wr
            );
        const QM_SLONG W = wr.right    - wr.left;
        const QM_SLONG H = wr.bottom   - wr.top;

        T_MinSwp_MenuItem * p_child =
            p_menu_item->Childs_FindByXY
                (
                    Mouse_X,
                    Mouse_Y,
                    W,
                    H
                );
        if (p_child == QM_NULL)
            return ;
        else
        {
            this__PlaySound(MINSWP_SOUND_HELP);
            if (p_child == this->Menu->p_BackButton)
            {
                this__p_CurrentMenuItem = p_menu_item->Nodes->p_Parent;
                if (this__p_CurrentMenuItem == QM_NULL)
                {
                    this->SetStageState(MINSWP_STAGESTATE_GAME);
                };
                this->Display();
                goto goto_RETURN;
            }
            else
            if (
                    ( p_child->ItemType == MINSWP_MENUITEM_T_FOLDER     ) ||
                    ( p_child->ItemType == MINSWP_MENUITEM_T_RADIOGROUP )
                )
            {
                this__p_CurrentMenuItem = p_child;
                this->Display();
                goto goto_RETURN;
            }
            else
            if (p_child == this->Menu->p_Exit)
            {
                this->GetExitAndClose();
                goto goto_RETURN;
            }
            else
            if (p_child == this->Menu->p_StartNewGame)
            {
                if (
                        this->GetStartNewGame()
                    )
                {
                    this__p_CurrentMenuItem = QM_NULL;
                    this->SetStageState(MINSWP_STAGESTATE_GAME);
                };
                this->Display();
                goto goto_RETURN;
            }
            else
            if (p_child == this->Menu->p_About)
            {
                this__p_AppWindow->ShowAboutDialog();
                goto goto_RETURN;
            }
            else
            if (p_child == this->Menu->p_Sound)
            {
                p_child->CheckBox->Enabled = !p_child->CheckBox->Enabled;
                this__Sound_Disable = !p_child->CheckBox->Enabled;
                if (p_child->CheckBox->Enabled)
                    this__PlaySound(MINSWP_SOUND_HELP);
                this->Display();
                goto goto_RETURN;
            }
            else
            if (p_child->ItemType == MINSWP_MENUITEM_T_RADIOBUTTON )
            {
                this__Prev_Menu_p_SelectGame_Radio_ItemIndex =
                p_menu_item->Radio->ItemIndex;
                p_menu_item->Radio->ItemIndex = p_menu_item->Childs_FindIndex(p_child);

                if (p_menu_item == this->Menu->p_SelectGame)
                    this->ResetGame();

                if (p_menu_item == this->Menu->p_Skin)
                    this__p_Displayer->Skin_SetCurrentIndex
                        (
                            p_menu_item->Radio->ItemIndex - 1
                        );

                this->Display();
                goto goto_RETURN;
            }
            else
            if (p_child->ItemType == MINSWP_MENUITEM_T_SLIDER )
            {
                QM_FRECT fr;
                const QM_FLOAT slider_v01 = p_child->Slider->GetVal01();
                p_menu_item->Childs_GetRect
                    (
                        p_menu_item->Childs_FindIndex(p_child),
                        &fr
                    );
                const QM_FLOAT x01 = (QM_FLOAT)Mouse_X / (QM_FLOAT)W;
                const QM_FLOAT v01 = (x01 - fr.L) / (fr.R - fr.L);
                p_child->Slider->SetVal01(v01);

                /*===============================================*/
                if (
                        (p_child == this->Menu->p_Classic_Width) ||
                        (p_child == this->Menu->p_Classic_Height) ||
                        (p_child == this->Menu->p_NumMines)
                    )
                    this->ResetGame();

                this->Display();
                goto goto_RETURN;
            };

            goto_RETURN:
                return ;
        };
    };

    QM_VOID this__OnMouse_Move__Menu
        (
            const QM_SLONG      Mouse_X,
            const QM_SLONG      Mouse_Y,
            const QM_ULONG      InButton
        )
    {
        if (this->GetGame() == QM_NULL) return ;

        T_MinSwp_MenuItem * p_menu_item =
            this->GetCurrentMenuItem();
        RECT wr;
        GetClientRect
            (
                this__p_AppWindow->GetWindowHandle(),
                &wr
            );
        const QM_SLONG W = wr.right    - wr.left;
        const QM_SLONG H = wr.bottom   - wr.top;

        T_MinSwp_MenuItem * p_child =
            p_menu_item->Childs_FindByXY
                (
                    Mouse_X,
                    Mouse_Y,
                    W,
                    H
                );
        if (p_child != QM_NULL)
        {
            if (p_child->ItemType == MINSWP_MENUITEM_T_SLIDER)
            {
                if (InButton & MINSWP_INBUTTON_MOUSE_L)
                    this->this__OnMouse_Down__Menu
                        (
                            Mouse_X,
                            Mouse_Y,
                            InButton
                        );
            };
        };
    };

    QM_VOID this__OnMouse_Up__Menu
        (
            const QM_SLONG      Mouse_X,
            const QM_SLONG      Mouse_Y,
            const QM_ULONG      InButton
        )
    {
        this->Display();
    };

public:
    #define OnMouse_DownMoveUp_DECL_MACRO(DownMoveUp)         \
    QM_VOID OnMouse_##DownMoveUp                              \
        (                                                     \
            const QM_SLONG      Mouse_X,                      \
            const QM_SLONG      Mouse_Y,                      \
            const QM_ULONG      InButton                      \
        )                                                     \
    {                                                         \
        switch (this->GetStageState())                        \
        {                                                     \
            case MINSWP_STAGESTATE_GAME:                      \
                {                                             \
                    this__OnMouse_##DownMoveUp##__Game        \
                        (                                     \
                            Mouse_X,                          \
                            Mouse_Y,                          \
                            InButton                          \
                        );                                    \
                }; break;                                     \
            case MINSWP_STAGESTATE_MENU:                      \
                {                                             \
                    this__OnMouse_##DownMoveUp##__Menu        \
                        (                                     \
                            Mouse_X,                          \
                            Mouse_Y,                          \
                            InButton                          \
                        );                                    \
                }; break;                                     \
            default:                                          \
                ;                                             \
        };                                                    \
    };


    OnMouse_DownMoveUp_DECL_MACRO(  Down    )
    OnMouse_DownMoveUp_DECL_MACRO(  Move    )
    OnMouse_DownMoveUp_DECL_MACRO(  Up      )
};

/*===========================================================*/

#endif /* __MINESWEEPER3D_HPP__DEFINED */
