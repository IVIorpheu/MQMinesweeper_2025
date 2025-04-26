/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : fmath3d.h             (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

/*===========================================================*/

/* #include "__fmath3d__140010080131.h" */

/*===========================================================*/

#ifndef __FMATH3D_H__DEFINED
#define __FMATH3D_H__DEFINED
#define __FM3DPROXY__

/*===========================================================*/

#include <stdint.h>
#include <stddef.h>

/*===========================================================*/

#ifndef FM3D_I32_DEFINED
    typedef int32_t   FM3D_I32;
#   define FM3D_I32_DEFINED
#endif

#ifndef FM3D_F32_DEFINED
    typedef float     FM3D_F32;
#   define FM3D_F32_DEFINED
#endif

#ifndef FM3D_F64_DEFINED
    typedef double    FM3D_F64;
#   define FM3D_F64_DEFINED
#endif

#ifndef FM3D_BOOL_DEFINED
    typedef uint32_t  FM3D_BOOL;
#   define FM3D_BOOL_DEFINED
#endif

#define FM3D_TRUE    ((FM3D_BOOL)1)
#define FM3D_FALSE   ((FM3D_BOOL)0)

/*===========================================================*/

/*==================*/
/* Vector Index */
#define VI_X         0
#define VI_Y         1
#define VI_Z         2
#define VI_W         3
/*==================*/
/* Ray2D Index */
/*     Position_XY */
#define R2I_PX       0
#define R2I_PY       1
/*     Direction_XY */
#define R2I_DX       2
#define R2I_DY       3
/*==================*/
/* Ray3D Index */
/*     Position_XYZ */
#define R3I_PX       0
#define R3I_PY       1
#define R3I_PZ       2
/*     Direction_XYZ */
#define R3I_DX       3
#define R3I_DY       4
#define R3I_DZ       5
/*==================*/
/* Ray4D Index */
/*     Position_XYZW */
#define R4I_PX       0
#define R4I_PY       1
#define R4I_PZ       2
#define R4I_PW       3
/*     Direction_XYZW */
#define R4I_DX       4
#define R4I_DY       5
#define R4I_DZ       6
#define R4I_DW       7
/*==================*/
/* Matrix2x2 Index */
#define M2X2I_11     0
#define M2X2I_12     1
#define M2X2I_21     2
#define M2X2I_22     3
/*==================*/
/* Matrix3x3 Index */
#define M3X3I_11     0
#define M3X3I_12     1
#define M3X3I_13     2
#define M3X3I_21     3
#define M3X3I_22     4
#define M3X3I_23     5
#define M3X3I_31     6
#define M3X3I_32     7
#define M3X3I_33     8
/*==================*/
/* Matrix4x4 Index */
#define M4X4I_11     0
#define M4X4I_12     1
#define M4X4I_13     2
#define M4X4I_14     3
#define M4X4I_21     4
#define M4X4I_22     5
#define M4X4I_23     6
#define M4X4I_24     7
#define M4X4I_31     8
#define M4X4I_32     9
#define M4X4I_33     10
#define M4X4I_34     11
#define M4X4I_41     12
#define M4X4I_42     13
#define M4X4I_43     14
#define M4X4I_44     15

/*===========================================================*/

#define FM3D_DIV_EPSILON_F32  1.0e-16f
#define FM3D_RAY_INTERSECTION_TRI_EPSILON_F32 0.00001f

#define FM3D_DIV_EPSILON_F64  1.0e-150
#define FM3D_RAY_INTERSECTION_TRI_EPSILON_F64 0.0000000001f

/*===========================================================*/

#define FM3D_DECL
#define FM3DCALL


#undef  FM3D_FXX
#undef  _fxx
#undef  _FXX
#undef  _f
#undef  CONST_FXX

#define FM3D_FXX                FM3D_F32
#define _fxx(name)              name##_f32
#define _FXX(name)              name##_F32
#define _FXX_DEFINED(name)      name##_F32_DEFINED
#define _f(name)                name##f


#define FM3D_DIV_EPSILON_F32  1.0e-16f

/*===========================================================*/

#ifndef FM3D_STD_MATH_DEFINED

#   define  FM3D_Sqrt_f64   sqrt
#   define  FM3D_Abs_f64    fabs
#   define  FM3D_Cos_f64    cos
#   define  FM3D_Sin_f64    sin

#   if defined(__CUDACC__) || \
       defined(_MSC_VER)   || \
      (defined(__clang__) && defined(_WIN64))
#       define  FM3D_Sqrt_f32   sqrtf
#       define  FM3D_Abs_f32    fabsf
#       define  FM3D_Cos_f32    cosf
#       define  FM3D_Sin_f32    sinf
#   else
#       define  FM3D_Sqrt_f32(x)   ((FM3D_FXX)sqrt(x))
#       define  FM3D_Abs_f32(x)    ((FM3D_FXX)fabs(x))
#       define  FM3D_Cos_f32(x)    ((FM3D_FXX)cos(x))
#       define  FM3D_Sin_f32(x)    ((FM3D_FXX)sin(x))
#   endif


#   define FM3D_STD_MATH_DEFINED
#endif

#undef FM3D_Sqrt_fxx
#undef FM3D_Abs_fxx
#undef FM3D_Cos_fxx
#undef FM3D_Sin_fxx

#define FM3D_Sqrt_fxx  _fxx(FM3D_Sqrt)
#define FM3D_Abs_fxx   _fxx(FM3D_Abs)
#define FM3D_Cos_fxx   _fxx(FM3D_Cos)
#define FM3D_Sin_fxx   _fxx(FM3D_Sin)

/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL
_fxx(FM3DVec3Normalize)
    (
              FM3D_FXX * pOut,
        const FM3D_FXX * pV
    );

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL
_fxx(FM3DVec3DistanceSqr)
    (
        const FM3D_FXX * pV1,
        const FM3D_FXX * pV2
    );

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL _fxx(FM3DVec3Subtract)
    (
              FM3D_FXX * pOut,
        const FM3D_FXX * pV1,
        const FM3D_FXX * pV2
    );

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL
_fxx(FM3DVec3Lerp)
    (
              FM3D_FXX * pOut,
        const FM3D_FXX * pV1,
        const FM3D_FXX * pV2,
              FM3D_FXX   s
    );

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL _fxx(FM3DVec3Length)
    (
        const FM3D_FXX * pV
    );

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL _fxx(FM3DVec3Dot)
    (
        const FM3D_FXX * pV1,
        const FM3D_FXX * pV2
    );

/*===========================================================*/

FM3D_DECL FM3D_FXX * FM3DCALL
_fxx(FM3DVec3Cross)
    (
              FM3D_FXX * pOut,
        const FM3D_FXX * pV1,
        const FM3D_FXX * pV2
    );

/*===========================================================*/

FM3D_DECL FM3D_BOOL FM3DCALL
_fxx(FM3DRay3IntersectSphere)
    (
              FM3D_FXX * pLength,
        const FM3D_FXX * pRayPos,
        const FM3D_FXX * pRayDir,
        const FM3D_FXX * pSpherePos,
        const FM3D_FXX   SphereRadius,
        const FM3D_BOOL  NormalizeDir
    );

/*===========================================================*/

FM3D_DECL FM3D_BOOL FM3DCALL
_fxx(FM3DRay3IntersectTrianglePD)
    (
        const FM3D_FXX * pRayPos,
        const FM3D_FXX * pRayDir,
        const FM3D_FXX * pV0,
        const FM3D_FXX * pV1,
        const FM3D_FXX * pV2
    );

/*===========================================================*/

FM3D_DECL FM3D_FXX FM3DCALL
_fxx(FM3DRay3IntersectPlaneDepth)
    (
              FM3D_FXX * pOutPos3D,
        const FM3D_FXX * pRayPos,
        const FM3D_FXX * pRayDir,
        const FM3D_FXX * pPlanePos,
        const FM3D_FXX * pPlaneDir,
        const FM3D_BOOL  NormalizeDirs
    );

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/

#endif /* __FMATH3D_H__DEFINED */









