#include "instruction.h"
#include "serum_assert_owner_instruction.h"
#include "sol/parser.h"
#include "sol/message.h"
#include "sol/print_config.h"
#include "spl_associated_token_account_instruction.h"
#include "spl_token_instruction.h"
#include "system_instruction.h"
#include "stake_instruction.h"
#include "vote_instruction.h"
#include "transaction_printers.h"
#include "util.h"
#include <string.h>

// change this if you want to be able to add succesive tx
#define MAX_INSTRUCTIONS 1

int process_message_body(const uint8_t* message_body,
                         int message_body_length,
                         const PrintConfig* print_config) {
    const MessageHeader* header = &print_config->header;

    size_t instruction_count = 0;
    InstructionInfo instruction_info[MAX_INSTRUCTIONS];
    explicit_bzero(instruction_info, sizeof(InstructionInfo) * MAX_INSTRUCTIONS);
    size_t display_instruction_count = 0;
    InstructionInfo* display_instruction_info[MAX_INSTRUCTIONS];

    // init parser body
    Parser parser = {message_body, message_body_length};
    Instruction instruction;
    InstructionInfo* info = &instruction_info[instruction_count];

    parse_system_transfer_instruction(&parser, &instruction, &info->system.transfer);
    switch (info->kind) {
        case ProgramIdSystem:
            display_instruction_info[display_instruction_count++] = info;
            break;
    }

    // Ensure we've consumed the entire message body
    BAIL_IF(!parser_is_empty(&parser));

    // If we don't know about all of the instructions, bail
    for (size_t i = 0; i < instruction_count; i++) {
        BAIL_IF(instruction_info[i].kind == ProgramIdUnknown);
    }

    return print_system_transfer_info(&display_instruction_info[0]->system.transfer);
}
