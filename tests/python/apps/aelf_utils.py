import base58

from ragger.bip import pack_derivation_path
from ragger.utils import create_currency_config

### Some utilities functions for amounts conversions ###

def elf_decimal_amount(elf_amount: int) -> int:
    return round(elf_amount * 10**8)


def decimal_to_bytes(decimal: int) -> str:
    hex:str = '{:x}'.format(decimal)
    if (len(hex) % 2 != 0):
        hex = "0" + hex
    return bytes.fromhex(hex)


### Proposed values for fees and amounts ###

AMOUNT          = elf_decimal_amount(2.078)
AMOUNT_BYTES    = decimal_to_bytes(AMOUNT)

AMOUNT_2        = elf_decimal_amount(101.00001234)
AMOUNT_2_BYTES  = decimal_to_bytes(AMOUNT_2)

FEES            = elf_decimal_amount(0.00000564)
FEES_BYTES      = decimal_to_bytes(FEES)


### Proposed foreign and owned addresses ###

FOREIGN_ADDRESS     = b"AxmUF3qkdz1zs151Q5WttVMkFpFGQPwghZs4d1mwY55d"
FOREIGN_PUBLIC_KEY  = base58.b58decode(FOREIGN_ADDRESS)

FOREIGN_ADDRESS_2       = b"8bjDMujLMttbmkTtoFgfw2sPYchSzzcTCEPGYDaNs3nj"
FOREIGN_PUBLIC_KEY_2    = base58.b58decode(FOREIGN_ADDRESS_2)

OWNED_ADDRESS       = b"3GJzvStsiYZonWE7WTsmt1BpWXkfcgWMGinaDwNs9HBc"
OWNED_PUBLIC_KEY    = base58.b58decode(OWNED_ADDRESS)


### Proposed Solana derivation paths for tests ###

ELF_PACKED_DERIVATION_PATH      = pack_derivation_path("m/44'/1616'/12345'")
ELF_PACKED_DERIVATION_PATH_2    = pack_derivation_path("m/44'/1616'/0'/0'")


### Package this currency configuration in exchange format ###

ELF_CONF = create_currency_config("ELF", "AELF")
