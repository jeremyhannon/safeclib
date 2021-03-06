/*------------------------------------------------------------------
 * test_memset_s
 * File 'memset_s.c'
 * Lines executed:100.00% of 21
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 256 )
#define MAX   RSIZE_MAX_MEM

#if defined HAVE_MEMSET_S && defined HAVE_C11 && defined WANT_C11
#define USE_LIBC_MEMSET
#endif

static uint8_t mem1[LEN];

int test_memset_s (void)
{
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint8_t value;
    int errs = 0;

/*--------------------------------------------------*/

    value = 34;
    rc = memset_s(NULL, LEN, value, LEN);
    /* C11 upstream */
#ifndef USE_LIBC_MEMSET
    if (rc != ESNULLP)
#else
    if (rc != EINVAL)
#endif
    {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }

/*--------------------------------------------------*/

    value = 34;

    /* no error with C11 */
    rc = memset_s(mem1, LEN, value, 0);
#ifdef HAVE_C11
    ERR(EOK);
#else
    ERR(ESZEROL)
#endif

/*--------------------------------------------------*/

    rc = memset_s(mem1, MAX+1, value, LEN);
#ifndef USE_LIBC_MEMSET
    if (rc != ESLEMAX)
#else
    if (rc != 0)
#endif
    {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }

/*--------------------------------------------------*/

    rc = memset_s(mem1, LEN, value, MAX+1);
#ifndef USE_LIBC_MEMSET
    if (rc != ESLEMAX)
#else
    if (rc != EOVERFLOW)
#endif
    {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }

/*--------------------------------------------------*/

    rc = memset_s(mem1, LEN, value, LEN+1);
#ifndef USE_LIBC_MEMSET
    if (rc != ESNOSPC)
#else
    if (rc != EOVERFLOW)
#endif
    {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 1;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 2;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 12;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 31;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 99; }

    len = 133;
    value = 34;

    rc = memset_s(mem1, LEN, value, len);
    ERR(EOK)
    for (i=0; i<len; i++) {
        if (mem1[i] != value) {
            debug_printf("%d - %d m1=%d \n",
                 __LINE__, i, mem1[i]);
            errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_memset_s());
}
#endif
