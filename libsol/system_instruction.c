#include "common_byte_strings.h"
#include "instruction.h"
#include "sol/parser.h"
#include "sol/transaction_summary.h"
#include "system_instruction.h"
#include "util.h"
#include <string.h>

const Pubkey system_program_id = {{PROGRAM_ID_SYSTEM}};

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
            return print_system_transfer_info(&info->transfer, print_config);
        // case SystemAdvanceNonceAccount:
        //     return print_system_advance_nonce_account(&info->advance_nonce, print_config);
        // case SystemCreateAccount:
        //     return print_system_create_account_info(CREATE_ACCOUNT_TITLE,
        //                                             &info->create_account,
        //                                             print_config);
        // case SystemCreateAccountWithSeed:
        //     return print_system_create_account_with_seed_info(CREATE_ACCOUNT_TITLE,
        //                                                       &info->create_account_with_seed,
        //                                                       print_config);
        // case SystemInitializeNonceAccount:
        //     return print_system_initialize_nonce_info("Init nonce acct",
        //                                               &info->initialize_nonce,
        //                                               print_config);
        // case SystemWithdrawNonceAccount:
        //     return print_system_withdraw_nonce_info(&info->withdraw_nonce, print_config);
        // case SystemAuthorizeNonceAccount:
        //     return print_system_authorize_nonce_info(&info->authorize_nonce, print_config);
        // case SystemAssign:
        //     return print_system_assign_info(&info->assign, print_config);
        // case SystemAllocate:
        //     return print_system_allocate_info(&info->allocate, print_config);
        // case SystemAllocateWithSeed:
        //     return print_system_allocate_with_seed_info("Allocate acct",
        //                                                 &info->allocate_with_seed,
        //                                                 print_config);
        // case SystemAssignWithSeed:
        //     break;
    }

    return 1;
}

// Returns 0 and populates SystemTransferInfo if provided a MessageHeader
// and a transfer instruction, otherwise non-zero.
int parse_system_transfer_instruction(Parser* parser,
                                             const Instruction* instruction,
                                             const MessageHeader* header,
                                             SystemTransferInfo* info) {
    InstructionAccountsIterator it;
    instruction_accounts_iterator_init(&it, header, instruction);

    BAIL_IF(instruction_accounts_iterator_next(&it, &info->from));
    BAIL_IF(instruction_accounts_iterator_next(&it, &info->to));

    BAIL_IF(parse_u64(parser, &info->lamports));

    return 0;
}

int parse_system_instructions(const Instruction* instruction,
                              const MessageHeader* header,
                              SystemInfo* info) {
    Parser parser = {instruction->data, instruction->data_length};

    BAIL_IF(parse_system_instruction_kind(&parser, &info->kind));

    switch (info->kind) {
        case SystemTransfer:
            return parse_system_transfer_instruction(&parser, instruction, header, &info->transfer);
    }

    return 1;
}

int print_system_transfer_info(const SystemTransferInfo* info,
                               const PrintConfig* print_config) {
    SummaryItem* item;

    item = transaction_summary_primary_item();
    summary_item_set_amount(item, "Transfer", info->lamports);

    if (print_config_show_authority(print_config, info->from)) {
        item = transaction_summary_general_item();
        summary_item_set_pubkey(item, "Sender", info->from);
    }

    item = transaction_summary_general_item();
    summary_item_set_pubkey(item, "Recipient", info->to);

    return 0;
}

