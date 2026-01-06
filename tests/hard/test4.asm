# sum of squares from 1 to 3 → 1+4+9 = 14

PUSH 0
STORE 0       # sum

PUSH 1
STORE 1       # i

loop:
LOAD 1
PUSH 4
CMP
JZ end

LOAD 1
CALL square
LOAD 0
ADD
STORE 0

LOAD 1
PUSH 1
ADD
STORE 1

JMP loop

end:
LOAD 0
HALT

square:
DUP
MUL
RET
