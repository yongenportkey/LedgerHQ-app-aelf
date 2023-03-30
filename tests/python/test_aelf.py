from ragger.backend import RaisePolicy
from ragger.navigator import NavInsID
from ragger.utils import RAPDU

from .apps.aelf import AelfClient, ErrorType
from .apps.aelf_cmd_builder import SystemInstructionTransfer, Message, verify_signature
from .apps.aelf_utils import FOREIGN_PUBLIC_KEY, FOREIGN_PUBLIC_KEY_2, AMOUNT, AMOUNT_2, ELF_PACKED_DERIVATION_PATH, ELF_PACKED_DERIVATION_PATH_2

from .utils import ROOT_SCREENSHOT_PATH

def test_solana_simple_transfer_ok_1(backend, navigator, test_name):
    aelf = AelfClient(backend)
    from_public_key = aelf.get_public_key(ELF_PACKED_DERIVATION_PATH)

    # Create instruction
    instruction: SystemInstructionTransfer = SystemInstructionTransfer(from_public_key, FOREIGN_PUBLIC_KEY, AMOUNT)
    message: bytes = Message([instruction]).serialize()
    print("message is =======\n",message)
    print("pubkey is =======\n",FOREIGN_PUBLIC_KEY)

    with aelf.send_async_sign_message(ELF_PACKED_DERIVATION_PATH, message):
        navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
                                                  [NavInsID.BOTH_CLICK],
                                                  "Approve",
                                                  ROOT_SCREENSHOT_PATH,
                                                  test_name)

    signature: bytes = aelf.get_async_response().data

    verify_signature(from_public_key, message, signature)
    assert False


# def test_solana_simple_transfer_ok_2(backend, navigator, test_name):
#     aelf = AelfClient(backend)
#     from_public_key = aelf.get_public_key(ELF_PACKED_DERIVATION_PATH_2)

#     # Create instruction
#     instruction: SystemInstructionTransfer = SystemInstructionTransfer(from_public_key, FOREIGN_PUBLIC_KEY_2, AMOUNT_2)
#     message: bytes = Message([instruction]).serialize()

#     with aelf.send_async_sign_message(ELF_PACKED_DERIVATION_PATH_2, message):
#         navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
#                                                   [NavInsID.BOTH_CLICK],
#                                                   "Approve",
#                                                   ROOT_SCREENSHOT_PATH,
#                                                   test_name)

#     signature: bytes = aelf.get_async_response().data

#     verify_signature(from_public_key, message, signature)


# def test_solana_simple_transfer_refused(backend, navigator, test_name):
#     aelf = AelfClient(backend)
#     from_public_key = aelf.get_public_key(ELF_PACKED_DERIVATION_PATH)

#     instruction: SystemInstructionTransfer = SystemInstructionTransfer(from_public_key, FOREIGN_PUBLIC_KEY, AMOUNT)
#     message: bytes = Message([instruction]).serialize()

#     backend.raise_policy = RaisePolicy.RAISE_NOTHING
#     with aelf.send_async_sign_message(ELF_PACKED_DERIVATION_PATH, message):
#         navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
#                                                   [NavInsID.BOTH_CLICK],
#                                                   "Reject",
#                                                   ROOT_SCREENSHOT_PATH,
#                                                   test_name)

#     rapdu: RAPDU = aelf.get_async_response()
#     assert rapdu.status == ErrorType.USER_CANCEL


# def test_solana_blind_sign_refused(backend):
#     aelf = AelfClient(backend)
#     from_public_key = aelf.get_public_key(ELF_PACKED_DERIVATION_PATH)

#     instruction: SystemInstructionTransfer = SystemInstructionTransfer(from_public_key, FOREIGN_PUBLIC_KEY, AMOUNT)
#     message: bytes = Message([instruction]).serialize()

#     backend.raise_policy = RaisePolicy.RAISE_NOTHING
#     rapdu: RAPDU = aelf.send_blind_sign_message(ELF_PACKED_DERIVATION_PATH, message)
#     assert rapdu.status == ErrorType.SDK_NOT_SUPPORTED

