/*------------------------------------------------------------------
 * test_wcscat_s
 * File 'wchar/wcscat_s.c'
 * Lines executed:96.49% of 57
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int test_wcscat_s (void)
{
    errno_t rc;
    int32_t  len1;
    int32_t  len2;
    int32_t  len3;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcscat_s(NULL, LEN, str2);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = wcscat_s(str1, LEN, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = wcscat_s(str1, 0, str2);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = wcscat_s(str1, (RSIZE_MAX_STR+1), str2);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 1, str2);
    ERR(ESUNTERM)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 2, str2);
    ERR(ESUNTERM)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(&str1[0], L"aaaaaaaaaa");
    wcscpy(&str2[0], L"keep it simple");

    len1 = wcslen(str1);
    len2 = wcslen(str2);

    rc = wcscat_s(str1, 50, str2);
    ERR(EOK)
    len3 = wcslen(str1);
    if (len3 != (len1+len2)) {
        debug_printf("%s %u lengths wrong: %u  %u  %u \n",
                     __FUNCTION__, __LINE__, len1, len2, len3);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 1, str2);
    ERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 2, str2);
    ERR(ESNOSPC)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 1, str2);
    ERR(ESUNTERM);
    WCHECK_SLACK(str1, 1);
    
/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 2, str2);
    ERR(ESUNTERM);
    WCHECK_SLACK(str1, 2);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcd");

    rc = wcscat_s(&str1[0], 8, &str1[3]);
    ERR(ESOVRLP);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 8);

    wcscpy(str1, L"abcd");

    rc = wcscat_s(&str1[0], 4, &str1[3]);
    ERR(ESOVRLP);
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 4);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcscat_s(&str1[3], 5, &str1[0]);
    ERR(ESUNTERM);
    WEXPNULL(&str1[3])
    WCHECK_SLACK(&str1[3], 5);

/*--------------------------------------------------*/

    wcscpy(str1, L"abcdefgh");

    rc = wcscat_s(&str1[3], 12, &str1[0]);
    ERR(ESOVRLP);
    WEXPNULL(&str1[3])
    WCHECK_SLACK(&str1[3], 12);

/*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");
    len2 = wcslen(str2);

    rc = wcscat_s(str1, 20, str2);
    ERR(EOK)
    WEXPSTR(str1, str2)
    WCHECK_SLACK(&str1[len2], 20-len2);

/*--------------------------------------------------*/

    str1[0] = L'\0';
    str2[0] = L'\0';

    rc = wcscat_s(str1, LEN, str2);
    ERR(EOK)
    WEXPNULL(str1)
    WCHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, LEN, str2);
    ERR(EOK)
    WEXPSTR(str1, str2)
    WCHECK_SLACK(&str1[len2], LEN-len2);

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqq");
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, LEN, str2);
    ERR(EOK)
    WEXPSTR(str1, L"qqweqqkeep it simple")
    len1 = wcslen(str1);
    WCHECK_SLACK(&str1[len1], LEN-len1);

/*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 12, str2);
    ERR(ESNOSPC)
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 12);

/*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = wcscat_s(str1, 52, str2);
    ERR(EOK)
    WEXPSTR(str1, L"1234keep it simple")
    len1 = wcslen(str1);
    WCHECK_SLACK(&str1[len1], 52-len1);

/*--------------------------------------------------*/

    wcscpy(str1, L"12345678901234567890");

    rc = wcscat_s(str1, 8, &str1[7]);
    ERR(ESOVRLP)
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 8);

/*--------------------------------------------------*/

    wcscpy(str1, L"123456789");

    rc = wcscat_s(str1, 9, &str1[8]);
    ERR(ESOVRLP)
    WEXPNULL(str1)
    WCHECK_SLACK(str1, 9);

/*--------------------------------------------------*/

    wcscpy(str2, L"123");
    wcscpy(str1, L"keep it simple");

    rc = wcscat_s(str2, 31, &str1[0]);
    ERR(EOK)
    WEXPSTR(str2, L"123keep it simple")
    len2 = wcslen(str2);
    WCHECK_SLACK(&str2[len2], 31-len2);

/*--------------------------------------------------*/

    wcscpy(str2, L"1234");
    wcscpy(str1, L"56789");

    rc = wcscat_s(str2, 10, str1);
    ERR(EOK)
    WEXPSTR(str2, L"123456789")
    len2 = wcslen(str2);
    WCHECK_SLACK(&str2[len2], 10-len2);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcscat_s());
}
#endif
