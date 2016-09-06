#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ifs.h>
#include <unistd.h>
#include "micptcom.mih"
#include "qp0ztrc.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef _Packed struct tagVARChar_T
{
   _Packed union {
     uint16_t length;
     uint16_t len;
   };
   _Packed union {
     char    data[5000];
     char    value[5000];
   };
} VARCHAR_T;


void main(int argc, char *argv[])
{

     int  hIFS = -1;
     int  len = 0;
     char szFile[5000];

     VARCHAR_T*  pFile;
     VARCHAR_T*  pData;
     VARCHAR_T*  pLF;

     int    fFlags = O_WRONLY | O_TEXTDATA | O_CCSID;
     mode_t fmode = S_IRWXU | S_IRWXO | S_IRWXG;

     unsigned int file_ccsid = 819;
     unsigned int open_ccsid = 0;

     if (argc > 1)
       pFile = (VARCHAR_T*) argv[1];
     if (argc > 2)
       pData = (VARCHAR_T*) argv[2];
     if (argc > 3)
       pLF   = (VARCHAR_T*) argv[3];
     if (argc > 4)
       file_ccsid   = *((unsigned int*) argv[4]);

     if (pFile == NULL) {
       Qp0zLprintf("ifsWriter: No file specified.\n");
       return ;
     }

     if (pFile->length > 0)
     {
        len = pFile->length;
        while (pFile->data[len-1]==' ')
        {
           --len;
        }
     }
     strncpy(szFile, pFile->data, len);
     szFile[len] = '\0';

     if ( access(szFile, F_OK) != 0) {  // Not found?
        fFlags += O_TEXT_CREAT + O_CREAT;
        hIFS = open(szFile, fFlags, fmode, file_ccsid, open_ccsid);
     }
     else
     {
        // if file already exists, then add to it
       fFlags += O_APPEND;
       hIFS = open(szFile, fFlags, fmode, 0);
     }
     if (hIFS < 0) {
       Qp0zLprintf("ifsWriter: Failed to open %s\n", szFile);
       return ;
     }

     write( hIFS, pData->data, pData->length );

     if (pLF->length > 0)   // Did the caller request that LF/CR be added?
     {
       if (pLF->data[0] != '\0')
          write( hIFS, pLF->data, pLF->length );
     }
     close( hIFS );

     return;
} 
