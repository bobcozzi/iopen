 CRTUSRSPC:  CMD        PROMPT('iOpen: Create User Space')
        /**********************************************************/
        /* Copyright 1992-2014 by R. Cozzi, Jr.                   */
        /* All rights reserved. Reproduction permitted            */
        /* for non-resale purposes only.                          */
        /* Part of the iOpen Project by Bob Cozzi                 */
        /* http://www.cozTools.com/iOpen                          */
        /**********************************************************/

             PARM       KWD(USRSPC) TYPE(QUAL) MIN(1) +
                          PROMPT('User Space')
 QUAL:       QUAL       TYPE(*NAME) MIN(1) EXPR(*YES)
             QUAL       TYPE(*NAME) DFT(*CURLIB) SPCVAL((*LIBL) +
                          (*CURLIB)) EXPR(*YES) PROMPT('Library')
             PARM       KWD(OBJATR) TYPE(*NAME) LEN(10) DFT(*BLANK) +
                          SPCVAL((*BLANK ' ')) EXPR(*YES) +
                          PROMPT('Object attribute') CHOICE('obj attr, *BLANK')
             PARM       KWD(SIZE) TYPE(*INT4) DFT(32K) RANGE(1 +
                          16776704) SPCVAL((1K 1024) (4K 4096) (32K +
                          32768) (1M 1048576) (2M 2097152) +
                          (*MAX16MB 16776704)) CHOICE('size, 1K, +
                          4K, 32K, 1M, 2M...') PROMPT('Initial size')

             PARM       KWD(AUTOEXT) TYPE(*LGL) RSTD(*YES) +
                          DFT(*YES) SPCVAL((*YES '1') (*NO '0')) +
                          EXPR(*YES) PROMPT('Auto extend')
             PARM       KWD(REPLACE) TYPE(*CHAR) RSTD(*YES) DFT(*NO) +
                          SPCVAL((*NO '0') (*YES '1')) EXPR(*YES) +
                          PROMPT('Replace')
             PARM       KWD(TEXT) TYPE(*CHAR) LEN(50) DFT(*BLANK) +
                          SPCVAL((*BLANK ' ')) EXPR(*YES) +
                          PROMPT('Text ''description''')
             PARM       KWD(AUT) TYPE(*CHAR) LEN(10) RSTD(*YES) +
                          DFT(*LIBCRTAUT) SPCVAL((*LIBCRTAUT) +
                          (*CHANGE) (*EXCLUDE) (*USE) (*ALL)) +
                          EXPR(*YES) PROMPT('Authority')
 