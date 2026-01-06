#include <vm/vm.h>
#include <assembler/assembler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vm/metrics.h>

long instruction_count = 0;

clock_t stack_time = 0;
clock_t memory_time = 0;
clock_t alu_time = 0;
clock_t control_time = 0;
clock_t other_time = 0;

clock_t exec_time = 0;

static void write_benchmark(){
    double total = (double)exec_time;

    FILE *txt = fopen("benchmarks/benchmark.txt", "w");
    FILE *csv = fopen("benchmarks/benchmark.csv", "w");

    fprintf(txt, "================ Execution Time Breakdown ================\n");
    fprintf(txt, "Total Instructions Executed : %ld\n\n", instruction_count);

    fprintf(txt, "Total Execution Time : %.6f s (100.00%%)\n",
            (double)exec_time / CLOCKS_PER_SEC);

    fprintf(txt, "Stack Operations     : %.6f s (%6.2f%%)\n",
            (double)stack_time / CLOCKS_PER_SEC,
            (stack_time * 100.0) / total);

    fprintf(txt, "Memory Operations    : %.6f s (%6.2f%%)\n",
            (double)memory_time / CLOCKS_PER_SEC,
            (memory_time * 100.0) / total);

    fprintf(txt, "ALU Operations       : %.6f s (%6.2f%%)\n",
            (double)alu_time / CLOCKS_PER_SEC,
            (alu_time * 100.0) / total);

    fprintf(txt, "Control Instructions : %.6f s (%6.2f%%)\n",
            (double)control_time / CLOCKS_PER_SEC,
            (control_time * 100.0) / total);

    fprintf(txt, "Other                : %.6f s (%6.2f%%)\n",
            (double)other_time / CLOCKS_PER_SEC,
            (other_time * 100.0) / total);

    fprintf(txt, "==========================================================\n");

    // Writing into a csv file.
    fprintf(csv, "metric,value\n");
    fprintf(csv, "instruction_count,%ld\n", instruction_count);
    fprintf(csv, "\n");
    fprintf(csv, "category,time_seconds,percentage\n");
    fprintf(csv, "stack,%.6f,%.2f\n",
            (double)stack_time / CLOCKS_PER_SEC,
            (stack_time * 100.0) / total);

    fprintf(csv, "memory,%.6f,%.2f\n",
            (double)memory_time / CLOCKS_PER_SEC,
            (memory_time * 100.0) / total);

    fprintf(csv, "alu,%.6f,%.2f\n",
            (double)alu_time / CLOCKS_PER_SEC,
            (alu_time * 100.0) / total);

    fprintf(csv, "control,%.6f,%.2f\n",
            (double)control_time / CLOCKS_PER_SEC,
            (control_time * 100.0) / total);

    fprintf(csv, "other,%.6f,%.2f\n",
            (double)other_time / CLOCKS_PER_SEC,
            (other_time * 100.0) / total);

    fprintf(csv, "total,%.6f,100.00\n",
            (double)exec_time / CLOCKS_PER_SEC);

    fclose(txt);
    fclose(csv);
}

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
    int benchmark = 0;

    char *mode = NULL;
    char *file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            benchmark = 1;
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
        printf("  %s [-d] [-b] -a  program.asm\n", argv[0]);
        printf("  %s [-d] [-b] -e  program.bin\n", argv[0]);
        printf("  %s [-d] [-b] -ae program.asm\n", argv[0]);
        return 1;
    }

    if (strcmp(mode, "-a") == 0) {
        if (assemble(file, NULL) < 0) return 1;
        printf("Assembly completed: %s.bin generated\n", file);
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

    vm vm;
    vm_init(&vm, code, code_size);
    clock_t t1 = clock();
    vm_run(&vm, debug, benchmark);
    clock_t t2 = clock();
    exec_time += (t2 - t1);

    other_time = exec_time - (stack_time + memory_time + alu_time + control_time);

    if (benchmark) write_benchmark();
    free(code);
    return 0;
}
