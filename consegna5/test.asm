@22
  @31
@1
// in tutti questi test a = 0
M=0
D=+1 
MD=-1
A=D
AM=A
AD=!D
AMD=!A

M=-D
D=-A 
MD=D+1
A=A+1
AM=D-1
AD=A-1
AMD=D+A

M=D-A
D=A-D
MD=D&A
A=D|A
// Da qui in poi a = 1
AM=M
AD=!M
AMD=-M

M=M+1
D=M-1
MD=D+M
A=D-M
AM=M-D
AD=D&M
AMD=D|M