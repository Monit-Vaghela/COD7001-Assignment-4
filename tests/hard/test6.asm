# A=[1,2,3,4], B=[10,20,30,40]
# C=A+B → C=[11,22,33,44]
# Final stack value = 44

# --------------------
# i = 0
# --------------------
PUSH 0
STORE 100

# --------------------
# Initialize arrays
# --------------------
init_loop:
LOAD 100
PUSH 4
CMP
JZ add_phase

# A[i] = i + 1
LOAD 100
PUSH 1
ADD
LOAD 100
ADD
STORE 0

# B[i] = (i + 1) * 10
LOAD 100
PUSH 1
ADD
PUSH 10
MUL
LOAD 100
ADD
STORE 10

# i++
LOAD 100
PUSH 1
ADD
STORE 100

JMP init_loop

# --------------------
# i = 0 again
# --------------------
add_phase:
PUSH 0
STORE 100

# --------------------
# C[i] = A[i] + B[i]
# --------------------
add_loop:
LOAD 100
PUSH 4
CMP
JZ end

# Load A[i]
LOAD 100
LOAD 0
ADD
LOAD 0

# Load B[i]
LOAD 100
LOAD 10
ADD
LOAD 10

# Add and store in C[i]
ADD
LOAD 100
LOAD 20
ADD
STORE 20

# i++
LOAD 100
PUSH 1
ADD
STORE 100

JMP add_loop

# --------------------
# End
# --------------------
end:
LOAD 23     # C[3] = 44
HALT
