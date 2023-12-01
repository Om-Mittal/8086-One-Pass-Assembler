PRINTM MACRO MESS
           LEA DX, MESS
           MOV AH, 09H
           INT 21H
ENDM

; Macro to take decimal values input
DEC_INPUT MACRO
              LOCAL INPUT,SKIP

              XOR   BX, BX
              MOV   AH, 01H
              INT   21H

              CMP   AL, 0DH
              JE    SKIP
    INPUT:    
              AND   AX, 000FH
              PUSH  AX

              MOV   AX, 10
              MUL   BX
              MOV   BX, AX
              POP   AX
              ADD   BX, AX

              MOV   AH, 01H
              INT   21H
              CMP   AL, 0DH
              JNE   INPUT
    SKIP:     
ENDM

; Macro to output decimal values
DEC_OUTPUT MACRO
               LOCAL START,REPEAT,DISPLAY
    START:                                   
               MOV   AX, BX                  
               XOR   CX, CX                  
               MOV   BX, 10                  
    REPEAT:                                  
               XOR   DX, DX                  
               DIV   BX                      
               PUSH  DX                      
               INC   CX                      
               OR    AX, AX                  
               JNE   REPEAT 
               MOV   AH, 2  
    DISPLAY:                
               POP   DX     
               OR    DL, 30H
               INT   21H                     
               LOOP  DISPLAY
ENDM

EXITP MACRO
          MOV AH, 4CH
          INT 21H
ENDM

PUSHALL MACRO
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
ENDM

POPALL MACRO
           POP DX
           POP CX
           POP BX
           POP AX
ENDM

.MODEL SMALL
.STACK 100H
.DATA
    NUM1     DW ?
    NUM2     DW ?
    RESULT   DW ?
    CHOICE   Dw ?
    
    MSG_MENU DB 10, 13, "   WELCOME TO OUR BASIC ARITHMETIC CALCULATOR!!", 10, 13
             DB "   ENTER 1 TO PERFORM ADDITION.", 10, 13
             DB "   ENTER 2 TO PERFORM SUBTRACTION.", 10, 13
             DB "   ENTER 3 TO PERFORM MULTIPLICATION.", 10, 13
             DB "   ENTER 4 TO PERFORM DIVISION.", 10, 13
             DB "   ENTER 5 TO PERFORM MODULUS.", 10, 13
             DB "   ENTER YOUR CHOICE (1-5) -> $"

    MSG1     DB 10, 13, "   ENTER THE FIRST NUMBER -> $"
    MSG2     DB 10, 13, "   ENTER THE SECOND NUMBER -> $"
    MSG3     DB 10, 13, "   RESULT -> $"
    MSGADD   DB 10, 13, "   OPERATION CHOSEN IS ADDITION $"
    MSGSUB   DB 10, 13, "   OPERATION CHOSEN IS SUBTRACTION $"
    MSGMUL   DB 10, 13, "   OPERATION CHOSEN IS MULTIPLICATION $"
    MSGDIV   DB 10, 13, "   OPERATION CHOSEN IS DIVISION $"
    MSGMOD   DB 10, 13, "   OPERATION CHOSEN IS MODULUS $"
    DEFAULT  DB 10, 13, "   INVALID OPTION CHOSEN, PROGRAM TERMINATED! $"

.CODE
MAIN PROC
                   MOV        AX, @DATA
                   MOV        DS, AX

                   PRINTM     MSG_MENU
                   DEC_INPUT
                   MOV        CHOICE, BX

                   PRINTM     MSG1
                   DEC_INPUT
                   MOV        NUM1, BX
                   
                   PRINTM     MSG2
                   DEC_INPUT
                   MOV        NUM2, BX

                   CMP        CHOICE, 1
                   JE         ADDITION

                   CMP        CHOICE, 2
                   JE         SUBTRACTION

                   CMP        CHOICE, 3
                   JE         MULTIPLICATION

                   CMP        CHOICE, 4
                   JE         DIVISION

                   CMP        CHOICE, 5
                   JE         MODULUS

                   JMP        DEF

    ADDITION:      
                   PRINTM     MSGADD
                   MOV        AX, NUM1
                   ADD        AX, NUM2
                   MOV        RESULT, AX
                   JMP        OUTPUT
    SUBTRACTION:   
                   PRINTM     MSGSUB
                   MOV        AX, NUM1
                   SUB        AX, NUM2
                   MOV        RESULT, AX
                   JMP        OUTPUT
    MULTIPLICATION:
                   PRINTM     MSGMUL
                   MOV        AX, NUM1
                   IMUL       NUM2
                   MOV        RESULT, AX
                   JMP        OUTPUT
    DIVISION:      
                   PRINTM     MSGDIV
                   MOV        AX, NUM1
                   XOR        DX, DX            
                   DIV        NUM2
                   MOV        RESULT, AX
                   JMP        OUTPUT

    MODULUS:       
                   PRINTM     MSGMOD
                   MOV        AX, NUM1
                   XOR        DX, DX
                   DIV        NUM2
                   MOV        RESULT, DX
                   JMP        OUTPUT

    OUTPUT:        
                   PRINTM     MSG3
                   MOV        BX, RESULT
                   PUSHALL
                   DEC_OUTPUT
                   POPALL
                   JMP        EXIT_PROG

    DEF:           
                   PRINTM     DEFAULT
                   JMP        EXIT_PROG
    EXIT_PROG:     
                   EXITP

MAIN ENDP
END MAIN