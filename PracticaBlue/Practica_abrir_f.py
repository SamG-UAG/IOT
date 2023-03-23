import os



os.system("sudo hciconfig hci0 up")

os.system("sudo hciconfig hci0 leadv 3")

os.system("sudo hcitool -i hci0 cmd 0x08 0x0008 1E 07 09 50 41 55 53 41 4D")
os.system("sudo hcitool -i hci0 cmd 0x08 0x0008 17 02 01 06 03 03 aa fe 0f 16 aa fe 10 00 01 66 61 63 65 62 6f 6f 6b 00 00 00 00 00 00 00 00 00")