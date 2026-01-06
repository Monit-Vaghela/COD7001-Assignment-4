# 5! = 120

PUSH 1
STORE 0       # result

PUSH 5
STORE 1       # n

loop:
PUSH 1
LOAD 1
CMP
JZ end

LOAD 0
LOAD 1
MUL
STORE 0

LOAD 1
PUSH 1
SUB
STORE 1

JMP loop

end:
LOAD 0
HALT
