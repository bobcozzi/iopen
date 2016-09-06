 CRTSTMF:    CMD        PROMPT('COZZI-Create IFS Stream File')
 STMF:       PARM       KWD(STMF) TYPE(*PNAME) LEN(5000) MIN(1) +
                          EXPR(*YES) VARY(*YES) PROMPT('New IFS +
                          Stream File name')
             PARM       KWD(CCSID) TYPE(*UINT4) DFT(819) range(1 65533) +
                          SPCVAL((*ASCII 819) (*UTF8 1208) (*EBCDIC +
                          37) (*JOB 0)) PROMPT('Stream File CCSID') 