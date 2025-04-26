/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : fmath3d.c             (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "fmath3d.h"

/*===========================================================*/

#ifdef __FM3DPROXY__

/*===========================================================*/

#include <math.h>

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL _fxx(FM3DVec3Normalize)(
          FM3D_FXX * pOut,
    const FM3D_FXX * pV)
{
    FM3D_FXX inv_len = _f(1.0) / (FM3D_Sqrt_fxx(
                                    pV[0] * pV[0]  +
                                    pV[1] * pV[1]  +
                                    pV[2] * pV[2]) + _FXX(FM3D_DIV_EPSILON));
    pOut[0] = pV[0] * inv_len;
    pOut[1] = pV[1] * inv_len;
    pOut[2] = pV[2] * inv_len;
    return pOut;
};

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL _fxx(FM3DVec3DistanceSqr)(
    const FM3D_FXX * pV1,
    const FM3D_FXX * pV2)
{
    FM3D_FXX x = pV1[0] - pV2[0];
    FM3D_FXX y = pV1[1] - pV2[1];
    FM3D_FXX z = pV1[2] - pV2[2];
    return (x * x + y * y + z * z);
};

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL _fxx(FM3DVec3Subtract)(
          FM3D_FXX * pOut,
    const FM3D_FXX * pV1,
    const FM3D_FXX * pV2)
{
    pOut[0] = pV1[0] - pV2[0];
    pOut[1] = pV1[1] - pV2[1];
    pOut[2] = pV1[2] - pV2[2];
    return pOut;
};

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL _fxx(FM3DVec3Lerp)(
          FM3D_FXX * pOut,
    const FM3D_FXX * pV1,
    const FM3D_FXX * pV2,
          FM3D_FXX   s)
{
    pOut[0] = pV1[0] + s * (pV2[0] - pV1[0]);
    pOut[1] = pV1[1] + s * (pV2[1] - pV1[1]);
    pOut[2] = pV1[2] + s * (pV2[2] - pV1[2]);
    return pOut;
};

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL _fxx(FM3DVec3Length)(
    const FM3D_FXX * pV)
{
    return FM3D_Sqrt_fxx(
        pV[0] * pV[0] +
        pV[1] * pV[1] +
        pV[2] * pV[2]);
};

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL _fxx(FM3DVec3Dot)(
    const FM3D_FXX * pV1,
    const FM3D_FXX * pV2)
{
    return (pV1[0] * pV2[0] +
            pV1[1] * pV2[1] +
            pV1[2] * pV2[2]);
};

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL _fxx(FM3DVec3Cross)(
          FM3D_FXX * pOut,
    const FM3D_FXX * pV1,
    const FM3D_FXX * pV2)
{
    FM3D_FXX x = pV1[VI_Y] * pV2[VI_Z] - pV1[VI_Z] * pV2[VI_Y];
    FM3D_FXX y = pV1[VI_Z] * pV2[VI_X] - pV1[VI_X] * pV2[VI_Z];
    pOut[2]    = pV1[VI_X] * pV2[VI_Y] - pV1[VI_Y] * pV2[VI_X];
    pOut[0] = x; pOut[1] = y;
    return pOut;
};

/*===========================================================*/

FM3D_DECL FM3D_BOOL FM3DCALL _fxx(FM3DRay3IntersectSphere)(
          FM3D_FXX * pLength,
    const FM3D_FXX * pRayPos,
    const FM3D_FXX * pRayDir,
    const FM3D_FXX * pSpherePos,
    const FM3D_FXX   SphereRadius,
    const FM3D_BOOL  NormalizeDir )
{
    FM3D_FXX dot_ray, sqr_dot_ray,
             //y, 
             sqr_r, sqr_len_r2s,
             sqr_tan_s, inv_ray_dir_len;
    FM3D_BOOL in_s, ret;
    FM3D_FXX r2s[3];

    r2s[0] = pSpherePos[0] - pRayPos[0];
    r2s[1] = pSpherePos[1] - pRayPos[1];
    r2s[2] = pSpherePos[2] - pRayPos[2];

    sqr_len_r2s =
        r2s[0] * r2s[0] +
        r2s[1] * r2s[1] +
        r2s[2] * r2s[2];

    sqr_r = SphereRadius * SphereRadius;

    in_s = sqr_r > sqr_len_r2s;
    if ((in_s) && (pLength == NULL))
        return FM3D_TRUE;

    if (NormalizeDir)
    {
        inv_ray_dir_len   = _f(1.0) / (_fxx(FM3DVec3Length)(pRayDir)   + _FXX(FM3D_DIV_EPSILON));
    }
    else
    {
        inv_ray_dir_len   = _f(1.0);
    };

    dot_ray = inv_ray_dir_len * (
        r2s[0] * pRayDir[0] +
        r2s[1] * pRayDir[1] +
        r2s[2] * pRayDir[2]);

    if ((dot_ray < _f(0.0)) && (!in_s)) /* The sphere is behind the ray. */
    {
        if (pLength != NULL)
            pLength[0] = -_f(1.0);
        return FM3D_FALSE;
    };

    sqr_dot_ray = dot_ray * dot_ray;

    sqr_tan_s = sqr_len_r2s - sqr_r;

    ret = (sqr_tan_s < sqr_dot_ray) ? FM3D_TRUE : FM3D_FALSE;

    if (pLength != NULL)
    {
        if (ret)
        {
            pLength[0] = FM3D_Sqrt_fxx(FM3D_Abs_fxx(sqr_r - sqr_len_r2s + sqr_dot_ray));
            if (!in_s)
                pLength[0] = dot_ray - pLength[0];
            else
                pLength[0] = dot_ray + pLength[0];
        }
        else
            pLength[0] = -_f(1.0);
    };

    return ret;
};

/*===========================================================*/

#undef  FM3D_Ray3_Line3_Intersection_Triangle_DECL_MACRO
#define FM3D_Ray3_Line3_Intersection_Triangle_DECL_MACRO()                     \
    FM3D_FXX e1[3], e2[3], h[3], s[3], q[3];                                   \
    FM3D_FXX a, f, u, v;

#undef  FM3D_Ray3_Line3_Intersection_Triangle_BASE_MACRO
#define FM3D_Ray3_Line3_Intersection_Triangle_BASE_MACRO(p_ray_pos, p_ray_dir) \
    _fxx(FM3DVec3Subtract)( e1, pV1, pV0);                                     \
    _fxx(FM3DVec3Subtract)( e2, pV2, pV0);                                     \
                                                                               \
    _fxx(FM3DVec3Cross)( h, p_ray_dir, e2);                                    \
    a = _fxx(FM3DVec3Dot)( e1, h);                                             \
                                                                               \
    if ((a > -(_FXX(FM3D_RAY_INTERSECTION_TRI_EPSILON))) &&                    \
        (a <   _FXX(FM3D_RAY_INTERSECTION_TRI_EPSILON)))                       \
        return FM3D_FALSE;                                                     \
                                                                               \
    f = _f(1.0) / a;                                                           \
    _fxx(FM3DVec3Subtract)( s, p_ray_pos, pV0);                                \
    u = f * (_fxx(FM3DVec3Dot)( s, h));                                        \
                                                                               \
    if ((u < _f(0.0)) || (u > _f(1.0)))                                        \
        return FM3D_FALSE;                                                     \
                                                                               \
    _fxx(FM3DVec3Cross)( q, s, e1);                                            \
    v = f * _fxx(FM3DVec3Dot)( p_ray_dir, q);                                  \
                                                                               \
    if ((v < _f(0.0)) || ((u + v) > _f(1.0)))                                  \
        return FM3D_FALSE;

FM3D_DECL FM3D_BOOL FM3DCALL _fxx(FM3DRay3IntersectTrianglePD)(
    const FM3D_FXX * pRayPos,
    const FM3D_FXX * pRayDir,
    const FM3D_FXX * pV0,
    const FM3D_FXX * pV1,
    const FM3D_FXX * pV2)
{
    FM3D_FXX t;
    FM3D_Ray3_Line3_Intersection_Triangle_DECL_MACRO();
    FM3D_Ray3_Line3_Intersection_Triangle_BASE_MACRO(pRayPos, pRayDir);

    /* at this stage we can compute t to find out ... */
    /* where the intersection point is on the line    */

    t = f * _fxx(FM3DVec3Dot)( e2, q);
    if (t > (_FXX(FM3D_RAY_INTERSECTION_TRI_EPSILON)) )
    {
        return FM3D_TRUE;
    }
    /* else // this means that there is a line intersection ... */
            /* but not a ray intersection                       */
    return FM3D_FALSE;
};

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL _fxx(FM3DRay3IntersectPlaneDepth)(
          FM3D_FXX * pOutPos3D,
    const FM3D_FXX * pRayPos,
    const FM3D_FXX * pRayDir,
    const FM3D_FXX * pPlanePos,
    const FM3D_FXX * pPlaneDir,
    const FM3D_BOOL  NormalizeDirs )
{
    FM3D_FXX dot_dirs, depth, inv_ray_dir_len; // , inv_plane_dir_len;
    //FM3D_FXX ray_dir[3],
    //         plane_dir[3];

    if (NormalizeDirs)
    {
        inv_ray_dir_len   = _f(1.0) / (_fxx(FM3DVec3Length)(pRayDir)   + _FXX(FM3D_DIV_EPSILON));
    }
    else
    {
        inv_ray_dir_len   = _f(1.0);
    };

    dot_dirs = (
        pRayDir[0] * pPlaneDir[0] +
        pRayDir[1] * pPlaneDir[1] +
        pRayDir[2] * pPlaneDir[2]
        ) /* * inv_ray_dir_len * inv_plane_dir_len */ ;

    if (FM3D_Abs_fxx(dot_dirs) < _FXX(FM3D_DIV_EPSILON))
    {
        if (dot_dirs < _f(0.0))
            dot_dirs = -_FXX(FM3D_DIV_EPSILON);
        else
            dot_dirs =  _FXX(FM3D_DIV_EPSILON);
    };

    depth = (
        pPlaneDir[0] * (pPlanePos[0] - pRayPos[0]) +
        pPlaneDir[1] * (pPlanePos[1] - pRayPos[1]) +
        pPlaneDir[2] * (pPlanePos[2] - pRayPos[2])
    ) /* inv_plane_dir_len */ / dot_dirs;
    if (pOutPos3D != NULL)
    {
        pOutPos3D[0] = pRayPos[0] + pRayDir[0] * depth;
        pOutPos3D[1] = pRayPos[1] + pRayDir[1] * depth;
        pOutPos3D[2] = pRayPos[2] + pRayDir[2] * depth;
    };
    return depth / inv_ray_dir_len;
};

/*===========================================================*/

#endif /* __FM3DPROXY__ */


