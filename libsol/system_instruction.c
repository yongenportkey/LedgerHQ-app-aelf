#include "instruction.h"
#include "sol/parser.h"
#include "sol/transaction_summary.h"
#include "system_instruction.h"
#include "util.h"
#include <string.h>

// const Pubkey system_program_id = {{PROGRAM_ID_SYSTEM}};

static int parse_system_instruction_kind(Parser* parser, enum SystemInstructionKind* kind) {
    uint32_t maybe_kind;
    BAIL_IF(parse_u32(parser, &maybe_kind));
    switch (maybe_kind) {
        case SystemTransfer:
            *kind = (enum SystemInstructionKind) maybe_kind;
            return 0;
    }
    return 1;
}

int print_system_info(const SystemInfo* info, const PrintConfig* print_config) {
    switch (info->kind) {
        case SystemTransfer:
            return print_system_transfer_info(&info->transfer);
    }

    return 1;
}

// Returns 0 and populates SystemTransferInfo if provided a MessageHeader
// and a transfer instruction, otherwise non-zero.
int parse_system_transfer_instruction(Parser* parser,
                                      Instruction* instruction,
                                      SystemTransferInfo* info) {
    BAIL_IF(parse_pubkey(parser, &info->to));
    BAIL_IF(parse_data(parser, &instruction->ticker, &instruction->ticker_length));
    BAIL_IF(parse_u64(parser, &info->lamports));
    return 0;
}

int parse_system_get_tx_result_instruction(Parser* parser,
                                           Instruction* instruction,
                                           SystemGetTxResultInfo* info) {

    BAIL_IF(parse_pubkey(parser, &info->from));
    BAIL_IF(parse_pubkey(parser, &info->chain));
    BAIL_IF(parse_u64(parser, &info->ref_block_number));
    BAIL_IF(parse_sized_string(parser, &info->method_name));
    BAIL_IF(parse_pubkey(parser, &info->to));
    BAIL_IF(parse_data(parser, &instruction->ticker, &instruction->ticker_length));
    BAIL_IF(parse_u64(parser, &info->lamports));
    return 0;
}

// int parse_system_instructions(const Instruction* instruction, SystemInfo* info) {
//     Parser parser = {instruction->data, instruction->data_length};

//     BAIL_IF(parse_system_instruction_kind(&parser, &info->kind));

//     switch (info->kind) {
//         case SystemTransfer:
//             return parse_system_transfer_instruction(&parser, instruction, &info->transfer);
//     }

//     return 1;
// }

int print_system_transfer_info(const SystemTransferInfo* info) {
    SummaryItem* item;

    item = transaction_summary_primary_item();
    summary_item_set_amount(item, "Transfer", info->lamports);

    item = transaction_summary_general_item();
    summary_item_set_pubkey(item, "Recipient", info->to);

    return 0;
}

int print_system_get_tx_result_info(const SystemGetTxResultInfo* info) {
    SummaryItem* item;

    item = transaction_summary_primary_item();
    summary_item_set_string(item, "Type", "Get Transaction result");

    item = transaction_summary_general_item();
    summary_item_set_pubkey(item, "From", info->from);

    item = transaction_summary_general_item();
    summary_item_set_pubkey(item, "Contract", info->chain);
    
    item = transaction_summary_general_item();
    summary_item_set_i64(item, "Ref block number", info->ref_block_number);

    item = transaction_summary_general_item();
    summary_item_set_sized_string(item, "Method name", &info->method_name);

    item = transaction_summary_general_item();
    summary_item_set_pubkey(item, "Recipient", info->to);

    item = transaction_summary_general_item();
    summary_item_set_amount(item, "Amount", info->lamports);

    return 0;
}

