#pragma once

#include "sol/print_config.h"

int print_transaction(const PrintConfig* print_config,
                      InstructionInfo* const* infos,
                      size_t infos_length);

int print_transaction_nonce_processed(const PrintConfig* print_config,
                                      InstructionInfo* const* infos,
                                      size_t infos_length);
