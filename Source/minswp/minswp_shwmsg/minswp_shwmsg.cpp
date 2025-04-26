/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_shwmsg.cpp     (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_shwmsg.h"

/*===========================================================*/

#include <vcl.h>

/*===========================================================*/

QM_VOID MinSwp_ShowMessage_W_Force
    (
        const QM_WCHAR    * p_Str
    )
{
    ShowMessage(p_Str);
};

/*===========================================================*/

#ifdef _DEBUG

/*===========================================================*/

QM_VOID MinSwp_ShowMessage_W(   const QM_WCHAR    * p_Str)
{
    ShowMessage(p_Str);
};

QM_VOID MinSwp_ShowMessage_A(   const QM_ACHAR    * p_Str)
{
    ShowMessage(p_Str);
};

QM_VOID MinSwp_ShowMessage_I(   const QM_SLONG      X)
{
    ShowMessage(X);
};

QM_VOID MinSwp_ShowMessage_F(   const QM_FLOAT      X)
{
    ShowMessage(X);
};

QM_VOID MinSwp_ShowMessage_WI(  const QM_WCHAR    * p_Str,
                                const QM_SLONG      X)
{
    ShowMessage(UnicodeString(p_Str) + X);
};

QM_VOID MinSwp_ShowMessage_WF(  const QM_WCHAR    * p_Str,
                                const QM_FLOAT      X)
{
    ShowMessage(UnicodeString(p_Str) + X);
};

/*===========================================================*/

#endif /* _DEBUG */






