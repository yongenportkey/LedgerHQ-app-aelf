#include "common_byte_strings.h"
#include "instruction.h"
#include "instruction.c"
#include "util.h"
#include <stdio.h>
#include <assert.h>

void test_parse_system_transfer_instruction() {
    uint8_t message[] = {102, 38, 95, 226, 213, 53, 183, 237, 66, 87, 167, 100, 59, 104, 28, 146,
                         132, 152, 243, 119, 211, 52, 7, 237, 187, 198, 146, 131, 169, 188, 76, 161,
                         3, 69, 76, 70, 64, 12, 170, 59, 0, 0, 0, 0};

    Parser parser = {message, sizeof(message)};
    Instruction instruction;
    InstructionInfo info;

    assert(parse_pubkey(&parser, &info.transfer.to) == 0);
    assert(parse_data(&parser, &instruction.ticker, &instruction.ticker_length) == 0);
    assert(parse_u64(&parser, &info.transfer.amount) == 0);

    assert(parser.buffer_length == 0);
}

void test_parse_system_get_tx_result_instruction() {
    uint8_t message[] = {222, 57, 245, 98, 241, 43, 126, 46, 147, 146, 232, 162, 134, 156, 9, 246, 
                         65, 244, 116, 159, 251, 208, 71, 249, 248, 106, 12, 101, 251, 194, 220, 110,
                         25, 243, 168, 153, 70, 176, 149, 11, 249, 97, 129, 59, 166, 179, 125, 34, 157,
                         206, 177, 207, 241, 207, 147, 41, 209, 214, 151, 188, 3, 221, 112, 57, 43, 66,
                         35, 9, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 84, 114, 97, 110, 115, 102, 101, 114, 25, 243, 168, 153,
                         70, 176, 149, 11, 249, 97, 129, 59, 166, 179, 125, 34, 157, 206, 177, 207, 241,
                         207, 147, 41, 209, 214, 151, 188, 3, 221, 112, 57, 3, 69, 76, 70, 64, 12, 170,
                         59, 0, 0, 0, 0};

    Parser parser = {message, sizeof(message)};
    Instruction instruction;
    InstructionInfo info;

    assert(parse_pubkey(&parser, &info.getTxResult.from) == 0);
    assert(parse_pubkey(&parser, &info.getTxResult.chain) == 0);
    assert(parse_u64(&parser, &info.getTxResult.ref_block_number) == 0);
    assert(parse_sized_string(&parser, &info.getTxResult.method_name) == 0);
    assert(parse_pubkey(&parser, &info.getTxResult.to) == 0);
    assert(parse_data(&parser, &instruction.ticker, &instruction.ticker_length) == 0);
    assert(parse_u64(&parser, &info.getTxResult.amount) == 0);
    
    assert(parser.buffer_length == 0);
}

// int test_print_system_transfer_info() {
// }

// int test_print_system_get_tx_result_info() {
// }

int main() {
    test_parse_system_transfer_instruction();
    test_parse_system_get_tx_result_instruction();
    // test_print_system_transfer_info();
    // test_print_system_get_tx_result_info();

    printf("passed\n");
    return 0;
}
