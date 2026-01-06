# If 3 < 5 → result = 100

PUSH 3
PUSH 5
CMP
JZ else

PUSH 100
HALT

else:
PUSH 0
HALT
