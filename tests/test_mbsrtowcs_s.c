/*------------------------------------------------------------------
 * test_mbsrtowcs_s
 * File 'wchar/mbsrtowcs_s.c'
 * Lines executed:97.44% of 39
 * locale specific, sets it to C and UTF-8
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static wchar_t   dest[LEN];
static char      src[LEN];

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#define CLRPS \
  memset (&ps, '\0', sizeof (mbstate_t))

int test_mbsrtowcs_s (void)
{
    errno_t rc;
    size_t ind;
    /*uint32_t i;*/
    const char *cs;
    mbstate_t ps;
    const char *lang;
    const char *lc_cat;
    int errs = 0;

/*--------------------------------------------------*/

    cs = "a";
    rc = mbsrtowcs_s(NULL, NULL, LEN, &cs, 0, &ps);
    ERR(ESNULLP);
    CLRPS;

    rc = mbsrtowcs_s(&ind, NULL, LEN, &cs, 0, NULL);
    ERR(ESNULLP);

    rc = mbsrtowcs_s(&ind, dest, LEN, NULL, 0, &ps);
    ERR(ESNULLP);
    CLRPS;

    src[0] = '\0';
    rc = mbsrtowcs_s(&ind, NULL, LEN, (const char**)&src, 0, &ps);
    ERR(ESNULLP);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, 0, &cs, 0, &ps);
    ERR(ESZEROL);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, RSIZE_MAX_STR+1, &cs, 3, &ps);
    ERR(ESLEMAX);
    CLRPS;

    cs = "abcdef";
    rc = mbsrtowcs_s(&ind, (wchar_t*)&cs, LEN, &cs, 3, &ps);
    ERR(ESOVRLP);
    CLRPS;

    dest[0] = L'a';
    rc = mbsrtowcs_s(&ind, dest, LEN, (const char**)&dest[0], 1, &ps);
    ERR(ESOVRLP);
    CLRPS;

/*--------------------------------------------------*/

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="abcdef",&cs), 3, &ps);
    ERR(EOK);
    INDCMP(!= 3);
    WCHECK_SLACK(&dest[3], LEN-3);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="abcdef",&cs), 8, &ps);
    ERR(EOK);
    INDCMP(!= 6);
    WCHECK_SLACK(&dest[6], LEN-6);
    CLRPS;

    rc = mbsrtowcs_s(&ind, NULL, LEN, (cs="abcdef",&cs), 2, &ps);
    ERR(EOK);
    INDCMP(!= 6);
    CLRPS;

    SETLOCALE_C;
    SETLANG("C");
    CHKLOCALE_C;

    /* no-breaking space illegal in ASCII, but legal in C */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xa0""abc",&cs), 32, &ps);
    if (rc == 0) { /* legal */
      ERR(EOK);
      INDCMP(!= 4);
      WCHECK_SLACK(&dest[4], LEN-4);
      /* musl on ASCII converts \xa0 to \xdfa0 */
      if ((int)dest[0] != 0xa0 && (int)dest[0] != 0xdfa0) {
        printf("%s %u  Error  ind=%d rc=%d 0x%lx\n",
               __FUNCTION__, __LINE__, (int)ind, rc, (long)dest[0]);
        errs++;
      }
      if (dest[1] != L'a') {
        printf("%s %u  Error  ind=%d rc=%d 0x%lx\n",
               __FUNCTION__, __LINE__, (int)ind, rc, (long)dest[1]);
        errs++;
      }
      if (cs) { /* needs to be at the end */
        printf("%s %u  Error  ind=%d rc=%d %s\n",
               __FUNCTION__, __LINE__, (int)ind, rc, cs);
        errs++;
      }
    } else {
      ERR(EILSEQ); /* or illegal */
      INDCMP(!= -1);
      WCHECK_SLACK(dest, LEN);
      if (dest[0] != L'\0') {
        printf("%s %u  Error  ind=%d rc=%d 0x%lx\n",
               __FUNCTION__, __LINE__, (int)ind, rc, (long)dest[0]);
        errs++;
      }
    }
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\x78",&cs), 1, &ps);
    ERR(EOK);
    INDCMP(!= 1);
    WCHECK_SLACK(&dest[1], LEN-1);
    CLRPS;

    SETLOCALE_UTF8;
    SETLANG("default");
    CHKLOCALE("UTF-8");

    /* illegal sequences (locale dependent) */
    
    /* illegal initial */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xc0",&cs), 1, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xc2",&cs), 1, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing nul */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xc0\x80",&cs), 2, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing slashes */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xc0\xaf",&cs), 2, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xe0\x80\xaf",&cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xf0\x80\x80\xaf",&cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xf8\x80\x80\x80\xaf",&cs), 5, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xfc\x80\x80\x80\x80\xaf",&cs), 6, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing U+0080 */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xe0\x82\x80",&cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing U+07FF */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xe0\x9f\xbf",&cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* aliasing U+0800 */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xf0\x80\xa0\x80",&cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;
      
    /* aliasing U+FFFD */
    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xf0\x8f\xbf\xbd",&cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    /* check enough space for src and conversion errors */

    rc = mbsrtowcs_s(&ind, dest, 6, (cs="abcdef",&cs), 6, &ps);
    ERR(ESNOSPC);
    WCHECK_SLACK(dest, 6);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, 3, (cs="\xf0\x8f\xbf\xbd",&cs), 4, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\x80\xbf\x80",&cs), 3, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);
    CLRPS;

    rc = mbsrtowcs_s(&ind, dest, LEN, (cs="\xfc\x80\x80\x80\x80\x80",&cs), 6, &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);
    WCHECK_SLACK(dest, LEN);

/*--------------------------------------------------*/
    
    return (errs);
}

#endif

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
#ifdef HAVE_WCHAR_H
    return (test_mbsrtowcs_s());
#else
    return 0;
#endif    
}
#endif
