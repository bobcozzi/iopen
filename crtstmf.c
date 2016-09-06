
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ifs.h>
#include <unistd.h>
#include <qp0ztrc.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <triml.mih>
#include <cpybytes.mih>



typedef _Packed struct tagVARChar_T
{
    union {
      uint16_t length;
      uint16_t len;
   };
    union {
     char    data[5000];
     char    value[5000];
   };
} VARCHAR_T;

int ifsCrtFile(const VARCHAR_T* pFile, unsigned int* pCCSID);

void main(int argc, char *argv[])
{
    // NOTE: in C, parm 1 is the program name.
    // Real parameters start with parm 2.
   const VARCHAR_T* pUserFile = NULL;
   unsigned int  ccsid = 819;

   if (argc >= 2) // Parm 1?
   {
      if ( ((VARCHAR_T*) argv[1])->length > 0)
      {
       pUserFile = (VARCHAR_T*) argv[1];
      }
   }
   if (argc >= 3)  // Parm 2?
   {
       ccsid = *((unsigned int*) argv[2]);
   }
   if (pUserFile != NULL)
   {
     ifsCrtFile(pUserFile, &ccsid);
   }

   return;
}


int ifsCrtFile(const VARCHAR_T* pFile, unsigned int* pCCSID)

{
     int  hIFS;
     char szFile[5000];

     int    i = 0;
     int    len = 0;
     int    fFlags = O_WRONLY | O_TEXTDATA | O_CCSID;
     mode_t fmode  = S_IRWXU | S_IRWXO | S_IRWXG;

     unsigned int file_ccsid = 819; // Default to PC ASCII
     unsigned int open_ccsid = 0;   // Default to JOB CCSID

     szFile[0] = 0x00;

     if (pFile->length > 0)
     {
        len = pFile->length;
        while (pFile->data[len-1]==' ')
        {
           --len;
        }
     }

     if (pFile == NULL || pFile->length == 0 ||
                          len > sizeof(szFile))
     {
       Qp0zLprintf("ifs CRTFILE: No file name specified. Name length(%d)\n",
                    pFile->length);
       return ;
     }

     _CPYBYTES(szFile,pFile->data,len);
     szFile[pFile->length] = 0x00;

     if (pCCSID != NULL && *pCCSID >= 0)
         file_ccsid = *pCCSID;

     if ( access(szFile, F_OK) != 0)   // File Not found?
     {
        fFlags += O_TEXT_CREAT + O_CREAT;  // Create the file
        hIFS = open(szFile, fFlags, fmode, file_ccsid, open_ccsid);
       if (hIFS < 0)
       {
         Qp0zLprintf("ifs CRTFILE: Failed to create %s\n", szFile);
       }
       else
       {
         Qp0zLprintf("Created ifs file: %s,%d\n", szFile,file_ccsid);
         close( hIFS );
       }
     }
     else
     {
       Qp0zLprintf("ifs CRTFILE: File already exists. %s\n", szFile);
     }

     return;
}
 