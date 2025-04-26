/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_life.c         (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_life.h"

#include <malloc.h>

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

typedef struct _MINSWP_LIFE
{
    HMINSWP_LIFE__      Handle[1];

    QM_SLONG            Default;
    QM_SLONG            Life;
} MINSWP_LIFE;

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

HMINSWP_LIFE MinSwp_Life_Create
    (
        const QM_SLONG DefaultLife
    )
{
    MINSWP_LIFE * p_life =
   (MINSWP_LIFE *)calloc(1, sizeof(
    MINSWP_LIFE));

    p_life->Default =
    p_life->Life    = DefaultLife;

    return p_life->Handle;
};

/*===========================================================*/

QM_VOID MinSwp_Life_Release
    (
              HMINSWP_LIFE          h_Life
    )
{
    if (h_Life)
    {
        MINSWP_LIFE * p_life =
       (MINSWP_LIFE *)h_Life;

        p_life->Life = 0;
        free(p_life);
    };
};

/*===========================================================*/

QM_SLONG MinSwp_Life_Get
    (
              HMINSWP_LIFE          h_Life
    )
{
    MINSWP_LIFE * p_life =
   (MINSWP_LIFE *)h_Life;

   return p_life->Life;
};

/*===========================================================*/

QM_SLONG MinSwp_Life_Give
    (
              HMINSWP_LIFE          h_Life,
        const QM_SLONG              Count
    )
{
    MINSWP_LIFE * p_life =
   (MINSWP_LIFE *)h_Life;

    p_life->Life += Count;
    return MinSwp_Life_Get(h_Life);
};

/*===========================================================*/

QM_SLONG MinSwp_Life_Take
    (
              HMINSWP_LIFE          h_Life,
        const QM_SLONG              Count
    )
{
    MINSWP_LIFE * p_life =
   (MINSWP_LIFE *)h_Life;

    if (Count > p_life->Life)
        return -1;
    p_life->Life -= Count;
    return MinSwp_Life_Get(h_Life);
};

/*===========================================================*/

QM_SLONG MinSwp_Life_Reset
    (
              HMINSWP_LIFE          h_Life
    )
{
    MINSWP_LIFE * p_life =
   (MINSWP_LIFE *)h_Life;

    p_life->Life = p_life->Default;
    return p_life->Life;
};

/*===========================================================*/


























