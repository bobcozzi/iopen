#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

  // This allows memicmp to be included
#define  __cplusplus__strings__
#include <string.h>

#include <QUSEC.h>
#include <QUSCRTUS.h>
#include <QUSCUSAT.h>

        /**********************************************************/
        /* Copyright 1992-2014 by R. Cozzi, Jr.                   */
        /* All rights reserved. Reproduction permitted            */
        /* MIT License                                            */
        /* for non-resale purposes only.                          */
        /* Part of the iOpen Project by Bob Cozzi                 */
        /* http://www.cozTools.com/iOpen                          */
        /**********************************************************/

     /*************************************************************/
     /* Create User Space (CRTUSRSPC) Command Processing Program  */
     /* Author  :  Bob Cozzi                                      */
     /* Date    :  07May2014                                      */
     /* Purpose :  Stand-alone program to create *USRSPC object.  */
     /*-----------------------------------------------------------*/
     /* Rev. Log:                                                 */
     /*  14June2014 - Released to public via midrangeNews.com     */
     /*  29June2014 - Added PubAut parameter to command.          */
     /*  30June2014 - Added AutoExtend parameter to command.      */
     /*************************************************************/


typedef _Packed struct Qus_Attr_t {
           int  count;
           int  key;
           int  dataLen;
           char data[10];
} usAttr_t;

#define autoExtend 3
#define copyParm(varName, parmNum) \
        if (argv[parmNum] != NULL) \
           memcpy(varName,argv[parmNum],sizeof(varName))

void main(int argc, char *argv[])
{
        char us_name[20];
        char us_attr[10];
        int  us_initSize = 0;
        char us_replace[10];
        char us_autoExtend = '1';
        char us_text[50];
        char us_pubAut[10];

        char     initValue = 0x00;
        char     rtnLib[10];
        usAttr_t spaceAttr;
        Qus_EC_t ec;

        spaceAttr.count = 1;
        spaceAttr.key = autoExtend;  // Auto-extend
        spaceAttr.data[0]='1';

        memset(us_name,' ',sizeof(us_name));
        memset(us_attr,' ',sizeof(us_attr));
        memset(us_text,' ',sizeof(us_text));
        memset(us_pubAut,' ',sizeof(us_pubAut));
        memset(us_replace,' ',sizeof(us_replace));

        memset(rtnLib,' ',sizeof(rtnLib));
        copyParm(us_name,1);
        copyParm(us_attr,2);
        copyParm(us_replace,5);
        copyParm(us_text,6);
        copyParm(us_pubAut,7);

        if (argv[3]!=NULL)
           us_initSize = *(int*)argv[3];
        if (argv[4]!=NULL)
           us_autoExtend = *(char*)argv[4];

        if (us_attr==NULL || us_attr[0]== ' ' ||
            memicmp(us_attr,"*NONE",5)   != 0 ||
            memicmp(us_attr,"*BLANKS",7) != 0 ||
            memicmp(us_attr,"*BLANK",6)  != 0)
         memset(us_attr,' ',sizeof(us_attr));

        if (us_text==NULL || us_text[0]== ' ' ||
            memicmp(us_text,"*NONE",5)   != 0  ||
            memicmp(us_text,"*BLANKS",7) != 0  ||
            memicmp(us_text,"*BLANK",6)  != 0)
         memset(us_text,' ',sizeof(us_text));

        if (us_replace!=NULL && us_replace[0]!= ' ' &&
            (memicmp(us_replace,"*YES",4)==0 ||
             memicmp(us_replace,"*REPLACE",8)==0 ||
             us_replace[0]=='1')) {
            memset(us_replace,' ',sizeof(us_replace));
            memcpy(us_replace,"*YES",4);
        }
        else {
            memset(us_replace,' ',sizeof(us_replace));
            memcpy(us_replace,"*NO",3);
        }

        memset(&ec,0x00,sizeof(ec));
        ec.Bytes_Provided = sizeof(ec);
        QUSCRTUS( us_name, us_attr, us_initSize, &initValue,
                  us_pubAut, us_text, us_replace, &ec);

        if (ec.Bytes_Available == 0 && us_autoExtend=='1') {  // Created it?
          memset(&ec,0x00,sizeof(ec));
          ec.Bytes_Provided = sizeof(ec);
          QUSCUSAT( rtnLib, us_name, &spaceAttr, &ec);
        }

    return;
} 