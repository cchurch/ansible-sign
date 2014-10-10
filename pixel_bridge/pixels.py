#!/usr/bin/env python

import requests

def set_pixel(p='-', r=-1, g=-1, b=-1):
    url = 'http://ansiblesign.local/arduino/pixel/%s/' % p
    if b >= 0 and g < 0:
        g = 0
    if g >= 0 and r < 0:
        r = 0
    if r >= 0:
        url = '%s/%d/' % (url, r)
    if g >= 0:
        url = '%s/%d/' % (url, g)
    if b >= 0:
        url = '%s/%d/' % (url, b)
    resp = requests.get(url, auth=('root', 'arduino'))
    return resp.text

def main():
    print set_pixel('-', 0x00, 0x00, 0x00)

if __name__ == '__main__':
    main()
