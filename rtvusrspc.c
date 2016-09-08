#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

  // This allows memicmp to be included
#define  __cplusplus__strings__

#include <string.h>

#include <micptcom.mih>
#include <rslvsp.mih>
#include <cpybytes.mih>

#include <except.h>
#include <QUSEC.h>
#include <qp0z1170.h>
#include <QP0ZTRC.h>
#include <QUSPTRUS.h>
#include <QUSRTVUS.h>
#include <QUSRUSAT.h>
#include <QMHRSNEM.h>
#include <QMHMOVPM.h>

        /**********************************************************/
        /* Copyright 1992-2014 by R. Cozzi, Jr.                   */
        /* All rights reserved. Reproduction permitted            */
        /* for non-resale purposes only.                          */
        /* MIT License                                            */
        /* Part of the iOpen Project by Bob Cozzi                 */
        /* http://www.cozTools.com/iOpen                          */
        /**********************************************************/

     /***************************************************************/
     /* Retrieve User Space (RTVUSRSPC) Command Processing Program  */
     /* Author  :  Bob Cozzi                                        */
     /* Date    :  21July2014                                       */
     /* Purpose :  Stand-alone program to retrieve user space data. */
     /*            Command Processing Program for RTVUSRSPC command.*/
     /*            Command Syntax is similar to RTVDTAARA.          */
     /*-------------------------------------------------------------*/
     /* Rev. Log:                                                   */
     /*  21July2014 - Ported to C from RPG IV by Bob Cozzi.         */
     /***************************************************************/

typedef _Packed struct tagDeclen_t
{
    uint8_t    decpos;
    uint8_t    declen;
}  declen_t;

typedef _Packed struct tagCLVar_t
{
   union {
   char       attr;
   char       dataType;
   uint8_t    type;
   };
    union  {
     uint16_t   length;
     uint16_t   len;
     declen_t   dec;    // Numeric length and decimal positions struct
   };
    union {
     char    data[5000];
     char    value[5000];
   };
}  CLVar_t;

typedef _Packed struct tagSpaceLoc_t {
    short elemCount;
    union {
       char usrspc[20];
       struct {
         char  name[10];
         char  library[10];
       };
    };
    short offset;
    short lCount;
    int   start;
    int   length;
} spaceLoc_t;

typedef _Packed struct tagApiError_t {
   Qus_EC_t  qusec;
   char      errmsg[255];
} apiError_t;

int globalError = 0;

static void excp_monitor(_INTRPT_Hndlr_Parms_T *excp_info)
 {

   char       msgKey[4];
   char       msgTypes[4][10];
   int        msgTypeCount;
   int        relInv = 1;
   Qus_EC_t   ec;

     //       1234567890
   memcpy(msgTypes[0], "*COMP     ",10);
   memcpy(msgTypes[1], "*DIAG     ",10);
   memcpy(msgTypes[2], "*INFO     ",10);
   memcpy(msgTypes[3], "*ESCAPE   ",10);
   msgTypeCount = 4;   // 3 instead of 4 means no escape msgs are pushed.
   memset((char*)msgKey,' ',sizeof(msgKey));

     memset((char*)&ec,0x00,sizeof(ec));
     ec.Bytes_Provided = sizeof(Qus_EC_t);
     QMHMOVPM(msgKey, msgTypes , msgTypeCount ,
                    "*PGMBDY   " , relInv , &ec);
     QMHRSNEM(msgKey, &ec);
     globalError = -1;
     return;
 }

void main(int argc, char *argv[])
{
        unsigned char* pSpace = NULL;
        spaceLoc_t* us;
        CLVar_t*    rtnVar;
        int         rtnSize = 0;
        int         start = 0;
        int         len   = 0;
        int         cursize = 0;
        div_t       rm;

        char       msgKey[4];
        char       msgTypes[4][10];
        int        msgTypeCount;

        apiError_t      ec;
        Qus_SPCA_0100_t spaceAttr;

        us  = (spaceLoc_t*) argv[1];
        rtnVar = (CLVar_t*) argv[2];

        switch (us->lCount)
        {
           case 1:
             start = us->start;
             len   = -1;
             break;
           case 2:
             start = us->start;
             len   = us->length;
             break;
        }

        if (rtnVar->attr == _T_PACKED)
        {
           if ((rtnVar->dec.declen % 2) > 0)
           {
              rm = div((rtnVar->dec.declen+1) , 2);
           }
           else
           {
              rm = div((rtnVar->dec.declen+2) , 2);
           }
           rtnSize = rm.quot;
        }
        else if (rtnVar->attr == _T_CHAR)
        {
           rtnSize = rtnVar->length;
        }
        else if (rtnVar->attr == _T_SIGNED)
        {
           rtnSize = rtnVar->dec.declen;
        }

        memset((char*)&ec,0x00,sizeof(ec));
#pragma exception_handler(excp_monitor, 0, 0, _C2_MH_ESCAPE)

        QUSPTRUS( us->usrspc, &pSpace, &ec);
#pragma disable_handler
        if (globalError != 0)
        {
            return;
        }

        if (start < 0)
        {                // Can't be less than zero, so adjust accordingly
            start = 0;
        }
        else if (start > 0)
        {    // We use the offset-of not position-of to retrieve the US.
            start -= 1;
        }
        if (len < 0)
        {     // A length of -1 means retrieve through the *END of the US.
 
          memset((char*)&ec,0x00,sizeof(ec));
          ec.qusec.Bytes_Provided  = sizeof(ec);
          QUSRUSAT( &spaceAttr, sizeof(spaceAttr), "SPCA0100", us->usrspc, &ec);
          if (ec.qusec.Bytes_Available == 0)   // Got the current size?
          {
             cursize = spaceAttr.Space_Size;
          }
          else  // Can't get current size? should NEVER happen.
          {
              cursize = 32;  // use 32 bytes and send message to caller.
              Qp0zLprintf("RTVUSRSPC: Could not retrieve current user space size\n");
          }
          len = (cursize - start);
        }
        if (len > rtnSize)
        {
           len = rtnSize;
        }

 #pragma exception_handler(excp_monitor, 0, 0, _C2_MH_ESCAPE | _C2_MH_FUNCTION_CHECK)

           _CPYBYTES(rtnVar->data,pSpace+start,len);
 #pragma disable_handler
         if (globalError != 0)
         {
             return;
         }

    return;
}
 
