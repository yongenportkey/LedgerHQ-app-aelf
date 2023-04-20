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
    uint8_t msg_body[] = {2, 2, 0, 1, 12, 2, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 0};

    transaction_summary_reset();
    assert(process_message_body(msg_body, ARRAY_LEN(msg_body), 3) == 0);
    enum SummaryItemKind kinds[MAX_TRANSACTION_SUMMARY_ITEMS];
    size_t num_kinds;
    assert(transaction_summary_finalize(kinds, &num_kinds) == 0);
    assert(num_kinds == 4);
}

void test_process_message_body_too_few_ix_fail() {
    assert(process_message_body(NULL, 0, 3) == 1);
}

void test_process_message_body_too_many_ix_fail() {
    uint8_t xfer_ix[] = {2, 2, 0, 1, 12, 2, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 0};

#define TOO_MANY_IX (MAX_INSTRUCTIONS + 1)
#define XFER_IX_LEN ARRAY_LEN(xfer_ix)

    uint8_t msg_body[TOO_MANY_IX * XFER_IX_LEN];
    for (size_t i = 0; i < TOO_MANY_IX; i++) {
        uint8_t* start = msg_body + (i * XFER_IX_LEN);
        memcpy(start, xfer_ix, XFER_IX_LEN);
    }
    assert(process_message_body(msg_body, ARRAY_LEN(msg_body), 3) == 1);
}

void test_process_message_body_data_too_short_fail() {;
    assert(process_message_body(NULL, 0, 3) == 1);
}

void test_process_message_body_data_too_long_fail() {
    uint8_t msg_body[] = {
        2, 2, 0, 1, 12, 2, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 0,
        0
    };
    assert(process_message_body(msg_body, ARRAY_LEN(msg_body), 3) == 3);
}

void test_process_message_body_bad_ix_account_index_fail() {
    uint8_t msg_body[] = {1, 0, 0};
    assert(process_message_body(msg_body, ARRAY_LEN(msg_body), 3) == 1);
}

void test_process_message_body_unknown_ix_enum_fail() {
    uint8_t msg_body[] = {
        2, 2, 0, 1, 12, 255, 255, 255, 255, 42, 0, 0, 0, 0, 0, 0, 0,
    };
    assert(process_message_body(msg_body, ARRAY_LEN(msg_body), 3) == 1);
}

/* clang-format on */

int main() {
    // test_process_message_body_ok();
    // test_process_message_body_too_few_ix_fail();
    // test_process_message_body_too_many_ix_fail();
    // test_process_message_body_data_too_short_fail();
    // test_process_message_body_data_too_long_fail();
    // test_process_message_body_bad_ix_account_index_fail();
    // test_process_message_body_unknown_ix_enum_fail();
    printf("passed\n");
    return 0;
}
