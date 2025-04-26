#if (0)

/*===========================================================*/
/*                                                           */
/*                Besmelaherrahmanerrahim                    */
/*                                                           */
/*                                                           */
/*                                                           */
/*===========================================================*/

#ifndef __MINESWEEPER3D_BASE_H__DEFINED
#define __MINESWEEPER3D_BASE_H__DEFINED

/*===========================================================*/

#include <vector>

/*===========================================================*/

#include "minswp_base.h"

/*===========================================================*/

#include "minswp_indices.h"
#include "minswp_polymesh.h"
#include "minswp_trimesh.h"
#include "minswp_life.h"
#include "minswp_game.h"


/*===========================================================*/


/*===========================================================*/

typedef class TMW3D_Geometry
{
private:
    /*
    typedef class TPoly
    {
    public:
        enum GAME_STATE
        {
            GS_START          = 0, // NOT PRESSED BUTTON
            GS_FIXED          = 1, // PRESSED BUTTON AND WAS FREE
            GS_ASSUM_MINS     = 2, // USER ASSUMED THIS BLOCK IS MINE
            GS_BOOM           = 3, // PRESSED BUTTON AND WAS MINE!!!!
            GS____FORCE_32BIT = 0x7fffffffL
        } this__game_state;

        class TMineValue
        {
        private:
            QM_SLONG            this__val;
        public:
            TMineValue(): this__val(0L) {};
            QM_VOID     ResetAllToZero();
            QM_VOID     SetMineValue(const QM_LBOOL value);

            QM_VOID     SetAdjacentsMineCount
                (
                    const QM_SLONG              mine_count,
                    const MINSWP_ADJACENT_TYPE  adjacent_type
                );

            QM_LBOOL    GetMineValue(QM_VOID);
            QM_SLONG    GetAdjacentsMineCount
                (
                    const MINSWP_ADJACENT_TYPE  adjacent_type
                );
        } this__mines;

        QM_SLONG                this__index;
        HMINSWP_INDICES         this__h_verts;
        HMINSWP_INDICES         this__h_verts_adjacent_polys;
        HMINSWP_INDICES         this__h_edges_adjacent_polys;
        MINSWP_VEC3D            this__sphere_pos;
        QM_FLOAT                this__sphere_radius;
        TPoly(const QM_SLONG index);
    } * PPoly;

    */

    /*
    std::vector< MINSWP_VEC3D  >    this__vec_verts;
    std::vector< HMINSWP_GPOLY >    this__vec_polys;

    QM_SLONG                        this__mines_count;
    QM_LBOOL                        this__mines_arranged;
    MINSWP_GAME_STATE               this__game_state;
    QM_SLONG                        this__fixed_block_count;
    QM_SLONG                        this__flaged_block_count;
    QM_SLONG                        this__max_adjacent_count;
    */

    TMW3D_Geometry();//TMW3D_BasicPolyMesh * p_polyMesh);
public:
    static TMW3D_Geometry *     this__CreateNew
        (
            TMW3D_Geometry           ** pp_outGeometry,
            HMINSWP_POLYMESH            h_polyMesh
        );
    static QM_LBOOL             this__DeleteAndNULL
        (
            TMW3D_Geometry           ** pp_Geometry
        );



    QM_SLONG                    game_CheckBlock
        (
            const QM_SLONG              poly_index,
            const MINSWP_ADJACENT_TYPE  adjacent_type,
                  HMINSWP_LIFE          h_life
        );
    QM_SLONG                    game_SetFlag
        (
            const QM_SLONG              poly_index
        );
    QM_SLONG                    GetFlagCount(       QM_VOID);
    QM_SLONG                    GetFixedBlockCount( QM_VOID);
    MINSWP_GAME_STATE           GetGameState(       QM_VOID);
private:

    class SetNewPolyMesh
    {
    private:
        //SetNewPolyMesh() {};
        virtual QM_VOID     this__ForceVirtual() = 0;
        static QM_LBOOL     Do
            (
                TMW3D_Geometry            * p_geometry,
                HMINSWP_POLYMESH            h_polyMesh
            );
        friend TMW3D_Geometry * TMW3D_Geometry::this__CreateNew
            (
                TMW3D_Geometry           ** pp_outGeometry,
                HMINSWP_POLYMESH            h_polyMesh
            );
        class Fn
        {
        private:
            virtual QM_VOID this__ForceVirtual() = 0;
            static QM_VOID  GenerateAdjacency( TMW3D_Geometry       * p_geometry);
            static QM_VOID  ClearAllAdjacency( TMW3D_Geometry       * p_geometry);
            friend QM_LBOOL SetNewPolyMesh::Do(TMW3D_Geometry       * p_geometry,
                                               HMINSWP_POLYMESH       h_polyMesh);
        };
    };


    class ArrangeMines
    {
    private:
        virtual QM_VOID this__ForceVirtual() = 0;
        static QM_VOID  Do
            (
                      TMW3D_Geometry  * p_geometry,
                const QM_SLONG          excluded_poly_index
            );
        friend QM_SLONG TMW3D_Geometry::game_CheckBlock
            (
                const QM_SLONG              poly_index,
                const MINSWP_ADJACENT_TYPE  adjacent_type,
                      HMINSWP_LIFE          h_life
            );
        class Fn
        {
        private:
            virtual QM_VOID this__ForceVirtual() = 0;
            static QM_VOID UpdateLinkedMines(TMW3D_Geometry   * p_geometry);
            friend QM_VOID ArrangeMines::Do( TMW3D_Geometry   * p_geometry,
                                       const QM_SLONG           excluded_poly_index);
        };
    };
    class ReleaseBlocks
    {
    private:
        virtual QM_VOID this__ForceVirtual() = 0;
        static QM_SLONG Do
            (
                      TMW3D_Geometry      * p_geometry,
                const QM_SLONG              poly_index,
                const MINSWP_ADJACENT_TYPE  adjacent_type
            );
        friend QM_SLONG TMW3D_Geometry::game_CheckBlock
            (
                const QM_SLONG              poly_index,
                const MINSWP_ADJACENT_TYPE  adjacent_type,
                      HMINSWP_LIFE          h_life
            );
    };

    /*=======================================================*/

    QM_SLONG        GetTrueMinesCount(  const QM_SLONG              mines_count);
    /*
    HMINSWP_GPOLY   GetPoly(            const QM_SLONG              poly_index);
    */
/*
    HMINSWP_INDICES GetAdjacentIndices(       HMINSWP_GPOLY         h_poly,
                                        const MINSWP_ADJACENT_TYPE  adjacent_type);
*/
    /*
    HMINSWP_GPOLY   GetPolyByAdjacency( const QM_SLONG              poly_index,
                                        const QM_SLONG              adjacent_index,
                                        const MINSWP_ADJACENT_TYPE  adjacent_type);
    */
    /*
    QM_LBOOL                poly_SetVerts(                  HMINSWP_GPOLY p_poly, HMINSWP_INDICES             h_source            );
    */
    /*
    QM_LBOOL                poly_AddAdjacent(               HMINSWP_GPOLY p_poly, const QM_SLONG              new_adjacent_index,
                                                                            const MINSWP_ADJACENT_TYPE  adjacent_type       );
    */
    /*
    QM_VOID                 poly_SetMineValue(              HMINSWP_GPOLY p_poly, const QM_LBOOL              value               );
    */
    /*
    QM_VOID                 poly_SetAdjacentsMineCount(     HMINSWP_GPOLY p_poly, const QM_SLONG              mine_count,
                                                                            const MINSWP_ADJACENT_TYPE  adjacent_type       );
    */

    /*
    QM_VOID                 poly_UpdateSphere(              HMINSWP_GPOLY p_poly);
    */
    /*
    QM_LBOOL                poly_IntersectSphereByRay(      HMINSWP_GPOLY p_poly, const MINSWP_RAY3D        * p_ray, QM_FLOAT * p_out_distance);
    */
    /*
    QM_LBOOL                poly_IntersectTrianglesByRay(   HMINSWP_GPOLY p_poly, const MINSWP_RAY3D        * p_ray, QM_FLOAT * p_out_distance);
    */
    QM_VOID                 poly_DetermineAdjacency(                        const QM_SLONG              polyA_index,
                                                                            const QM_SLONG              polyB_index         );
public:
    QM_SLONG                poly_GetVertsCount(         const QM_SLONG poly_index);
    QM_SLONG                poly_GetVertIndex(          const QM_SLONG poly_index,  const QM_SLONG              index           );
    MINSWP_VEC3D          * poly_GetVertPos(            const QM_SLONG poly_index,  const QM_SLONG              vert_index,
                                                                                          MINSWP_VEC3D        * p_out_pos       );
    QM_SLONG                poly_GetAdjacentCount(      const QM_SLONG poly_index,  const MINSWP_ADJACENT_TYPE  adjacent_type   );
    QM_SLONG                poly_GetAdjacent(           const QM_SLONG poly_index,  const QM_SLONG              adjacent_index,
                                                                                    const MINSWP_ADJACENT_TYPE  adjacent_type   );
    QM_LBOOL                poly_IsAdjacent(            const QM_SLONG polyA_index,
                                                        const QM_SLONG polyB_index, const MINSWP_ADJACENT_TYPE  adjacent_type   );
    QM_LBOOL                poly_GetMineValue(          const QM_SLONG poly_index);
    QM_SLONG                poly_GetAdjacentsMineCount( const QM_SLONG poly_index,  const MINSWP_ADJACENT_TYPE  adjacent_type   );
    QM_FLOAT                poly_GetSphereRadius(       const QM_SLONG poly_index);
    MINSWP_VEC3D          * poly_GetSpherePos(          const QM_SLONG poly_index,       MINSWP_VEC3D         * p_out_pos       );
    HMINSWP_TRIMESH         poly_GetTriMesh(            const QM_SLONG poly_index,       HMINSWP_TRIMESH      * ph_mesh         );
    MINSWP_DRAW_TYPE        poly_GetDrawType(           const QM_SLONG poly_index, const MINSWP_ADJACENT_TYPE   adjacent_type   );

    /*
    MINSWP_VEC3D          * vert_GetPos(                const QM_SLONG vert_index,       MINSWP_VEC3D         * p_out_pos       );
    */

    QM_SLONG    GetMinMinesCount(   QM_VOID );
    QM_SLONG    GetMaxMinesCount(   QM_VOID );
    QM_SLONG    GetMinesCount(      QM_VOID );
    QM_SLONG    GetVertsCount(      QM_VOID );
    QM_SLONG    GetPolysCount(      QM_VOID );

    QM_SLONG    GetIntersectedPolyByRay(
                              const MINSWP_RAY3D  * p_ray);
    QM_LBOOL    RestartCurrentGame( QM_VOID );
    QM_LBOOL    StartNewGame(const  QM_SLONG        mines_count);

} * PMW3D_Geometry;




/*===========================================================*/

#endif /* __MINESWEEPER3D_BASE_H__DEFINED */

#endif /* (0) */





