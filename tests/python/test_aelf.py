from ragger.backend import RaisePolicy
from ragger.navigator import NavInsID
from ragger.utils import RAPDU

from .apps.aelf import AelfClient, ErrorType
from .apps.aelf_cmd_builder import SystemInstructionTransfer, SystemInstructionGetTxResult, MessageTransfer, MessageTxResult, verify_signature
from .apps.aelf_utils import FOREIGN_PUBLIC_KEY, FOREIGN_PUBLIC_KEY_2, CHAIN_PUBLIC_KEY, AMOUNT, AMOUNT_2, TICKER, REF_BLOCK_NUMBER, METHOD_NAME, ELF_PACKED_DERIVATION_PATH, ELF_PACKED_DERIVATION_PATH_2

from .utils import ROOT_SCREENSHOT_PATH

def test_aelf_simple_transfer_ok_1(backend, navigator, test_name):
    aelf = AelfClient(backend)
    from_public_key = aelf.get_public_key(ELF_PACKED_DERIVATION_PATH)
    # Create instruction
    instruction: SystemInstructionTransfer = SystemInstructionTransfer(FOREIGN_PUBLIC_KEY, TICKER, AMOUNT)
    message: bytes = MessageTransfer(instruction).serialize()

    with aelf.send_async_sign_transfer(ELF_PACKED_DERIVATION_PATH, message):
        navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
                                                  [NavInsID.BOTH_CLICK],
                                                  "Approve",
                                                  ROOT_SCREENSHOT_PATH,
                                                  test_name)
    signature: bytes = aelf.get_async_response().data

    verify_signature(from_public_key, message, signature)


def test_aelf_get_tx_result_ok(backend, navigator, test_name):
    aelf = AelfClient(backend)
    from_public_key = aelf.get_public_key(ELF_PACKED_DERIVATION_PATH_2)

    # Create instruction
    instruction: SystemInstructionGetTxResult = SystemInstructionGetTxResult(from_public_key, CHAIN_PUBLIC_KEY, REF_BLOCK_NUMBER, METHOD_NAME, FOREIGN_PUBLIC_KEY_2, TICKER, AMOUNT_2)
    message: bytes = MessageTxResult(instruction).serialize()

    with aelf.send_async_get_tx_result(ELF_PACKED_DERIVATION_PATH_2, message):
        navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
                                                  [NavInsID.BOTH_CLICK],
                                                  "Approve",
                                                  ROOT_SCREENSHOT_PATH,
                                                  test_name)

    signature: bytes = aelf.get_async_response().data

    verify_signature(from_public_key, message, signature)