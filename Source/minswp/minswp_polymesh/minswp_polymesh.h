/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_polymesh.h     (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_POLYMESH_H__DEFINED
#define __MINSWP_POLYMESH_H__DEFINED

/*===========================================================*/

#include "minswp_base.h"
#include "minswp_indices.h"

/*===========================================================*/



/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

MINSWP_DECL_HANDLE( HMINSWP_POLYMESH    )

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/



/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

HMINSWP_POLYMESH            MinSwp_PolyMesh_Create
    (
        QM_VOID
    );

/*===========================================================*/

QM_VOID                     MinSwp_PolyMesh_Release
    (
              HMINSWP_POLYMESH          h_BasicMesh
    );

/*===========================================================*/

MINSWP_VEC3D *              MinSwp_PolyMesh_Verts_Get
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  I
    );

/*===========================================================*/

HMINSWP_INDICES             MinSwp_PolyMesh_Polys_Get
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  I
    );

/*===========================================================*/

QM_VOID                     MinSwp_PolyMesh_Verts_Resize
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  NewSize
    );

/*===========================================================*/

QM_VOID                     MinSwp_PolyMesh_Polys_ReSize
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  NewSize
    );

/*===========================================================*/

QM_ULONG                    MinSwp_PolyMesh_Verts_Size
    (
        const HMINSWP_POLYMESH          h_PolyMesh
    );

/*===========================================================*/

QM_ULONG                    MinSwp_PolyMesh_Polys_Size
    (
        const HMINSWP_POLYMESH          h_PolyMesh
    );

/*===========================================================*/

HMINSWP_POLYMESH            MinSwp_PolyMesh_Create_Plane
    (
        const QM_ULONG                  WidthSegs,
        const QM_ULONG                  HeightSegs
    );

/*===========================================================*/

HMINSWP_POLYMESH            MinSwp_PolyMesh_Create_FromFileInMemory
    (
        const QM_VOID                 * p_Memory
    );

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/





/*===========================================================*/

#endif /* __MINSWP_POLYMESH_H__DEFINED */














