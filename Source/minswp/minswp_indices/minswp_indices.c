/*************************************************************/
/*                                                           */
/*                   In the name of God                      */
/*                                                           */
/*  file    : minswp_indices.c      (Part of MQMinesweeper)  */
/*  author  : Mohammad_Qadiany                               */
/*                                                           */
/*  https://github.com/Qadiany3D                             */
/*                                                           */
/*************************************************************/

#include "minswp_indices.h"

#include <malloc.h>
#include <memory.h>
#include "minswp_shwmsg.h"

/*===========================================================*/

#pragma pack(push, 1)

/*===========================================================*/

typedef struct _MINSWP_INDICES
{
    HMINSWP_INDICES__   Handle[1];
    QM_SLONG          * p_Indices;
    QM_ULONG            NumIndices;
} MINSWP_INDICES;

/*===========================================================*/

#pragma pack(pop)

/*===========================================================*/

HMINSWP_INDICES             MinSwp_Indices_Create(QM_VOID)
{
    MINSWP_INDICES * p_indices;
    p_indices = (MINSWP_INDICES *)calloc(1, sizeof(MINSWP_INDICES));
    p_indices->p_Indices    = QM_NULL;
    p_indices->NumIndices   = 0;
    return p_indices->Handle;
};

/*===========================================================*/

QM_VOID                     MinSwp_Indices_Release
    (
              HMINSWP_INDICES   h_Indices
    )
{
    if (h_Indices)
    {
        MINSWP_INDICES * p_indices = (MINSWP_INDICES *)h_Indices;
        p_indices->NumIndices = 0;
        if (        p_indices->p_Indices)
        {
            free(   p_indices->p_Indices);
                    p_indices->p_Indices = QM_NULL;
        };
        free(h_Indices);
    };
};

/*===========================================================*/

HMINSWP_INDICES             MinSwp_Indices_CloneCopy
    (
        const HMINSWP_INDICES   h_Indices
    )
{
    MINSWP_INDICES * p_indices;

    const MINSWP_INDICES * p_src =
   (const MINSWP_INDICES *)h_Indices;

    const QM_ULONG  count   = p_src->NumIndices;
    const size_t    sz      = count * sizeof(p_indices->p_Indices[0]);
    p_indices = (MINSWP_INDICES *)MinSwp_Indices_Create();
    p_indices->NumIndices = count;
    p_indices->p_Indices = (QM_SLONG *)malloc(sz);
    memcpy
        (
            p_indices->p_Indices,
            p_src->p_Indices,
            sz
        );
    return p_indices->Handle;
};

/*===========================================================*/

QM_LBOOL                    MinSwp_Indices_Copy
    (
              HMINSWP_INDICES   h_Dst_Indices,
        const HMINSWP_INDICES   h_Src_Indices
    )
{
    if (
            (h_Dst_Indices) &&
            (h_Src_Indices)
        )
    {
        MINSWP_INDICES * p_indices;

        const MINSWP_INDICES * p_src =
       (const MINSWP_INDICES *)h_Src_Indices;

        const QM_ULONG  count   = p_src->NumIndices;
        const size_t    sz      = count * sizeof(p_indices->p_Indices[0]);
        p_indices = (MINSWP_INDICES *)h_Dst_Indices;
        p_indices->NumIndices = count;
        if (p_indices->p_Indices)
            p_indices->p_Indices = realloc(p_indices->p_Indices, sz);
        else
            p_indices->p_Indices = (QM_SLONG *)malloc(sz);

        memcpy
            (
                p_indices->p_Indices,
                p_src->p_Indices,
                sz
            );

        return QM_TRUE;
    }
    else
        return QM_FALSE;
};

/*===========================================================*/

QM_VOID                     MinSwp_Indices_Clear(HMINSWP_INDICES h_Indices)
{
    MINSWP_INDICES * p_indices = (MINSWP_INDICES *)h_Indices;
    p_indices->NumIndices = 0;
};

/*===========================================================*/

QM_SLONG                    MinSwp_Indices_GetCount(const HMINSWP_INDICES h_Indices)
{
    return ((const MINSWP_INDICES *)h_Indices)->NumIndices;
};

/*===========================================================*/

QM_SLONG                    MinSwp_Indices_GetIndex
    (
        const HMINSWP_INDICES   h_Indices,
        const QM_SLONG          I
    )
{
    const MINSWP_INDICES * p_indices =
   (const MINSWP_INDICES *)h_Indices;

    if (
            (0 <= I) &&
            (     I < (QM_SLONG)p_indices->NumIndices)
        )
        return p_indices->p_Indices[I];
    else
        return -1;
};

/*===========================================================*/

QM_SLONG                    MinSwp_Indices_AddIndex
    (
              HMINSWP_INDICES   h_Indices,
        const QM_SLONG          Index
    )
{
    MINSWP_INDICES * p_indices =
   (MINSWP_INDICES *)h_Indices;

    if (
            !MinSwp_Indices_IsMember
                (
                    h_Indices,
                    Index
                )
        )
    {
        const QM_ULONG c = p_indices->NumIndices;
        if (p_indices->p_Indices)
        {
            p_indices->p_Indices = (QM_SLONG *)
                realloc
                    (
                        p_indices->p_Indices,
                        sizeof(p_indices->p_Indices[0]) * (c + 1)
                    );
        }
        else
        {
            p_indices->p_Indices = (QM_SLONG *)
                calloc
                    (
                        c + 1,
                        sizeof(p_indices->p_Indices[0])
                    );
        };
        p_indices->p_Indices[c] = Index;
        p_indices->NumIndices++;
    };
    return (QM_SLONG)p_indices->NumIndices;
};

/*===========================================================*/

QM_LBOOL                    MinSwp_Indices_IsMember
    (
        const HMINSWP_INDICES   h_Indices,
        const QM_SLONG          Index
    )
{
    const MINSWP_INDICES * p_indices =
         (MINSWP_INDICES *)h_Indices;

    /*
    MinSwp_ShowMessage_I((QM_SLONG)p_indices);
    */
    if (p_indices)
    {
        QM_ULONG i;
        const QM_ULONG count = p_indices->NumIndices;

        for (i = 0; i < count; i++)
        {
            if (p_indices->p_Indices[i] == Index)
                return QM_TRUE;
        };
    };
    return QM_FALSE;
};

/*===========================================================*/







