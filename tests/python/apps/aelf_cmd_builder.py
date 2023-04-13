from typing import List
from enum import IntEnum
import base58
from nacl.signing import VerifyKey


PROGRAM_ID_SYSTEM = "11111111111111111111111111111111"

# Fake blockhash so this example doesn't need a network connection. It should be queried from the cluster in normal use.
FAKE_RECENT_BLOCKHASH = "11111111111111111111111111111111"


def verify_signature(from_public_key: bytes, message: bytes, signature: bytes):
    assert len(signature) == 64, "signature size incorrect"
    verify_key = VerifyKey(from_public_key)
    verify_key.verify(message, signature)


class SystemInstruction(IntEnum):
    CreateAccount           = 0x00
    Assign                  = 0x01
    Transfer                = 0x02


# class MessageHeader:
#     def __init__(self, num_required_signatures: int, num_readonly_signed_accounts: int, num_readonly_unsigned_accounts: int):
#         self.num_required_signatures = num_required_signatures
#         self.num_readonly_signed_accounts = num_readonly_signed_accounts
#         self.num_readonly_unsigned_accounts = num_readonly_unsigned_accounts

#     def serialize(self) -> bytes:
#         return self.num_required_signatures.to_bytes(1, byteorder='little') + \
#                self.num_readonly_signed_accounts.to_bytes(1, byteorder='little') + \
#                self.num_readonly_unsigned_accounts.to_bytes(1, byteorder='little')

# class AccountMeta:
#     pubkey: bytes
#     is_signer: bool
#     is_writable: bool
#     def __init__(self, pubkey: bytes, is_signer: bool, is_writable: bool):
#         self.pubkey = pubkey
#         self.is_signer = is_signer
#         self.is_writable = is_writable

# Only support Transfer instruction for now
# TODO add other instructions if the need arises
class Instruction:
    data: bytes
    to_pubkey: bytes
    ticker: int
    def serialize(self) -> bytes:
        serialized: bytes = len(self.data).to_bytes(1, byteorder='little')
        serialized += self.data
        serialized += self.to_pubkey
        serialized += self.ticker
        return serialized

class SystemInstructionTransfer(Instruction):
    def __init__(self, to_pubkey: bytes, ticker: int, amount: int):
        self.to_pubkey = to_pubkey
        self.ticker = ticker
        self.data = (SystemInstruction.Transfer).to_bytes(4, byteorder='little') + (amount).to_bytes(8, byteorder='little')

# Cheat as we only support 1 SystemInstructionTransfer currently
# TODO add support for multiple transfers and other instructions if the needs arises
# class CompiledInstruction:
#     program_id_index: int
#     accounts: List[int]
#     data: bytes

#     def __init__(self, program_id_index: int, accounts: List[int], data: bytes):
#         self.program_id_index = program_id_index
#         self.accounts = accounts
#         self.data = data

#     def serialize(self) -> bytes:
#         serialized: bytes = self.program_id_index.to_bytes(1, byteorder='little')
#         serialized += len(self.accounts).to_bytes(1, byteorder='little')
#         for account in self.accounts:
#             serialized += (account).to_bytes(1, byteorder='little')
#         serialized += len(self.data).to_bytes(1, byteorder='little')
#         serialized += self.data
#         return serialized

# Solana communication message, header + list of public keys used by the instructions + instructions
# with references to the keys array
class Message:
    recent_blockhash: bytes
    instruction: Instruction

    def __init__(self, instruction: Instruction):
        # Cheat as we only support 1 SystemInstructionTransfer currently
        # TODO add support for multiple transfers and other instructions if the needs arises
        self.recent_blockhash = base58.b58decode(FAKE_RECENT_BLOCKHASH)
        self.instruction = instruction

    def serialize(self) -> bytes:
        serialized: bytes = self.recent_blockhash
        serialized += self.instruction.serialize()
        return serialized
