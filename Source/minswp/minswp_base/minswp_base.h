/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_base.h         (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_BASE_H__DEFINED
#define __MINSWP_BASE_H__DEFINED

/*===========================================================*/

#include "qm_basetype.h"
#include "minswp_shwmsg.h"

/*===========================================================*/

typedef enum _MINSWP_ADJACENT_TYPE
{
    MINSWP_ADJT_BYVERTS,
    MINSWP_ADJT_BYEDGES
} MINSWP_ADJACENT_TYPE;

/*===========================================================*/

typedef enum _MINSWP_GAME_STATE
{
        MINSWP_GS_VOID              = 0L,
        MINSWP_GS_STARTED           = 0x8000L,
        MINSWP_GS_IN_PLAY           = 0x8001L,
        MINSWP_GS_IN_STOP           = 0x8002L,
#define MINSWP_GS_ENDED               0x18000L
        MINSWP_GS_WIN               = 0x18004L,
        MINSWP_GS_LOSS              = 0x18008L,

        MINSWP_GS____FORCE_32BIT    = 0x7fffffffL
} MINSWP_GAME_STATE;

/*===========================================================*/

typedef enum _MINSWP_DRAW_TYPE
{
    #define MINSWP_DT_X_MINE_LOCAL_MACRO(i) MINSWP_DT_##i##_MINE = i
    #define MINSWP_DT_X10_MINE_LOCAL_MACRO(                   \
                                        i01,                  \
                                        i02,                  \
                                        i03,                  \
                                        i04,                  \
                                        i05,                  \
                                        i06,                  \
                                        i07,                  \
                                        i08,                  \
                                        i09,                  \
                                        i10)                  \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i01 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i02 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i03 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i04 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i05 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i06 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i07 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i08 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i09 ),                \
        MINSWP_DT_X_MINE_LOCAL_MACRO(   i10 )


    MINSWP_DT_FREE = 0,
    MINSWP_DT_X10_MINE_LOCAL_MACRO( 1,  2,  3,  4,  5,  6,  7,  8,  9,  10),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(11, 12, 13, 14, 15, 16, 17, 18, 19,  20),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(21, 22, 23, 24, 25, 26, 27, 28, 29,  30),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(31, 32, 33, 34, 35, 36, 37, 38, 39,  40),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(41, 42, 43, 44, 45, 46, 47, 48, 49,  50),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(51, 52, 53, 54, 55, 56, 57, 58, 59,  60),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(61, 62, 63, 64, 65, 66, 67, 68, 69,  70),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(71, 72, 73, 74, 75, 76, 77, 78, 79,  80),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(81, 82, 83, 84, 85, 86, 87, 88, 89,  90),
    MINSWP_DT_X10_MINE_LOCAL_MACRO(91, 92, 93, 94, 95, 96, 97, 98, 99, 100),
    MINSWP_DT_HEART = -4L,
    MINSWP_DT_BOOM  = -3L,
    MINSWP_DT_FLAG  = -2L,
    MINSWP_DT_START = -1L,
    MINSWP_DT____FORCE_32BIT = 0x7fffffff
} MINSWP_DRAW_TYPE;

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

typedef struct _MINSWP_COLOR
{
    union
    {
        QM_FLOAT    f[1];
        QM_FLOAT    r;
    };
        QM_FLOAT    g;
        QM_FLOAT    b;
        QM_FLOAT    a;
} MINSWP_COLOR;

typedef struct _MINSWP_VEC3D
{
    union
    {
        struct
        {
            QM_FLOAT    x,
                        y,
                        z;
        };
        QM_FLOAT f[3];
    };
} MINSWP_VEC3D;

/*===========================================================*/

typedef struct _MINSWP_TRIINDICES
{
    union
    {
        struct
        {
            QM_SSHRT    i1,
                        i2,
                        i3;
        };
        QM_SSHRT i[3];
    };
} MINSWP_TRIINDICES;

/*===========================================================*/

typedef struct _MINSWP_RAY3D
{
    union
    {
        struct
        {
            MINSWP_VEC3D    Pos,
                            Dir;
        };
        QM_FLOAT f[2][3];
    };
} MINSWP_RAY3D;

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

MINSWP_VEC3D * MinSwp_Vec3D_SetVal
    (
              MINSWP_VEC3D        * p_Out,
        const QM_FLOAT              x,
        const QM_FLOAT              y,
        const QM_FLOAT              z
    );

/*===========================================================*/

MINSWP_TRIINDICES * MinSwp_TriIndices_SetVal
    (
              MINSWP_TRIINDICES   * p_Out,
        const QM_SSHRT              i1,
        const QM_SSHRT              i2,
        const QM_SSHRT              i3
    );

/*===========================================================*/

MINSWP_RAY3D * MinSwp_Ray3D_SetVal
    (
              MINSWP_RAY3D        * p_Out,
        const QM_FLOAT              px,
        const QM_FLOAT              py,
        const QM_FLOAT              pz,
        const QM_FLOAT              dx,
        const QM_FLOAT              dy,
        const QM_FLOAT              dz
    );

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/

#define MinSwp__ResizePtr__MACRO(                             \
    p_values, old_size, new_size)                             \
    if (p_values)                                             \
    {                                                         \
        if (old_size < new_size)                              \
        {                                                     \
            *((void **)&p_values) =                           \
                realloc                                       \
                    (                                         \
                        p_values,                             \
                        new_size *                            \
                        sizeof(p_values[0])                   \
                    );                                        \
        };                                                    \
    }                                                         \
    else                                                      \
    {                                                         \
        *((void **)&p_values) =                               \
            calloc                                            \
                (                                             \
                    new_size,                                 \
                    sizeof(p_values[0])                       \
                );                                            \
    };


/*===========================================================*/

#define MINSWP_DECL_HANDLE(h)                                 \
typedef struct _##h##__                                       \
{                                                             \
    QM_ULONG        __R_e_s_e_r_v_e_d__;                      \
} h##__, * h;

/*===========================================================*/

#endif /* __MINSWP_BASE_H__DEFINED */











