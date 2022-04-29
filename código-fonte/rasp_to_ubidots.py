#!/usr/bin/env python3
import serial
import string
import time
import requests
import math
import sys
from sys import argv

TOKEN = "BBFF-L48Awxo7p33L5GSmURXCcaqt3Cl5Bl"  # Put your TOKEN here
DEVICE_LABEL = "ECG_signal_monitoring_raspberry"  # Put your device label here
VARIABLE_LABEL_1 = "ecg_sensor_data"  # Put your first variable label here
serialdata=[]
def post_request(payload):
    # Creates the headers for the HTTP requests
    url = "http://industrial.api.ubidots.com"
    url = "{}/api/v1.6/devices/{}".format(url, DEVICE_LABEL)
    headers = {"X-Auth-Token": TOKEN, "Content-Type": "application/json"}

    # Makes the HTTP requests
    status = 400
    attempts = 0
    while status >= 400 and attempts <= 5:
        req = requests.post(url=url, headers=headers, json=payload)
        status = req.status_code
        attempts += 1
        #time.sleep(0.1)

    # Processes results
    print(req.status_code, req.json())
    if status >= 400:
        print("[ERROR] Could not send data after 5 attempts, please check \
            your token credentials and internet connection")
        return False

    print("[INFO] request made properly, your device is updated")
    return True

if __name__ == '__main__':
    serial_data= sys.argv[1:]
    payload = {VARIABLE_LABEL_1:serial_data}
    post_request(payload)

