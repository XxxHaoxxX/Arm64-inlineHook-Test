#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

static int calc(int a, int b) {
    int result = 999;
    if (a < b)
        return result - a * b;
    else
        return (int)(result / a - b);
    return a + b;
}

static int hook_calc(int a, int b) {
    printf("call calc(%d, %d)\n", a, b);
    return 666;
}

static void make_writable(void* addr) {
    size_t page_size = sysconf(_SC_PAGESIZE);
    uintptr_t page = (uintptr_t)addr & ~(page_size - 1);
    mprotect((void*)page, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);
}

static void inline_hook(void* target, void* replacement) {
    make_writable(target);

    uint32_t* code = (uint32_t*)target;
    printf("[hook] target      = %p\n", target);
    printf("[hook] replacement = %p\n", replacement);

    printf("[hook] before patch:\n");
    for (int i = 0; i < 4; i++)
        printf("    %08x\n", code[i]);

    code[0] = 0x58000051;
    code[1] = 0xD61F0220;

    uint64_t addr = (uint64_t)replacement;
    code[2] = (uint32_t)(addr & 0xffffffff);
    code[3] = (uint32_t)(addr >> 32);

    __builtin___clear_cache((char*)target, (char*)target + 16);

    printf("[hook] after patch:\n");
    for (int i = 0; i < 4; i++)
        printf("    %08x\n", code[i]);
}

using Calc = int (*)(int, int);
static volatile Calc calc_ptr = nullptr;

__attribute__((constructor)) static void init() {
    calc_ptr = calc;
    printf("[hook] calc before = %d\n", calc_ptr(2, 5));
    inline_hook((void*)calc, (void*)hook_calc);
    usleep(1000);
    while(true) {
        printf("[hook] calc after = %d\n", calc_ptr(2, 5));
        sleep(1);
    }
}