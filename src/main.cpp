#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D3
#define SS_PIN          D8

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define the hierarchy of the pieces
enum PieceRank {
  PRIVATE,
  SPY,
  GENERAL_FIVE,
  // Add other ranks here...
};

// Define the UIDs and their ranks
byte pieceUids[][4] = {
  {0x65, 0x3A, 0xFC, 0xA9},
  {0x1B, 0x15, 0xFD, 0xA9},
  {0x99, 0x4C, 0xFC, 0xA9},
  // Add other UIDs here...
};

PieceRank pieceRanks[] = {
  PRIVATE,
  SPY,
  GENERAL_FIVE,
  // Add other ranks here...
};

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Get the rank of the current piece
  PieceRank currentRank = PRIVATE; // Default rank
  for (int i = 0; i < 3; i++) {
    if (memcmp(mfrc522.uid.uidByte, pieceUids[i], 4) == 0) {
      currentRank = pieceRanks[i];
      break;
    }
  }

  // Wait for a second card
  while ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(10);
  }

  // Select the second card
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the second card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Get the rank of the second piece
  PieceRank secondRank = PRIVATE; // Default rank
  for (int i = 0; i < 3; i++) {
    if (memcmp(mfrc522.uid.uidByte, pieceUids[i], 4) == 0) {
      secondRank = pieceRanks[i];
      break;
    }
  };

  // Compare the ranks of the pieces
  if (currentRank == SPY && secondRank != PRIVATE) {
    Serial.println(F("First piece (Spy) kills second piece."));
  } else if (secondRank == SPY && currentRank != PRIVATE) {
    Serial.println(F("Second piece (Spy) kills first piece."));
  } else if (currentRank > secondRank) {
    Serial.println(F("First piece kills second piece."));
  } else if (secondRank > currentRank) {
    Serial.println(F("Second piece kills first piece."));
  } else {
    Serial.println(F("Both pieces are of the same rank. No piece is killed."));
  }
}