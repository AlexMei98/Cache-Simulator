#include <iostream>

#include "Reader.h"
#include "Op.h"

int main() {
    Reader reader("../trace/test.trace");
    Op op;
    while (reader.next_op(op)) {
        printf("w: %u | addr: 0x%016lx\n", op.w, op.addr);
    }
    return 0;
}
