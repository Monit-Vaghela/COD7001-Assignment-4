# f(x)=x*x , g(x)=f(x)+1 → g(4)=17

PUSH 4
CALL g
HALT

g:
CALL f
PUSH 1
ADD
RET

f:
DUP
MUL
RET
