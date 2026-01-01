#include <vm/vm.h>
#include <assembler/assembler.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <program.asm>\n", argv[0]);
        return 1;
    }

    instruction_t *code;
    int size = assemble(argv[1], &code);

    vm_t vm;
    vm_init(&vm, code, size);
    vm_run(&vm);

    return 0;
}
