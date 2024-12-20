# RFID Arbiter for Game of the Generals

An arbiter with the use of RFID stickers placed on each piece, along with an RFID-RC522 module and ESP8266 to read the RFID stickers.

# Components
- NodeMcu ESP8266/ESP32/Arduino Uno/Arduino Mega
- RFID-RC522
- [RFID Stickers](https://shopee.ph/10pcs-set-NFC-Chip-Sticker-NFC-Tag-RFID-Label-Sticker-Ntag213-14443A-13.56MHZ-i.753262751.19386104069) for every piece.


# Prerequirements
- Git for Windows
- PlatformIO Extension for VSCode

Open PlatformIO Extension on VSCode, then create new project. Type a name, then you can pick any board that also uses Arduino Framework. However, not guaranteed to work on all arduino boards. Click finish.

After that, make sure you are in the root directory. Delete the src folder and the platformio.ini, as it will be overwritten later in a command prompt in VSCode by clicking ``` Ctrl + ` ``` or opening the terminal in VScode.

Then type 

``` git clone https://github.com/lawrenceee04/Game-of-the-Generals-RFID-Arbiter.git ```

Now you can upload the program into your NodeMCU ESP8266.

# License
Everything in this repository is under a GPL-3.0 license, see [here](https://github.com/lawrenceee04/Game-of-the-Generals-RFID-Arbiter/blob/main/LICENSE) for more details.
