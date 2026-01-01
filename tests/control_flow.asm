# if (5 == 5) push 1 else push 0

PUSH 5
PUSH 5
CMP
JZ equal

PUSH 0
JMP end

equal:
PUSH 1

end:
HALT
