#!/usr/bin/env python3
import serial
import string
import time
if __name__ == '__main__':
        #abertura da porta serial
        ser = serial.Serial('/dev/ttyS0', 9600, timeout=0)
        ser.reset_input_buffer()
        while True:
                serialdata=ser.read()
                serialdata= int.from_bytes(serialdata,"little",signed=True)
                print(serialdata)
        #lendo dados da porta serial e printando na tela