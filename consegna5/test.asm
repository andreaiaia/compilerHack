@22
  @31
@1
// in tutti questi test a = 0
M=0
D=1 
MD=-1
A=D
(ETICHETTA)
D;JGT
AM=A
AD=!D
AMD=!A
M=-D
D=-A
D;JEQ
MD=D+1
A=A+1
AM=D-1
AD=A-1
AMD=D+A
(PROVA)
M=D-A
D;JGE
D=A-D
(TERZA)
MD=D&A
A=D|A
// Da qui in poi a = 11
AM=M; JLT
AD=!M
AMD=-M
M=M+1
M; JNE
D=M-1
MD=D+M
A=D-M
M; JLE
AM=M-D
AD=D&M
AMD=D|M
0;JMP