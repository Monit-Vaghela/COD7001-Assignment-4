# memory[0]=10, memory[1]=20 → sum = 30

PUSH 10
STORE 0

PUSH 20
STORE 1

LOAD 0
LOAD 1
ADD
HALT
