/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <linux/kernel.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16)
#include <linux/autoconf.h>
#include <linux/config.h>
#endif

#include <linux/time.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0)
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#endif

#include "csr_types.h"
#include "csr_time.h"

CsrTime CsrTimeGet(CsrTime *high)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
    struct timespec64 ts;
#else
    struct timespec ts;
#endif
    CsrUint64 time;
    CsrTime low;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
    ktime_get_coarse_ts64(&ts);
#else
    ts = current_kernel_time();
#endif
    time = (CsrUint64) ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

    if (high != NULL)
    {
        *high = (CsrTime) ((time >> 32) & 0xFFFFFFFF);
    }

    low = (CsrTime) (time & 0xFFFFFFFF);

    return low;
}

void CsrTimeUtcGet(CsrTimeUtc *tod, CsrTime *low, CsrTime *high)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
    struct timespec64 ts;
#else
    struct timespec ts;
#endif
    CsrUint64 time;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
    ktime_get_coarse_ts64(&ts);
#else
    ts = current_kernel_time();
#endif
    time = (CsrUint64) ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

    if (high != NULL)
    {
        *high = (CsrTime) ((time >> 32) & 0xFFFFFFFF);
    }

    if (low != NULL)
    {
        *low = (CsrTime) (time & 0xFFFFFFFF);
    }

    if (tod != NULL)
    {
        struct timeval tv;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
        tv.tv_sec = ts.tv_sec;
        tv.tv_usec = ts.tv_nsec/1000;
#else
        do_gettimeofday(&tv);
#endif
        tod->sec = tv.tv_sec;
        tod->msec = tv.tv_usec / 1000;
    }
}
