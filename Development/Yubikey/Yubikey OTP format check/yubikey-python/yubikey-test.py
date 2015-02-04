from decrypt import decrypt

"""
Raw token:
B0 D4 A2 D6 9B C4 40 00 6A 14 00 04 14 0A CD F5
(counter = 0x0040; timestamp_h = 0x00; timestamp_l = 0x146A; session = 0x04; random = 0x0A14; crc = 0xF5CD)
Complete OTP:
00 00 00 00 00 01 6A F9 6E 4E DF 67 4B 81 53 BB D8 2E EC 16 4E 3C
Output OTP:
cccccccccccbhlvkhufutvhifnjbgenntjduurbhfuer
"""

otp='cccccccccccbhlvkhufutvhifnjbgenntjduurbhfuer'
key='7a1858592fcb76bd5eb2685421aed45e'

yubikey = decrypt.YubikeyToken(otp, key)

print('secret_id: ' + str(yubikey.secret_id))
print('counter:   ' + str(hex(yubikey.counter)))
print('timestamp: ' + str(hex(yubikey.timestamp)))
print('session:   ' + str(hex(yubikey.counter_session)))
print('random:    ' + str(hex(yubikey.random_number)))

print('crc:       ' + str(hex(yubikey.crc)))
print('crc ok:    ' + str(yubikey.crc_ok))
