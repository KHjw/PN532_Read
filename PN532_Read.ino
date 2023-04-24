#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SCK 18
#define PN532_MISO 19
#define PN532_MOSI 23
#define PN532_SS 5

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  Serial.println("Found chip PN5");           // Got ok data, print it out!
  nfc.SAMConfig();                            // configure board to read RFID tags
  Serial.println("Waiting for an ISO14443A Card ...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    if (uidLength == 7)
    {
      uint8_t data[32];

      Serial.print("NTAG2xx tag Detected : ");     

      success = nfc.ntag2xx_ReadPage(7, data); 

      if (success) 
      {
        nfc.PrintHexChar(data, 4);
      }
    }
    else
    {
      Serial.println("This doesn't seem to be an NTAG203 tag");
    }
    Serial.flush();    
  }
  delay (50);
}
