#include <dlfcn.h>
#include <stdio.h>

int main() {
    printf("[Loader] loading libhook.so...\n\n");
    void* handle = dlopen("./libhook.so", RTLD_NOW);
    if (!handle) {
        printf("[Loader] dlopen failed:\n%s\n", dlerror());
        return 1;
    }
    printf("\n\n[Loader] libhook.so loaded successfully!\n");
    dlclose(handle);
    return 0;
}