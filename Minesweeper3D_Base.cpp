#if (0)


#include  "Minesweeper3D_Base.h"
#include <stdlib.h>
#include <math.h>
#include "fmath3d.h"
//#include <vcl.h>



/*===========================================================*/

/*===========================*/
/*                           */
/* TMW3D_Geometry::TPoly     */
/*                           */
/*===========================*/

#if (0)
TMW3D_Geometry::TPoly::TPoly
    (
        const QM_SLONG    index
    )
{
    this__index         = index;

    MinSwp_Vec3D_SetVal
        (
            &this__sphere_pos,
            0.0f,
            0.0f,
            0.0f
        );
    this__sphere_radius             = 0.0f;
    this__game_state                = GAME_STATE::GS_START;
    this__h_verts                   = MinSwp_Indices_Create();
    this__h_verts_adjacent_polys    = MinSwp_Indices_Create();
    this__h_edges_adjacent_polys    = MinSwp_Indices_Create();
    
};

/*===========================================================*/

QM_VOID TMW3D_Geometry::TPoly::TMineValue::SetMineValue
    (
        const QM_LBOOL  value
    )
{
    if (value)
        this__val |= 0x80000000L;
    else
        this__val &= 0x7fffffffL;
};

/*===========================================================*/

QM_VOID TMW3D_Geometry::TPoly::TMineValue::SetAdjacentsMineCount
    (
        const QM_SLONG              mine_count,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    switch (adjacent_type)
    {
        case MINSWP_ADJT_BYVERTS:
            {
                this__val &= 0xffff0000L;
                this__val |= (QM_SLONG)mine_count & 0x0000ffffL;
            } break;
        case MINSWP_ADJT_BYEDGES:
            {
                this__val &= 0x8000ffffL;
                this__val |= ((QM_SLONG)mine_count << 16) & 0x7fff0000L;
            } break;
    };
};

/*===========================================================*/

QM_LBOOL TMW3D_Geometry::TPoly::TMineValue::GetMineValue(QM_VOID)
{
    return ((this__val & 0x80000000L) != 0L);
};

/*===========================================================*/

QM_SLONG TMW3D_Geometry::TPoly::TMineValue::GetAdjacentsMineCount
    (
        const MINSWP_ADJACENT_TYPE    adjacent_type
    )
{
    switch (adjacent_type)
    {
        case MINSWP_ADJT_BYVERTS:   return (QM_SLONG)( this__val & 0x0000ffffL);        /* break; */
        case MINSWP_ADJT_BYEDGES:   return (QM_SLONG)((this__val & 0x7fffffffL) >> 16); /* break; */
    default:                        return 0;
        ;
    };
    /* return 0; */
};

/*===========================================================*/

QM_VOID TMW3D_Geometry::TPoly::TMineValue::ResetAllToZero
    (
        QM_VOID
    )
{
    this__val = 0L;
};
#endif /* (0) */
/*===========================================================*/

/*============================*/
/*                            */
/* Geometry Creation Methoths */
/*                            */
/*============================*/
/*
TMW3D_Geometry::TMW3D_Geometry()
{
    this__max_adjacent_count    = 0;
    this__mines_count           = 0;
    this__mines_arranged        = QM_FALSE;
    this__game_state            = MINSWP_GAME_STATE::MINSWP_GS_VOID;
    this__fixed_block_count     = 0;
    this__flaged_block_count    = 0;
};
*/
/*===========================================================*/

TMW3D_Geometry * TMW3D_Geometry::this__CreateNew
    (
        TMW3D_Geometry             ** pp_outGeometry,
        HMINSWP_POLYMESH                h_polyMesh
    )
{
    if (h_polyMesh == QM_NULL)
        return QM_NULL;

    if (pp_outGeometry == QM_NULL)
        return QM_NULL;
    else
        *pp_outGeometry = QM_NULL;

    TMW3D_Geometry * p_geometry = new TMW3D_Geometry();
    if (
            (!TMW3D_Geometry::SetNewPolyMesh::Do(p_geometry, h_polyMesh)) ||
            (p_geometry->GetMinMinesCount() > p_geometry->GetMaxMinesCount())
        )
    {
        delete p_geometry;
        return QM_NULL;
    }
    else
    {
        *pp_outGeometry = p_geometry;
        /* p_geometry = NULL; */
    };
    return *pp_outGeometry;
};

/*===========================================================*/

QM_LBOOL TMW3D_Geometry::this__DeleteAndNULL
    (
        TMW3D_Geometry      ** pp_Geometry
    )
{
    if (pp_Geometry)
    {
        if (*pp_Geometry)
        {
            delete *pp_Geometry;
            *pp_Geometry = QM_NULL;
            return QM_TRUE;
        };
    };
    return QM_FALSE;
};

/*===========================================================*/

/*===========================*/
/*                           */
/* private methods           */
/*                           */
/*===========================*/

/*
MINSWP_VEC3D * TMW3D_Geometry::vert_GetPos
    (
        const QM_SLONG      vert_index,
              MINSWP_VEC3D  * p_out_pos
    )
{
    if (
            (vert_index < 0) ||
            (vert_index >= (QM_SLONG)this__vec_verts.size()) ||
            (p_out_pos  == QM_NULL)
        )
        return QM_NULL;

    *p_out_pos = this__vec_verts[vert_index];
    return p_out_pos;
};
*/


/*===========================================================*/

/*
HMINSWP_GPOLY TMW3D_Geometry::GetPoly
    (
        const QM_SLONG      poly_index
    )
{
    if (
            (poly_index < 0) ||
            (poly_index >= (QM_SLONG)this__vec_polys.size())
        )
        return QM_NULL;
    else
        return this__vec_polys[poly_index];
};
*/

/*===========================================================*/

/*
HMINSWP_INDICES TMW3D_Geometry::GetAdjacentIndices
    (
              HMINSWP_GPOLY         h_poly,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    if (h_poly == QM_NULL)
        return QM_NULL;
    switch (adjacent_type)
    {
        case MINSWP_ADJT_BYVERTS:   return p_poly->this__h_verts_adjacent_polys; // break;
        case MINSWP_ADJT_BYEDGES:   return p_poly->this__h_edges_adjacent_polys; // break;
    default:                        return QM_NULL;
    };
    // return QM_NULL;
};
*/

/*===========================================================*/
/*
HMINSWP_GPOLY TMW3D_Geometry::GetPolyByAdjacency
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
QM_VOID TMW3D_Geometry::SetNewPolyMesh::Fn::ClearAllAdjacency
    (
        TMW3D_Geometry  * p_geometry
    )
{
    const QM_SLONG count = p_geometry->GetPolysCount();
    for (QM_SLONG i = 0; i < count; i++)
    {
        MinSwp_GPoly_Adjacent_Clear
            (
                p_geometry->p_pthis__vec_polys[i]
            );
    };
};
*/

/*===========================================================*/
/*
QM_LBOOL TMW3D_Geometry::poly_SetVerts(HMINSWP_GPOLY h_poly, HMINSWP_INDICES h_source)
{
    if (
            (h_poly     == QM_NULL) ||
            (h_source   == QM_NULL)
        )
        return QM_FALSE;

    MinSwp_Indices_Copy
        (
            p_poly->this__h_verts,
            h_source
        );
    this->poly_UpdateSphere(p_poly);
    return QM_TRUE;
};
*/
/*===========================================================*/

/*
QM_LBOOL TMW3D_Geometry::poly_AddAdjacent
    (
              TPoly               * p_poly,
        const QM_SLONG              new_adjacent_index,
        const MINSWP_ADJACENT_TYPE  adjacent_type
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
            MinSwp_Indices_AddIndex(h_i, new_adjacent_index);
            return QM_TRUE;
        };
    };
    return QM_FALSE;
};
*/

/*===========================================================*/

/*
QM_VOID TMW3D_Geometry::poly_SetMineValue
    (
              TPoly       * p_poly,
        const QM_LBOOL      value
    )
{
    if (p_poly == QM_NULL) return ;
    p_poly->this__mines.SetMineValue(value);
};
*/
/*===========================================================*/

/*
QM_VOID TMW3D_Geometry::poly_SetAdjacentsMineCount
    (
              TPoly               * p_poly,
        const QM_SLONG              mine_count,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    if (p_poly == QM_NULL) return ;
    p_poly->this__mines.SetAdjacentsMineCount
        (
            mine_count,
            adjacent_type
        );
};
*/

/*===========================================================*/

/*
QM_VOID TMW3D_Geometry::poly_UpdateSphere
    (
        TPoly * p_poly
    )
{
    if (p_poly == QM_NULL) return ;
    QM_FLOAT dist = -1.0f;
    std::vector< MINSWP_VEC3D > vec3s;
    const QM_SLONG count = MinSwp_Indices_GetCount(p_poly->this__h_verts);
    vec3s.resize(count);
    for (QM_SLONG i = 0; i < count; i++)
    {
        this->vert_GetPos
            (
                MinSwp_Indices_GetIndex(p_poly->this__h_verts, i),
                &vec3s[i]
            );
    };

    const
    QM_SLONG count_1 = count - 1;
    QM_SLONG index_a = 0, index_b = 0;
    for (QM_SLONG i = 0; i < count_1; i++)
    {
        for (QM_SLONG j = i + 1; j < count; j++)
        {
            const QM_FLOAT d =
                FM3DVec3DistanceSqr_f32
                    (
                        vec3s[i].f,
                        vec3s[j].f
                    );
            if (d > dist)
            {
                dist    = d;
                index_a = i;
                index_b = j;
            };
        };
    };
    p_poly->this__sphere_radius = sqrt(dist);
    FM3DVec3Lerp_f32
        (
            p_poly->this__sphere_pos.f,
            vec3s[index_a].f,
            vec3s[index_b].f,
            0.5f
        );
};
*/

/*===========================================================*/

/*
QM_LBOOL TMW3D_Geometry::poly_IntersectSphereByRay
    (
              TPoly         * p_poly,
        const MINSWP_RAY3D  * p_ray,
              QM_FLOAT      * p_out_distance
    )
{
    return
        FM3DRay3IntersectSphere_f32
            (
                p_out_distance,
               &p_ray->Pos.x,
               &p_ray->Dir.x,
               &p_poly->this__sphere_pos.x,
                p_poly->this__sphere_radius,
                FM3D_FALSE
            );
};
*/

/*===========================================================*/
/*
QM_LBOOL TMW3D_Geometry::poly_IntersectTrianglesByRay
    (
              TPoly         * p_poly,
        const MINSWP_RAY3D  * p_ray,
              QM_FLOAT      * p_out_distance
    )
{
    HMINSWP_TRIMESH h_mesh = QM_NULL;
    this->poly_GetTriMesh
        (
            p_poly->this__index,
            &h_mesh
        );

    const QM_SLONG tri_count = (QM_SLONG)MinSwp_TriMesh_Faces_Size(h_mesh);
    for (QM_SLONG i = 0; i < tri_count; i++)
    {
        const MINSWP_TRIINDICES * p_face_i = MinSwp_TriMesh_Faces_Get(h_mesh, i);
        const QM_FLOAT * p1 = MinSwp_TriMesh_Verts_Get(h_mesh, p_face_i->i1)->f;
        const QM_FLOAT * p2 = MinSwp_TriMesh_Verts_Get(h_mesh, p_face_i->i2)->f;
        const QM_FLOAT * p3 = MinSwp_TriMesh_Verts_Get(h_mesh, p_face_i->i3)->f;

        if (
                FM3DRay3IntersectTrianglePD_f32
                    (
                        p_ray->Pos.f,
                        p_ray->Dir.f,
                        p1,
                        p2,
                        p3
                    )
            )
        {
            if (p_out_distance)
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

                *p_out_distance =
                    FM3DRay3IntersectPlaneDepth_f32
                        (
                            NULL,
                            p_ray->Pos.f,
                            p_ray->Dir.f,
                            p1,
                            vN,
                            FM3D_FALSE
                        );
            };
            return QM_TRUE;
        };
    };
    if (h_mesh)
    {
        MinSwp_TriMesh_Release(h_mesh);
        h_mesh = QM_NULL;
    };
    return QM_FALSE;
};
*/
/*===========================================================*/

/*===========================*/
/*                           */
/* public methods            */
/*                           */
/*===========================*/
/*
QM_SLONG TMW3D_Geometry::poly_GetVertsCount
    (
        const QM_SLONG  poly_index
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == QM_NULL)
        return -1;
    return MinSwp_Indices_GetCount(p_poly->this__h_verts);
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::poly_GetVertIndex
    (
        const QM_SLONG      poly_index,
        const QM_SLONG      index
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == NULL)
        return -1;
    return MinSwp_Indices_GetIndex(p_poly->this__h_verts, index);
};
*/

/*===========================================================*/
/*
MINSWP_VEC3D * TMW3D_Geometry::poly_GetVertPos
    (
        const QM_SLONG      poly_index,
        const QM_SLONG      vert_index,
              MINSWP_VEC3D  * p_out_pos
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (
            (p_poly == NULL) ||
            (vert_index < 0) ||
            (vert_index >= MinSwp_Indices_GetCount(p_poly->this__h_verts)) ||
            (p_out_pos == QM_NULL)
        )
        return QM_NULL;
    p_out_pos[0] =
        this__vec_verts
            [
                MinSwp_Indices_GetIndex(p_poly->this__h_verts, vert_index)
            ];
    return p_out_pos;
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::poly_GetAdjacentCount
    (
        const QM_SLONG              poly_index,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == QM_NULL)
        return -1;

    const HMINSWP_INDICES h_i =
        GetAdjacentIndices
            (
                p_poly,
                adjacent_type
            );
    if (h_i == QM_NULL)
        return -1;

    return MinSwp_Indices_GetCount(h_i);
};
*/
/*===========================================================*/

/*
QM_SLONG TMW3D_Geometry::poly_GetAdjacent(const QM_SLONG poly_index, const QM_SLONG adjacent_index, const MINSWP_ADJACENT_TYPE adjacent_type)
{
    HMINSWP_INDICES h_i = GetAdjacentIndices(this->GetPoly(poly_index), adjacent_type);
    if (h_i == NULL)
        return -1;

    return MinSwp_Indices_GetIndex(h_i, adjacent_index);
};
*/
/*===========================================================*/
/*
QM_LBOOL TMW3D_Geometry::poly_IsAdjacent
    (
        const QM_SLONG              polyA_index,
        const QM_SLONG              polyB_index,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    HMINSWP_INDICES h_i =
        this->GetAdjacentIndices
            (
                this->GetPoly(polyA_index),
                adjacent_type
            );
    if (h_i == QM_NULL)
        return QM_FALSE;
    return MinSwp_Indices_IsMember(h_i, polyB_index);
};
*/
/*===========================================================*/
/*
QM_VOID TMW3D_Geometry::poly_DetermineAdjacency
    (
        const QM_SLONG      polyA_index,
        const QM_SLONG      polyB_index
    )
{
    TMW3D_Geometry::TPoly * p_polyA = this->GetPoly(polyA_index);
    TMW3D_Geometry::TPoly * p_polyB = this->GetPoly(polyB_index);
    if (
            (p_polyA == NULL) ||
            (p_polyB == NULL)
        )
        return ;

    const
    QM_SLONG polyA_verts_count = MinSwp_Indices_GetCount(p_polyA->this__h_verts);
    QM_SLONG overlap = 0;
    for (QM_SLONG i = 0; i < polyA_verts_count; i++)
    {
        if (
                MinSwp_Indices_IsMember
                    (
                        p_polyB->this__h_verts,
                        MinSwp_Indices_GetIndex
                            (
                                p_polyA->this__h_verts,
                                i
                            )
                    )
            )
            overlap++ ;
    };
    if (overlap > 0)
    {
        MinSwp_Indices_AddIndex(p_polyA->this__h_verts_adjacent_polys, polyB_index);
        MinSwp_Indices_AddIndex(p_polyB->this__h_verts_adjacent_polys, polyA_index);
    };
    if (overlap > 1)
    {
        MinSwp_Indices_AddIndex(p_polyA->this__h_edges_adjacent_polys, polyB_index);
        MinSwp_Indices_AddIndex(p_polyB->this__h_edges_adjacent_polys, polyA_index);
    };
};
*/
/*===========================================================*/
/*
QM_LBOOL TMW3D_Geometry::poly_GetMineValue
    (
        const QM_SLONG      poly_index
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == QM_NULL)
        return QM_FALSE;
    return p_poly->this__mines.GetMineValue();
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::poly_GetAdjacentsMineCount
    (
        const QM_SLONG              poly_index,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == QM_NULL)
        return -1;
    return p_poly->this__mines.GetAdjacentsMineCount(adjacent_type);
};
*/
/*===========================================================*/
/*
QM_FLOAT TMW3D_Geometry::poly_GetSphereRadius
    (
        const QM_SLONG      poly_index
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == QM_NULL)
        return 0.0f;
    return p_poly->this__sphere_radius;
};
*/
/*===========================================================*/
/*
MINSWP_VEC3D * TMW3D_Geometry::poly_GetSpherePos
    (
        const QM_SLONG          poly_index,
              MINSWP_VEC3D    * p_out_pos
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (
            (p_poly     == QM_NULL) ||
            (p_out_pos  == QM_NULL)
        )
        return QM_NULL;
    p_out_pos[0] = p_poly->this__sphere_pos;
    return p_out_pos;
};
*/
/*===========================================================*/
/*
HMINSWP_TRIMESH  TMW3D_Geometry::poly_GetTriMesh
    (
        const QM_SLONG                  poly_index,
              HMINSWP_TRIMESH         * ph_mesh
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);

    if (p_poly ==   QM_NULL)
        return      QM_NULL;
    if (ph_mesh ==  QM_NULL)
        return      QM_NULL;

    *ph_mesh = MinSwp_TriMesh_Create();
    const QM_SLONG verts_count = MinSwp_Indices_GetCount(p_poly->this__h_verts);
    const QM_SLONG faces_count = verts_count - 2;
    MinSwp_TriMesh_Verts_Resize(ph_mesh[0], verts_count);
    MinSwp_TriMesh_Faces_Resize(ph_mesh[0], faces_count);
    for (QM_SLONG i = 0; i < verts_count; i++)
    {
        MinSwp_TriMesh_Verts_Get(ph_mesh[0], i)[0] = this__vec_verts[MinSwp_Indices_GetIndex(p_poly->this__h_verts, i)];
    };
    for (QM_SLONG i = 0; i < faces_count; i++)
    {
        MinSwp_TriIndices_SetVal
            (
                MinSwp_TriMesh_Faces_Get(ph_mesh[0], i),
                0,
                1 + i,
                2 + i
            );
    };
    return ph_mesh[0];
};
*/
/*===========================================================*/
/*
MINSWP_DRAW_TYPE TMW3D_Geometry::poly_GetDrawType
    (
        const QM_SLONG              poly_index,
        const MINSWP_ADJACENT_TYPE  adjacent_type
    )
{
    TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == QM_NULL)
        return MINSWP_DT_FREE;

    switch (p_poly->this__game_state)
    {
        case TMW3D_Geometry::TPoly::GS_START:       return MINSWP_DT_START;   // break;
        case TMW3D_Geometry::TPoly::GS_FIXED:       return MINSWP_DRAW_TYPE
                                                            (
                                                                this->poly_GetAdjacentsMineCount
                                                                    (
                                                                        poly_index,
                                                                        adjacent_type
                                                                    )
                                                            );                // break;
        case TMW3D_Geometry::TPoly::GS_ASSUM_MINS:  return MINSWP_DT_FLAG;    // break;
        case TMW3D_Geometry::TPoly::GS_BOOM:        return MINSWP_DT_BOOM;    // break;
    default:                                        return MINSWP_DT_FREE;
    };
    // return MW3D_DT_FREE;
};
*/
/*===========================================================*/

/******************/
/* TMW3D_Geometry */
/******************/
/*
QM_SLONG TMW3D_Geometry::GetMinMinesCount(QM_VOID)
{
    return 10;
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::GetMaxMinesCount(QM_VOID)
{
    QM_SLONG a = this->GetPolysCount() - (this__max_adjacent_count + 1 + 10);
    QM_SLONG b = (QM_SLONG)( ((QM_FLOAT)this->GetPolysCount()) / 1.1f);
    return (a < b) ? a : b;
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::GetMinesCount(QM_VOID)
{
    return this__mines_count;
};
*/
/*===========================================================*/
/*
QM_LBOOL TMW3D_Geometry::SetNewPolyMesh::Do
    (
        TMW3D_Geometry              * p_geometry,
        HMINSWP_POLYMESH              h_polyMesh
    )
{
    if (h_polyMesh == QM_NULL)
    {
        return QM_FALSE;
    };
    const QM_SLONG verts_count = (QM_SLONG)MinSwp_PolyMesh_Verts_Size(h_polyMesh);
    const QM_SLONG polys_count = (QM_SLONG)MinSwp_PolyMesh_Polys_Size(h_polyMesh);

    if (
            (verts_count == 0) ||
            (polys_count == 0)
        )
    {
        return QM_FALSE;
    };
    HMINSWP_INDICES h_poly;
    QM_SLONG count;
    QM_SLONG index;
    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        h_poly = MinSwp_PolyMesh_Polys_Get(h_polyMesh, i);
        count  = MinSwp_Indices_GetCount(h_poly);
        if (count == 0)
        {
            return QM_FALSE;
        };
        for (QM_SLONG j = 0; j < count; j++)
        {
            index = MinSwp_Indices_GetIndex(h_poly, j);
            if ((index < 0) || (index >= verts_count))
            {
                return QM_FALSE;
            };
        };
    };

    //============================
    //                            
    // true polygon               
    //                            
    //============================

    {
        // p_geometry->this__vec_verts = p_polyMesh->Verts;
        const QM_ULONG num_verts = MinSwp_PolyMesh_Verts_Size(h_polyMesh);
        p_geometry->this__vec_verts.resize(num_verts);
        for (QM_ULONG i = 0; i < num_verts; i++)
        {
            p_geometry->this__vec_verts[i] = MinSwp_PolyMesh_Verts_Get(h_polyMesh, i)[0];
        };
    };

    QM_SLONG old_polys_count = (QM_SLONG)p_geometry->this__vec_polys.size();
    for (QM_SLONG i = 0; i < old_polys_count; i++)
    {
        delete p_geometry->this__vec_polys[i];
        p_geometry->this__vec_polys[i] = QM_NULL;
    };

    p_geometry->this__vec_polys.resize(polys_count, QM_NULL);
    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        p_geometry->this__vec_polys[i] = new TPoly(i);
    };

    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        p_geometry->poly_SetVerts
            (
                p_geometry->this__vec_polys[i],
                MinSwp_PolyMesh_Polys_Get(h_polyMesh, i)
            );
    };

    p_geometry->this__mines_arranged        = QM_FALSE;
    p_geometry->this__game_state            = MINSWP_GS_VOID;
    p_geometry->this__fixed_block_count     = 0;
    p_geometry->this__flaged_block_count    = 0;
    p_geometry->this__max_adjacent_count    = 0;
    TMW3D_Geometry::SetNewPolyMesh::Fn::GenerateAdjacency(p_geometry);
    return QM_TRUE;
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::GetTrueMinesCount(const QM_SLONG mines_count)
{
    QM_SLONG min_count = this->GetMinMinesCount();
    QM_SLONG max_count = this->GetMaxMinesCount();
    QM_SLONG val = mines_count;
    if (val < min_count)
        val = min_count;
    if (val > max_count)
        val = max_count;
    return val;
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::GetVertsCount()
{
    return (QM_SLONG)this__vec_verts.size();
};

QM_SLONG TMW3D_Geometry::GetPolysCount()
{
    return (QM_SLONG)this__vec_polys.size();
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::GetIntersectedPolyByRay(const MINSWP_RAY3D * p_ray)
{
    QM_SLONG poly_count = this->GetPolysCount();
    if (poly_count == 0) return -1;
    MINSWP_VEC3D sphere_pos;
    float sphere_radius;
    std::vector< QM_SLONG   >   vec_sph_indx;
    std::vector< float >   vec_sph_dist;
    float len;

    MINSWP_RAY3D ray = *p_ray;
    FM3DVec3Normalize_f32(&ray.Dir.x, &ray.Dir.x);

    for (QM_SLONG i = 0; i < poly_count; i++)
    {
        if (this->poly_IntersectSphereByRay(
            this__vec_polys[i],
            &ray,
            &len))
        {
            vec_sph_dist.push_back(len);
            vec_sph_indx.push_back(i);
        };
    };

    QM_SLONG sph_count = (QM_SLONG)vec_sph_dist.size();
    if (sph_count == 0)
    {
        return -1;
    };

    QM_SLONG sph_count_1 = sph_count - 1;
    for (QM_SLONG i = 0; i < sph_count_1; i++)
    {
        for (QM_SLONG j = i + 1; j < sph_count; j++)
        {
            if (vec_sph_dist[i] > vec_sph_dist[j])
            {
                float local_f = vec_sph_dist[i];
                vec_sph_dist[i] = vec_sph_dist[j];
                vec_sph_dist[j] = local_f;
                QM_SLONG local_i = vec_sph_indx[i];
                vec_sph_indx[i] = vec_sph_indx[j];
                vec_sph_indx[j] = local_i;
            };
        };
    };

    QM_SLONG ret_index = -1;
    float dist = 1.0e16;
    float dist_add_rad;
    for (QM_SLONG i = 0; i < sph_count; i++)
    {
        this->poly_GetSpherePos(vec_sph_indx[i], &sphere_pos);
        sphere_radius = this->poly_GetSphereRadius(vec_sph_indx[i]);
        dist_add_rad = dist + sphere_radius;
        if (FM3DVec3DistanceSqr_f32(
                        &ray.Pos.x,
                        &sphere_pos.x) <=
                    (dist_add_rad * dist_add_rad)
                    ) // sphere baedi kheili doortar hast
        {
            if (this->poly_IntersectTrianglesByRay(
                    this__vec_polys[vec_sph_indx[i]],
                    &ray,
                    &len))
            {
                if (len < dist)
                {
                    dist = len;
                    ret_index = vec_sph_indx[i];
                };
            };

        };
    };
    return ret_index;
};
*/
/*===========================================================*/
/*
QM_LBOOL TMW3D_Geometry::RestartCurrentGame()
{
    if (this__game_state == MINSWP_GS_VOID)
        return QM_FALSE;
    QM_SLONG poly_count = this->GetPolysCount();
    if (poly_count <= 0)
        return QM_FALSE;

    for (QM_SLONG i = 0; i < poly_count; i++)
    {
        this__vec_polys[i]->this__game_state = TMW3D_Geometry::TPoly::GS_START;
    };
    this__game_state             = MINSWP_GAME_STATE::MINSWP_GS_STARTED;
    this__flaged_block_count     = 0;
    this__fixed_block_count      = 0;
    return QM_TRUE;
};
*/
/*===========================================================*/
/*
QM_LBOOL TMW3D_Geometry::StartNewGame(const QM_SLONG mines_count)
{
    QM_SLONG poly_count = this->GetPolysCount();
    if (poly_count <= 0)
        return QM_FALSE;

    for (QM_SLONG i = 0; i < poly_count; i++)
    {
        this__vec_polys[i]->this__game_state = TMW3D_Geometry::TPoly::GS_START;
        this__vec_polys[i]->this__mines.ResetAllToZero();
    };

    if (mines_count != 0)
    {
        this__mines_count = this->GetTrueMinesCount(mines_count);
    }
    else
    {
        if (this__mines_count == 0)
        {
            this__mines_count = this->GetTrueMinesCount(this->GetPolysCount() / 5);
        };
    };
    this__mines_arranged          = QM_FALSE;
    this__flaged_block_count      = 0;
    this__fixed_block_count       = 0;
    this__game_state              = MINSWP_GAME_STATE::MINSWP_GS_STARTED;
    return QM_TRUE;
};
*/
/*===========================================================*/

/*
QM_VOID TMW3D_Geometry::StartGame(const QM_SLONG poly_index, const QM_SLONG mines_count) // if (mines_count == 0) do_not_reArrange_mines;
{
    TMW3D_Geometry::TPoly * p_poly = this->GetPoly(poly_index);
    if (p_poly == NULL) return ;
    std::vector< QM_SLONG > vec_index;
    QM_SLONG count = this->GetPolysCount();
    for (QM_SLONG i = 0; i < count; i++)
    {
        if (i != poly_index)
        {
            if (!this->poly_IsAdjacent(poly_index, i, MW3D_ADJT_BYVERTS))
            {
                vec_index.push_back(i);
            };
        };
    };

    count = this->GetMineCount();
    randomize();

    for (QM_SLONG i = 0; i < count; i++)
    {
        random()
    };
};
*/

/*===========================================================*/
/*
QM_VOID TMW3D_Geometry::SetNewPolyMesh::Fn::GenerateAdjacency(TMW3D_Geometry * p_geometry)
{
    QM_SLONG polys_count = p_geometry->GetPolysCount();

    //TMW3D_Geometry::SetNewPolyMesh::Fn::
    ClearAllAdjacency(p_geometry);

    for (QM_SLONG i = 0; i < polys_count - 1; i++)
    {
        for (QM_SLONG j = i + 1; j < polys_count; j++)
        {
            p_geometry->poly_DetermineAdjacency(i, j);
        };
    };
    //-----------------------------------------
    QM_SLONG max_adjacent_count = 0;
    QM_SLONG x;
    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        x = MinSwp_Indices_GetCount
            (
                p_geometry->this__vec_polys[i]->this__h_verts_adjacent_polys
            );
        if (x > max_adjacent_count)
            max_adjacent_count = x;
    };
    p_geometry->this__max_adjacent_count = max_adjacent_count;
};
*/
/*===========================================================*/
/*
QM_VOID TMW3D_Geometry::ArrangeMines::Fn::UpdateLinkedMines(TMW3D_Geometry * p_geometry)
{
    QM_SLONG polys_count = p_geometry->GetPolysCount();
    MINSWP_ADJACENT_TYPE adjacent_types[2] = {MINSWP_ADJT_BYVERTS, MINSWP_ADJT_BYEDGES};
    TMW3D_Geometry::TPoly * p_poly; // = NULL;
    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        QM_SLONG adjacent_count[2] =
        {
            p_geometry->poly_GetAdjacentCount(i, adjacent_types[0]),
            p_geometry->poly_GetAdjacentCount(i, adjacent_types[1])
        };

        QM_SLONG poly_index;
        p_poly = p_geometry->GetPoly(i);
        for (QM_SLONG k = 0; k < 2; k++)
        {
            QM_SLONG mine_val = 0;
            for (QM_SLONG j = 0; j < adjacent_count[k]; j++)
            {
                if (p_geometry->poly_GetMineValue(
                    p_geometry->poly_GetAdjacent(i, j, adjacent_types[k])))
                    mine_val++;
            };
            p_geometry->poly_SetAdjacentsMineCount(p_poly, mine_val, adjacent_types[k]);
        };
    };
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::ReleaseBlocks::Do(TMW3D_Geometry * p_geometry, const QM_SLONG poly_index, const MINSWP_ADJACENT_TYPE adjacent_type)
{
    TMW3D_Geometry::TPoly * p_poly = p_geometry->GetPoly(poly_index); // search blocks[index].
    if (p_poly == NULL) return QM_FALSE;
    if (p_poly->this__game_state != TMW3D_Geometry::TPoly::GS_START) // block is checked.
    {
        return QM_FALSE;
    };
    p_poly->this__game_state = TMW3D_Geometry::TPoly::GS_FIXED; // mark to checked.
    p_geometry->this__fixed_block_count++; // increas fixed blocks number.
    if (!(p_geometry->poly_GetAdjacentsMineCount(poly_index, adjacent_type) > 0)) // in around of blocks was mor than 0 mine.
    //{
    //    return QM_TRUE;
    //};
    {
        TMW3D_Geometry::TPoly * p_adj_poly;
        QM_SLONG adj_count = p_geometry->poly_GetAdjacentCount(poly_index, adjacent_type);
        for (QM_SLONG i = 0; i < adj_count; i++) // check for release all linked blocks.
        {
            //p_adj_poly =
            QM_SLONG adj_poly_index = p_geometry->poly_GetAdjacent(poly_index, i, adjacent_type);
            //if (pms[i] != NULL) // virtual block fond in real puzzel.
            {
                if (p_geometry->this__vec_polys[adj_poly_index]->this__game_state == TMW3D_Geometry::TPoly::GS_START) // blocks not checked | allowed to release.
                {
                    if (p_geometry->poly_GetMineValue(adj_poly_index) == 0) // ther are not mine in the block.
                    {
                        //ReleaseBlocks::
                        Do(p_geometry, adj_poly_index, adjacent_type);
                    };
                };
            };
        };
    };
    if (p_geometry->this__fixed_block_count == ((QM_SLONG)p_geometry->this__vec_polys.size() - p_geometry->GetMinesCount()))
    {
        p_geometry->this__game_state = MINSWP_GS_WIN;
    };
    return QM_TRUE;
};
*/
/*===========================================================*/
#if (0)
QM_VOID TMW3D_Geometry::ArrangeMines::Do
    (
              TMW3D_Geometry  * p_geometry,
        const QM_SLONG          excluded_poly_index
    )
{
    std::vector< QM_SLONG >  vec_boom_indices;
    QM_SLONG polys_count = p_geometry->this__vec_polys.size();
    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        p_geometry->poly_SetMineValue
            (
                p_geometry->this__vec_polys[i],
                QM_FALSE
            );
    };
    /*
    ShowMessage(L"After poly_SetMineValue 1");
    */
    vec_boom_indices.resize(polys_count);
    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        vec_boom_indices[i] = i;
    };

    QM_SLONG adj_count = p_geometry->poly_GetAdjacentCount(excluded_poly_index, MINSWP_ADJT_BYVERTS);
    vec_boom_indices[excluded_poly_index] = -1;
    for (QM_SLONG i = 0; i < adj_count; i++)
    {
        vec_boom_indices[p_geometry->poly_GetAdjacent(excluded_poly_index, i, MINSWP_ADJT_BYVERTS)] = -1;
        //ShowMessage(L"poly_GetAdjacent : " + IntToStr(p_geometry->poly_GetAdjacent(excluded_poly_index, i, MW3D_ADJT_BYVERTS)));
    };




    /*
    UnicodeString s;
    for (QM_SLONG i = 0; i < polys_count; i++)
    {
        s = s + IntToStr(vec_boom_indices[i]) + L"    ";
    };
    ShowMessage(s);
    */





    QM_SLONG j = 0;
    QM_SLONG free_blocks_count = polys_count - (adj_count + 1);
    for (QM_SLONG i = 0; i < free_blocks_count; i++)
    {
        while (vec_boom_indices[j] == -1)
        {
            j++;
            if (j >= polys_count)
            {
                return ; // error!!!
            };
        };
        vec_boom_indices[i] = vec_boom_indices[j];
        j++;
    };
    vec_boom_indices.resize(free_blocks_count);

    /*
    for (QM_SLONG i = 0; i < vec_boom_indices.size(); i++)
    {
        if (vec_boom_indices[i] == -1)
        {
            ShowMessage(L"i == -1 " + IntToStr(i));
        };

    };
    */
    
    QM_SLONG free_count = vec_boom_indices.size() - p_geometry->GetMinesCount();
    randomize();
    QM_SLONG count;
    for (QM_SLONG i = 0; i < free_count; i++)
    {
        count = vec_boom_indices.size();
        j = random(count);// - free_count - 1);
        vec_boom_indices[j] = vec_boom_indices[count - 1];
        vec_boom_indices.resize(count - 1);
    };

    /*
    UnicodeString boom_s;
    for (QM_SLONG i = 0; i < vec_boom_indices.size(); i++)
    {
        boom_s = boom_s + L"        " + vec_boom_indices[i];
    };
    ShowMessage(boom_s);
    */
    
    // ShowMessage(L"Befor poly_SetMineValue 2");
    count = (QM_SLONG)vec_boom_indices.size();
    for (QM_SLONG i = 0; i < count; i++)
    {
        if (vec_boom_indices[i] >= (QM_SLONG)(p_geometry->this__vec_polys.size()) )
        {
            // ShowMessage(UnicodeString(L"OutOfRange") + vec_boom_indices[i]);
        }
        else
            p_geometry->poly_SetMineValue(p_geometry->this__vec_polys[vec_boom_indices[i]], QM_TRUE);
    };
    // ShowMessage(L"After poly_SetMineValue 2");
    p_geometry->this__mines_arranged = QM_TRUE;
    TMW3D_Geometry::ArrangeMines::Fn::UpdateLinkedMines(p_geometry);
};
#endif /* (0) */
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::game_CheckBlock
    (
        const QM_SLONG                  poly_index,
        const MINSWP_ADJACENT_TYPE      adjacent_type,
              HMINSWP_LIFE              h_life
    )
{
    if ((poly_index < 0) ||
        (poly_index >= (QM_SLONG)(this__vec_polys.size()) ))
        return -1;
    if (this__game_state == MINSWP_GAME_STATE::MINSWP_GS_STARTED)
    {
        this__game_state =  MINSWP_GAME_STATE::MINSWP_GS_IN_PLAY;
        // start timer...
    }
    else if (!(this__game_state == MINSWP_GAME_STATE::MINSWP_GS_IN_PLAY))
    {
        return -1;
    };
    if (!this__mines_arranged) // not sorted mines in blocks
    {
        // ShowMessage(L"Befor ArrangeMines::Do");
        TMW3D_Geometry::ArrangeMines::Do
            (
                this,
                poly_index
            ); // sort mine in blocks.
        // ShowMessage(L"After ArrangeMines::Do");
    };
    TMW3D_Geometry::TPoly * p_poly = this->GetPoly(poly_index); // focused blocks.
    if (p_poly == NULL) // fcused blocks not fond.
    {
        return 0;
    };
    if (p_poly->this__game_state != TMW3D_Geometry::TPoly::GS_START) // fcused block if checked.
    {
        return 0;
    };
    if (this->poly_GetMineValue(poly_index) == 1) // BOOM! (you lose).
    {
        if (
                (h_life) &&
                (MinSwp_Life_Take(h_life, 1) >= 0)
            )
        {
            this->game_SetFlag(poly_index);
            return 0;
        }
        else
        {
            this__game_state = MINSWP_GS_LOSS;
            QM_SLONG polys_count = (QM_SLONG)this__vec_polys.size();
            for (QM_SLONG i = 0; i < polys_count; i++)
            {
                if (this__vec_polys[i]->this__game_state == TMW3D_Geometry::TPoly::GS_START)
                {
                    if (this->poly_GetMineValue(i) == 1)
                    {
                        this__vec_polys[i]->this__game_state = TMW3D_Geometry::TPoly::GS_BOOM;
                    }
                    else
                    {
                        this__vec_polys[i]->this__game_state = TMW3D_Geometry::TPoly::GS_FIXED;
                    };
                };
            };
            return -1;
        };
    }
    return
        TMW3D_Geometry::ReleaseBlocks::Do
            (
                this,
                poly_index,
                adjacent_type
            ); // release around blocks.
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::game_SetFlag
    (
        const QM_SLONG      poly_index
    )
{
    if (this__game_state != MINSWP_GS_IN_PLAY)
        return -1;

    TMW3D_Geometry::TPoly * p_poly = this->GetPoly(poly_index); // search for block.
    if (p_poly == NULL) // block is not fond;
    {
        return 0;
    };
    if (p_poly->this__game_state == TMW3D_Geometry::TPoly::GS_ASSUM_MINS) // marked with flag.
    {
        p_poly->this__game_state = TMW3D_Geometry::TPoly::GS_START;
        this__flaged_block_count--;
    }
    else if (p_poly->this__game_state == TMW3D_Geometry::TPoly::GS_START) // allowed to mark.
    {
        p_poly->this__game_state = TMW3D_Geometry::TPoly::GS_ASSUM_MINS;
        this__flaged_block_count++;
    };
    return 0;
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::GetFlagCount(QM_VOID)
{
    return this__flaged_block_count;
};
*/
/*===========================================================*/
/*
QM_SLONG TMW3D_Geometry::GetFixedBlockCount(QM_VOID)
{
    return this__fixed_block_count;
};
*/
/*===========================================================*/
/*
MINSWP_GAME_STATE TMW3D_Geometry::GetGameState(QM_VOID)
{
    return this__game_state;
};
*/
/*===========================================================*/

#endif /* (0) */








