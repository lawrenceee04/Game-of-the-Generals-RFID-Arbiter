#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9
#define SS_PIN          10
#define NO_OF_PIECES    21
#define RED_RGB_LED     6
#define GREEN_RGB_LED   5
#define BLUE_RGB_LED    3
#define RESET_PIECES    2

MFRC522 mfrc522(SS_PIN, RST_PIN);

void ledWinnerIndicator(int whoWon = 0) {
  switch (whoWon)
  {
  case 1:
    analogWrite(GREEN_RGB_LED, 255);
    delay(1500);
    analogWrite(GREEN_RGB_LED, 0);
    break;
  case 2:
    analogWrite(RED_RGB_LED, 255);
    delay(1500);
    analogWrite(RED_RGB_LED, 0);
    break;
  default:
    analogWrite(BLUE_RGB_LED, 255);
    delay(1500);
    analogWrite(BLUE_RGB_LED, 0);
    break;
  }
}

enum PieceRank {
  FLAG,
  SPY,
  PRIVATE,
  SERGEANT,
  LIEUTENANT_2ND,
  LIEUTENANT_1ST,
  CAPTAIN,
  MAJOR,
  LT_COLONEL,
  COLONEL,
  GENERAL_ONE,
  GENERAL_TWO,
  GENERAL_THREE,
  GENERAL_FOUR,
  GENERAL_FIVE,
};

// Define the UIDs and their ranks
byte whitePiece[21][4] = {
  {0x65, 0x3A, 0xFC, 0xA9},
	{0x1B, 0x15, 0xFD, 0xA9},
	{0x34, 0x56, 0x78, 0x9A},
	{0x12, 0x24, 0x56, 0x78},
	{0xAB, 0xCD, 0xEF, 0x01},
	{0x98, 0x76, 0x54, 0x32},
	{0xFE, 0xDC, 0xBA, 0x09},
	{0x11, 0x22, 0x33, 0x44},
	{0x55, 0x66, 0x77, 0x88},
	{0x99, 0xAA, 0xBB, 0xCC},
	{0xDD, 0xEE, 0xFF, 0x00},
	{0x12, 0x35, 0x56, 0x78},
	{0x4B, 0xCD, 0xEF, 0x01},
	{0x18, 0x76, 0x54, 0x32},
	{0xF3, 0xDC, 0xBA, 0x09},
	{0x19, 0x22, 0x33, 0x44},
	{0x55, 0x66, 0x79, 0x88},
	{0x99, 0xAA, 0xBB, 0xC1},
	{0xD3, 0xEE, 0xFF, 0x00},
	{0x12, 0x32, 0x56, 0x78},
	{0xAB, 0xC4, 0xEF, 0x01}
	// Add other UIDs here...
};

byte blackPiece[21][4] = {
  {0x99, 0x4C, 0xFC, 0xA9},
	{0x86, 0x18, 0xFC, 0xA9},
	{0x34, 0x56, 0x78, 0x9A},
	{0x12, 0x24, 0x56, 0x78},
	{0xAB, 0xCD, 0xEF, 0x01},
	{0x98, 0x76, 0x54, 0x32},
	{0xFE, 0xDC, 0xBA, 0x09},
	{0x11, 0x22, 0x33, 0x44},
	{0x55, 0x66, 0x77, 0x88},
	{0x99, 0xAA, 0xBB, 0xCC},
	{0xDD, 0xEE, 0xFF, 0x00},
	{0x12, 0x35, 0x56, 0x78},
	{0x4B, 0xCD, 0xEF, 0x01},
	{0x18, 0x76, 0x54, 0x32},
	{0xF3, 0xDC, 0xBA, 0x09},
	{0x19, 0x22, 0x33, 0x44},
	{0x55, 0x66, 0x79, 0x88},
	{0x99, 0xAA, 0xBB, 0xC1},
	{0xD3, 0xEE, 0xFF, 0x00},
	{0x12, 0x32, 0x56, 0x78},
	{0xAB, 0xC4, 0xEF, 0x01}
	// Add other UIDs here...
};


PieceRank pieceRanks[21] = {
  FLAG,
  SPY,
  SPY,
  PRIVATE,
  PRIVATE,
  PRIVATE,
  PRIVATE,
  PRIVATE,
  PRIVATE,
  SERGEANT,
  LIEUTENANT_2ND,
  LIEUTENANT_1ST,
  CAPTAIN,
  MAJOR,
  LT_COLONEL,
  COLONEL,
  GENERAL_ONE,
  GENERAL_TWO,
  GENERAL_THREE,
  GENERAL_FOUR,
  GENERAL_FIVE
};

// void ISR_Button();

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  // pinMode(RESET_PIECES, INPUT);
  // attachInterrupt(digitalPinToInterrupt(RESET_PIECES), ISR_Button, RISING); // attach interrupt
}

volatile bool resetFlag = false; // Add this line at the top of your file

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
  PieceRank whitePieceRank = FLAG;
  PieceRank blackPieceRank = FLAG;
  int firstScannedPiece = 0; // 1 for white, 2 for black
  for (int i = 0; i < NO_OF_PIECES; i++) {
    if (memcmp(mfrc522.uid.uidByte, whitePiece[i], 4) == 0) {
      whitePieceRank = pieceRanks[i];
	  firstScannedPiece = 1;
      break;
    } else if (memcmp(mfrc522.uid.uidByte, blackPiece[i], 4) == 0) {
	  blackPieceRank = pieceRanks[i];
	  firstScannedPiece = 2;
	  break;
	  }
  }

  // Wait for a second card
  while ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(1000);
  }

  // Select the second card
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the second card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Get the rank of the second piece
  for (int i = 0; i < NO_OF_PIECES; i++) {
    if (memcmp(mfrc522.uid.uidByte, whitePiece[i], 4) == 0) {
      whitePieceRank = pieceRanks[i];
	  firstScannedPiece = 1;
      break;
    } else if (memcmp(mfrc522.uid.uidByte, blackPiece[i], 4) == 0) {
	  blackPieceRank = pieceRanks[i];
	  firstScannedPiece = 2;
	  break;
	  }
  }

  // Compare the ranks of the pieces
  if (whitePieceRank == SPY && blackPieceRank == PRIVATE) {
    Serial.println("BLACK piece wins.");
    ledWinnerIndicator(2);
  } else if (blackPieceRank == SPY && whitePieceRank == PRIVATE) {
    Serial.println("WHITE piece wins.");
    ledWinnerIndicator(1);
// Flag is captured
  } else if (firstScannedPiece == 2 && whitePieceRank == FLAG && blackPieceRank == FLAG) {
    Serial.println("WHITE piece wins, black's flag is captured.");
    ledWinnerIndicator(2);
  } else if (firstScannedPiece == 1 && blackPieceRank == FLAG && whitePieceRank == FLAG) {
    Serial.println("BLACK piece wins, white's flag is captured.");
    ledWinnerIndicator(1);
  } else if (whitePieceRank > blackPieceRank) {
    Serial.println("WHITE piece wins.");
    ledWinnerIndicator(1);
  } else if (blackPieceRank > whitePieceRank) {
    Serial.println("BLACK piece wins.");
    ledWinnerIndicator(2);
  } else {
    Serial.println("BOTH piece lose.");
    ledWinnerIndicator();
  }
}