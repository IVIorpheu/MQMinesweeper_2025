/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_game.h         (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_GAME_H__DEFINED
#define __MINSWP_GAME_H__DEFINED

/*===========================================================*/

#include "minswp_base.h"
#include "minswp_trimesh.h"
#include "minswp_polymesh.h"
#include "minswp_life.h"

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

MINSWP_DECL_HANDLE( HMINSWP_GEOMETRY    )
MINSWP_DECL_HANDLE( HMINSWP_GAME        )

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

typedef enum _MINSWP_GPOLY_GAME_STATE
{
    MINSWP_GPGS_START           = 0,    __qm_comment__( 'NOT PRESSED BUTTON              ' )
    MINSWP_GPGS_FIXED           = 1,    __qm_comment__( 'PRESSED BUTTON AND WAS FREE     ' )
    MINSWP_GPGS_ASSUM_MINS      = 2,    __qm_comment__( 'USER ASSUMED THIS BLOCK IS MINE ' )
    MINSWP_GPGS_BOOM            = 3,    __qm_comment__( 'PRESSED BUTTON AND WAS MINE!!!! ' )
    MINSWP_GPGS_HEART           = 4,    __qm_comment__( 'PRESSED BUTTON AND WAS MINE!!!! BUT TAKE LIFE' )
    MINSWP_GPGS____FORCE_32BIT  = 0x7fffffffL
} MINSWP_GPOLY_GAME_STATE;

typedef enum _MINSWP_CHECKBLOCK_RESULT
{
        MINSWP_CHKBLKRES_OK_BOOMSETHEART            = 0x00000001L,
        MINSWP_CHKBLKRES_OK_RELEASEBLOCKS           = 0x00000002L,
        MINSWP_CHKBLKRES_OK_FINISHLOSS              = 0x00000100L,
        MINSWP_CHKBLKRES_OK_FINISHWIN               = 0x00000200L,
#define MINSWP_CHKBLKRES_OK__MASK                     0x0000ffffL
#define MINSWP_CHKBLKRES_OK_FINISH__MASK              0x0000ff00L

        MINSWP_CHKBLKRES_ERR_OUTOFRANGEPOLYINDEX    = 0x00010000L,
        MINSWP_CHKBLKRES_ERR_NULLPOLYPTR            = 0x00020000L,
        MINSWP_CHKBLKRES_ERR_BLOCKISCHECKED         = 0x00030000L,
        MINSWP_CHKBLKRES_ERR_OUTOFGAME              = 0x00040000L,
#define MINSWP_CHKBLKRES_ERR__MASK                    0xffff0000L

        MINSWP_CHKBLKRES____FORCE32BIT              = 0x7fffffffL
} MINSWP_CHECKBLOCK_RESULT;

typedef enum _MINSWP_SETFLAG_RESULT
{
        MINSWP_SETFLAGRES_OK_INSTALL                = 0x00000001L,
        MINSWP_SETFLAGRES_OK_REMOVE                 = 0x00000002L,
#define MINSWP_SETFLAGRES_OK__MASK                    0x0000ffffL

        MINSWP_SETFLAGRES_ERR_FIXEDPOLY             = 0x00010000L,
        MINSWP_SETFLAGRES_ERR_NULLPOLYPTR           = 0x00020000L,
        MINSWP_SETFLAGRES_ERR_OUTOFGAME             = 0x00030000L,
#define MINSWP_SETFLAGRES_ERR__MASK                   0xffff0000L

        MINSWP_SETFLAGRES____FORCE32BIT             = 0x7fffffffL
} MINSWP_SETFLAG_RESULT;


/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

HMINSWP_GAME                MinSwp_Game_Create
    (
        QM_VOID
    );

/*===========================================================*/

HMINSWP_TRIMESH             MinSwp_Geometry_Poly_TriMesh_Create
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
              HMINSWP_TRIMESH         * ph_Mesh
    );

/*===========================================================*/

HMINSWP_GEOMETRY            MinSwp_Game_Geometry_Get
    (
              HMINSWP_GAME              h_Game
    );

/*===========================================================*/

MINSWP_DRAW_TYPE            MinSwp_Geometry_Poly_DrawType_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    );

/*===========================================================*/

QM_LBOOL                    MinSwp_Geometry_Poly_IsAdjacent
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  PolyA_Index,
        const QM_SLONG                  PolyB_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type
    );

/*===========================================================*/

QM_FLOAT MinSwp_Geometry_Poly_Sphere_Radius_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index
    );

/*===========================================================*/

MINSWP_VEC3D *              MinSwp_Geometry_Poly_Sphere_Pos_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry,
        const QM_SLONG                  Poly_Index,
              MINSWP_VEC3D            * p_Out_Pos
    );

/*===========================================================*/

QM_ULONG                    MinSwp_Geometry_NumPolys_Get
    (
        const HMINSWP_GEOMETRY          h_Geometry
    );

/*===========================================================*/

QM_LBOOL                    MinSwp_Game_SetNewPolyMesh
    (
              HMINSWP_GAME              h_Game,
        const HMINSWP_POLYMESH          h_PolyMesh
    );

/*===========================================================*/

QM_SLONG                    MinSwp_Game_MinNumMines
    (
        const HMINSWP_GAME              h_Game
    );

/*===========================================================*/

QM_SLONG                    MinSwp_Game_MaxNumMines
    (
        const HMINSWP_GAME              h_Game
    );

/*===========================================================*/

QM_SLONG                    MinSwp_Game_NumMines_Get
    (
        const HMINSWP_GAME              h_Game
    );

/*===========================================================*/

QM_SLONG                    MinSwp_Game_NumFlags_Get
    (
        const HMINSWP_GAME              h_Game
    );

/*===========================================================*/

QM_LBOOL                    MinSwp_Game_New_Start
    (
              HMINSWP_GAME              h_Game,
        const QM_SLONG                  NumMines
    );

/*===========================================================*/

QM_DOUBL                    MinSwp_Game_Timer_GetSecound
    (
              HMINSWP_GAME              h_Game
    );

/*===========================================================*/

QM_SLONG                    MinSwp_Game_Poly_IntersectedByRay_Find
    (
        const HMINSWP_GAME              h_Game,
        const MINSWP_RAY3D            * p_Ray
    );

/*===========================================================*/

MINSWP_GAME_STATE           MinSwp_Game_State_Get
    (
        const HMINSWP_GAME              h_Game
    );

/*===========================================================*/

MINSWP_CHECKBLOCK_RESULT    MinSwp_Game_CheckBlock
    (
              HMINSWP_GAME              h_Game,
        const QM_SLONG                  Poly_Index,
        const MINSWP_ADJACENT_TYPE      Adjacent_Type,
              HMINSWP_LIFE              h_Life
    );

/*===========================================================*/

MINSWP_SETFLAG_RESULT       MinSwp_Game_Flag_Set
    (
              HMINSWP_GAME              h_Game,
        const QM_SLONG                  Poly_Index
    );

/*===========================================================*/

QM_LBOOL                    MinSwp_Game_Timer_IsPaused
    (
              HMINSWP_GAME              h_Game
    );

/*===========================================================*/

QM_LBOOL                    MinSwp_Game_Timer_Pause
    (
              HMINSWP_GAME              h_Game
    );

/*===========================================================*/

QM_LBOOL                    MinSwp_Game_Timer_Play
    (
              HMINSWP_GAME              h_Game
    );

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/


#ifdef __cplusplus

class T_MinSwp_Game
{
    HMINSWP_GAME            this__h_Game;
    HMINSWP_GEOMETRY        this__h_Geometry;
public:
    class T_Poly
    {
        friend T_MinSwp_Game;

        HMINSWP_GAME            this__h_Game;
    public:
        class T_PolyFns
        {
            friend T_Poly;
            T_PolyFns() {};
            T_PolyFns
                (
                          HMINSWP_GAME  h_Game,
                    const QM_SLONG      Index
                ) : Fns(h_Game, Index) {};
        public:
            class T_Fns
            {
                friend T_PolyFns;

                HMINSWP_GAME        this__h_Game;
                HMINSWP_GEOMETRY    this__h_Geometry;
                QM_SLONG            this__Index;

                T_Fns() {};
                T_Fns
                    (
                              HMINSWP_GAME      h_Game,
                        const QM_SLONG          Index
                    )
                {
                    this__h_Game        = h_Game;
                    this__Index         = Index;
                    this__h_Geometry    =
                        MinSwp_Game_Geometry_Get
                            (
                                this__h_Game
                            );
                };
            public:
                HMINSWP_TRIMESH GetTriMesh(HMINSWP_TRIMESH  * ph_Out)
                {
                    /*
                    MinSwp_ShowMessage_I(this__Index);
                    */
                    return
                        MinSwp_Geometry_Poly_TriMesh_Create
                            (
                                this__h_Geometry,
                                this__Index,
                                ph_Out
                            );
                };

                MINSWP_DRAW_TYPE GetDrawType
                    (
                        const MINSWP_ADJACENT_TYPE      Adjacent_Type
                    )
                {
                    return
                        MinSwp_Geometry_Poly_DrawType_Get
                            (
                                this__h_Geometry,
                                this__Index,
                                Adjacent_Type
                            );
                };

                QM_LBOOL    IsAdjacent
                    (
                        const QM_SLONG                  Index,
                        const MINSWP_ADJACENT_TYPE      Adjacent_Type
                    )
                {
                    return
                        MinSwp_Geometry_Poly_IsAdjacent
                            (
                                this__h_Geometry,
                                this__Index,
                                Index,
                                Adjacent_Type
                            );
                };

                QM_FLOAT GetSphereRadius
                    (
                        QM_VOID
                    )
                {
                    return
                        MinSwp_Geometry_Poly_Sphere_Radius_Get
                            (
                                this__h_Geometry,
                                this__Index
                            );
                };

                MINSWP_VEC3D * GetSpherePos

                    (
                        MINSWP_VEC3D * p_Out
                    )
                {
                    return
                        MinSwp_Geometry_Poly_Sphere_Pos_Get
                            (
                                this__h_Geometry,
                                this__Index,
                                p_Out
                            );
                };
            };
        private:
            T_Fns Fns;
        public:
            T_Fns * operator->() { return &this->Fns; };

        };

        T_PolyFns operator[](const QM_SLONG i)
        {
            return T_PolyFns(this__h_Game, i);
        };
    } Poly;

    QM_ULONG GetNumPolys(QM_VOID)
    {
        return
            MinSwp_Geometry_NumPolys_Get
                (
                    this__h_Geometry
                );
    };

    QM_ULONG GetNumMines(QM_VOID)
    {
        return (QM_SLONG)
            MinSwp_Game_NumMines_Get
                (
                    this__h_Game
                );
    };

    QM_ULONG GetNumFlags(QM_VOID)
    {
        return (QM_SLONG)
            MinSwp_Game_NumFlags_Get
                (
                    this__h_Game
                );
    };

    QM_LBOOL StartNewGame
        (
            const QM_SLONG  NumMines
        )
    {

        return
            MinSwp_Game_New_Start
                (
                    this__h_Game,
                    NumMines
                );
    };

    QM_SLONG GetIntersectedPolyByRay
        (
            const MINSWP_RAY3D  * p_Ray
        )
    {
        return
            MinSwp_Game_Poly_IntersectedByRay_Find
                (
                    this__h_Game,
                    p_Ray
                );
    };

    MINSWP_GAME_STATE GetGameState()
    {
        return
            MinSwp_Game_State_Get
                (
                    this__h_Game
                );
    };

    MINSWP_CHECKBLOCK_RESULT    CheckBlock
        (
            const QM_SLONG              Poly_Index,
            const MINSWP_ADJACENT_TYPE  Adjacent_Type,
                  HMINSWP_LIFE          h_Life
        )
    {
        return
            MinSwp_Game_CheckBlock
                (
                    this__h_Game,
                    Poly_Index,
                    Adjacent_Type,
                    h_Life
                );
    };

    MINSWP_SETFLAG_RESULT SetFlag
        (
            const QM_SLONG      Poly_Index
        )
    {
        return
            MinSwp_Game_Flag_Set
                (
                    this__h_Game,
                    Poly_Index
                );
    };

    QM_LBOOL Timer_IsPaused(            QM_VOID) { return MinSwp_Game_Timer_IsPaused(   this__h_Game); };
    QM_LBOOL Timer_Pause(               QM_VOID) { return MinSwp_Game_Timer_Pause(      this__h_Game); };
    QM_LBOOL Timer_Play(                QM_VOID) { return MinSwp_Game_Timer_Play(       this__h_Game); };
    QM_DOUBL Timer_GetTimePerSec_f64(   QM_VOID) { return MinSwp_Game_Timer_GetSecound( this__h_Game); };
    QM_SLL64 Timer_GetTimePerSec_i64(   QM_VOID)
    {
        return (QM_SLL64)this->Timer_GetTimePerSec_f64();
    };

    /*=======================================================*/

    static T_MinSwp_Game * CreateNew
        (
            HMINSWP_POLYMESH        h_PolyMesh
        )
    {
        if (h_PolyMesh == QM_NULL)
            return QM_NULL;

        T_MinSwp_Game * p_out = new T_MinSwp_Game();

        if (
                (
                    !MinSwp_Game_SetNewPolyMesh
                        (
                            p_out->this__h_Game,
                            h_PolyMesh
                        )
                ) ||
                (
                    MinSwp_Game_MinNumMines(p_out->this__h_Game) >
                    MinSwp_Game_MaxNumMines(p_out->this__h_Game)
                )
            )
        {
            MinSwp_ShowMessage_I(MinSwp_Game_MaxNumMines(p_out->this__h_Game));
            delete p_out;
            return QM_NULL;
        }
        else
            return p_out;
    };
private:
    T_MinSwp_Game()
    {
        this__h_Game        = MinSwp_Game_Create();
        this__h_Geometry    =
            MinSwp_Game_Geometry_Get
                (
                    this__h_Game
                );
        this->Poly.this__h_Game = this__h_Game;
    };
};
#endif /* __cplusplus */




/*===========================================================*/

#endif /* __MINSWP_GAME_H__DEFINED */













