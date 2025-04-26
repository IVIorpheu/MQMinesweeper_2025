/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_base.c         (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_base.h"

/*===========================================================*/

/*===========================*/
/*                           */
/* MINSWP_VEC3D              */
/*                           */
/*===========================*/

MINSWP_VEC3D * MinSwp_Vec3D_SetVal
    (
              MINSWP_VEC3D    * p_Out,
        const QM_FLOAT          x,
        const QM_FLOAT          y,
        const QM_FLOAT          z
    )
{
    p_Out->x = x;
    p_Out->y = y;
    p_Out->z = z;

    return p_Out;
};

/*===========================================================*/

/*===========================*/
/*                           */
/* MINSWP_TRIINDICES         */
/*                           */
/*===========================*/

MINSWP_TRIINDICES * MinSwp_TriIndices_SetVal
    (
              MINSWP_TRIINDICES   * p_Out,
        const QM_SSHRT              i1,
        const QM_SSHRT              i2,
        const QM_SSHRT              i3
    )
{
    p_Out->i1 = i1;
    p_Out->i2 = i2;
    p_Out->i3 = i3;

    return p_Out;
};

/*===========================================================*/

/*===========================*/
/*                           */
/* MINSWP_RAY3D              */
/*                           */
/*===========================*/

MINSWP_RAY3D * MinSwp_Ray3D_SetVal
    (
              MINSWP_RAY3D        * p_Out,
        const QM_FLOAT              px,
        const QM_FLOAT              py,
        const QM_FLOAT              pz,
        const QM_FLOAT              dx,
        const QM_FLOAT              dy,
        const QM_FLOAT              dz
    )
{
    p_Out->Pos.x = px;
    p_Out->Pos.y = py;
    p_Out->Pos.z = pz;
    p_Out->Dir.x = dx;
    p_Out->Dir.y = dy;
    p_Out->Dir.z = dz;

    return p_Out;
};

/*===========================================================*/









