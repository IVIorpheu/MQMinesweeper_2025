/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : MinSwp_PolyMesh_FileInMemory.hpp               */
/*                                  (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_POLYMESH_FILEINMEMORY_HPP__DEFINED
#define __MINSWP_POLYMESH_FILEINMEMORY_HPP__DEFINED

/*===========================================================*/

#include "qm_basetype.h"

/*===========================================================*/

typedef enum _MINSWP_GAME_TYPE
{
    MINSWP_GAMETYPE_CLASSIC             = 0,
    MINSWP_GAMETYPE_CLASSICWAVE         = 1,
    MINSWP_GAMETYPE_VORONOI             = 2,
    MINSWP_GAMETYPE_INFINITY            = 3,
    MINSWP_GAMETYPE_DELPHI              = 4,
    MINSWP_GAMETYPE_CPPBUILDER          = 5,

    /*
    MINSWP_GAMETYPE_CUBE3D              = 4,
    */
    MINSWP_GAMETYPE_CUSTOM              = 1000,
    MINSWP_GAMETYPE____FORCE32BIT       = 0x7fffffff
} MINSWP_GAME_TYPE;

class T_MinSwp_PolyMesh_FileInMemory
{
public:
    static const QM_VOID * GetFile
        (
            const MINSWP_GAME_TYPE      Index
        );
};

/*===========================================================*/

#endif /* __MINSWP_POLYMESH_FILEINMEMORY_HPP__DEFINED */





