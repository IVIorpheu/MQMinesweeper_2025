/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : qm_basetype.h         (Part of QM3DStudio)     */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __QM_BASETYPE_H__DEFINED
#define __QM_BASETYPE_H__DEFINED

/*===========================================================*/

#include <stdint.h>
#include <tchar.h>

/*===========================================================*/

#ifndef __QM_BASETYPE__
#define __QM_BASETYPE__

typedef char                    QM_ACHAR;
typedef wchar_t                 QM_WCHAR;

typedef uint8_t                 QM_UCHAR;
typedef  int8_t                 QM_SCHAR;
typedef uint16_t                QM_USHRT;
typedef  int16_t                QM_SSHRT;
typedef uint32_t                QM_ULONG;
typedef  int32_t                QM_SLONG;
typedef uint64_t                QM_ULL64;
typedef  int64_t                QM_SLL64;

typedef float                   QM_FLOAT;
typedef double                  QM_DOUBL;

typedef QM_ULONG                QM_LBOOL;
#define QM_FALSE              ((QM_LBOOL)0)
#define QM_TRUE               ((QM_LBOOL)1)

#define                         QM_VOID     void

#ifdef __cplusplus
#   define                      QM_NULL     0
#else
#   define                      QM_NULL     ((QM_VOID *)0)
#endif /* __cplusplus */

#pragma pack(push, 1)
typedef struct _QM_FRECT
{
    QM_FLOAT    L, T, R, B;
} QM_FRECT;
#pragma pack(pop)

#endif /* __QM_BASETYPE__ */


/*===========================================================*/

#ifndef     __QM_CODE__ON__
#   define  __QM_CODE__ON__(x) x
#endif /*   __QM_CODE__ON__ */

#ifndef     __QM_CODE__OFF__
#   define  __QM_CODE__OFF__(x)
#endif /*   __QM_CODE__OFF__ */

#ifndef     __QM_COMMENT__
#   define  __QM_COMMENT__(x)   __QM_CODE__OFF__(x)
#endif /*   __QM_COMMENT__ */

#ifndef     __qm_comment__
#   define  __qm_comment__(x)   __QM_COMMENT__(x)
#endif /*   __qm_comment__ */

/*===========================================================*/

#endif /* __QM_BASETYPE_H__DEFINED */












