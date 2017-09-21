/*------------------------------------------------------------------
 * test_wcsnorm_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <locale.h>

#define LEN   ( 128 )

#define PERL_TEST
/* Must have the same Unicode version 10, since 5.27.3
   perl -MUnicode::UCD -e'print Unicode::UCD::UnicodeVersion()'
*/
#ifndef PERL
/*# define PERL "perl"*/
/*# define PERL "cperl5.27.2"*/
# define PERL   "perl5.27.3"
#endif
#define TESTPL  "test-norm.pl"

int main()
{
    errno_t rc;
    int ind;
    wchar_t str[LEN];
    wchar_t str1[LEN];
    size_t len;
    int errs = 0;
    char *lc_cat, *lang;
#ifdef PERL_TEST
    FILE *pl;
    struct stat st;

    pl = fopen(TESTPL, "w");
#endif

/*--------------------------------------------------*/

    rc = wcsnorm_s(NULL, LEN, L"test");
    ERR(ESNULLP);

    rc = wcsnorm_s(str, LEN, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = wcsnorm_s(str, 0, L"test");
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = wcsnorm_s(str, 99999, L"test");
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = wcsnorm_s(str, LEN, L"Caf\xe9");
    ERR(EOK);
    WEXPSTR(str, L"Cafe" L"\x301");
    WCHECK_SLACK(&str[5], LEN-5);

/*--------------------------------------------------*/

    rc = wcsnorm_s(str, LEN, L"Café");
    ERR(EOK)
    WEXPSTR(str, L"Cafe" L"\x301");
    WCHECK_SLACK(&str[5], LEN-5);

/*--------------------------------------------------*/

    rc = wcsnorm_s(str, LEN, L"A" L"\x1fb3");
    ERR(EOK);
    wcscpy(str1, L"A" L"\x3b1" L"\x3b9");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[3], LEN-3);

    rc = wcsnorm_s(str, LEN, L"A" L"\x1fb7");
    ERR(EOK);
    wcscpy(str1, L"a" L"\x3b1" L"\x342" L"\x3b9");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[4], LEN-4);

    rc = wcsnorm_s(str, LEN, L"\x101");
    ERR(EOK);
    wcscpy(str1, L"\x61" L"\x304");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\x115");
    ERR(EOK);
    wcscpy(str1, L"\x65" L"\x306");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\xdf"); /* !nfd */
    ERR(EOK);
    wcscpy(str1, L"\xdf");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\x385");
    ERR(EOK);
    wcscpy(str1, L"\xa8" L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\x3ac");
    ERR(EOK);
    wcscpy(str1, L"\x3b1" L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\x3ad");
    ERR(EOK);
    wcscpy(str1, L"\x3b5" L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\x1f71");
    ERR(EOK);
    wcscpy(str1, L"\x3b1" L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\x1f02");
    ERR(EOK);
    wcscpy(str1, L"\x3b1" L"\x313" L"\x300" L"\x3b9");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[4], LEN-4);

    rc = wcsnorm_s(str, LEN, L"\x1feb");
    ERR(EOK);
    wcscpy(str1, L"\x3c5" L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

    rc = wcsnorm_s(str, LEN, L"\x1fee");
    ERR(EOK);
    wcscpy(str1, L"\xa8" L"\x301");
    WEXPSTR(str, str1);
    WCHECK_SLACK(&str[2], LEN-2);

/*--------------------------------------------------*/

    /* see if we can lower-case and decompose all */
#ifdef PERL_TEST
    fprintf_s(pl, "use v5.27.2;\nno warnings;\nuse Unicode::Normalize;\nmy $err;"
              "sub chk {\n"
              "  my ($ch, $got) = @_;\n"
              "  my $nfd = NFD($ch);\n"
              "  if ($nfd ne $got) {\n"
              "    printf \"Error NFD \\\\x{%%X} = %%s; got: %%s\\n\",\n"
              "         unpack('W*',$ch),\n"
              "         join('',map{sprintf'\\x{%%X}',$_} unpack 'W*',$nfd),\n"
              "         join('',map{sprintf'\\x{%%X}',$_} unpack 'W*',$got);\n"
              "    1\n"
              "  }\n"
              "}\n");
#endif
    for (ind=0xc0; ind<0x02fa20; ind++) {
        static wchar_t src[5];
        int c;
        src[0] = ind;
        src[1] = 0;
        c = iswfc(ind);
        /*debug_printf("%s %u  U+%04X\n", __FUNCTION__, __LINE__, (int)ind);*/
        rc = wcsnorm_s(str, 5, src);
        if (!c) {
            ERR(EOK);
        } else if (rc) { /* 409 is not found, a iswfc() mismatch */
            debug_printf("%s %u  Error %d U+%4X L\"%ls\" => L\"%ls\"\n",
                         __FUNCTION__, __LINE__, (int)rc, (int)ind, src, str);
            errs++;
        }
#ifdef PERL_TEST
        {
            int len = wcslen(str);
            int i;
            /* cross-check with perl */
            fprintf_s(pl, "$err += chk(\"\\N{U+%04X}\",\"\\N{U+%04X}", ind, str[0]);
            for (i=1;i<len;i++) {
                fprintf_s(pl, "\\N{U+%04X}", str[i]);
            }
            fprintf_s(pl, "\");\n");
        }
#endif
    }

/*--------------------------------------------------*/

#ifdef PERL_TEST
    fprintf_s(pl, "exit $err;\n");
#ifdef BSD_ALL_LIKE
    fstat(pl->_file, &st);
    fclose(pl);
#elif defined __GLIBC__
    fstat(pl->_fileno, &st);
    fclose(pl);
#else
    fclose(pl);
    stat(TESTPL, &st);
#endif
    if (st.st_size) {
        printf("Cross check with " PERL ":\n");
        fflush(stdout);
        if (system(PERL " " TESTPL) < 0) {
            printf("Redo with perl (probably wrong Unicode version):\n");
            fflush(stdout);
            system("perl " TESTPL);
        }
    }
#ifndef DEBUG
    unlink(TESTPL);
#endif

#endif /* PERL_TEST */

/*--------------------------------------------------*/

    return (errs);
}