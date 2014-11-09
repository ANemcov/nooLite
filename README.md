nooLite
=======

Conlose utility for nooLite PC118 (using libusb)
How to INSTALL:

1. Download noolite.c

2. Install libusb
`apt-get install libusb-1.0-0 libusb-1.0-0-dev`

3. Compile
`gcc nooLite.c -o noolite -lusb-1.0`

4. Copy to /usr/local/bin
`cp noolite /usr/local/bin/noolite`

5. Set owner and access rights
`chown root /usr/local/bin/noolite && chmod a+s /usr/local/bin/noolite`

6. PROFIT

use `./noolite -help`
