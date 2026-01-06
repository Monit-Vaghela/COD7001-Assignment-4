# Count down from 5 → final value = 0

PUSH 5
STORE 0

loop:
PUSH 0
LOAD 0
CMP
JZ end

LOAD 0
PUSH 1
SUB
STORE 0

JMP loop

end:
LOAD 0
HALT
