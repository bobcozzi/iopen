 RTVUSRSPC:  CMD        PROMPT('iOpen: Retrieve *USRSPC Data') +                                    
                          ALLOW(*IPGM *BPGM *IMOD *BMOD)                                            
        /**********************************************************/                                
        /* Copyright 1992-2014 by R. Cozzi, Jr.                   */                                
        /* All rights reserved. Reproduction permitted            */                                
        /* for non-resale purposes only.                          */                                
        /* Part of the iOpen Project by Bob Cozzi                 */                                
        /* http://www.cozTools.com/iOpen                          */                                
        /**********************************************************/                                
                                                                                                    
             PARM       KWD(USRSPC) TYPE(E1) MIN(1) PROMPT('User space')                    
 E1:         ELEM       TYPE(Q1) MIN(1) PROMPT('User space')                                        
             ELEM       TYPE(E2) SNGVAL((*ALL 0)) MIN(1) +                                          
                          PROMPT('Substring specifications')                                        
 Q1:         QUAL       TYPE(*NAME) MIN(1) EXPR(*YES)                                               
             QUAL       TYPE(*NAME) DFT(*LIBL) SPCVAL((*LIBL) +                                     
                          (*CURLIB)) EXPR(*YES) PROMPT('Library')                                   
 E2:         ELEM       TYPE(*INT4) RANGE(1 16776704) MIN(1) +                                      
                          EXPR(*YES) PROMPT('Substring starting +                                   
                          position')                                                                
             ELEM       TYPE(*INT4) RANGE(1 5000) SPCVAL((*END -1)) +                               
                          MIN(1) EXPR(*YES) PROMPT('Substring length')                              
                                                                                                    
             PARM       KWD(RTNVAR) TYPE(*X) LEN(1) RTNVAL(*YES) +                                  
                          MIN(1) VARY(*YES *INT2) PASSATR(*YES) +                                   
                          PROMPT('CL Variable for return value')                                    
                                                                                                     