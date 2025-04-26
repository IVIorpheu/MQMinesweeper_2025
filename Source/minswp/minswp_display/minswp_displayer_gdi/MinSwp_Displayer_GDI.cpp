/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : MinSwp_Displayer_GDI.cpp                       */
/*                                  (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "MinSwp_Displayer_GDI.hpp"

/*===========================================================*/

#include <vcl.h>
#include <vector>
#include <string>
#include <math.h>

/*===========================================================*/

class T_BrushPenState
{
private:
    struct
    {
        TColor      Color;
        TBrushStyle Style;
    } Brush[1];

    struct
    {
        TColor      Color;
        TPenStyle   Style;
        int         Width;
    } Pen[1];

    QM_VOID     GetFromBitmap
        (
            Graphics::TBitmap * p_Bitm
        )
    {
        this->Brush->Color  = p_Bitm->Canvas->Brush->Color;
        this->Brush->Style  = p_Bitm->Canvas->Brush->Style;
        this->Pen->Color    = p_Bitm->Canvas->Pen->Color;
        this->Pen->Style    = p_Bitm->Canvas->Pen->Style;
        this->Pen->Width    = p_Bitm->Canvas->Pen->Width;
    };

    QM_VOID     SetToBitmap
        (
            Graphics::TBitmap * p_Bitm
        )
    {
        p_Bitm->Canvas->Brush->Color = this->Brush->Color;
        p_Bitm->Canvas->Brush->Style = this->Brush->Style;
        p_Bitm->Canvas->Pen->Color   = this->Pen->Color;
        p_Bitm->Canvas->Pen->Style   = this->Pen->Style;
        p_Bitm->Canvas->Pen->Width   = this->Pen->Width;
    };

    Graphics::TBitmap * this__p_Bitm;
    T_BrushPenState() {};
public:
    T_BrushPenState(Graphics::TBitmap * p_Bitm)
    {
        this__p_Bitm = p_Bitm;
        if (this__p_Bitm)
            this->GetFromBitmap(this__p_Bitm);
    };
    ~T_BrushPenState()
    {
        if (this__p_Bitm)
            this->SetToBitmap(this__p_Bitm);
    };
};

/*===========================================================*/

void MinSwp_Texture_CreateChess
    (
        const int                   W,
        const int                   H,
        const int                   Size,
        const TColor                C0,
        const TColor                C1,
              Graphics::TBitmap  ** pp_Out_Tex
    )
{
    Graphics::TBitmap * p_lowtex = new Graphics::TBitmap();
    p_lowtex->PixelFormat = pf32bit;
    p_lowtex->Canvas->Brush->Color = C0;
    int W2 = W + ((W & 1) != 0);
    int H2 = H + ((H & 1) != 0);
    p_lowtex->SetSize(W2, H2);

    long C_1 = (C1 & 0xff00ff00L) |
        ((C1 & 0x00ff0000L) >> 16) | 
        ((C1 & 0x000000ffL) << 16);
    ((long *)p_lowtex->ScanLine[0])[1] = C_1;
    ((long *)p_lowtex->ScanLine[1])[0] = C_1;
    for (int i = 0; i < W2; i += 2)
    {
        p_lowtex->Canvas->Draw(i, 0, p_lowtex);
    };
    for (int j = 0; j < H2; j += 2)
    {
        p_lowtex->Canvas->Draw(0, j, p_lowtex);
    };

    Graphics::TBitmap * p_tex = new Graphics::TBitmap();
    p_tex->SetSize(W * Size, H * Size);
    StretchBlt(
        p_tex->Canvas->Handle,
        0, 0, W * Size, H * Size,
        p_lowtex->Canvas->Handle,
        0, 0, W, H, SRCCOPY);
    delete p_lowtex;
    *pp_Out_Tex = p_tex;
};

/*===========================================================*/

#define HELP_CLR(c, hc) ( \
        (( (((c & 0x000000ffUL)      ) + ((hc      ) & 0x000000ffUL) * 1 ) >> 1)      ) |  \
        (( (((c & 0x0000ff00UL) >>  8) + ((hc >>  8) & 0x000000ffUL) * 1 ) >> 1) <<  8) |  \
        (( (((c & 0x00ff0000UL) >> 16) + ((hc >> 16) & 0x000000ffUL) * 1 ) >> 1) << 16)    \
    )


T_MinSwp_Displayer_GDI::T_MinSwp_Displayer_GDI
    (
        I_MinSwp_AppWindow    * p_AppWindow
    )
{
    this__Skin_Index        = 0;
    this__p_AppWindow       = p_AppWindow;
    this__Default_Life      = 3;
    this->p_RenderTarget    = QM_NULL;

    MinSwp_Texture_CreateChess
        (
            64,
            64,
            3,
            (TColor)0x00000000UL,
            (TColor)0x00404040UL,
            &this->p_TexBack
        );

    #define _L_txt(x) L##x

    #define __MinSwp_Skins_Init__MACRO(SkinName,              \
        NC_Free_0 , NC_Free_1 ,                               \
        NC_Boom_0 , NC_Boom_1 ,                               \
        NC_Flag_0 , NC_Flag_1 ,                               \
        NC_Start_0, NC_Start_1,                               \
        H_Color,                                              \
     /* HC_Free_0 , HC_Free_1 , */                            \
     /* HC_Boom_0 , HC_Boom_1 , */                            \
     /* HC_Flag_0 , HC_Flag_1 , */                            \
     /* HC_Start_0, HC_Start_1, */                            \
        Font_Color,                                           \
        Border_Color_Start, Border_Color,                     \
        NSz_Free,                                             \
        NSz_Boom,                                             \
        NSz_Flag,                                             \
        NSz_Start,                                            \
        HSz_Free,                                             \
        HSz_Boom,                                             \
        HSz_Flag,                                             \
        HSz_Start                                             \
        )                                                     \
    {                                                         \
        wcscpy                                                \
            (                                                 \
                this->Skins->SkinName->Name,                  \
                _L_txt(#SkinName)                             \
            );                                                \
                                                              \
        this->Skins->SkinName->Norm->Colors->Font =           \
            (TColor)Font_Color;                               \
        this->Skins->SkinName->Help->Colors->Font =           \
            (TColor)HELP_CLR(Font_Color, H_Color);            \
                                                              \
        this->Skins->SkinName->Norm->Colors->Border_Start =   \
            (TColor)Border_Color_Start;                       \
        this->Skins->SkinName->Help->Colors->Border_Start =   \
            (TColor)HELP_CLR(Border_Color_Start, H_Color);    \
        this->Skins->SkinName->Norm->Colors->Border =         \
            (TColor)Border_Color;                             \
        this->Skins->SkinName->Help->Colors->Border =         \
            (TColor)HELP_CLR(Border_Color, H_Color);          \
                                                              \
        MinSwp_Texture_CreateChess(64, 64, NSz_Free , (TColor)(         NC_Free_0           ), (TColor)(         NC_Free_1           ), &this->Skins->SkinName->Norm->Textures->p_Free  ); \
        MinSwp_Texture_CreateChess(64, 64, HSz_Free , (TColor)(HELP_CLR(NC_Free_0 , H_Color)), (TColor)(HELP_CLR(NC_Free_1 , H_Color)), &this->Skins->SkinName->Help->Textures->p_Free  ); \
                                                                                                                                                                                           \
        MinSwp_Texture_CreateChess(64, 64, NSz_Boom , (TColor)(         NC_Boom_0           ), (TColor)(         NC_Boom_1           ), &this->Skins->SkinName->Norm->Textures->p_Boom  ); \
        MinSwp_Texture_CreateChess(64, 64, HSz_Boom , (TColor)(HELP_CLR(NC_Boom_0 , H_Color)), (TColor)(HELP_CLR(NC_Boom_1 , H_Color)), &this->Skins->SkinName->Help->Textures->p_Boom  ); \
                                                                                                                                                                                           \
        MinSwp_Texture_CreateChess(64, 64, NSz_Flag , (TColor)(         NC_Flag_0           ), (TColor)(         NC_Flag_1           ), &this->Skins->SkinName->Norm->Textures->p_Flag  ); \
        MinSwp_Texture_CreateChess(64, 64, HSz_Flag , (TColor)(HELP_CLR(NC_Flag_0 , H_Color)), (TColor)(HELP_CLR(NC_Flag_1 , H_Color)), &this->Skins->SkinName->Help->Textures->p_Flag  ); \
                                                                                                                                                                                           \
        MinSwp_Texture_CreateChess(64, 64, NSz_Start, (TColor)(         NC_Start_0          ), (TColor)(         NC_Start_1          ), &this->Skins->SkinName->Norm->Textures->p_Start ); \
        MinSwp_Texture_CreateChess(64, 64, HSz_Start, (TColor)(HELP_CLR(NC_Start_0, H_Color)), (TColor)(HELP_CLR(NC_Start_1, H_Color)), &this->Skins->SkinName->Help->Textures->p_Start ); \
    };


    __MinSwp_Skins_Init__MACRO
        (
            Snowman,
            0x00ffffffUL, 0x00d8d2cbUL, __qm_comment__('NC_Free (0, 1)')
            0x00003cd5UL, 0x0000008fUL, __qm_comment__('NC_Boom (0, 1)')
            0x000474ffUL, 0x001038ffUL, __qm_comment__('NC_Flag (0, 1)')
            0x00e0e0d2UL, 0x00272727UL, __qm_comment__('NC_Start(0, 1)')
            0x0050a020UL,               __qm_comment__('Help_Color')
            0x00000000UL,               __qm_comment__('Font_Color')
            0x004f4e4eUL, 0x00000000UL, __qm_comment__('Border_Color(Start, Fixed)')
            1, 5, 4, 2, 1, 5, 4, 2
        );

    __MinSwp_Skins_Init__MACRO
        (
            Golden,
            0x0079cbffUL, 0x0000a8ffUL, __qm_comment__('NC_Free (0, 1)')
            0x000145a2UL, 0x00000038UL, __qm_comment__('NC_Boom (0, 1)')
            0x000ea1ffUL, 0x00000103UL, __qm_comment__('NC_Flag (0, 1)')
            0x005789bdUL, 0x00005079UL, __qm_comment__('NC_Start(0, 1)')
            0x000040f0UL,               __qm_comment__('Help_Color')
            0x00000000UL,               __qm_comment__('Font_Color')
            0x0000375dUL, 0x00000000UL, __qm_comment__('Border_Color(Start, Fixed)')
            2, 5, 4, 2, 2, 5, 4, 2
        );

    __MinSwp_Skins_Init__MACRO
        (
            Green,
            0x000bd07eUL, 0x0000b231UL, __qm_comment__('NC_Free (0, 1)')
            0x001a569aUL, 0x000e2e68UL, __qm_comment__('NC_Boom (0, 1)')
            0x002781efUL, 0x000054a3UL, __qm_comment__('NC_Flag (0, 1)')
            0x002f710bUL, 0x000f1a00UL, __qm_comment__('NC_Start(0, 1)')
            0x004080f0UL,               __qm_comment__('Help_Color')
            0x00000000UL,               __qm_comment__('Font_Color')
            0x001d8006UL, 0x00002f06UL, __qm_comment__('Border_Color(Start, Fixed)')
            2, 5, 4, 2, 2, 5, 4, 2
        );

    __MinSwp_Skins_Init__MACRO
        (
            Pink,
            0x0058a0ffUL, 0x009837ffUL, __qm_comment__('NC_Free (0, 1)')
            0x004b28d7UL, 0x00142674UL, __qm_comment__('NC_Boom (0, 1)')
            0x00442bffUL, 0x004a2cbdUL, __qm_comment__('NC_Flag (0, 1)')
            0x006b75d8UL, 0x0000082cUL, __qm_comment__('NC_Start(0, 1)')
            0x00e0c0ffUL,               __qm_comment__('Help_Color')
            0x00000045UL,               __qm_comment__('Font_Color')
            0x00061945UL, 0x00000a51UL, __qm_comment__('Border_Color(Start, Fixed)')
            1, 5, 4, 2, 1, 5, 4, 2
        );

    __MinSwp_Skins_Init__MACRO
        (
            Gray,
            0x00d0d0d0UL, 0x00808080UL, __qm_comment__('NC_Free (0, 1)')
            0x0020008fUL, 0x002000ffUL, __qm_comment__('NC_Boom (0, 1)')
            0x00404040UL, 0x0080ff80UL, __qm_comment__('NC_Flag (0, 1)')
            0x00404040UL, 0x00808080UL, __qm_comment__('NC_Start(0, 1)')
            0x00c0c070UL,               __qm_comment__('Help_Color')
            0x00000000UL,               __qm_comment__('Font_Color')
            0x00202020UL, 0x00000000UL, __qm_comment__('Border_Color(Start, Fixed)')
            1, 5, 4, 2, 1, 5, 4, 2
        );

    __MinSwp_Skins_Init__MACRO
        (
            Sky,
            0x00ffecc8UL, 0x00f6d07eUL, __qm_comment__('NC_Free (0, 1)')
            0x004ca0ffUL, 0x008e1d4dUL, __qm_comment__('NC_Boom (0, 1)')
            0x0054d9ffUL, 0x002490ffUL, __qm_comment__('NC_Flag (0, 1)')
            0x00b1e8e0UL, 0x0099566bUL, __qm_comment__('NC_Start(0, 1)')
            0x00403838UL,               __qm_comment__('Help_Color')
            0x00402020UL,               __qm_comment__('Font_Color')
            0x00b8e2f3UL, 0x00f4b939UL, __qm_comment__('Border_Color(Start, Fixed)')
            1, 5, 4, 2, 1, 5, 4, 2
        );

    __MinSwp_Skins_Init__MACRO
        (
            Dark,
            0x00101010UL, 0x00808080UL, __qm_comment__('NC_Free (0, 1)')
            0x00200040UL, 0x002000d0UL, __qm_comment__('NC_Boom (0, 1)')
            0x00202020UL, 0x0040d040UL, __qm_comment__('NC_Flag (0, 1)')
            0x00000000UL, 0x00101010UL, __qm_comment__('NC_Start(0, 1)')
            0x00b0b060UL,               __qm_comment__('Help_Color')
            0x00d0d0d0UL,               __qm_comment__('Font_Color')
            0x00404040UL, 0x00000000UL, __qm_comment__('Border_Color(Start, Fixed)')
            1, 5, 4, 2, 1, 5, 4, 2
        );

    __MinSwp_Skins_Init__MACRO
        (
            Military,
            0x0083b3c3UL, 0x005f7177UL, __qm_comment__('NC_Free (0, 1)')
            0x00365d8bUL, 0x00151b2bUL, __qm_comment__('NC_Boom (0, 1)')
            0x005093b2UL, 0x00316b87UL, __qm_comment__('NC_Flag (0, 1)')
            0x005e7694UL, 0x000b0e1aUL, __qm_comment__('NC_Start(0, 1)')
            0x0068b8ffUL,               __qm_comment__('Help_Color')
            0x000a0c1dUL,               __qm_comment__('Font_Color')
            0x00080808UL, 0x00202e3bUL, __qm_comment__('Border_Color(Start, Fixed)')
            1, 5, 4, 2, 1, 5, 4, 2
        );
};

/*===========================================================*/

QM_VOID T_MinSwp_Displayer_GDI::Draw_Poly
    (
              T_MinSwp_Game       * p_Game,
        const QM_SLONG              Index,
        const QM_FLOAT              X,
        const QM_FLOAT              Y,
        const QM_FLOAT              W,
        const QM_FLOAT              H,
        const QM_SLONG              Sel_Index,
        const MINSWP_ADJACENT_TYPE  Adjacent_Type
    )
{
    T_Skin * p_Skin = this->Skins->__i__[this__Skin_Index];

    T_BrushPenState brush_pen_state(this->p_RenderTarget);

    //--------------------------------------

    this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x000000ffUL;
    this->p_RenderTarget->Canvas->Brush->Style = bsSolid;
    this->p_RenderTarget->Canvas->Pen->Style   = psSolid;
    this->p_RenderTarget->Canvas->Pen->Color   = (TColor)0x00000000UL;
    this->p_RenderTarget->Canvas->Pen->Width   = 1;

    std::vector< POINT > vec_points;

    HMINSWP_TRIMESH h_mesh = QM_NULL;

    p_Game->Poly[Index]->GetTriMesh(&h_mesh);
    if (h_mesh == QM_NULL)
    {
        ShowMessage(L"h_Mesh == NULL!");
    };
    const int verts_count = (int)MinSwp_TriMesh_Verts_Size(h_mesh);
    vec_points.resize(verts_count);
    for (int i = 0; i < verts_count; i++)
    {
        vec_points[i].x = (int)(MinSwp_TriMesh_Verts_Get(h_mesh, i)->x * W + X);
        vec_points[i].y = (int)(MinSwp_TriMesh_Verts_Get(h_mesh, i)->y * H + Y);
    };
    MinSwp_TriMesh_Release(h_mesh);
    MINSWP_DRAW_TYPE poly_draw_type = p_Game->Poly[Index]->GetDrawType(Adjacent_Type);

    long C; // = 0x00000000L;

    bool is_selected = (Index == Sel_Index);
    bool is_adjacent = p_Game->Poly[Sel_Index]->IsAdjacent(Index, Adjacent_Type);
    bool is_seloraddj = (is_selected || is_adjacent);

    T_Skin_Base * p_skin_local =
        (is_seloraddj) ?
            p_Skin->Help :
            p_Skin->Norm;

    Graphics::TBitmap * p_CB; // = NULL;
    switch (poly_draw_type)
    {
        case MINSWP_DT_HEART:
        case MINSWP_DT_FLAG :   p_CB = p_skin_local->Textures->p_Flag;  break;
        case MINSWP_DT_BOOM :   p_CB = p_skin_local->Textures->p_Boom;  break;
        case MINSWP_DT_START:   p_CB = p_skin_local->Textures->p_Start; break;
        default:                p_CB = p_skin_local->Textures->p_Free;
    };

    int  pen_width = 2 + 2;
    long pen_color =
        (poly_draw_type == MINSWP_DT_START) ?
            p_skin_local->Colors->Border_Start :
            p_skin_local->Colors->Border;

    if (is_seloraddj)
    {
        pen_width += 1;
        //pen_color = 0x00800000L;
        // C = (C & 0x00fff0a0L) | 0x00ff8040L;
    };

    this->p_RenderTarget->Canvas->Pen->Width = pen_width;
    this->p_RenderTarget->Canvas->Pen->Color = (TColor)pen_color;
    this->p_RenderTarget->Canvas->Brush->Bitmap = p_CB;
    this->p_RenderTarget->Canvas->Polygon((TPoint *)&vec_points[0], verts_count - 1);

    //MINSWP_VEC3D v;
    //const QM_FLOAT sphere_radius = p_Game->Poly[Index]->GetSphereRadius() * 0.05f;
    //p_Game->Poly[Index]->GetSpherePos(&v);
    UnicodeString str = L"";
    switch (poly_draw_type)
    {
        case MINSWP_DT_HEART:   str = /* L"H" */ L"💔" ; C = 0x000000ffUL; break;
        case MINSWP_DT_FLAG:    str = /* L"F" */ L"🚩" ; C = 0x00ffffffUL; break;
        case MINSWP_DT_BOOM:    str = /* L"X" */ L"💣" ; C = 0x000000ffUL; break;
        case MINSWP_DT_START:   str =            L""   ; C = 0x00ffffffUL; break;
        case MINSWP_DT_FREE:    str =            L""   ; C = 0x00808080UL; break;

    default:
        str = IntToStr((int)poly_draw_type);
        C  =  ((int)poly_draw_type % 2 + 1) * 8 - 1;
        C |= (((int)poly_draw_type % 4 + 1) * 16 - 1) << 8;
        C |= (((int)poly_draw_type % 8 + 1) * 32 - 1) << 16;
        //C |= C << (((int)poly_draw_type % 3) * 16);
    };
    this->p_RenderTarget->Canvas->Brush->Style = bsClear;

    {
        int x_min = vec_points[0].x;
        int x_max = vec_points[0].x;
        int y_min = vec_points[0].y;
        int y_max = vec_points[0].y;
        for (int i = 1; i < verts_count; i++)
        {
            const int x = vec_points[i].x;
            const int y = vec_points[i].y;

            if (x_min > x)
                x_min = x;
            if (x_max < x)
                x_max = x;
            if (y_min > y)
                y_min = y;
            if (y_max < y)
                y_max = y;
        };
        int dx = x_max - x_min;
        int dy = y_max - y_min;
        this->p_RenderTarget->Canvas->Font->Size = 1 + (((dx < dy) ? dx : dy) * 2) / 5;
    };
    this->p_RenderTarget->Canvas->Font->Name = L"Courier New";
                                   //L"Times New Roman";
    this->p_RenderTarget->Canvas->Font->Style = TFontStyles() << fsBold;
    this->p_RenderTarget->Canvas->Font->Color = (TColor)(C);/// & 0xffff00ffL);
    //this->p_RenderTarget->Canvas->TextOutW((int)(v.x * w) - 4, (int)(v.y * h) - 10, str);

    QM_FLOAT s_r = 1.0f; //p_geom->poly_GetSphereRadius(index);
    s_r *= sqrt((W * W) + (H * H));
    MINSWP_VEC3D v3_c;
    p_Game->Poly[Index]->GetSpherePos(&v3_c);
    //this->p_RenderTarget->Canvas->Font->Quality = TFontQuality::fqAntialiased;
    //this->p_RenderTarget->Canvas->Font->Size = 14;//(int)(s_r * 0.011f);
    v3_c.x *= W;
    v3_c.y *= H;
    TRect r_tr = Rect(
            (int)(X + v3_c.x - s_r),
            (int)(Y + v3_c.y - s_r),
            (int)(X + v3_c.x + s_r + 2),
            (int)(Y + v3_c.y + s_r + 2));
    /*
    if (poly_draw_type == MINSWP_DT_FLAG)
    {
        this->p_RenderTarget->Canvas->Pen->Style = psClear;//(TColor)0x00d0d0d0L;
        this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x000000ffL;
        int ell_x = (r_tr.left + r_tr.right) / 2;
        int ell_y = (r_tr.top  + r_tr.bottom) / 2;
        this->p_RenderTarget->Canvas->Ellipse(
            ell_x - 10, 
            ell_y - 10,
            ell_x + 10, 
            ell_y + 10);
        this->p_RenderTarget->Canvas->Brush->Style = bsClear;
    };
    */
    this->p_RenderTarget->Canvas->Font->Color =
        (poly_draw_type == MINSWP_DT_BOOM) ?
            (TColor)0x00000000UL :
            (
                (poly_draw_type == MINSWP_DT_FLAG) ?
                    (TColor)0x00000000UL :
                    (
                        (poly_draw_type == MINSWP_DT_HEART) ?
                            (TColor)0x00000000UL :
                            p_skin_local->Colors->Font
                    )
            );
    this->p_RenderTarget->Canvas->TextRect
        (
            r_tr,
            str,
            TTextFormat()
                << tfSingleLine
                << tfCenter
                << tfVerticalCenter
        );



};

/*===========================================================*/

QM_VOID T_MinSwp_Displayer_GDI::Draw_Heart
    (
        const QM_SLONG          Size,
        const QM_SLONG          Left,
        const QM_SLONG          Top,
        const QM_ULONG          Color
    )
{
    T_BrushPenState brush_pen_state(p_RenderTarget);
    //--------------------------------------
    this->p_RenderTarget->Canvas->Brush->Color  = (TColor)Color;
    this->p_RenderTarget->Canvas->Brush->Style  = bsSolid;
    this->p_RenderTarget->Canvas->Pen->Style    = psSolid;
    this->p_RenderTarget->Canvas->Pen->Color    = this->p_RenderTarget->Canvas->Brush->Color;
    this->p_RenderTarget->Canvas->Pen->Width    = 1;
    //-------------------------------------
    TPoint triangle[4];
    QM_FLOAT radius = (QM_FLOAT)Size * 0.25f;
    #define COS_45 0.7071067811f
    QM_FLOAT center_x = Size * 0.5f - radius * COS_45;
    QM_FLOAT center_y = radius + radius * 2.5f * (1.0f - COS_45);
    int X = (int)center_x + Left;
    int Y = (int)center_y + Top;
    int R = (int)radius;

    triangle[0].x = Left + Size / 2;
    triangle[0].y = Y + (int)(radius * COS_45 * 3.0f);
    triangle[1].x = X - (int)(radius * COS_45);
    triangle[1].y = Y + (int)(radius * COS_45);
    triangle[2].x = triangle[0].x;
    triangle[2].y = (int)center_y;

    this->p_RenderTarget->Canvas->Ellipse
        (
            X - R,
            Y - R,
            X + R,
            Y + R
        );
    center_x = Size * 0.5f + radius * COS_45;
    X = (int)center_x + Left;
    triangle[3].x = X + (int)(radius * COS_45);
    triangle[3].y = triangle[1].y;
    this->p_RenderTarget->Canvas->Ellipse
        (
            X - R,
            Y - R,
            X + R,
            Y + R
        );
    //this->p_RenderTarget->Canvas->Pen->Color = 0;
    this->p_RenderTarget->Canvas->Polygon(triangle, 3);

};

/*===========================================================*/

QM_VOID T_MinSwp_Displayer_GDI::Draw_Time
    (
        T_MineSweeper3D           * p_MineSweeper3D,
        const QM_SLONG              W,
        const QM_SLONG              H,
        const QM_SLONG              CX
    )
{
    T_BrushPenState brush_pen_state(p_RenderTarget);

    const QM_FLOAT panel_top_H = (QM_FLOAT)H * p_MineSweeper3D->GetPanelTopHeightPercent();

    const QM_SLL64 t = p_MineSweeper3D->GetGame()->Timer_GetTimePerSec_i64();
    const QM_WCHAR ws[] =
        {
            (QM_WCHAR)(L'0' + ((t / 3600) % 24  ) / 10),
            (QM_WCHAR)(L'0' + ((t / 3600) % 24  ) % 10),
            (QM_WCHAR)(L':'),
            (QM_WCHAR)(L'0' + ((t / 60  ) % 60  ) / 10),
            (QM_WCHAR)(L'0' + ((t / 60  ) % 60  ) % 10),
            (QM_WCHAR)(L':'),
            (QM_WCHAR)(L'0' + ((t       ) % 60  ) / 10),
            (QM_WCHAR)(L'0' + ((t       ) % 60  ) % 10),
            (QM_WCHAR)0
        };
    UnicodeString s    = UnicodeString(L"        ") + ws;
    UnicodeString s_tm =               L"      ⏰ " L"        ";
                                    /*              L"hh:mm:ss" */

    p_RenderTarget->Canvas->Brush->Style = bsClear;

    p_RenderTarget->Canvas->Font->Color  = (TColor)0x00f0f0f0UL;
    p_RenderTarget->Canvas->Font->Style  = TFontStyles() << fsBold;
    p_RenderTarget->Canvas->Font->Height = panel_top_H * 0.8f;

    {
        const QM_FLOAT h_scl = 0.8f;
        const QM_FLOAT txtop = (1.0f - h_scl) * 0.5f;
        TRect r =
            Rect(
                    (int)( ( CX         ) - panel_top_H * 5.0f),
                    (int)( (0.0  + txtop) * panel_top_H       ),
                    (int)( ( CX         ) + panel_top_H * 5.0f),
                    (int)( (1.0f - txtop) * panel_top_H )
                );
        p_RenderTarget->Canvas->TextRect
            (
                r,
                s,
                TTextFormat()
                    << TTextFormats::tfCenter
                    << TTextFormats::tfVerticalCenter
            );
    };

    {
        const QM_FLOAT h_scl = 0.8f;
        const QM_FLOAT txtop = (1.0f - h_scl) * 0.5f;
        TRect r =
            Rect(
                    (int)( ( CX         ) - panel_top_H * 5.0f),
                    (int)( (0.0  + txtop) * panel_top_H       ),
                    (int)( ( CX         ) + panel_top_H * 5.0f),
                    (int)( (1.0f - txtop) * panel_top_H )
                );
        p_RenderTarget->Canvas->Font->Color  = (TColor)0x0040ff80UL;
        p_RenderTarget->Canvas->Font->Style  = TFontStyles() << fsBold;
        p_RenderTarget->Canvas->Font->Height = (int)(h_scl * panel_top_H);
        p_RenderTarget->Canvas->TextRect
            (
                r,
                s_tm,
                TTextFormat()
                    << TTextFormats::tfCenter
                    << TTextFormats::tfVerticalCenter
            );
    };
};

QM_VOID T_MinSwp_Displayer_GDI::Draw_NumMines
    (
        T_MineSweeper3D           * p_MineSweeper3D,
        const QM_SLONG              W,
        const QM_SLONG              H,
        const QM_SLONG              CX
    )
{
    T_BrushPenState brush_pen_state(p_RenderTarget);

    const QM_FLOAT h = (QM_FLOAT)H;
 /* const MINSWP_GAME_STATE gs = p_MineSweeper3D->GetGame()->GetGameState();
 */ const QM_FLOAT panel_top_H = h * p_MineSweeper3D->GetPanelTopHeightPercent();

    {
        const int num_mines =
            (int)p_MineSweeper3D->GetGame()->GetNumMines() -
            (int)p_MineSweeper3D->GetGame()->GetNumFlags() -
            (int)(
                    3 -
                    MinSwp_Life_Get
                        (
                            p_MineSweeper3D->GetLife()
                        )
                );
        UnicodeString s = L"   " + IntToStr( num_mines );

        QM_ULONG font_color = (num_mines < 0) ? 0x000000f0UL : 0x00f0f0f0UL;

        const QM_FLOAT h_scl = 0.8f;
        const QM_FLOAT txtop = (1.0f - h_scl) * 0.5f;

        TRect r =
            Rect(
                    (int)( ( CX         ) - (panel_top_H * 5.0f) ),
                    (int)( (0.0f + txtop) * (panel_top_H       ) ),
                    (int)( ( CX         ) + (panel_top_H * 5.0f) ),
                    (int)( (1.0f - txtop) * (panel_top_H       ) )
                );
        p_RenderTarget->Canvas->Brush->Style    = bsClear;
     /* p_RenderTarget->Canvas->Font->Name      = L"Times New Roman";
     */ p_RenderTarget->Canvas->Font->Style     = TFontStyles() << fsBold;
        p_RenderTarget->Canvas->Font->Color     = (TColor)font_color;
        p_RenderTarget->Canvas->Font->Height    = (int)(h_scl * panel_top_H);
        p_RenderTarget->Canvas->TextRect
            (
                r,
                s,
                TTextFormat()
                    << TTextFormats::tfCenter
                    << TTextFormats::tfVerticalCenter
            );
    };

    {
        const QM_FLOAT h_scl = 0.7f;
        const QM_FLOAT txtop = (1.0f - h_scl) * 0.5f;

        UnicodeString s = L" 💣    ";

        p_RenderTarget->Canvas->Brush->Style    = bsSolid;
        p_RenderTarget->Canvas->Brush->Color    = (TColor)0x000000f0UL;
        p_RenderTarget->Canvas->Ellipse
            (
                CX - (int)(panel_top_H * ( 0.4f + 0.6f)),
                     (int)(panel_top_H *   0.1f),
                CX - (int)(panel_top_H * (-0.4f + 0.6f)),
                     (int)(panel_top_H *   0.9f)
            );

        TRect r =
            Rect(
                    (int)( ( CX         ) - (panel_top_H * 5.0f) ),
                    (int)( (0.0f + txtop) * (panel_top_H       ) ),
                    (int)( ( CX         ) + (panel_top_H * 5.0f) ),
                    (int)( (1.0f - txtop) * (panel_top_H       ) )
                );
        p_RenderTarget->Canvas->Brush->Style    = bsClear;
     /* p_RenderTarget->Canvas->Font->Name      = L"Times New Roman";
     */ p_RenderTarget->Canvas->Font->Style     = TFontStyles() << fsBold;
        p_RenderTarget->Canvas->Font->Color     = (TColor)0x00000000UL;
        p_RenderTarget->Canvas->Font->Height    = (int)(h_scl * panel_top_H);
        p_RenderTarget->Canvas->TextRect
            (
                r,
                s,
                TTextFormat()
                    << TTextFormats::tfCenter
                    << TTextFormats::tfVerticalCenter
            );
    };
};

QM_VOID T_MinSwp_Displayer_GDI::Draw_Emoji
    (
        T_MineSweeper3D           * p_MineSweeper3D,
        const QM_SLONG              W,
        const QM_SLONG              CX,
        const QM_SLONG              CY
    )
{
    T_BrushPenState brush_pen_state(p_RenderTarget);

    const MINSWP_GAME_STATE
        game_state =
            p_MineSweeper3D->GetGame()->GetGameState();

    p_RenderTarget->Canvas->Pen->Style = psSolid;
    p_RenderTarget->Canvas->Pen->Color = (TColor)0x00000000UL;
    p_RenderTarget->Canvas->Pen->Width = 2 + W / 10;

    p_RenderTarget->Canvas->Brush->Color = (TColor)0x0000c0ffUL;
    p_RenderTarget->Canvas->Ellipse
        (
            CX - W,
            CY - W,
            CX + W,
            CY + W
        );

    p_RenderTarget->Canvas->Pen->Style = psClear;
    p_RenderTarget->Canvas->Brush->Color = (TColor)0x00000000UL;
    const QM_SLONG eye_cx_L = CX - W / 3;
    const QM_SLONG eye_cx_R = CX + W / 3;
    const QM_SLONG eye_cy = CY - W / 4;
    const QM_SLONG w_eye =
        (game_state == MINSWP_GS_LOSS) ?
            (1 + W / 4) :
            (1 + W / 5);
    const QM_SLONG h_eye =
        (game_state == MINSWP_GS_WIN) ?
            (1 + W / 4) :
            (
                (game_state == MINSWP_GS_LOSS) ?
                    (1 + W / 7) :
                    (1 + W / 5)
            );

    p_RenderTarget->Canvas->Ellipse
        (
            eye_cx_L - w_eye,
            eye_cy   - h_eye,
            eye_cx_L + w_eye,
            eye_cy   + h_eye
        );
    p_RenderTarget->Canvas->Ellipse
        (
            eye_cx_R - w_eye,
            eye_cy   - h_eye,
            eye_cx_R + w_eye,
            eye_cy   + h_eye
        );

    p_RenderTarget->Canvas->Pen->Style = psSolid;
    p_RenderTarget->Canvas->Pen->Width = 2 + W / 10;

    switch (game_state)
    {
        case MINSWP_GS_WIN:
            {
                const QM_SLONG c = 8;

                p_RenderTarget->Canvas->Pen->Color = (TColor)0x00ffffffUL;
                {
                    const QM_FLOAT r = ((QM_FLOAT)W) * 0.8f;
                    for (QM_SLONG i = 0; i <= c; i++)
                    {
                        const QM_FLOAT x_11 = 1.0f - 2.0f * (QM_FLOAT)i / (QM_FLOAT)c;
                        const QM_FLOAT a = x_11 * M_PI * 0.2f + M_PI_2;

                        const QM_FLOAT r_x_cos = r * cosf(a);
                        const QM_FLOAT r_x_sin = r * sinf(a);

                        const QM_SLONG X = (QM_SLONG)(r_x_cos + CX);
                        const QM_SLONG Y = (QM_SLONG)(r_x_sin + CY) - (QM_SLONG)((QM_FLOAT)W * 0.3f);

                        if (i)
                            p_RenderTarget->Canvas->LineTo(X, Y);
                        else
                            p_RenderTarget->Canvas->MoveTo(X, Y);
                    };
                };
                p_RenderTarget->Canvas->Pen->Color = (TColor)0x00000000UL;
                {
                    const QM_FLOAT r = ((QM_FLOAT)W) * 0.55f;
                    for (QM_SLONG i = 0; i <= c; i++)
                    {
                        const QM_FLOAT x_11 = 1.0f - 2.0f * (QM_FLOAT)i / (QM_FLOAT)c;
                        const QM_FLOAT a = x_11 * M_PI * 0.4f + M_PI_2;

                        const QM_FLOAT r_x_cos = r * cosf(a);
                        const QM_FLOAT r_x_sin = r * sinf(a) * 1.1f;

                        const QM_SLONG X = (QM_SLONG)(r_x_cos + CX);
                        const QM_SLONG Y = (QM_SLONG)(r_x_sin + CY) + (QM_SLONG)((QM_FLOAT)W * 0.0f);

                        if (i)
                            p_RenderTarget->Canvas->LineTo(X, Y);
                        else
                            p_RenderTarget->Canvas->MoveTo(X, Y);
                    };
                };

                {
                    const QM_FLOAT r = ((QM_FLOAT)W) * 1.0f;
                    for (QM_SLONG i = 0; i <= c; i++)
                    {
                        const QM_FLOAT x_11 = 1.0f - 2.0f * (QM_FLOAT)i / (QM_FLOAT)c;
                        const QM_FLOAT a = x_11 * M_PI * 0.2f + M_PI_2;

                        const QM_FLOAT r_x_cos = r * cosf(a);
                        const QM_FLOAT r_x_sin = r * sinf(a);

                        const QM_SLONG X = (QM_SLONG)(r_x_cos + CX);
                        const QM_SLONG Y = (QM_SLONG)(r_x_sin + CY) - (QM_SLONG)((QM_FLOAT)W * 0.6f);

                        if (i)
                            p_RenderTarget->Canvas->LineTo(X, Y);
                        else
                            p_RenderTarget->Canvas->MoveTo(X, Y);
                    };
                };
            }; break;
        case MINSWP_GS_LOSS:
            {
                const QM_SLONG c = 8;
                const QM_FLOAT r = ((QM_FLOAT)W) * 0.4f;
                for (QM_SLONG i = 0; i <= c; i++)
                {
                    const QM_FLOAT x_11 = 1.0f - 2.0f * (QM_FLOAT)i / (QM_FLOAT)c;
                    const QM_FLOAT a = x_11 * M_PI * 0.4f + M_PI_2 + M_PI;

                    const QM_FLOAT r_x_cos = r * cosf(a);
                    const QM_FLOAT r_x_sin = r * sinf(a) * 0.9f;

                    const QM_SLONG X = (QM_SLONG)(r_x_cos + CX);
                    const QM_SLONG Y = (QM_SLONG)(r_x_sin + CY) + (QM_SLONG)((QM_FLOAT)W * 0.6f);

                    if (i)
                        p_RenderTarget->Canvas->LineTo(X, Y);
                    else
                        p_RenderTarget->Canvas->MoveTo(X, Y);
                };
            }; break;
        default:
            {
                const QM_SLONG c = 8;
                const QM_FLOAT r = ((QM_FLOAT)W) * 0.6f;
                for (QM_SLONG i = 0; i <= c; i++)
                {
                    const QM_FLOAT x_11 = 1.0f - 2.0f * (QM_FLOAT)i / (QM_FLOAT)c;
                    const QM_FLOAT a = x_11 * M_PI * 0.225f + M_PI_2;

                    const QM_FLOAT r_x_cos = r * cosf(a);
                    const QM_FLOAT r_x_sin = r * sinf(a);

                    const QM_SLONG X = (QM_SLONG)(r_x_cos + CX);
                    const QM_SLONG Y = (QM_SLONG)(r_x_sin + CY) - (QM_SLONG)((QM_FLOAT)W * 0.175f);

                    if (i)
                        p_RenderTarget->Canvas->LineTo(X, Y);
                    else
                        p_RenderTarget->Canvas->MoveTo(X, Y);
                };
            };
    };
};

QM_VOID T_MinSwp_Displayer_GDI::Draw_GameState
    (
        T_MineSweeper3D           * p_MineSweeper3D,
        const QM_SLONG              W,
        const QM_SLONG              H,
        const QM_SLONG              CX
    )
{
    T_BrushPenState brush_pen_state(p_RenderTarget);

    const QM_FLOAT h = (QM_FLOAT)H;
    const MINSWP_GAME_STATE gs = p_MineSweeper3D->GetGame()->GetGameState();
    const QM_FLOAT panel_top_H = h * p_MineSweeper3D->GetPanelTopHeightPercent();

    {
        UnicodeString s;
        QM_ULONG font_color;
        switch (gs)
        {
            case MINSWP_GS_WIN:
                {
                    s = L"           You Win ";
                    font_color = 0x0000ff00UL;
                }; break;
            case MINSWP_GS_LOSS:
                {
                    s = L"           You Lose";
                    font_color = 0x000000ffUL;
                }; break;
            default:
                {
                    s = L"                   ";
                     /* L"           Playing " */
                    font_color = 0x00f0f0f0UL;
                };
        };

        const QM_FLOAT h_scl = 0.7f;
        const QM_FLOAT txtop = (1.0f - h_scl) * 0.5f;

        TRect r =
            Rect(
                    (int)( ( CX         ) - (panel_top_H * 5.0f) ),
                    (int)( (0.0f + txtop) * (panel_top_H       ) ),
                    (int)( ( CX         ) + (panel_top_H * 5.0f) ),
                    (int)( (1.0f - txtop) * (panel_top_H       ) )
                );
        p_RenderTarget->Canvas->Brush->Style    = bsClear;
     /* p_RenderTarget->Canvas->Font->Name      = L"Times New Roman";
     */ p_RenderTarget->Canvas->Font->Style     = TFontStyles() << fsBold;
        p_RenderTarget->Canvas->Font->Color     = (TColor)font_color;
        p_RenderTarget->Canvas->Font->Height    = (int)(h_scl * panel_top_H);
        p_RenderTarget->Canvas->TextRect
            (
                r,
                s,
                TTextFormat()
                    << TTextFormats::tfCenter
                    << TTextFormats::tfVerticalCenter
            );
    };

    /*
    {
        const QM_FLOAT h_scl = 0.5f;
        const QM_FLOAT txtop = (1.0f - h_scl) * 0.5f;

        UnicodeString s =
            L"State          ";

        TRect r =
            Rect(
                    (int)( ( CX         ) - (panel_top_H * 5.0f) ),
                    (int)( (0.0f + txtop) * (panel_top_H       ) ),
                    (int)( ( CX         ) + (panel_top_H * 5.0f) ),
                    (int)( (1.0f - txtop) * (panel_top_H       ) )
                );
        p_RenderTarget->Canvas->Brush->Style    = bsClear;
        p_RenderTarget->Canvas->Font->Style     = TFontStyles() << fsBold;
        p_RenderTarget->Canvas->Font->Color     = (TColor)0x00606060UL;
        p_RenderTarget->Canvas->Font->Height    = (int)(h_scl * panel_top_H);
        p_RenderTarget->Canvas->TextRect
            (
                r,
                s,
                TTextFormat()
                    << TTextFormats::tfCenter
                    << TTextFormats::tfVerticalCenter
            );
    };
    */

    this->Draw_Emoji
        (
            p_MineSweeper3D,
            (QM_SLONG)(panel_top_H * 0.8f * 0.5f),
            CX,
            (QM_SLONG)(panel_top_H * 0.5f)
        );
};

QM_VOID T_MinSwp_Displayer_GDI::Draw_Game
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
    )
{
    if (
            (p_MineSweeper3D) &&
            (p_MineSweeper3D->GetGame()) &&
            (p_MineSweeper3D->GetLife())
        )
    {
        /*
        this->p_RenderTarget->Canvas->Pen->Style   = psClear;
        this->p_RenderTarget->Canvas->Brush->Style = bsDiagCross;
        this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x00fff080L;
        */
        this->p_RenderTarget->Canvas->Brush->Bitmap = p_TexBack;
        this->p_RenderTarget->Canvas->FillRect(Rect(0, 0, W, H));

        const QM_FLOAT w = (QM_FLOAT)W;
        const QM_FLOAT h = (QM_FLOAT)H;
        this->p_RenderTarget->Canvas->Pen->Style   = psSolid;
        const QM_ULONG polys_count = p_MineSweeper3D->GetGame()->GetNumPolys();

        const QM_FLOAT panel_top_H = h * p_MineSweeper3D->GetPanelTopHeightPercent();

        const QM_SLONG selected_poly_index = p_MineSweeper3D->GetSelectedPolyIndex();
        const MINSWP_ADJACENT_TYPE adjacenct_type = p_MineSweeper3D->GetAdjacentType();

        for (QM_ULONG i = 0; i < polys_count; i++)
        {
            this->Draw_Poly
                (
                    p_MineSweeper3D->GetGame(),
                    (QM_SLONG)i,
                    0,
                        (QM_FLOAT)panel_top_H,
                    w,
                    h - (QM_FLOAT)panel_top_H,
                    selected_poly_index,
                    adjacenct_type
                );
        };

        { __qm_comment__('Draw Menu Icon')
            const int x0 = panel_top_H / 5.0f;
            const int x1 = panel_top_H - x0;
            this->p_RenderTarget->Canvas->Pen->Style = psSolid;
            this->p_RenderTarget->Canvas->Pen->Width = 1 + panel_top_H / 10;
            this->p_RenderTarget->Canvas->Pen->Color = (TColor)0x00f0f0f0UL;
            for (int i = 0; i < 3; i++)
            {
                const int y = (panel_top_H * (i + 1)) / 4;
                this->p_RenderTarget->Canvas->MoveTo(x0, y);
                this->p_RenderTarget->Canvas->LineTo(x1, y);
            };
        };

        { __qm_comment__('Draw_Heart')
            const int size_of_hearth = panel_top_H * 0.8f;
            const QM_SLONG num_life = MinSwp_Life_Get(p_MineSweeper3D->GetLife());
            for (int i = 0; i < this__Default_Life; i++)
            {
                const int left = w - size_of_hearth * (i + 1);
                const int top = (int)((panel_top_H - size_of_hearth) * 0.5f);
                this->p_RenderTarget->Canvas->Brush->Style = bsSolid;
                this->p_RenderTarget->Canvas->Brush->Color = (TColor)((i < num_life) ? 0x00f0f0f0UL : 0x00808080UL);
                this->p_RenderTarget->Canvas->Pen->Color   = (TColor)0x00000000L;
                this->p_RenderTarget->Canvas->Pen->Width   = 1;
                this->p_RenderTarget->Canvas->Pen->Style   = psSolid;
                this->p_RenderTarget->Canvas->Ellipse
                    (
                        left,
                        top,
                        left    + size_of_hearth,
                        top     + size_of_hearth
                    );
                /*===============================================*/
                Draw_Heart
                    (
                        size_of_hearth,
                        left,
                        top,
                        (i < num_life) ? 0x000000ffUL : 0x00000000UL
                    );
            };
        };

        { __qm_comment__('Draw_Time')
            Draw_Time
                (
                    p_MineSweeper3D,
                    W,
                    H,
                    (W * 1) / 4
                );
        };

        { __qm_comment__('Draw_Game_State')
            this->Draw_GameState
                (
                    p_MineSweeper3D,
                    W,
                    H,
                    (W * 2) / 4
                );
        };

        { __qm_comment__('Draw_NumMines')
            this->Draw_NumMines
                (
                    p_MineSweeper3D,
                    W,
                    H,
                    (W * 3) / 4
                );
        };

    };
};

/*===========================================================*/

QM_VOID T_MinSwp_Displayer_GDI::Draw_Menu
    (
              T_MineSweeper3D         * p_MineSweeper3D,
        const QM_SLONG                  W,
        const QM_SLONG                  H
    )
{
    /*
    this->p_RenderTarget->Canvas->Brush->Bitmap = NULL;
    this->p_RenderTarget->Canvas->Brush->Color  = (TColor)0x00202020UL;
    this->p_RenderTarget->Canvas->FillRect(Rect(0, 0, W, H));
    */

    QM_FLOAT w = (QM_FLOAT)W;
    QM_FLOAT h = (QM_FLOAT)H;
    this->p_RenderTarget->Canvas->Pen->Style   = psSolid;
    T_MinSwp_MenuItem * p_menu_item = p_MineSweeper3D->GetCurrentMenuItem();
    if (p_menu_item == QM_NULL)
        return ;


    this->Draw_Game
        (
            p_MineSweeper3D,
            /*
            p_MineSweeper3D->GetGame(),
            p_MineSweeper3D->GetLife(),
            p_MineSweeper3D->GetSelectedPolyIndex(),
            p_MineSweeper3D->GetMouse_L_Down(),
            p_MineSweeper3D->GetMouse_R_Down(),
            p_MineSweeper3D->GetAdjacentType(),
            */
            W,
            H
        );

    static Graphics::TBitmap * p_TrancparentBitm = new Graphics::TBitmap();
    p_TrancparentBitm->Canvas->Brush->Color = (TColor)0x00000000UL;
    p_TrancparentBitm->SetSize(W, H);
    this->p_RenderTarget->Canvas->Draw(0, 0, p_TrancparentBitm, 32);

    QM_SLONG childs_count = p_menu_item->Childs_GetCount();

    this->p_RenderTarget->Canvas->Brush->Style = bsSolid;
    this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x00404040UL;
    this->p_RenderTarget->Canvas->Font->Name = L"Courier New";
    this->p_RenderTarget->Canvas->Font->Color = clWhite;

    for (int i = 0; i < childs_count; i++)
    {
        T_MinSwp_MenuItem * p_child = p_menu_item->Childs_GetByIndex(i);

        QM_FRECT fr;
        p_menu_item->Childs_GetRect
            (
                i,
                &fr
            );
        fr.L *= w;
        fr.T *= h;
        fr.R *= w;
        fr.B *= h;
        QM_FLOAT h = fr.B - fr.T;
        TRect r_full =
            Rect
                (
                    (int)(fr.L - w * 0.02f),
                    (int)fr.T,
                    (int)(fr.R + w * 0.02f),
                    (int)fr.B
                );
        fr.T += h * 0.15f;
        fr.B -= h * 0.15f;
        const TRect r =
            Rect
                (
                    (int)fr.L,
                    (int)fr.T,
                    (int)fr.R,
                    (int)fr.B
                );

        this->p_RenderTarget->Canvas->Brush->Style = bsSolid;
        this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x00101010UL;
        this->p_RenderTarget->Canvas->FillRect(r_full);

        this->p_RenderTarget->Canvas->Brush->Color =
            (i) ?
                (
                    (
                        (     p_menu_item->ItemType          == MINSWP_MENUITEM_T_RADIOGROUP) &&
                        ((int)p_menu_item->Radio->ItemIndex  == i)
                    ) ?
                    (TColor)0x00606060UL : __qm_comment__('Selected Item')
                    (TColor)0x00303030UL   __qm_comment__('Default')
                ) :
                (TColor)0x00404020UL; __qm_comment__('BackButton')

        this->p_RenderTarget->Canvas->FillRect(r);

        if (p_child)
        {
            if (p_child->ItemType == MINSWP_MENUITEM_T_SLIDER)
            {
                const QM_FLOAT slider_v01 = p_child->Slider->GetVal01();

                TRect r_slider = r;
                r_slider.right = (QM_SLONG)
                    (
                        (((QM_FLOAT)(r_slider.right - r_slider.left)) * slider_v01) +
                        r_slider.left
                    );
                this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x001d328bUL;
                this->p_RenderTarget->Canvas->FillRect(r_slider);
            };

            this->p_RenderTarget->Canvas->Font->Height = (int)(h * 0.5f);
            TRect r_txt = r;
            r_txt.Top = (int)(fr.T + h * 0.125f);

            {
                UnicodeString s;
                if (i)
                    s = p_child->Caption.c_str();
                else
                {
                    T_MinSwp_MenuItem * p_node = p_menu_item->Nodes->p_Parent;
                    while (p_node)
                    {
                        s = L"/" + UnicodeString(p_node->Caption.c_str()) + s;
                        p_node = p_node->Nodes->p_Parent;
                    };
                    s = L"Play_Game" + s;
                };
                this->p_RenderTarget->Canvas->Brush->Style = bsClear;
                this->p_RenderTarget->Canvas->Font->Color = clWhite;
                this->p_RenderTarget->Canvas->TextRect
                    (
                        r_txt,
                        s,
                        (
                            TTextFormat() <<
                            TTextFormats::tfCenter <<
                            TTextFormats::tfVerticalCenter
                        )
                    );
            };

            if (p_child->ItemType == MINSWP_MENUITEM_T_SLIDER)
            {
                UnicodeString s = IntToStr((int)p_child->Slider->GetVal()) + L"  ";
                this->p_RenderTarget->Canvas->TextRect
                    (
                        r_txt,
                        s,
                        (
                            TTextFormat() <<
                            TTextFormats::tfRight <<
                            TTextFormats::tfVerticalCenter
                        )
                    );
            }
            else
            if (p_child->ItemType == MINSWP_MENUITEM_T_CHECK)
            {
                const QM_LBOOL check_en = p_child->CheckBox->Enabled;
                const QM_SLONG r_h = r.Bottom - r.Top;
                const QM_SLONG r_h_4 = r_h / 4;
                const QM_SLONG circle_r = r_h / 6;
                const QM_SLONG r_btn_w = r_h;
                TRect       r_btn;

                r_btn           = r;
                r_btn.Top      += r_h_4;
                r_btn.Bottom   -= r_h_4;
                r_btn.Left      = r.Right - r_btn_w - r_h_4 - (r_h + r_h / 2);
                r_btn.Right     = r_btn.Left + r_btn_w;

                this->p_RenderTarget->Canvas->Brush->Color =
                    (TColor)
                        (
                            (check_en) ?
                                0x00808080UL :
                                0x00101010UL
                        );
                this->p_RenderTarget->Canvas->Pen->Style = psSolid;
                this->p_RenderTarget->Canvas->Pen->Width = 3;
                this->p_RenderTarget->Canvas->Pen->Color = (TColor)0x00808080UL;
                this->p_RenderTarget->Canvas->RoundRect
                    (
                        r_btn,
                        r_h / 2,
                        r_h / 2
                    );

                this->p_RenderTarget->Canvas->Pen->Style   = psClear;
                this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x00f0f0f0UL;
                {
                    const QM_SLONG circle_cx = r_btn.Left + r_h_4 + ((check_en) ? (r_h / 2) : 0);
                    const QM_SLONG circle_cy = (r_btn.Top + r_btn.Bottom) / 2;
                    this->p_RenderTarget->Canvas->Ellipse
                        (
                            circle_cx - circle_r,
                            circle_cy - circle_r,
                            circle_cx + circle_r,
                            circle_cy + circle_r
                        );
                };

                this->p_RenderTarget->Canvas->Brush->Style = bsClear;
                UnicodeString s = (check_en) ? L"On  " : L"Off ";
                this->p_RenderTarget->Canvas->TextRect
                    (
                        r_txt,
                        s,
                        (
                            TTextFormat() <<
                            TTextFormats::tfRight <<
                            TTextFormats::tfVerticalCenter
                        )
                    );
            }
            else
            if (i == 0)
            {
                r_txt.Top += (int)(h * 0.125f * 2.0f / 3.0f);
                UnicodeString s = L"    Back to";
                this->p_RenderTarget->Canvas->Font->Color = (TColor)0x00808080UL;
                this->p_RenderTarget->Canvas->Font->Size =
               (this->p_RenderTarget->Canvas->Font->Size * 2) / 3;
                this->p_RenderTarget->Canvas->TextRect
                    (
                        r_txt,
                        s,
                        (
                            TTextFormat() <<
                            TTextFormats::tfLeft <<
                            TTextFormats::tfVerticalCenter
                        )
                    );
            };
        };

    };
};

/*===========================================================*/

QM_VOID T_MinSwp_Displayer_GDI::Display
    (
              T_MineSweeper3D     * p_MineSweeper3D
    )
{
    RECT r;
    ::GetClientRect(this__p_AppWindow->GetWindowHandle(), &r);
    const QM_SLONG W = r.right  - r.left;
    const QM_SLONG H = r.bottom - r.top;
    if (this->p_RenderTarget == NULL)
    {
        this->p_RenderTarget = new Graphics::TBitmap();
    };
    this->p_RenderTarget->SetSize(W, H);
    if (p_MineSweeper3D == QM_NULL)
    {
        this->p_RenderTarget->Canvas->Brush->Color = (TColor)0x00808080L;
        this->p_RenderTarget->Canvas->Pen->Style   = psClear;
        this->p_RenderTarget->Canvas->FillRect(Rect(0, 0, W, H));
        this__Present();
        return ;
    };

    switch (p_MineSweeper3D->GetStageState())
    {
        case MINSWP_STAGESTATE_GAME:
            {
                this->Draw_Game
                    (
                        p_MineSweeper3D,
                        /*
                        p_MineSweeper3D->GetGame(),
                        p_MineSweeper3D->GetLife(),
                        p_MineSweeper3D->GetSelectedPolyIndex(),
                        p_MineSweeper3D->GetMouse_L_Down(),
                        p_MineSweeper3D->GetMouse_R_Down(),
                        p_MineSweeper3D->GetAdjacentType(),
                        */
                        W,
                        H
                    );
            }; break;
        case MINSWP_STAGESTATE_MENU:
            {
                this->Draw_Menu
                    (
                        p_MineSweeper3D,
                        W,
                        H
                    );
            }; break;
        default:
            ;
    };

    this__Present();
};













