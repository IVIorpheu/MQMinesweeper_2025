/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_time.c         (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_time.h"

/*===========================================================*/

#include <windows.h>

/*===========================================================*/

QM_DOUBL MinSwp_Time_Now
    (
        QM_VOID
    )
{
    LARGE_INTEGER lic, lif;

    QueryPerformanceCounter(    &lic    );
    QueryPerformanceFrequency(  &lif    );

    return (QM_DOUBL)lic.QuadPart / (QM_DOUBL)lif.QuadPart;
};








