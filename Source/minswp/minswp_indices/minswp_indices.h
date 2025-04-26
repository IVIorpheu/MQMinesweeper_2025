/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_indices.h      (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_INDICES_H__DEFINED
#define __MINSWP_INDICES_H__DEFINED

/*===========================================================*/

#include "minswp_base.h"

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

MINSWP_DECL_HANDLE( HMINSWP_INDICES  )

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

HMINSWP_INDICES MinSwp_Indices_Create
    (
        QM_VOID
    );

/*===========================================================*/

QM_VOID     MinSwp_Indices_Release
    (
              HMINSWP_INDICES   h_Indices
    );

/*===========================================================*/

HMINSWP_INDICES   MinSwp_Indices_CloneCopy
    (
        const HMINSWP_INDICES   h_Indices
    );

/*===========================================================*/

QM_LBOOL MinSwp_Indices_Copy
    (
              HMINSWP_INDICES   h_Dst_Indices,
        const HMINSWP_INDICES   h_Src_Indices
    );

/*===========================================================*/

QM_VOID MinSwp_Indices_Clear
    (
              HMINSWP_INDICES   h_Indices
    );

/*===========================================================*/

QM_SLONG MinSwp_Indices_GetCount
    (
        const HMINSWP_INDICES   h_Indices
    );

/*===========================================================*/

QM_SLONG  MinSwp_Indices_GetIndex
    (
        const HMINSWP_INDICES   h_Indices,
        const QM_SLONG          I
    );

/*===========================================================*/

QM_SLONG    MinSwp_Indices_AddIndex
    (
              HMINSWP_INDICES   h_Indices,
        const QM_SLONG          Index
    );

/*===========================================================*/

QM_LBOOL    MinSwp_Indices_IsMember
    (
        const HMINSWP_INDICES   h_Indices,
        const QM_SLONG          Index
    );

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/

#endif /* __MINSWP_INDICES_H__DEFINED */










