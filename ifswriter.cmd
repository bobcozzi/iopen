 IFSWRITE:   CMD        PROMPT('COZZI - Write Text to IFS file')

             PARM       KWD(STMF) TYPE(*PNAME) LEN(5000) MIN(1) +
                          EXPR(*YES) VARY(*YES) INLPMTLEN(50) +
                          PROMPT('IFS Stream File name')

             PARM       KWD(DATA) TYPE(*CHAR) LEN(5000) EXPR(*YES) +
                          VARY(*YES) INLPMTLEN(80) PROMPT('Data to +
                          write')
             PARM       KWD(RCDDLM) TYPE(*CHAR) LEN(10) DFT(*NONE) +
                          SPCVAL((*CRLF X'0D25') (*CR X'0D') (*LF +
                          X'25') (*LFCR X'250D') (*NONE X'0000')) +
                          EXPR(*YES) VARY(*YES) PROMPT('End of line +
                          symbol or *NONE')

             PARM       KWD(CCSID) TYPE(*UINT4) DFT(819) RANGE(1 +
                          65533) SPCVAL((*ASCII 819) (*UTF8 1208) +
                          (*EBCDIC 37) (*JOB 0)) PROMPT('Stream +
                          File CCSID') 