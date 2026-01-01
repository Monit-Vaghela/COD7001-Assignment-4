# Sum numbers from 1 to 5 → 15

PUSH 0        # sum
PUSH 1        # i

loop:
DUP
PUSH 6
CMP
JZ end

DUP
LOAD 0
ADD
STORE 0

PUSH 1
ADD
JMP loop

end:
LOAD 0
HALT
