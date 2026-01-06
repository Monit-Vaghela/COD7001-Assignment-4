# GCD(48, 18) = 6

# a = 48
PUSH 48
STORE 0

# b = 18
PUSH 18
STORE 1

loop:
# if a < b → b = b - a
LOAD 0
LOAD 1
CMP
JZ a_ge_b

# b = b - a
LOAD 1
LOAD 0
SUB
STORE 1
JMP loop

a_ge_b:
# if b < a → a = a - b
LOAD 1
LOAD 0
CMP
JZ end

# a = a - b
LOAD 0
LOAD 1
SUB
STORE 0
JMP loop

end:
# a == b → GCD
LOAD 0
HALT
