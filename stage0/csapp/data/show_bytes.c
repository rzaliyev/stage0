#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_ptr;

void show_byte(byte_ptr start, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x)
{
    show_byte((byte_ptr) &x, sizeof(int));
}

void show_float(float x)
{
    show_byte((byte_ptr) &x, sizeof(float));
}

void show_pointer(void *x)
{
    show_byte((byte_ptr) &x, sizeof(void *));
}

void test_show_bytes(int val)
{
    int   ival = val;
    float fval = (float) ival;
    int  *pval = &ival;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}

int main(void)
{
    test_show_bytes(12345);
    const char *str = "mnopqr";
    show_byte((byte_ptr) str, strlen(str));
    return 0;
}
