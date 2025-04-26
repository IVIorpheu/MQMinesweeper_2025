/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_life.h         (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_LIFE_H__DEFINED
#define __MINSWP_LIFE_H__DEFINED

/*===========================================================*/

#include "minswp_base.h"

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

MINSWP_DECL_HANDLE( HMINSWP_LIFE    )

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

HMINSWP_LIFE MinSwp_Life_Create
    (
        const QM_SLONG DefaultLife
    );

/*===========================================================*/

QM_VOID MinSwp_Life_Release
    (
              HMINSWP_LIFE          h_Life
    );

/*===========================================================*/

QM_SLONG MinSwp_Life_Get
    (
              HMINSWP_LIFE          h_Life
    );

/*===========================================================*/

QM_SLONG MinSwp_Life_Give
    (
              HMINSWP_LIFE          h_Life,
        const QM_SLONG              Count
    );

/*===========================================================*/

QM_SLONG MinSwp_Life_Take
    (
              HMINSWP_LIFE          h_Life,
        const QM_SLONG              Count
    );

/*===========================================================*/

QM_SLONG MinSwp_Life_Reset
    (
              HMINSWP_LIFE          h_Life
    );

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/




/*===========================================================*/

#endif /* __MINSWP_LIFE_H__DEFINED */








