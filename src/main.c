#include <vm/vm.h>
#include <assembler/assembler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *load_bin(const char *filename, int *size) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    rewind(fp);

    unsigned char *buf = malloc(*size);
    if (!buf) {
        fclose(fp);
        return NULL;
    }

    fread(buf, 1, *size, fp);
    fclose(fp);
    return buf;
}

int main(int argc, char **argv) {
    unsigned char *code = NULL;
    int code_size = 0;
    int debug = 0;

    char *mode = NULL;
    char *file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = 1;
        } else if (strcmp(argv[i], "-a") == 0 ||
                   strcmp(argv[i], "-e") == 0 ||
                   strcmp(argv[i], "-ae") == 0) {
            mode = argv[i];
        } else {
            file = argv[i];
        }
    }

    if (!mode || !file) {
        printf("Usage:\n");
        printf("  %s [-d] -a  program.asm\n", argv[0]);
        printf("  %s [-d] -e  program.bin\n", argv[0]);
        printf("  %s [-d] -ae program.asm\n", argv[0]);
        return 1;
    }

    if (strcmp(mode, "-a") == 0) {
        if (assemble(file, NULL) < 0) return 1;
        printf("Assembly completed\n");
        return 0;
    }

    if (strcmp(mode, "-e") == 0) {
        code = load_bin(file, &code_size);
        if (!code) return 1;
    }

    if (strcmp(mode, "-ae") == 0) {
    code_size = assemble(file, &code);
    if (code_size < 0) return 1;
}


    printf("Debug mode: %s\n", debug ? "ON" : "OFF");

    vm vm;
    vm_init(&vm, code, code_size);
    vm_run(&vm, debug);
    free(code);
    return 0;
}
