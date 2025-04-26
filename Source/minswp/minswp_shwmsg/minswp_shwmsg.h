/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_shwmsg.h       (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#ifndef __MINSWP_SHWMSG_H__DEFINED
#define __MINSWP_SHWMSG_H__DEFINED

/*===========================================================*/

#include "qm_basetype.h"

/*===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*===========================================================*/

QM_VOID MinSwp_ShowMessage_W_Force
    (
        const QM_WCHAR    * p_Str
    );

/*===========================================================*/

#ifndef _DEBUG
#   define MinSwp_ShowMessage_W(   p_Str    )
#   define MinSwp_ShowMessage_A(   p_Str    )
#   define MinSwp_ShowMessage_I(   X        )
#   define MinSwp_ShowMessage_F(   X        )
#   define MinSwp_ShowMessage_WI(  p_Str, X )
#   define MinSwp_ShowMessage_WF(  p_Str, X )
#else

/*===========================================================*/

QM_VOID MinSwp_ShowMessage_W(   const QM_WCHAR    * p_Str   );
QM_VOID MinSwp_ShowMessage_A(   const QM_ACHAR    * p_Str   );
QM_VOID MinSwp_ShowMessage_I(   const QM_SLONG      X       );
QM_VOID MinSwp_ShowMessage_F(   const QM_FLOAT      X       );
QM_VOID MinSwp_ShowMessage_WI(  const QM_WCHAR    * p_Str,
                                const QM_SLONG      X       );
QM_VOID MinSwp_ShowMessage_WF(  const QM_WCHAR    * p_Str,
                                const QM_FLOAT      X       );
/*===========================================================*/

#endif /* _DEBUG */

/*===========================================================*/

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

/*===========================================================*/

#endif /* __MINSWP_SHWMSG_H__DEFINED */









