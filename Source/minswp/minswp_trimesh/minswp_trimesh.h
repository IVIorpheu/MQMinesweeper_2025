/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_trimesh.h      (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_TRIMESH_H__DEFINED
#define __MINSWP_TRIMESH_H__DEFINED

/*===========================================================*/

#include "minswp_base.h"

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

MINSWP_DECL_HANDLE( HMINSWP_TRIMESH )

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

HMINSWP_TRIMESH         MinSwp_TriMesh_Create
    (
        QM_VOID
    );

/*===========================================================*/

QM_VOID                 MinSwp_TriMesh_Release
    (
              HMINSWP_TRIMESH           h_TriMesh
    );

/*===========================================================*/

MINSWP_VEC3D *          MinSwp_TriMesh_Verts_Get
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  I
    );

/*===========================================================*/

MINSWP_TRIINDICES *     MinSwp_TriMesh_Faces_Get
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  I
    );

/*===========================================================*/

QM_ULONG                MinSwp_TriMesh_Verts_Size
    (
              HMINSWP_TRIMESH           h_TriMesh
    );

/*===========================================================*/

QM_ULONG                MinSwp_TriMesh_Faces_Size
    (
              HMINSWP_TRIMESH           h_TriMesh
    );

/*===========================================================*/

QM_VOID                 MinSwp_TriMesh_Verts_Resize
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  NewSize
    );

/*===========================================================*/

QM_VOID                 MinSwp_TriMesh_Faces_Resize
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  NewSize
    );

/*===========================================================*/

QM_LBOOL                    MinSwp_TriMesh_IntersectTrianglesByRay
    (
        const HMINSWP_TRIMESH       h_TriMesh,
        const MINSWP_RAY3D        * p_Ray,
              QM_FLOAT            * p_Out_Distance
    );

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/

#endif /* __MINSWP_TRIMESH_H__DEFINED */










