/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_polymesh.c     (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_polymesh.h"

#include <malloc.h>
#include <memory.h>

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

typedef struct _MINSWP_POLYMESH
{
    HMINSWP_POLYMESH__      Handle[1];

    MINSWP_VEC3D          * p_Verts;
    HMINSWP_INDICES       * p_Polys;

    QM_ULONG                NumVerts;
    QM_ULONG                NumPolys;
} MINSWP_POLYMESH;

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

HMINSWP_POLYMESH            MinSwp_PolyMesh_Create
    (
        QM_VOID
    )
{
    MINSWP_POLYMESH * p_polymesh =
   (MINSWP_POLYMESH *)calloc
        (
            1,
            sizeof(MINSWP_POLYMESH)
        );

    p_polymesh->NumVerts    = 0;
    p_polymesh->NumPolys    = 0;
    p_polymesh->p_Verts     = QM_NULL;
    p_polymesh->p_Polys     = QM_NULL;

    return p_polymesh->Handle;
};

/*===========================================================*/

QM_VOID                     MinSwp_PolyMesh_Release
    (
              HMINSWP_POLYMESH          h_PolyMesh
    )
{
    if (h_PolyMesh)
    {
        MINSWP_POLYMESH * p_polymesh =
       (MINSWP_POLYMESH *)h_PolyMesh;

        if (        p_polymesh->p_Verts)
        {   free(   p_polymesh->p_Verts);
                    p_polymesh->p_Verts = QM_NULL;
        };
        if (        p_polymesh->p_Polys)
        {
            QM_ULONG i;
            const QM_ULONG count = p_polymesh->NumPolys;

            for (i = 0; i < count; i++)
            {
                MinSwp_Indices_Release
                    (
                        p_polymesh->p_Polys[i]
                    );  p_polymesh->p_Polys[i] = QM_NULL;
            };

            free(   p_polymesh->p_Polys);
                    p_polymesh->p_Polys = QM_NULL;
        };
        p_polymesh->NumVerts = 0;
        p_polymesh->NumPolys = 0;

        free(p_polymesh);
    };
};

/*===========================================================*/

MINSWP_VEC3D *              MinSwp_PolyMesh_Verts_Get
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  I
    )
{
    MINSWP_POLYMESH * p_polymesh =
   (MINSWP_POLYMESH *)h_PolyMesh;

    return &p_polymesh->p_Verts[I];
};

/*===========================================================*/

HMINSWP_INDICES             MinSwp_PolyMesh_Polys_Get
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  I
    )
{
    MINSWP_POLYMESH * p_polymesh =
   (MINSWP_POLYMESH *)h_PolyMesh;

    return p_polymesh->p_Polys[I];
};

/*===========================================================*/

QM_VOID                     MinSwp_PolyMesh_Verts_Resize
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  NewSize
    )
{
    MINSWP_POLYMESH * p_polymesh =
   (MINSWP_POLYMESH *)h_PolyMesh;

    const QM_ULONG old_sz = p_polymesh->NumVerts;

    MinSwp__ResizePtr__MACRO(p_polymesh->p_Verts, old_sz, NewSize)

    if (old_sz < NewSize)
    {
        memset
            (
                &p_polymesh->p_Verts[old_sz],
                0,
                (NewSize - old_sz) * sizeof(p_polymesh->p_Verts[0])
            );
    };

    p_polymesh->NumVerts = NewSize;
};

/*===========================================================*/

QM_VOID                     MinSwp_PolyMesh_Polys_ReSize
    (
              HMINSWP_POLYMESH          h_PolyMesh,
        const QM_ULONG                  NewSize
    )
{
    MINSWP_POLYMESH * p_polymesh =
   (MINSWP_POLYMESH *)h_PolyMesh;

    const QM_ULONG old_sz = p_polymesh->NumPolys;

    if (old_sz > NewSize)
    {
        QM_ULONG i;
        for (i = NewSize; i < old_sz; i++)
        {
            MinSwp_Indices_Release(p_polymesh->p_Polys[i]);
        };
    };
    /*=======================================================*/

    MinSwp__ResizePtr__MACRO(p_polymesh->p_Polys, old_sz, NewSize)

    /*=======================================================*/
    if (old_sz < NewSize)
    {
        QM_ULONG i;
        for (i = old_sz; i < NewSize; i++)
        {
            p_polymesh->p_Polys[i] = MinSwp_Indices_Create();
        };
    };

    p_polymesh->NumPolys = NewSize;
};

/*===========================================================*/

QM_ULONG                    MinSwp_PolyMesh_Verts_Size
    (
        const HMINSWP_POLYMESH          h_PolyMesh
    )
{
    return ((MINSWP_POLYMESH *)h_PolyMesh)->NumVerts;
};

/*===========================================================*/

QM_ULONG                    MinSwp_PolyMesh_Polys_Size
    (
        const HMINSWP_POLYMESH          h_PolyMesh
    )
{
    return ((MINSWP_POLYMESH *)h_PolyMesh)->NumPolys;
};

/*===========================================================*/

HMINSWP_POLYMESH            MinSwp_PolyMesh_Create_Plane
    (
        const QM_ULONG      WidthSegs,
        const QM_ULONG      HeightSegs
    )
{
    HMINSWP_POLYMESH h_pm = MinSwp_PolyMesh_Create();
    MinSwp_PolyMesh_Verts_Resize
        (
            h_pm,
            (
                (WidthSegs  + 1) *
                (HeightSegs + 1)
            )
        );

    {
        const QM_FLOAT inv_ws = 1.0f / (QM_FLOAT)WidthSegs;
        const QM_FLOAT inv_hs = 1.0f / (QM_FLOAT)HeightSegs;

        QM_ULONG j, k;

        k = 0;
        for (j = 0; j <= HeightSegs; j++)
        {
            QM_ULONG i;
            for (i = 0; i <= WidthSegs; i++)
            {
                MinSwp_Vec3D_SetVal
                    (
                        MinSwp_PolyMesh_Verts_Get(h_pm, k),
                        (QM_FLOAT)i * inv_ws,
                        (QM_FLOAT)j * inv_hs,
                        0.0f
                    );
                k++;
            };
        };
    };

    /*
    0   1   2   3   4   5
    6   7   8   9   10  11
    12  13  14  15  16  17
    18  19  20  21  22  23
    24  25  26  27  28  29


    [0, 6,  7,  1] [1, 7,  8,  2] [2,  8,  9, 3] [3,  9, 10,  4] [4,  10, 11,  5]
    [6, 12, 13, 7] [7, 13, 14, 8] [8, 14, 15, 9] [9, 15, 16, 10] [10, 16, 17, 11]
    */

    MinSwp_PolyMesh_Polys_ReSize
        (
            h_pm,
            WidthSegs * HeightSegs
        );
    {
        QM_SLONG poly[4];
        poly[0] = 0;
        poly[1] = (QM_SLONG)(WidthSegs + 1);
        poly[2] = (QM_SLONG)(WidthSegs + 2);
        poly[3] = 1;

        {
            QM_ULONG j;
            for (j = 0; j < HeightSegs; j++)
            {
                QM_ULONG i;
                for (i = 0; i < WidthSegs; i++)
                {
                    QM_ULONG k;
                    for (k = 0; k < 4; k++)
                    {
                        MinSwp_Indices_AddIndex
                            (
                                MinSwp_PolyMesh_Polys_Get
                                    (
                                        h_pm,
                                        WidthSegs * j + i
                                    ),
                                poly[k]
                            );
                        poly[k]++;
                    };
                };

                {
                    QM_ULONG m;
                    for (m = 0; m < 4; m++)
                        poly[m]++;
                };
            };
        };
    };
    return h_pm;
};

/*===========================================================*/

#define MINSWP_POLYMESH_FILE_ID_0       0x0F24606B1BAB4E67ULL
#define MINSWP_POLYMESH_FILE_ID_1       0xB8E1906CD3053E02ULL
#define MINSWP_POLYMESH_FILE_VERSION          139511271405ULL

HMINSWP_POLYMESH MinSwp_PolyMesh_Create_FromFileInMemory
    (
        const QM_VOID       * p_Memory
    )
{
    if (p_Memory)
    {
        HMINSWP_POLYMESH    h_polymesh;

        h_polymesh = MinSwp_PolyMesh_Create();

        {
            const QM_ULL64 * p_id64 = (const QM_ULL64 *)p_Memory;
            if (
                    (p_id64[0] != MINSWP_POLYMESH_FILE_ID_0) ||
                    (p_id64[1] != MINSWP_POLYMESH_FILE_ID_1) ||
                    (p_id64[2] >  MINSWP_POLYMESH_FILE_VERSION)
                )
            {
                return QM_NULL;
            };
            p_Memory = (const QM_VOID *)(&((const QM_ULL64 *)p_Memory)[3]);
        };

        {
            QM_ULONG    verts_count,
                        faces_count;

            verts_count = ((QM_ULONG *)p_Memory)[0];
            faces_count = ((QM_ULONG *)p_Memory)[1];
            p_Memory = (const QM_VOID *)(&((const QM_ULONG *)p_Memory)[2]);

            MinSwp_PolyMesh_Verts_Resize
                (
                    h_polymesh,
                    verts_count
                );
            {
                QM_ULONG i;
                for (i = 0; i < verts_count; i++)
                {
                    MinSwp_PolyMesh_Verts_Get
                        (
                            h_polymesh,
                            i
                        )[0] = ((const MINSWP_VEC3D *)p_Memory)[i];
                };

                p_Memory = (const QM_VOID *)(&((const MINSWP_VEC3D *)p_Memory)[verts_count]);
            };

            MinSwp_PolyMesh_Polys_ReSize
                (
                    h_polymesh,
                    faces_count
                );

            {
                QM_ULONG    i;
                for (i = 0; i < faces_count; i++)
                {
                    QM_ULONG    j,
                                index_count;

                    index_count = ((const QM_ULONG *)p_Memory)[0];
                    p_Memory    = (const QM_VOID *)(&((const QM_ULONG *)p_Memory)[1]);
                    for (j = 0; j < index_count; j++)
                    {
                        MinSwp_Indices_AddIndex
                            (
                                MinSwp_PolyMesh_Polys_Get
                                    (
                                        h_polymesh,
                                        i
                                    ),
                                ((const QM_ULONG *)p_Memory)[j]
                            );
                    };
                    p_Memory = (const QM_VOID *)(&((const QM_ULONG *)p_Memory)[index_count]);
                };
            };
        };

        return h_polymesh;
    }
    else
        return QM_NULL;
};

/*===========================================================*/


















