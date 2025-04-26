/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_game.c         (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_game.h"
#include "minswp_indices.h"
#include "minswp_trimesh.h"
#include "minswp_polymesh.h"
#include "minswp_life.h"
#include "minswp_time.h"


#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <stdlib.h>


#include "fmath3d.h"

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

typedef struct _MINSWP_MINESVAL
{
    QM_SLONG                    Val;
} MINSWP_MINESVAL;

typedef struct _MINSWP_GPOLY
{
    MINSWP_GPOLY_GAME_STATE     Game_State;
    MINSWP_MINESVAL             MinesVal[1];
    QM_SLONG                    Index;
    HMINSWP_INDICES             h_Verts;
    HMINSWP_INDICES             h_Verts_Adjacent_Polys;
    HMINSWP_INDICES             h_Edges_Adjacent_Polys;

    struct
    {
        MINSWP_VEC3D            Pos;
        QM_FLOAT                Radius;
    } Sphere[1];

} MINSWP_GPOLY;

typedef struct _MINSWP_GEOMETRY
{
    HMINSWP_GEOMETRY__          Handle[1];

    MINSWP_VEC3D              * p_Verts;
    MINSWP_GPOLY              * p_Polys;
    QM_ULONG                    NumVerts;
    QM_ULONG                    NumPolys;
} MINSWP_GEOMETRY;

typedef struct _MINSWP_GAME
{
    union
    {
        HMINSWP_GAME__                  Handle[1];

        MINSWP_GEOMETRY                 Geometry[1];
    };

    QM_SLONG                        Num_Mines;
    QM_LBOOL                        Mines_Arranged;
    MINSWP_GAME_STATE               Game_State;
    QM_SLONG                        Num_Fixed_Blocks;
    QM_SLONG                        Num_Flaged_Blocks;
    QM_SLONG                        Max_Num_Adjacents;
    struct
    {
        QM_DOUBL                  * p_Start;
        QM_DOUBL                  * p_Pause;
        QM_DOUBL                  * p_Stop;
    } Timer[1];
} MINSWP_GAME;

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

MINSWP_GPOLY *              MinSwp_GPoly_Init
    (
              MINSWP_GPOLY            * p_GPoly,
        const QM_SLONG                  Index
    )
{
    p_GPoly->Index       = Index;

    MinSwp_Vec3D_SetVal
        (
            &p_GPoly->Sphere->Pos,
            0.0f,
            0.0f,
            0.0f
        );

    p_GPoly->MinesVal->Val           = 0L;
    p_GPoly->Sphere->Radius          = 0.0f;
    p_GPoly->Game_State              = MINSWP_GPGS_START;
    p_GPoly->h_Verts                 = MinSwp_Indices_Create();
    p_GPoly->h_Verts_Adjacent_Polys  = MinSwp_Indices_Create();
    p_GPoly->h_Edges_Adjacent_Polys  = MinSwp_Indices_Create();

    return p_GPoly;
};

/*===========================================================*/

QM_VOID                     MinSwp_MinesVal_Mine_Set
    (
              MINSWP_MINESVAL         * p_MinesVal,
        const QM_LBOOL                  Value
    )
{
    if (p_MinesVal)
    {
        if (Value)
            p_MinesVal->Val |= 0x80000000L;
        else
            p_MinesVal->Val &= 0x7fffffffL;
    };
};

/*===========================================================*/

QM_LBOOL                    MinSwp_MinesVal_Mine_Get
    (
        const MINSWP_MINESVAL         * p_MinesVal
    )
{
    return ((p_MinesVal->Val & 0x80000000L) != 0L);
};

/*===========================================================*/

QM_VOID                     MinSwp_MinesVal_NumAdjaccents_Set
    (
              MINSWP_MINESVAL         * p_MinesVal,
        const QM_SLONG                  NumMines,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    switch (Adjacent_Type)
    {
        case MINSWP_ADJT_BYVERTS:
            {
                p_MinesVal->Val &= 0xffff0000L;
                p_MinesVal->Val |=  (QM_SLONG)NumMines        & 0x0000ffffL;
            } break;
        case MINSWP_ADJT_BYEDGES:
            {
                p_MinesVal->Val &= 0x8000ffffL;
                p_MinesVal->Val |= ((QM_SLONG)NumMines << 16) & 0x7fff0000L;
            } break;
    };
};

/*===========================================================*/

QM_SLONG                    MinSwp_MinesVal_NumAdjaccents_Get
    (
        const MINSWP_MINESVAL         * p_MinesVal,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    switch (Adjacent_Type)
    {
        case MINSWP_ADJT_BYVERTS:   return (QM_SLONG)( p_MinesVal->Val & 0x0000ffffL);        __qm_comment__( break; )
        case MINSWP_ADJT_BYEDGES:   return (QM_SLONG)((p_MinesVal->Val & 0x7fffffffL) >> 16); __qm_comment__( break; )
    default:                        return 0;
        ;
    };
    /* return 0; */
};

/*===========================================================*/

QM_VOID                     MinSwp_MinesVal_ResetAllToZero
    (
              MINSWP_MINESVAL         * p_MinesVal
    )
{
    p_MinesVal->Val = 0L;
};

/*===========================================================*/

HMINSWP_INDICES             MinSwp_GPoly_AdjacentIndices_Get
    (
              MINSWP_GPOLY            * p_GPoly,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    if (p_GPoly)
    {
        switch (Adjacent_Type)
        {
            case MINSWP_ADJT_BYVERTS:   return p_GPoly->h_Verts_Adjacent_Polys; __qm_comment__( break; )
            case MINSWP_ADJT_BYEDGES:   return p_GPoly->h_Edges_Adjacent_Polys; __qm_comment__( break; )
        default:                        return QM_NULL;
        };
        /* return QM_NULL; */
    }
    else
        return QM_NULL;
};

const HMINSWP_INDICES           MinSwp_GPoly_AdjacentIndices_GetConst
    (
        const MINSWP_GPOLY            * p_GPoly,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    if (p_GPoly)
    {
        switch (Adjacent_Type)
        {
            case MINSWP_ADJT_BYVERTS:   return p_GPoly->h_Verts_Adjacent_Polys; __qm_comment__( break; )
            case MINSWP_ADJT_BYEDGES:   return p_GPoly->h_Edges_Adjacent_Polys; __qm_comment__( break; )
        default:                        return QM_NULL;
        };
        /* return QM_NULL; */
    }
    else
        return QM_NULL;
};

/*===========================================================*/

QM_VOID                     MinSwp_GPoly_Adjacent_Clear
    (
              MINSWP_GPOLY        * p_GPoly
    )
{
    MinSwp_Indices_Clear(p_GPoly->h_Verts_Adjacent_Polys);
    MinSwp_Indices_Clear(p_GPoly->h_Edges_Adjacent_Polys);
};

/*===========================================================*/

QM_VOID                     MinSwp_Geometry_Poly_Sphere_Update
    (
              MINSWP_GEOMETRY     * p_Geometry,
        const QM_ULONG              Poly_Index
    )
{
    if (Poly_Index < p_Geometry->NumPolys)
    {
        MINSWP_GPOLY      * p_gpoly;

        p_gpoly = &p_Geometry->p_Polys[Poly_Index];

        {
            QM_FLOAT            dist;
            MINSWP_VEC3D      * p_vec3s;

            const QM_SLONG count = MinSwp_Indices_GetCount(p_gpoly->h_Verts);

            dist = -1.0f;

            *((void **)&p_vec3s) = malloc(count * sizeof(p_vec3s[0]));

            {
                QM_SLONG i;
                for (i = 0; i < count; i++)
                {
                    p_vec3s[i] =
                        p_Geometry->p_Verts
                            [
                                MinSwp_Indices_GetIndex
                                    (
                                        p_gpoly->h_Verts,
                                        i
                                    )
                            ];
                };
            };

            {
                QM_SLONG    index_a,
                            index_b,
                            i;
                const
                QM_SLONG count_1 = count - 1;
                index_a = 0;
                index_b = 0;
                for (i = 0; i < count_1; i++)
                {
                    QM_SLONG j;
                    for (j = i + 1; j < count; j++)
                    {
                        const QM_FLOAT d =
                            FM3DVec3DistanceSqr_f32
                                (
                                    p_vec3s[i].f,
                                    p_vec3s[j].f
                                );
                        if (d > dist)
                        {
                            dist    = d;
                            index_a = i;
                            index_b = j;
                        };
                    };
                };
                p_gpoly->Sphere->Radius = sqrt(dist);
                FM3DVec3Lerp_f32
                    (
                        p_gpoly->Sphere->Pos.f,
                        p_vec3s[index_a].f,
                        p_vec3s[index_b].f,
                        0.5f
                    );
            };

            free(p_vec3s);
        };
    };
};

/*===========================================================*/

QM_LBOOL                    MinSwp_Geometry_Poly_Verts_Set
    (
              MINSWP_GEOMETRY     * p_Geometry,
        const QM_ULONG              Poly_Index,
        const HMINSWP_INDICES       h_Src
    )
{
    if (
            (h_Src) &&
            (p_Geometry) &&
            (Poly_Index < p_Geometry->NumPolys)
        )
    {
        MinSwp_Indices_Copy
            (
                p_Geometry->p_Polys[Poly_Index].h_Verts,
                h_Src
            );

        MinSwp_Geometry_Poly_Sphere_Update
            (
                p_Geometry,
                Poly_Index
            );
        return QM_TRUE;
    }
    else
        return QM_FALSE;
};

/*===========================================================*/

/*
QM_SLONG TMW3D_Geometry::GetPolyByAdjacency
    (
        const QM_SLONG              poly_index,
        const QM_SLONG              adjacent_index,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    return
        this->GetPoly
            (
                this->poly_GetAdjacent
                    (
                        poly_index,
                        adjacent_index,
                        adjacent_type
                    )
            );
};
*/

/*===========================================================*/
/*
QM_LBOOL MinSwp_Geometry_Poly_AddAdjacent
    (
              MINSWP_GPOLY        * p_GPoly,
        const QM_SLONG              New_Adjacent_Index,
        const MINSWP_ADJACENT_TYPE  Adjacent_Type
    )
{
    HMINSWP_INDICES h_i = this->GetAdjacentIndices(p_poly, adjacent_type);
    if (h_i)
    {
        if (
                (new_adjacent_index >= 0) &&
                (new_adjacent_index < (QM_SLONG)this__vec_polys.size())
            )
        {
            MinSwp_Indices_AddIndex
                (
                    h_i,
                    new_adjacent_index
                );
            return QM_TRUE;
        };
    };
    return QM_FALSE;
};
*/

/*===========================================================*/

QM_LBOOL                    MinSwp_GPoly_IntersectSphereByRay
    (
        const MINSWP_GPOLY  * p_GPoly,
        const MINSWP_RAY3D  * p_Ray,
              QM_FLOAT      * p_Out_Distance
    )
{
    return
        FM3DRay3IntersectSphere_f32
            (
                p_Out_Distance,
                p_Ray->Pos.f,
                p_Ray->Dir.f,
                p_GPoly->Sphere->Pos.f,
                p_GPoly->Sphere->Radius,
                FM3D_FALSE
            );
};

/*===========================================================*/



HMINSWP_GAME MinSwp_Game_Create()
{
    MINSWP_GAME * p_game =
   (MINSWP_GAME *)calloc(1, sizeof(
    MINSWP_GAME));

    p_game->Max_Num_Adjacents   = 0;
    p_game->Num_Mines           = 0;
    p_game->Mines_Arranged      = QM_FALSE;
    p_game->Game_State          = MINSWP_GS_VOID;
    p_game->Num_Fixed_Blocks    = 0;
    p_game->Num_Flaged_Blocks   = 0;

    p_game->Geometry->NumVerts  = 0;
    p_game->Geometry->NumPolys  = 0;
    p_game->Geometry->p_Verts   = QM_NULL;
    p_game->Geometry->p_Polys   = QM_NULL;

    p_game->Timer->p_Start      = QM_NULL;
    p_game->Timer->p_Pause      = QM_NULL;
    p_game->Timer->p_Stop       = QM_NULL;

    return p_game->Handle;
};

/*===========================================================*/

MINSWP_VEC3D * MinSwp_Geometry_Vert_Pos_Get
    (
        const HMINSWP_GEOMETRY      h_Geometry,
        const QM_SLONG              Vert_Index,
              MINSWP_VEC3D        * p_Out_Pos
    )
{
    const MINSWP_GEOMETRY * p_geometry =
         (MINSWP_GEOMETRY *)h_Geometry;

    if (
            (p_Out_Pos) &&
            (0 < Vert_Index) &&
            (    Vert_Index >= (QM_SLONG)p_geometry->NumVerts)
        )
    {
        *p_Out_Pos = p_geometry->p_Verts[Vert_Index];
    };

    return p_Out_Pos;
};

/*===========================================================*/

MINSWP_GPOLY *              MinSwp_Geometry_Poly_GetPtr
    (
              HMINSWP_GEOMETRY      h_Geometry,
        const QM_SLONG              Poly_Index
    )
{
    MINSWP_GEOMETRY * p_geometry =
   (MINSWP_GEOMETRY *)h_Geometry;

    if (
            (0 <= Poly_Index) &&
            (     Poly_Index < (QM_SLONG)p_geometry->NumPolys)
        )
        return &p_geometry->p_Polys[Poly_Index];
    else
        return QM_NULL;
};

/*===========================================================*/

const MINSWP_GPOLY *        MinSwp_Geometry_Poly_GetConstPtr
    (
        const HMINSWP_GEOMETRY      h_Geometry,
        const QM_SLONG              Poly_Index
    )
{
    const MINSWP_GEOMETRY * p_geometry =
   (const MINSWP_GEOMETRY *)h_Geometry;

    if (
            (0 <= Poly_Index) &&
            (     Poly_Index < (QM_SLONG)p_geometry->NumPolys)
        )
        return &p_geometry->p_Polys[Poly_Index];
    else
        return QM_NULL;
};

/*===========================================================*/

QM_VOID MinSwp_GPoly_Adjacents_NumMines_Set
    (
              MINSWP_GPOLY        * p_GPoly,
        const QM_SLONG              NumMines,
        const MINSWP_ADJACENT_TYPE  Adjacent_Type
    )
{
    if (p_GPoly)
    {
        MinSwp_MinesVal_NumAdjaccents_Set
            (
                p_GPoly->MinesVal,
                NumMines,
                Adjacent_Type
            );
    };
};

/*===========================================================*/

QM_LBOOL MinSwp_Geometry_NumVerts_Set
    (
              MINSWP_GEOMETRY         * p_Geometry,
        const QM_ULONG                  NewSize
    )
{
    if (p_Geometry)
    {
        MinSwp__ResizePtr__MACRO
            (
                p_Geometry->p_Verts,
                p_Geometry->NumVerts,
                NewSize
            );
        p_Geometry->NumVerts = NewSize;

        return QM_TRUE;
    }
    else
        return QM_FALSE;
};

/*===========================================================*/
/*===========================*/
/*                           */
/* public methods            */
/*                           */
/*===========================*/

QM_SLONG MinSwp_Geometry_Poly_NumVerts_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index
    )
{
    const MINSWP_GEOMETRY * p_geometry =
   (const MINSWP_GEOMETRY *)h_Geometry;
    if (p_geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    (HMINSWP_GEOMETRY)p_geometry->Handle,
                    Poly_Index
                );
        if (p_poly)
            return
                MinSwp_Indices_GetCount
                    (
                        p_poly->h_Verts
                    );
    };

    return -1;
};

/*===========================================================*/

QM_SLONG MinSwp_Geometry_Poly_VertIndex_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
        const QM_SLONG                  Index
    )
{
    if (h_Geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    h_Geometry,
                    Poly_Index
                );
        if (p_poly)
            return MinSwp_Indices_GetIndex
                (
                    p_poly->h_Verts,
                    Index
                );
    };

    return -1;
};

/*===========================================================*/

QM_SLONG MinSwp_Geometry_Poly_Adjacent_Num_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    if (h_Geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    h_Geometry,
                    Poly_Index
                );
        if (p_poly)
        {
            const HMINSWP_INDICES h_i =
                MinSwp_GPoly_AdjacentIndices_GetConst
                    (
                        p_poly,
                        Adjacent_Type
                    );
            if (h_i)
                return
                    MinSwp_Indices_GetCount(h_i);
        };
    };

    return -1;
};

/*===========================================================*/

QM_SLONG MinSwp_Geometry_Poly_Adjacent_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
        const QM_SLONG                  Adjacent_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    if (h_Geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    h_Geometry,
                    Poly_Index
                );
        if (p_poly)
        {
            const HMINSWP_INDICES h_i =
                MinSwp_GPoly_AdjacentIndices_GetConst
                    (
                        p_poly,
                        Adjacent_Type
                    );

            if (h_i)
                return
                    MinSwp_Indices_GetIndex
                        (
                            h_i, 
                            Adjacent_Index
                        );
        };
    };

    return -1;
};

/*===========================================================*/

QM_LBOOL MinSwp_Geometry_Poly_IsAdjacent
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  PolyA_Index,
        const QM_SLONG                  PolyB_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    if (h_Geometry)
    {
        const MINSWP_GPOLY  * p_polyA,
                            * p_polyB;

        p_polyA = MinSwp_Geometry_Poly_GetConstPtr( h_Geometry, PolyA_Index );
        p_polyB = MinSwp_Geometry_Poly_GetConstPtr( h_Geometry, PolyB_Index );

        if (
                (p_polyA) &&
                (p_polyB)
            )
        {
            const HMINSWP_INDICES h_i =
                MinSwp_GPoly_AdjacentIndices_GetConst
                    (
                        p_polyA,
                        Adjacent_Type
                    );
            if (h_i)
                return
                    MinSwp_Indices_IsMember
                        (
                            h_i,
                            PolyB_Index
                        );
        };
    };

    return QM_FALSE;
};

/*===========================================================*/

QM_VOID MinSwp_Geometry_Poly_DetermineAdjacency
    (
              HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  PolyA_Index,
        const QM_SLONG                  PolyB_Index
    )
{
    if (h_Geometry)
    {
        MINSWP_GPOLY    * p_polyA,
                        * p_polyB;

        p_polyA = MinSwp_Geometry_Poly_GetPtr( h_Geometry, PolyA_Index );
        p_polyB = MinSwp_Geometry_Poly_GetPtr( h_Geometry, PolyB_Index );

        if (
                (p_polyA) &&
                (p_polyB)
            )
        {
            QM_SLONG    overlap,
                        i;
            const
            QM_SLONG polyA_verts_count = MinSwp_Indices_GetCount(p_polyA->h_Verts);
            overlap = 0;
            for (i = 0; i < polyA_verts_count; i++)
            {
                if (
                        MinSwp_Indices_IsMember
                            (
                                p_polyB->h_Verts,
                                MinSwp_Indices_GetIndex
                                    (
                                        p_polyA->h_Verts,
                                        i
                                    )
                            )
                    )
                    overlap++ ;
            };

            if (    overlap > 0)
            {
                    MinSwp_Indices_AddIndex(p_polyA->h_Verts_Adjacent_Polys, PolyB_Index);
                    MinSwp_Indices_AddIndex(p_polyB->h_Verts_Adjacent_Polys, PolyA_Index);
                if (overlap > 1)
                {
                    MinSwp_Indices_AddIndex(p_polyA->h_Edges_Adjacent_Polys, PolyB_Index);
                    MinSwp_Indices_AddIndex(p_polyB->h_Edges_Adjacent_Polys, PolyA_Index);
                };
            };
        };
    };
};

/*===========================================================*/

QM_LBOOL MinSwp_Geometry_Poly_MineValue_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index
    )
{
    if (h_Geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    h_Geometry,
                    Poly_Index
                );
        if (p_poly)
        {
            return
                MinSwp_MinesVal_Mine_Get
                    (
                        p_poly->MinesVal
                    );
        };
    };

    return QM_FALSE;
};

/*===========================================================*/

QM_SLONG MinSwp_Geometry_Poly_Adjacents_NumMines
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    const MINSWP_GEOMETRY * p_geometry =
   (const MINSWP_GEOMETRY *)h_Geometry;
    if (p_geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    (const HMINSWP_GEOMETRY)p_geometry->Handle,
                    Poly_Index
                );
        if (p_poly)
            return
                MinSwp_MinesVal_NumAdjaccents_Get
                    (
                        p_poly->MinesVal,
                        Adjacent_Type
                    );
    }
    return -1;
};

/*===========================================================*/

QM_FLOAT MinSwp_Geometry_Poly_Sphere_Radius_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index
    )
{
    const MINSWP_GEOMETRY * p_geometry =
   (const MINSWP_GEOMETRY *)h_Geometry;
    if (p_geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    (const HMINSWP_GEOMETRY)p_geometry->Handle,
                    Poly_Index
                );
        if (p_poly)
            return p_poly->Sphere->Radius;
    };
    return 0.0f;
};

/*===========================================================*/

MINSWP_VEC3D * MinSwp_Geometry_Poly_Sphere_Pos_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
              MINSWP_VEC3D            * p_Out_Pos
    )
{
    const MINSWP_GEOMETRY * p_geometry =
   (const MINSWP_GEOMETRY *)h_Geometry;
    if (p_geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    (const HMINSWP_GEOMETRY)p_geometry->Handle,
                    Poly_Index
                );
        if (
                (p_poly) &&
                (p_Out_Pos)
            )
            p_Out_Pos[0] = p_poly->Sphere->Pos;
    };
    return p_Out_Pos;
};

/*===========================================================*/

HMINSWP_GEOMETRY            MinSwp_Game_Geometry_Get
    (
              HMINSWP_GAME              h_Game
    )
{
    return ((MINSWP_GAME *)h_Game)->Geometry->Handle;
};

/*===========================================================*/

HMINSWP_TRIMESH  MinSwp_Geometry_Poly_TriMesh_Create
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
              HMINSWP_TRIMESH         * ph_Mesh
    )
{
    const MINSWP_GEOMETRY * p_geometry =
   (const MINSWP_GEOMETRY *)h_Geometry;
    if (p_geometry)
    {
        const MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetConstPtr
                (
                    (const HMINSWP_GEOMETRY)p_geometry->Handle,
                    Poly_Index
                );
        if (
                (p_poly) &&
                (ph_Mesh)
            )
        {
            HMINSWP_TRIMESH h_mesh;
            h_mesh = MinSwp_TriMesh_Create();

            {
                const QM_SLONG verts_count = MinSwp_Indices_GetCount(p_poly->h_Verts);
                const QM_SLONG faces_count = verts_count - 2;

                MinSwp_TriMesh_Verts_Resize(h_mesh, verts_count);
                MinSwp_TriMesh_Faces_Resize(h_mesh, faces_count);

                {
                    QM_SLONG i;
                    for (i = 0; i < verts_count; i++)
                    {
                        MinSwp_TriMesh_Verts_Get
                            (
                                h_mesh,
                                i
                            )[0] = p_geometry->p_Verts
                                [
                                    MinSwp_Indices_GetIndex
                                        (
                                            p_poly->h_Verts,
                                            i
                                        )
                                ];
                    };
                };

                {
                    QM_SLONG i;
                    for (i = 0; i < faces_count; i++)
                    {
                        MinSwp_TriIndices_SetVal
                            (
                                MinSwp_TriMesh_Faces_Get
                                    (
                                        h_mesh,
                                        i
                                    ),
                                0,
                                1 + i,
                                2 + i
                            );
                    };
                };

            };

            *ph_Mesh = h_mesh;
            return h_mesh;
        };
    };
    return QM_NULL;
};

/*===========================================================*/

MINSWP_DRAW_TYPE MinSwp_Geometry_Poly_DrawType_Get
    (
              HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    MINSWP_GEOMETRY * p_geometry =
   (MINSWP_GEOMETRY *)h_Geometry;
    if (p_geometry)
    {
        MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetPtr
                (
                    p_geometry->Handle,
                    Poly_Index
                );
        if (p_poly)
        {
            switch (p_poly->Game_State)
            {
                case MINSWP_GPGS_START:         return MINSWP_DT_START;     __qm_comment__( break; )
                case MINSWP_GPGS_FIXED:         return (MINSWP_DRAW_TYPE)
                                                    (
                                                        MinSwp_Geometry_Poly_Adjacents_NumMines
                                                            (
                                                                p_geometry->Handle,
                                                                Poly_Index,
                                                                Adjacent_Type
                                                            )
                                                    );                      __qm_comment__( break; )
                case MINSWP_GPGS_ASSUM_MINS:    return MINSWP_DT_FLAG;      __qm_comment__( break; )
                case MINSWP_GPGS_BOOM:          return MINSWP_DT_BOOM;      __qm_comment__( break; )
                case MINSWP_GPGS_HEART:         return MINSWP_DT_HEART;     __qm_comment__( break; )
                default:                        return MINSWP_DT_FREE;
            };
        };
    };

    return MINSWP_DT_FREE;
};

/*===========================================================*/

/*===========================*/
/*                           */
/* MINSWP_GAME               */
/*                           */
/*===========================*/

QM_SLONG MinSwp_Game_MinNumMines
    (
        const HMINSWP_GAME              h_Game
    )
{
    return 5;
};

/*===========================================================*/

QM_SLONG MinSwp_Game_MaxNumMines
    (
         const HMINSWP_GAME              h_Game
    )
{
    const MINSWP_GAME * p_game =
   (const MINSWP_GAME *)h_Game;
    if (p_game)
    {
        const QM_SLONG num_polys = p_game->Geometry->NumPolys;
        const QM_SLONG a = num_polys - (p_game->Max_Num_Adjacents + 1 + 10);
        const QM_SLONG b = (QM_SLONG)( ((QM_FLOAT)num_polys) * 0.9f);
        return (a < b) ? a : b;
    };

    return 0;
};

/*===========================================================*/

QM_SLONG MinSwp_Game_NumMines_Get
    (
        const HMINSWP_GAME              h_Game
    )
{
    return ((const MINSWP_GAME *)h_Game)->Num_Mines;
};

/*===========================================================*/

QM_SLONG MinSwp_Game_NumFlags_Get
    (
        const HMINSWP_GAME              h_Game
    )
{
    return ((const MINSWP_GAME *)h_Game)->Num_Flaged_Blocks;
};

/*===========================================================*/

QM_VOID MinSwp_Game_SetNewPolyMesh_ClearAllAdjacency
    (
              MINSWP_GAME             * p_Game
    )
{
    QM_SLONG i;
    const QM_SLONG count = p_Game->Geometry->NumPolys;
    for (i = 0; i < count; i++)
    {
        MinSwp_GPoly_Adjacent_Clear
            (
                &p_Game->Geometry->p_Polys[i]
            );
    };
};

/*===========================================================*/

QM_VOID MinSwp_Game_SetNewPolyMesh_GenerateAdjacency
    (
              MINSWP_GAME             * p_Game
    )
{
    const QM_SLONG polys_count = p_Game->Geometry->NumPolys;

    MinSwp_Game_SetNewPolyMesh_ClearAllAdjacency(p_Game);

    {
        QM_SLONG i;
        for (i = 0; i < polys_count - 1; i++)
        {
            QM_SLONG j;
            for (j = i + 1; j < polys_count; j++)
            {
                MinSwp_Geometry_Poly_DetermineAdjacency
                    (
                        p_Game->Geometry->Handle,
                        i,
                        j
                    );
            };
        };
    };
    /*=======================================================*/
    {
        QM_SLONG    i,
                    max_adjacent_count;
        max_adjacent_count = 0;
        for (i = 0; i < polys_count; i++)
        {
            const QM_SLONG x =
                MinSwp_Indices_GetCount
                    (
                        p_Game->Geometry->p_Polys[i].h_Verts_Adjacent_Polys
                    );
            if (x > max_adjacent_count)
                max_adjacent_count = x;
        };
        p_Game->Max_Num_Adjacents = max_adjacent_count;
    };
};

/*===========================================================*/

QM_LBOOL MinSwp_Game_SetNewPolyMesh
    (
              HMINSWP_GAME              h_Game,
        const HMINSWP_POLYMESH          h_PolyMesh
    )
{
    MINSWP_GAME * p_Game =
   (MINSWP_GAME *)h_Game;

    if (h_PolyMesh)
    {
        MINSWP_GEOMETRY * const p_Geometry = p_Game->Geometry;
        const QM_SLONG verts_count = (QM_SLONG)MinSwp_PolyMesh_Verts_Size(h_PolyMesh);
        const QM_SLONG polys_count = (QM_SLONG)MinSwp_PolyMesh_Polys_Size(h_PolyMesh);

        if (
                (verts_count) &&
                (polys_count)
            )
        {
            HMINSWP_INDICES     h_poly;
            QM_SLONG            count;
            QM_SLONG            i,
                                index;
            for (i = 0; i < polys_count; i++)
            {
                h_poly = MinSwp_PolyMesh_Polys_Get(h_PolyMesh, i);
                count  = MinSwp_Indices_GetCount(h_poly);
                if (count)
                {
                    QM_SLONG j;
                    for (j = 0; j < count; j++)
                    {
                        index = MinSwp_Indices_GetIndex(h_poly, j);
                        if (!(
                                (0 <= index              ) &&
                                (     index < verts_count)
                            ))
                        {
                            return QM_FALSE;
                        };
                    };
                }
                else
                    return QM_FALSE;
            };

            /*============================*/
            /*                            */
            /* true polygon               */
            /*                            */
            /*============================*/

            {
                QM_ULONG i;
                const QM_ULONG num_verts = MinSwp_PolyMesh_Verts_Size(h_PolyMesh);

                MinSwp_Geometry_NumVerts_Set
                    (
                        p_Geometry,
                        num_verts
                    );
                for (i = 0; i < num_verts; i++)
                {
                    p_Geometry->p_Verts[i] = MinSwp_PolyMesh_Verts_Get(h_PolyMesh, i)[0];
                };
            };

            {
                const QM_SLONG old_polys_count = (QM_SLONG)p_Geometry->NumPolys;
                {
                    QM_SLONG i;
                    for (i = 0; i < old_polys_count; i++)
                    {
                        MinSwp_Indices_Release(p_Geometry->p_Polys[i].h_Verts);
                        MinSwp_Indices_Release(p_Geometry->p_Polys[i].h_Verts_Adjacent_Polys);
                        MinSwp_Indices_Release(p_Geometry->p_Polys[i].h_Edges_Adjacent_Polys);
                        memset
                            (
                               &p_Geometry->p_Polys[i],
                                0,
                                sizeof(
                                p_Geometry->p_Polys[i])
                            );
                    };
                };

                MinSwp__ResizePtr__MACRO
                    (
                        p_Geometry->p_Polys,
                        old_polys_count,
                            polys_count
                    );
                p_Geometry->NumPolys = polys_count;

                {
                    QM_SLONG i;
                    for (i = 0; i < polys_count; i++)
                    {
                        MinSwp_GPoly_Init
                            (
                                &p_Geometry->p_Polys[i],
                                i
                            );
                    };
                };

                {
                    QM_SLONG i;
                    for (i = 0; i < polys_count; i++)
                    {
                        MinSwp_Geometry_Poly_Verts_Set
                            (
                                p_Geometry,
                                i,
                                MinSwp_PolyMesh_Polys_Get
                                    (
                                        h_PolyMesh,
                                        i
                                    )
                            );
                    };
                };
            };

            p_Game->Mines_Arranged      = QM_FALSE;
            p_Game->Game_State          = MINSWP_GS_VOID;
            p_Game->Num_Fixed_Blocks    = 0;
            p_Game->Num_Flaged_Blocks   = 0;
            p_Game->Max_Num_Adjacents   = 0;
            MinSwp_Game_SetNewPolyMesh_GenerateAdjacency(p_Game);
            return QM_TRUE;
        };
    };

    return QM_FALSE;
};

/*===========================================================*/

QM_ULONG MinSwp_Geometry_NumVerts_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry
    )
{
    return ((const MINSWP_GEOMETRY *)h_Geometry)->NumVerts;
};

/*===========================================================*/

QM_ULONG MinSwp_Geometry_NumPolys_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry
    )
{
    return ((const MINSWP_GEOMETRY *)h_Geometry)->NumPolys;
};

/*===========================================================*/

QM_ULONG MinSwp_Game_NumVerts_Get
    (
        const HMINSWP_GAME              h_Game
    )
{
    return
        MinSwp_Geometry_NumVerts_Get
            (
                (const HMINSWP_GEOMETRY)
                    ((const MINSWP_GAME *)h_Game)->Geometry->Handle
            );
};

/*===========================================================*/

QM_ULONG MinSwp_Game_NumPolys_Get
    (
        const HMINSWP_GAME              h_Game
    )
{
    return
        MinSwp_Geometry_NumPolys_Get
            (
                (const HMINSWP_GEOMETRY)
                    ((const MINSWP_GAME *)h_Game)->Geometry->Handle
            );
};

/*===========================================================*/

QM_SLONG MinSwp_Game_TrueNumMines_Get
    (
        const HMINSWP_GAME          h_Game,
        const QM_SLONG              NumMines
    )
{
    const QM_SLONG min_count = MinSwp_Game_MinNumMines(h_Game);
    const QM_SLONG max_count = MinSwp_Game_MaxNumMines(h_Game);

    if (NumMines > max_count)
        return max_count;
    else
    if (NumMines < min_count)
        return min_count;
    else
        return NumMines;
};

/*===========================================================*/

QM_SLONG MinSwp_Game_Poly_IntersectedByRay_Find
    (
        const HMINSWP_GAME          h_Game,
        const MINSWP_RAY3D        * p_Ray
    )
{
    const MINSWP_GAME * p_game =
   (const MINSWP_GAME *)h_Game;

    if (p_game)
    {
        const QM_SLONG poly_count = p_game->Geometry->NumPolys;
        MinSwp_ShowMessage_W(L"MinSwp_Game_Poly_IntersectedByRay_Find");
        if (poly_count)
        {
            MINSWP_RAY3D                ray;
            struct
            {
                struct
                {
                    QM_SLONG            indx;
                    QM_FLOAT            dist;
                } * p;
                QM_SLONG                sz;
            } vec_sphere;

            MinSwp_ShowMessage_WI(L"poly_count : ", poly_count);

            *((void **)&vec_sphere.p) = malloc(poly_count * sizeof(vec_sphere.p[0]));
            vec_sphere.sz = 0;

            ray = *p_Ray;
            FM3DVec3Normalize_f32
                (
                    ray.Dir.f,
                    ray.Dir.f
                );

            {
                QM_SLONG i;
                for (i = 0; i < poly_count; i++)
                {
                    QM_FLOAT    len;
                    if (
                            MinSwp_GPoly_IntersectSphereByRay
                                (
                                    &p_game->Geometry->p_Polys[i],
                                    &ray,
                                    &len
                                )
                        )
                    {
                        /*
                        MinSwp_ShowMessage_WI(L"ntersectSphereByRay(vec_sphere.p[i]). i : ", vec_sphere.sz);
                        */
                        vec_sphere.p[vec_sphere.sz].dist    = len;
                        vec_sphere.p[vec_sphere.sz].indx    = i;
                        vec_sphere.sz++;
                    };
                };
            };

            if (vec_sphere.sz)
            {
                const QM_SLONG sph_count_1 = vec_sphere.sz - 1;

                MinSwp_ShowMessage_WI(L"vec_sphere.sz : ", vec_sphere.sz);

                {
                    QM_SLONG i;
                    for (i = 0; i < sph_count_1; i++)
                    {
                        QM_SLONG j;
                        for (j = i + 1; j < vec_sphere.sz; j++)
                        {
                            if (
                                    vec_sphere.p[i].dist >
                                    vec_sphere.p[j].dist
                                )
                            {
                                {
                                    const
                                    QM_FLOAT local_f        = vec_sphere.p[i].dist;
                                    vec_sphere.p[i].dist    = vec_sphere.p[j].dist;
                                    vec_sphere.p[j].dist    = local_f;
                                };
                                {
                                    const
                                    QM_SLONG local_i        = vec_sphere.p[i].indx;
                                    vec_sphere.p[i].indx    = vec_sphere.p[j].indx;
                                    vec_sphere.p[j].indx    = local_i;
                                };
                            };
                        };
                    };
                };

                {
                    QM_SLONG    ret_index;
                    QM_FLOAT    dist,
                                dist_add_rad;

                    ret_index   = -1;
                    dist        = 1.0e16;

                    {
                        QM_SLONG i;
                        for (i = 0; i < vec_sphere.sz; i++)
                        {
                            MINSWP_VEC3D        sphere_pos;
                            QM_FLOAT            sphere_radius;

                            MinSwp_Geometry_Poly_Sphere_Pos_Get
                                (
                                    (const HMINSWP_GEOMETRY)p_game->Geometry->Handle,
                                    vec_sphere.p[i].indx,
                                    &sphere_pos
                                );
                            sphere_radius =
                            MinSwp_Geometry_Poly_Sphere_Radius_Get
                                (
                                    (const HMINSWP_GEOMETRY)p_game->Geometry->Handle,
                                    vec_sphere.p[i].indx
                                );
                            dist_add_rad = dist + sphere_radius;
                            if (
                                    FM3DVec3DistanceSqr_f32
                                        (
                                            ray.Pos.f,
                                            sphere_pos.f
                                        ) <=
                                    (
                                        dist_add_rad *
                                        dist_add_rad
                                    )
                                ) /* the next sphere is very far */
                            {
                                HMINSWP_TRIMESH h_mesh = QM_NULL;
                                MinSwp_Geometry_Poly_TriMesh_Create
                                    (
                                        (const HMINSWP_GEOMETRY)p_game->Geometry->Handle,
                                        vec_sphere.p[i].indx,
                                        &h_mesh
                                    );
                                if (h_mesh)
                                {
                                    QM_FLOAT    len;
                                    if (MinSwp_TriMesh_IntersectTrianglesByRay
                                            (
                                                h_mesh,
                                                &ray,
                                                &len
                                            )
                                        )
                                    {
                                        if (len < dist)
                                        {
                                            dist = len;
                                            ret_index = vec_sphere.p[i].indx;
                                        };
                                    };

                                    MinSwp_TriMesh_Release(h_mesh);
                                    MinSwp_ShowMessage_WI(L"h_mesh != NULL. index : ", vec_sphere.p[i].indx);
                                }
                                else
                                    MinSwp_ShowMessage_WI(L"h_mesh == NULL. index : ", vec_sphere.p[i].indx);
                            };
                        };
                        return ret_index;
                    };
                };
            }
            else /* if (vec_sphere.sz == 0) */
                return -1;
        }
        else /* if (poly_count == 0) */
             return -1;
    }
    else /* if (p_game == QM_NULL) */
        return -1;
};

/*===========================================================*/

/*
QM_LBOOL MinSwp_Game_Current_Restart
    (
        HMINSWP_GAME            h_Game
    )
{
    MINSWP_GAME * p_game =
   (MINSWP_GAME *)h_Game;

    if (p_game->Game_State == MINSWP_GS_VOID)
        return QM_FALSE;
    else
    {
        const QM_ULONG poly_count = p_game->Geometry->NumPolys;
        if (poly_count)
        {
            QM_ULONG i;
            for (i = 0; i < poly_count; i++)
            {
                p_game->Geometry->p_Polys[i].Game_State = MINSWP_GPGS_START;
            };
            p_game->Game_State          = MINSWP_GS_STARTED;
            p_game->Num_Flaged_Blocks   = 0;
            p_game->Num_Fixed_Blocks    = 0;
            return QM_TRUE;
        }
        else __qm_comment__('if (poly_count == 0)')
            return QM_FALSE;
    };
};
*/

/*===========================================================*/

QM_LBOOL MinSwp_Game_New_Start
    (
              HMINSWP_GAME              h_Game,
        const QM_SLONG                  NumMines
    )
{
    MINSWP_GAME * p_game =
   (MINSWP_GAME *)h_Game;

    {
        const QM_ULONG poly_count = p_game->Geometry->NumPolys;
        if (poly_count)
        {
            {
                QM_ULONG i;
                for (i = 0; i < poly_count; i++)
                {
                    p_game->Geometry->p_Polys[i].Game_State = MINSWP_GPGS_START;
                    MinSwp_MinesVal_ResetAllToZero
                        (
                            p_game->Geometry->p_Polys[i].MinesVal
                        );
                };
            };

            if (NumMines)
            {
                p_game->Num_Mines =
                    MinSwp_Game_TrueNumMines_Get
                        (
                            h_Game,
                            NumMines
                        );
            }
            else
            {
                if (p_game->Num_Mines == 0)
                {
                    p_game->Num_Mines =
                        MinSwp_Game_TrueNumMines_Get
                            (
                                h_Game,
                                poly_count / 5
                            );
                };
            };
            p_game->Mines_Arranged      = QM_FALSE;
            p_game->Num_Flaged_Blocks   = 0;
            p_game->Num_Fixed_Blocks    = 0;
            p_game->Game_State          = MINSWP_GS_STARTED;

            {
                if (        p_game->Timer->p_Start) {
                    free(   p_game->Timer->p_Start);
                            p_game->Timer->p_Start  = QM_NULL; };
                if (        p_game->Timer->p_Pause) {
                    free(   p_game->Timer->p_Pause);
                            p_game->Timer->p_Pause  = QM_NULL; };
                if (        p_game->Timer->p_Stop)  {
                    free(   p_game->Timer->p_Stop);
                            p_game->Timer->p_Stop   = QM_NULL; };
            };

            return QM_TRUE;
        }
        else __qm_comment__('if (poly_count == 0)')
            return QM_FALSE;
    };
};

/*===========================================================*/

QM_VOID MinSwp_Geometry_ArrangeMines_LinkedMines_Update
    (
        MINSWP_GEOMETRY             * p_Geometry
    )
{
    const QM_SLONG polys_count = (QM_SLONG)p_Geometry->NumPolys;
    const MINSWP_ADJACENT_TYPE adjacent_types[2] =
        {
            MINSWP_ADJT_BYVERTS,
            MINSWP_ADJT_BYEDGES
        };

    {
        QM_SLONG i;
        for (i = 0; i < polys_count; i++)
        {
            QM_SLONG k;

            MINSWP_GPOLY * const p_poly =
                MinSwp_Geometry_Poly_GetPtr
                    (
                        p_Geometry->Handle,
                        i
                    );

            for (k = 0; k < 2; k++)
            {
                QM_SLONG j;
                QM_SLONG mine_val;

                const QM_SLONG adjacent_count =
                    MinSwp_Geometry_Poly_Adjacent_Num_Get
                        (
                            p_Geometry->Handle,
                            i,
                            adjacent_types[k]
                        );
                mine_val = 0;
                for (j = 0; j < adjacent_count; j++)
                {
                    MINSWP_GPOLY * p_adjpoly =
                        MinSwp_Geometry_Poly_GetPtr
                            (
                                p_Geometry->Handle,
                                MinSwp_Geometry_Poly_Adjacent_Get
                                    (
                                        p_Geometry->Handle,
                                        i,
                                        j,
                                        adjacent_types[k]
                                    )
                            );
                    if (
                            MinSwp_MinesVal_Mine_Get
                                (
                                    p_adjpoly->MinesVal
                                )
                        )
                        mine_val++;
                };

                MinSwp_GPoly_Adjacents_NumMines_Set
                    (
                        p_poly,
                        mine_val,
                        adjacent_types[k]
                    );
            };
        };
    };
};

/*===========================================================*/

QM_LBOOL MinSwp_Game_Timer_IsPaused
    (
              HMINSWP_GAME              h_Game
    )
{
    MINSWP_GAME * const p_Game =
   (MINSWP_GAME *      )h_Game;

    return (p_Game->Timer->p_Pause) ?
        QM_TRUE :
        QM_FALSE;
};

/*===========================================================*/

QM_LBOOL MinSwp_Game_Timer_Pause
    (
              HMINSWP_GAME              h_Game
    )
{
    MINSWP_GAME * const p_Game =
   (MINSWP_GAME *      )h_Game;

    if (
            (p_Game->Timer->p_Start) &&
           !(p_Game->Timer->p_Stop)
        )
    {
        if (p_Game->Timer->p_Pause)
            return QM_FALSE;
        else
        {
            *((void **)&p_Game->Timer->p_Pause) = malloc(
                sizeof( p_Game->Timer->p_Pause[0]));
                        p_Game->Timer->p_Pause[0] = MinSwp_Time_Now();
            return QM_TRUE;
        };
    }
    else
        return QM_FALSE;
};

/*===========================================================*/

QM_LBOOL MinSwp_Game_Timer_Play
    (
              HMINSWP_GAME              h_Game
    )
{
    MINSWP_GAME * const p_Game =
   (MINSWP_GAME *      )h_Game;

    if (p_Game->Timer->p_Start)
    {
        if (p_Game->Timer->p_Pause)
        {
            const QM_DOUBL dt =
                p_Game->Timer->p_Pause[0] -
                p_Game->Timer->p_Start[0];
            p_Game->Timer->p_Start[0] = MinSwp_Time_Now() - dt;

            free(   p_Game->Timer->p_Pause);
                    p_Game->Timer->p_Pause =
            QM_NULL;

            return QM_TRUE;
        }
        else
            return QM_FALSE;
    }
    else
        return QM_FALSE;
};

/*===========================================================*/

QM_DOUBL MinSwp_Game_Timer_GetSecound
    (
              HMINSWP_GAME              h_Game
    )
{
    MINSWP_GAME * const p_Game =
   (MINSWP_GAME *      )h_Game;

    if (p_Game->Timer->p_Start)
        return
            (
                (   p_Game->Timer->p_Pause
                ) ? p_Game->Timer->p_Pause[0] :
                    (
                        (   p_Game->Timer->p_Stop
                        ) ? p_Game->Timer->p_Stop[0] :
                            MinSwp_Time_Now()
                    )
            ) -
            p_Game->Timer->p_Start[0];
    else
        return 0.0;
};

/*===========================================================*/

QM_VOID MinSwp_Game_Finish
    (
              MINSWP_GAME             * p_Game,
        const MINSWP_GAME_STATE         GameState
    )
{
    p_Game->Game_State = GameState;

    if (        p_Game->Timer->p_Pause)
    {   free(   p_Game->Timer->p_Pause);
                p_Game->Timer->p_Pause =
        QM_NULL;
    };

    if (QM_NULL ==      p_Game->Timer->p_Stop)
        *(    (void **)&p_Game->Timer->p_Stop) =
        malloc(  sizeof(p_Game->Timer->p_Stop[0]));
                        p_Game->Timer->p_Stop[0] = MinSwp_Time_Now();
};

/*===========================================================*/

MINSWP_CHECKBLOCK_RESULT MinSwp_Game_ReleaseBlocks
    (
              MINSWP_GAME             * p_Game,
        const QM_SLONG                  Poly_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    )
{
    MINSWP_GPOLY * p_poly =                             __qm_comment__('search blocks[index].')
        MinSwp_Geometry_Poly_GetPtr
            (
                p_Game->Geometry->Handle,
                Poly_Index
            );

    if (p_poly)
    {
        if (p_poly->Game_State == MINSWP_GPGS_START)    __qm_comment__('block is not checked.')
        {
            p_poly->Game_State = MINSWP_GPGS_FIXED;     __qm_comment__('mark to checked.')
            p_Game->Num_Fixed_Blocks++;                 __qm_comment__('increas fixed blocks number.')
            if (
                    MinSwp_Geometry_Poly_Adjacents_NumMines
                        (
                            p_Game->Geometry->Handle,
                            Poly_Index,
                            Adjacent_Type
                        ) <=
                    0
                )                                       __qm_comment__('in around of blocks was more than 0 mine.')
            {
                QM_SLONG        i;
                const
                QM_SLONG  adj_count =
                    MinSwp_Geometry_Poly_Adjacent_Num_Get
                        (
                            p_Game->Geometry->Handle,
                            Poly_Index,
                            Adjacent_Type
                        );

                for (i = 0; i < adj_count; i++)         __qm_comment__('check for release all linked blocks.')
                {
                    const
                    QM_SLONG adj_poly_index =
                        MinSwp_Geometry_Poly_Adjacent_Get
                            (
                                p_Game->Geometry->Handle,
                                Poly_Index,
                                i,
                                Adjacent_Type
                            );
                    /* if (pms[i] != NULL) */           __qm_comment__('virtual block fond in real puzzel.')
                    {
                        if (
                                p_Game->Geometry->p_Polys[adj_poly_index].Game_State ==
                                MINSWP_GPGS_START
                            )                           __qm_comment__('blocks not checked | allowed to release.')
                        {
                            if (
                                    MinSwp_Geometry_Poly_MineValue_Get
                                        (
                                            p_Game->Geometry->Handle,
                                            adj_poly_index
                                        ) ==
                                    0
                                )                       __qm_comment__('ther are not mine in the block.')
                            {
                                MinSwp_Game_ReleaseBlocks
                                    (
                                        p_Game,
                                        adj_poly_index,
                                        Adjacent_Type
                                    );                  __qm_comment__('Calling yourself (Recurcive).')
                            };
                        };
                    };
                };
            };

            if (
                    p_Game->Num_Fixed_Blocks ==
                    (
                        (QM_SLONG)p_Game->Geometry->NumPolys -
                        MinSwp_Game_NumMines_Get(p_Game->Handle)
                    )
                )
            {
                MinSwp_Game_Finish
                    (
                        p_Game,
                        MINSWP_GS_WIN
                    );
                return MINSWP_CHKBLKRES_OK_FINISHWIN;
            };

            return MINSWP_CHKBLKRES_OK_RELEASEBLOCKS;
        }
        else /* if (p_poly->Game_State != MINSWP_GPGS_START) */ __qm_comment__('block is checked.')
            return MINSWP_CHKBLKRES_ERR_BLOCKISCHECKED;
    }
    else /* if (p_poly == QM_NULL) */
        return MINSWP_CHKBLKRES_ERR_NULLPOLYPTR;
};

/*===========================================================*/

QM_VOID MinSwp_Game_ArrangeMines
    (
              HMINSWP_GAME      h_Game,
        const QM_SLONG          Excluded_Poly_Index
    )
{
    QM_SLONG    * p_vec_boom_indices,
                    vec_boom_indices_size;

    MINSWP_GAME * const p_Game =
   (MINSWP_GAME *      )h_Game;

    const QM_SLONG polys_count = p_Game->Geometry->NumPolys;

    {
        QM_SLONG i;
        for (i = 0; i < polys_count; i++)
        {
            MinSwp_MinesVal_Mine_Set
                (
                    p_Game->Geometry->p_Polys[i].MinesVal,
                    QM_FALSE
                );
        };
    };

    /*=======================================================*/
    *((void **)&p_vec_boom_indices) =
        malloc
            (
                polys_count *
                sizeof(p_vec_boom_indices[0])
            );
    /*
    vec_boom_indices_size = polys_count;
    */
    /*=======================================================*/

    {
        QM_SLONG i;
        for (i = 0; i < polys_count; i++)
        {
            p_vec_boom_indices[i] = i;
        };
    };

    {
        const QM_SLONG adj_count =
            MinSwp_Geometry_Poly_Adjacent_Num_Get
                (
                    p_Game->Geometry->Handle,
                    Excluded_Poly_Index,
                    MINSWP_ADJT_BYVERTS
                );

        p_vec_boom_indices[Excluded_Poly_Index] = -1;

        /*===================================================*/
        {
            QM_SLONG i;
            for (i = 0; i < adj_count; i++)
            {
                p_vec_boom_indices
                    [
                        MinSwp_Geometry_Poly_Adjacent_Get
                            (
                                p_Game->Geometry->Handle,
                                Excluded_Poly_Index,
                                i,
                                MINSWP_ADJT_BYVERTS
                            )
                    ] = -1;
            };
        };
        /*===================================================*/
        {
            QM_SLONG i, j;
            const QM_SLONG free_blocks_count = polys_count - (adj_count + 1);
            j = 0;
            for (i = 0; i < free_blocks_count; i++)
            {
                while (p_vec_boom_indices[j] == -1)
                {
                    j++;
                    if (j >= polys_count)
                    {
                        free(p_vec_boom_indices);
                        return ; /* error!!! */
                    };
                };
                p_vec_boom_indices[i] = p_vec_boom_indices[j];
                j++;
            };
            vec_boom_indices_size = free_blocks_count;
        };
    };
    /*=======================================================*/
    {
        QM_SLONG i;
        const QM_SLONG free_count =
            vec_boom_indices_size -
            MinSwp_Game_NumMines_Get
                (
                    p_Game->Handle
                );

        /*
        randomize();
        */
        for (i = 0; i < free_count; i++)
        {
            const QM_SLONG j = rand() % vec_boom_indices_size; /* - free_count - 1); */
            p_vec_boom_indices[j] = p_vec_boom_indices[vec_boom_indices_size - 1];
              vec_boom_indices_size--;
        };
    };
    /*=======================================================*/

    {
        QM_SLONG i;
        for (i = 0; i < vec_boom_indices_size; i++)
        {
            if (p_vec_boom_indices[i] >= (QM_SLONG)p_Game->Geometry->NumPolys )
            {
                /* ShowMessage(UnicodeString(L"OutOfRange") + vec_boom_indices[i]); */
            }
            else
                MinSwp_MinesVal_Mine_Set
                    (
                        p_Game->Geometry->p_Polys
                            [
                                p_vec_boom_indices[i]
                            ].MinesVal,
                        QM_TRUE
                    );
        };
    };

    p_Game->Mines_Arranged = QM_TRUE;
    MinSwp_Geometry_ArrangeMines_LinkedMines_Update
        (
            p_Game->Geometry
        );

    free(p_vec_boom_indices);
};

/*===========================================================*/

MINSWP_SETFLAG_RESULT MinSwp_Game_Flag_Set
    (
              HMINSWP_GAME      h_Game,
        const QM_SLONG          Poly_Index
    )
{
    MINSWP_GAME * const p_Game =
   (MINSWP_GAME *      )h_Game;

    if (p_Game->Game_State == MINSWP_GS_IN_PLAY)
    {
        MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetPtr
                (
                    p_Game->Geometry->Handle,
                    Poly_Index
                ); /* search for block. */
        if (p_poly) /* block fond. */
        {
            if (
                    (p_poly->Game_State == MINSWP_GPGS_ASSUM_MINS) && /* marked with flag. */
                    (p_Game->Num_Flaged_Blocks)
                )
            {
                p_poly->Game_State = MINSWP_GPGS_START;
                p_Game->Num_Flaged_Blocks--;
                return MINSWP_SETFLAGRES_OK_REMOVE;
            }
            else
            if (
                    (p_poly->Game_State == MINSWP_GPGS_START) && /* allowed to mark. */
                    (p_Game->Num_Flaged_Blocks < p_Game->Num_Mines)
                )
            {
                p_poly->Game_State = MINSWP_GPGS_ASSUM_MINS;
                p_Game->Num_Flaged_Blocks++;
                return MINSWP_SETFLAGRES_OK_INSTALL;
            };
            return MINSWP_SETFLAGRES_ERR_FIXEDPOLY;
        }
        else /* if (p_poly == QM_NULL) */ /* block not fond. */
            return MINSWP_SETFLAGRES_ERR_NULLPOLYPTR;
    }
    else /* if (p_Game->Game_State != MINSWP_GS_IN_PLAY) */
        return MINSWP_SETFLAGRES_ERR_OUTOFGAME;
};

QM_SLONG MinSwp_Game_Flag_SetAsHeart
    (
              MINSWP_GAME     * p_Game,
        const QM_SLONG          Poly_Index
    )
{
    if (p_Game->Game_State == MINSWP_GS_IN_PLAY)
    {
        MINSWP_GPOLY * p_poly =
            MinSwp_Geometry_Poly_GetPtr
                (
                    p_Game->Geometry->Handle,
                    Poly_Index
                ); /* search for block. */
        if (p_poly) /* block fond. */
        {
                if (p_poly->Game_State == MINSWP_GPGS_START) /* allowed to mark. */
                {
                    p_poly->Game_State = MINSWP_GPGS_HEART;
                    return 0;
                }
                else
                    return -3;
        }
        else /* if (p_poly == QM_NULL) */ /* block not fond. */
            return -2;
    }
    else /* if (p_Game->Game_State != MINSWP_GS_IN_PLAY) */
        return -1;
};

/*===========================================================*/

MINSWP_CHECKBLOCK_RESULT    MinSwp_Game_CheckBlock
    (
              HMINSWP_GAME              h_Game,
        const QM_SLONG                  Poly_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type,
              HMINSWP_LIFE              h_Life
    )
{
    MINSWP_GAME * p_Game =
   (MINSWP_GAME *)h_Game;

    if (
            (Poly_Index < 0) ||
            (Poly_Index >= (QM_SLONG)p_Game->Geometry->NumPolys )
        )
        return MINSWP_CHKBLKRES_ERR_OUTOFRANGEPOLYINDEX;
    else
    {
        if (p_Game->Game_State == MINSWP_GS_STARTED)
        {
            p_Game->Game_State =  MINSWP_GS_IN_PLAY;

            /* start timer... */
            if (QM_NULL ==      p_Game->Timer->p_Start)
                *(    (void **)&p_Game->Timer->p_Start) =
                malloc(sizeof(  p_Game->Timer->p_Start[0]));
                                p_Game->Timer->p_Start[0] = MinSwp_Time_Now();
        }
        else if (!(p_Game->Game_State == MINSWP_GS_IN_PLAY))
        {
            return MINSWP_CHKBLKRES_ERR_OUTOFGAME;
        };
        if (!p_Game->Mines_Arranged) /* not sorted mines in blocks */
        {
            MinSwp_Game_ArrangeMines
                (
                    h_Game,
                    Poly_Index
                ); /* sort mine in blocks. */
        };

        {
            MINSWP_GPOLY * p_poly =
                MinSwp_Geometry_Poly_GetPtr
                    (
                        p_Game->Geometry->Handle,
                        Poly_Index
                    ); /* focused blocks. */
            if (p_poly) /* focused blocks fond. */
            {
                if (p_poly->Game_State == MINSWP_GPGS_START) /* if focused block is not checked.*/
                {
                    if (
                            MinSwp_MinesVal_Mine_Get
                                (
                                    p_Game->Geometry->p_Polys
                                        [
                                            Poly_Index
                                        ].MinesVal
                                ) == 1
                        ) /* BOOM! (you lose). */
                    {
                        if (
                                (h_Life) &&
                                (MinSwp_Life_Take(h_Life, 1) >= 0)
                            )
                        {
                            /*
                            MinSwp_Game_Flag_Set
                                (
                                    p_Game->Handle,
                                    Poly_Index
                                );
                            */
                            MinSwp_Game_Flag_SetAsHeart
                                (
                                    p_Game,
                                    Poly_Index
                                );
                            return MINSWP_CHKBLKRES_OK_BOOMSETHEART;
                        }
                        else
                        {
                            QM_SLONG i;

                            const QM_SLONG polys_count = p_Game->Geometry->NumPolys;

                            MinSwp_Game_Finish
                                (
                                    p_Game,
                                    MINSWP_GS_LOSS
                                );

                            for (i = 0; i < polys_count; i++)
                            {
                                if (p_Game->Geometry->p_Polys[i].Game_State == MINSWP_GPGS_START)
                                {
                                    p_Game->Geometry->p_Polys[i].Game_State =
                                        (
                                            MinSwp_MinesVal_Mine_Get
                                                (
                                                    p_Game->Geometry->p_Polys[i].MinesVal
                                                ) == 1
                                        ) ?
                                            MINSWP_GPGS_BOOM :
                                            MINSWP_GPGS_FIXED;
                                };
                            };
                            return MINSWP_CHKBLKRES_OK_FINISHLOSS;
                        };
                    };

                    return
                        MinSwp_Game_ReleaseBlocks
                            (
                                p_Game,
                                Poly_Index,
                                Adjacent_Type
                            ); /* release around blocks. */
                }
                else /* if (p_poly->Game_State != MINSWP_GPGS_START) */ /* if focused block is checked.*/
                    return MINSWP_CHKBLKRES_ERR_BLOCKISCHECKED;
            }
            else /* if (p_poly == QM_NULL) */
                return MINSWP_CHKBLKRES_ERR_NULLPOLYPTR;
        };
    };
};

/*===========================================================*/

QM_SLONG MinSwp_Game_Flag_Num_Get
    (
        const HMINSWP_GAME      h_Game
    )
{
    return ((const MINSWP_GAME *)h_Game)->Num_Flaged_Blocks;
};

/*===========================================================*/

QM_SLONG MinSwp_Game_FixedBlock_Num_Get
    (
        const HMINSWP_GAME      h_Game
    )
{
    return ((const MINSWP_GAME *)h_Game)->Num_Fixed_Blocks;
};

/*===========================================================*/

MINSWP_GAME_STATE MinSwp_Game_State_Get
    (
        const HMINSWP_GAME      h_Game
    )
{
    return ((const MINSWP_GAME *)h_Game)->Game_State;
};

/*===========================================================*/


















