#include "instruction.h"
#include "sol/parser.h"
#include "sol/transaction_summary.h"
#include "util.h"
#include <string.h>

int parse_system_transfer_instruction(Parser* parser,
                                      Instruction* instruction,
                                      SystemTransferInfo* info) {
    BAIL_IF(parse_pubkey(parser, &info->to));
    BAIL_IF(parse_data(parser, &instruction->ticker, &instruction->ticker_length));
    BAIL_IF(parse_u64(parser, &info->amount));
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
    BAIL_IF(parse_u64(parser, &info->amount));
    return 0;
}

int print_system_transfer_info(const SystemTransferInfo* info) {
    SummaryItem* item;

    item = transaction_summary_primary_item();
    summary_item_set_amount(item, "Transfer", info->amount);

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
    summary_item_set_amount(item, "Amount", info->amount);

    return 0;
}