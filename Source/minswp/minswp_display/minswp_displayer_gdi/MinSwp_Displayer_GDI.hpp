/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : MinSwp_Displayer_GDI.hpp                       */
/*                                  (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_DISPLAYER_GDI_HPP__DEFINED
#define __MINSWP_DISPLAYER_GDI_HPP__DEFINED

/*===========================================================*/

#include "MineSweeper3D.hpp"

/*===========================================================*/

#include <vcl.h>

/*===========================================================*/

class T_MinSwp_Displayer_GDI : public I_MinSwp_Displayer
{
private:
    struct T_Skin_Base
    {
        struct
        {
            Graphics::TBitmap * p_Free;
            Graphics::TBitmap * p_Start;
            Graphics::TBitmap * p_Flag;
            Graphics::TBitmap * p_Boom;
        } Textures[1];
        struct
        {
            TColor              Font;
            TColor              Border_Start;
            TColor              Border;
        } Colors[1];
    };
    struct T_Skin
    {
        T_Skin_Base Norm[1],
                    Help[1];

        QM_WCHAR    Name[32];
    };

    struct
    {
        union
        {
            T_Skin          __i__[1][1];
            T_Skin          Dark[1];
        };
            T_Skin          Military[1],
                            Golden[1],
                            Snowman[1],
                            Green[1],
                            Sky[1],
                            Pink[1],
                            Gray[1];
    } Skins[1];



    Graphics::TBitmap     * p_TexBack;
 /* Graphics::TBitmap     * p_TexBlockFree;
    Graphics::TBitmap     * p_TexBlockFlag;
    Graphics::TBitmap     * p_TexBlockBoom;
 */

    QM_ULONG                this__Skin_Index;
    QM_SLONG                this__Default_Life;
    I_MinSwp_AppWindow    * this__p_AppWindow;

    QM_VOID this__Present()
    {
        HDC dc = GetDC(this__p_AppWindow->GetWindowHandle());
        BitBlt
            (
                dc,
                0,
                0,
                this->p_RenderTarget->Width,
                this->p_RenderTarget->Height,
                this->p_RenderTarget->Canvas->Handle,
                0,
                0,
                SRCCOPY
            );
        ReleaseDC(this__p_AppWindow->GetWindowHandle(), dc);
    };

    QM_VOID Draw_Poly
        (
                  T_MinSwp_Game       * p_Game,
            const QM_SLONG              Index,
            const QM_FLOAT              X,
            const QM_FLOAT              Y,
            const QM_FLOAT              W,
            const QM_FLOAT              H,
            const QM_SLONG              Sel_Index,
            const MINSWP_ADJACENT_TYPE  Adjacent_Type
        );

    QM_VOID Draw_Heart
        (
            const QM_SLONG              Size,
            const QM_SLONG              Left,
            const QM_SLONG              Top,
            const QM_ULONG              Color
        );

    QM_VOID Draw_Time
        (
            T_MineSweeper3D           * p_MineSweeper3D,
            const QM_SLONG              W,
            const QM_SLONG              H,
            const QM_SLONG              CX
        );

    QM_VOID Draw_NumMines
        (
            T_MineSweeper3D           * p_MineSweeper3D,
            const QM_SLONG              W,
            const QM_SLONG              H,
            const QM_SLONG              CX
        );

    QM_VOID Draw_Emoji
        (
            T_MineSweeper3D           * p_MineSweeper3D,
            const QM_SLONG              W,
            const QM_SLONG              CX,
            const QM_SLONG              CY
        );

    QM_VOID Draw_GameState
        (
            T_MineSweeper3D           * p_MineSweeper3D,
            const QM_SLONG              W,
            const QM_SLONG              H,
            const QM_SLONG              CX
        );


    __QM_CODE__OFF__(virtual)
    QM_VOID Draw_Game
        (
            T_MineSweeper3D           * p_MineSweeper3D,
            /*
                  T_MinSwp_Game       * p_Game,
                  HMINSWP_LIFE          h_Life,
            const QM_SLONG              Selected_Poly,
            const QM_LBOOL              Mouse_L_Down,
            const QM_LBOOL              Mouse_R_Down,
            const MINSWP_ADJACENT_TYPE  Adjacent_Type,
            */
            const QM_SLONG              W,
            const QM_SLONG              H
        ) __QM_CODE__OFF__(minswp_override);

    QM_VOID Draw_Menu
        (
            T_MineSweeper3D           * p_MineSweeper3D,
            const QM_SLONG              W,
            const QM_SLONG              H
        );

    Graphics::TBitmap      * p_RenderTarget;


    /*=======================================================*/
public:
    T_MinSwp_Displayer_GDI
        (
            I_MinSwp_AppWindow    * p_AppWindow
        );
    virtual QM_ULONG Skin_GetCount() minswp_override { return sizeof(this->Skins[0]) / sizeof(this->Skins->__i__[0]); };
    virtual QM_ULONG Skin_GetCurrentIndex() minswp_override
        {
            return this__Skin_Index;
        };
    virtual QM_ULONG Skin_SetCurrentIndex(const QM_ULONG SkinIndex) minswp_override
        {
            if (SkinIndex < this->Skin_GetCount())
            {
                this__Skin_Index = SkinIndex;
            };
            return this__Skin_Index;
        };
    virtual const QM_WCHAR * Skin_GetName(const QM_ULONG SkinIndex) minswp_override
        {
            return
                (SkinIndex < this->Skin_GetCount()) ?
                    this->Skins->__i__[SkinIndex]->Name :
                    L"";
            /*
            switch (SkinIndex)
            {
                case 0  : return L"Dark";
                case 1  : return L"Snowman";
                case 2  : return L"Golden";
                case 3  : return L"Green";
                case 4  : return L"Pink";
                case 5  : return L"Gray";
                case 6  : return L"Sky";
                default : return L"";
            };
            */
        };
    virtual QM_VOID Display
        (
                  T_MineSweeper3D     * p_MineSweeper3D
        ) minswp_override ;
};

/*===========================================================*/

#endif /* __MINSWP_DISPLAYER_GDI_HPP__DEFINED */







