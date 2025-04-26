/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_trimesh.c      (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_trimesh.h"

#include <malloc.h>
#include <memory.h>

#include "fmath3d.h"

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

typedef struct _MINSWP_TRIMESH
{
    HMINSWP_TRIMESH__       Handle[1];

    MINSWP_VEC3D          * p_Verts;
    MINSWP_TRIINDICES     * p_Faces;

    QM_ULONG                NumVerts;
    QM_ULONG                NumFaces;
} MINSWP_TRIMESH;

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

HMINSWP_TRIMESH             MinSwp_TriMesh_Create
    (
        QM_VOID
    )
{
    MINSWP_TRIMESH * p_trimesh =
   (MINSWP_TRIMESH *)calloc
        (
            1,
            sizeof(MINSWP_TRIMESH)
        );

    p_trimesh->NumVerts    = 0;
    p_trimesh->NumFaces    = 0;
    p_trimesh->p_Verts     = QM_NULL;
    p_trimesh->p_Faces     = QM_NULL;

    return p_trimesh->Handle;
};

/*===========================================================*/

QM_VOID                     MinSwp_TriMesh_Release
    (
              HMINSWP_TRIMESH           h_TriMesh
    )
{
    if (h_TriMesh)
    {
        MINSWP_TRIMESH * p_trimesh =
       (MINSWP_TRIMESH *)h_TriMesh;

        if (        p_trimesh->p_Verts)
        {   free(   p_trimesh->p_Verts);
                    p_trimesh->p_Verts = QM_NULL;
        };
        if (        p_trimesh->p_Faces)
        {   free(   p_trimesh->p_Faces);
                    p_trimesh->p_Faces = QM_NULL;
        };
        p_trimesh->NumVerts = 0;
        p_trimesh->NumFaces = 0;

        free(p_trimesh);
    };
};

/*===========================================================*/

MINSWP_VEC3D *              MinSwp_TriMesh_Verts_Get
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  I
    )
{
    MINSWP_TRIMESH * p_trimesh =
   (MINSWP_TRIMESH *)h_TriMesh;

    return &p_trimesh->p_Verts[I];
};

/*===========================================================*/

MINSWP_TRIINDICES *         MinSwp_TriMesh_Faces_Get
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  I
    )
{
    MINSWP_TRIMESH * p_trimesh =
   (MINSWP_TRIMESH *)h_TriMesh;

    return &p_trimesh->p_Faces[I];
};

/*===========================================================*/

QM_ULONG                    MinSwp_TriMesh_Verts_Size
    (
              HMINSWP_TRIMESH           h_TriMesh
    )
{
    MINSWP_TRIMESH * p_trimesh =
   (MINSWP_TRIMESH *)h_TriMesh;

    return p_trimesh->NumVerts;
};

/*===========================================================*/

QM_ULONG                    MinSwp_TriMesh_Faces_Size
    (
              HMINSWP_TRIMESH           h_TriMesh
    )
{
    MINSWP_TRIMESH * p_trimesh =
   (MINSWP_TRIMESH *)h_TriMesh;

    return p_trimesh->NumFaces;
};

/*===========================================================*/

QM_VOID                     MinSwp_TriMesh_Verts_Resize
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  NewSize
    )
{
    MINSWP_TRIMESH * p_trimesh =
   (MINSWP_TRIMESH *)h_TriMesh;

    const QM_ULONG old_sz = p_trimesh->NumVerts;

    MinSwp__ResizePtr__MACRO(p_trimesh->p_Verts, old_sz, NewSize)

    if (old_sz < NewSize)
    {
        memset
            (
                &p_trimesh->p_Verts[old_sz],
                0,
                (NewSize - old_sz) * sizeof(p_trimesh->p_Verts[0])
            );
    };

    p_trimesh->NumVerts = NewSize;
};

/*===========================================================*/

QM_VOID                     MinSwp_TriMesh_Faces_Resize
    (
              HMINSWP_TRIMESH           h_TriMesh,
        const QM_ULONG                  NewSize
    )
{
    MINSWP_TRIMESH * p_trimesh =
   (MINSWP_TRIMESH *)h_TriMesh;

    const QM_ULONG old_sz = p_trimesh->NumFaces;

    MinSwp__ResizePtr__MACRO(p_trimesh->p_Faces, old_sz, NewSize)

    if (old_sz < NewSize)
    {
        memset
            (
                &p_trimesh->p_Faces[old_sz],
                0,
                (NewSize - old_sz) * sizeof(p_trimesh->p_Faces[0])
            );
    };

    p_trimesh->NumFaces = NewSize;
};

/*===========================================================*/

QM_LBOOL                    MinSwp_TriMesh_IntersectTrianglesByRay
    (
        const HMINSWP_TRIMESH       h_TriMesh,
        const MINSWP_RAY3D        * p_Ray,
              QM_FLOAT            * p_Out_Distance
    )
{
    QM_ULONG i;
    const QM_ULONG tri_count = MinSwp_TriMesh_Faces_Size(h_TriMesh);
    for (i = 0; i < tri_count; i++)
    {
        const MINSWP_TRIINDICES * p_face_i = MinSwp_TriMesh_Faces_Get(h_TriMesh, i);
        const QM_FLOAT * p1 = MinSwp_TriMesh_Verts_Get(h_TriMesh, p_face_i->i1)->f;
        const QM_FLOAT * p2 = MinSwp_TriMesh_Verts_Get(h_TriMesh, p_face_i->i2)->f;
        const QM_FLOAT * p3 = MinSwp_TriMesh_Verts_Get(h_TriMesh, p_face_i->i3)->f;

        if (
                FM3DRay3IntersectTrianglePD_f32
                    (
                        p_Ray->Pos.f,
                        p_Ray->Dir.f,
                        p1,
                        p2,
                        p3
                    )
            )
        {
            if (p_Out_Distance)
            {
                QM_FLOAT vAB[3];
                QM_FLOAT vAC[3];
                QM_FLOAT vN[3];
                FM3DVec3Normalize_f32
                    (
                        vN,
                        FM3DVec3Cross_f32
                            (
                                vN,
                                FM3DVec3Subtract_f32(vAB, p2, p1),
                                FM3DVec3Subtract_f32(vAC, p3, p1)
                            )
                    );

                *p_Out_Distance =
                    FM3DRay3IntersectPlaneDepth_f32
                        (
                            NULL,
                            p_Ray->Pos.f,
                            p_Ray->Dir.f,
                            p1,
                            vN,
                            FM3D_FALSE
                        );
            };
            return QM_TRUE;
        };
    };

    return QM_FALSE;
};

/*===========================================================*/











