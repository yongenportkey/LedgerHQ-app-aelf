#include "os.h"
#include "cx.h"
#include "globals.h"

#ifndef _SIGN_MESSAGE_H_
#define _SIGN_MESSAGE_H_

void handle_get_tx_result_parse(volatile unsigned int *tx);

void handle_get_tx_result_ui(volatile unsigned int *flags);

#endif
