# Sum 1..10 = 55

PUSH 0        # sum
STORE 0

PUSH 1        # i
STORE 1

loop:
LOAD 1
PUSH 11
CMP
JZ end

LOAD 0
LOAD 1
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
