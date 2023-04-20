#include "common_byte_strings.h"
#include "message.c"
#include "sol/parser.h"
#include "sol/transaction_summary.h"
#include "util.h"
#include <assert.h>
#include <stdio.h>

// Disable clang format for this file to keep clear buffer formating
/* clang-format off */

void test_process_message_body_ok() {
    uint8_t msg_body[] = {102, 38, 95, 226, 213, 53, 183, 237, 66, 87, 167, 100, 59, 104, 28, 146,
                          132, 152, 243, 119, 211, 52, 7, 237, 187, 198, 146, 131, 169, 188, 76, 161,
                          3, 69, 76, 70, 64, 12, 170, 59, 0, 0, 0, 0};

    transaction_summary_reset();
    assert(process_message_body(msg_body, ARRAY_LEN(msg_body), 3) == 0);
}

/* clang-format on */

int main() {
    test_process_message_body_ok();

    printf("passed\n");
    return 0;
}
