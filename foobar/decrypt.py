import base64
import itertools

msg = b'F04QEAgXCh8aREVRVEgLGwYEH1NDTE4ACgcYCg0OFgBMVFVMTgYWHxEKAQwHQkdUSAkPBQoZABxL\nSVlFTB0BDxsGAQIWAwlOT0VMFQwEAAYTDhkKAh1ERVFUSBkHDwoIHwoITk9FTAYODgsKERhTT1ZJ\nRBYKEgpLRUNCDRsAS0lZRUwDBgJIRBg='
key = str.encode('licekto')

decoded = base64.b64decode(msg)
decrypted = ''

for i in range(0, len(decoded)):
    decrypted += chr((key[i % len(key)] ^ decoded[i]))

print(decrypted)
