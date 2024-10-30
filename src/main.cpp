#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// Typical pin layout used:
// ----------------------------------------------------
//             MFRC522      NodeMCU      NodeMCU
//             Reader/PCD   GPIO         Name
// ----------------------------------------------------
// RST/Reset    RST          D3           RX
// SPI SS       SDA(SS)      D8           D8
// SPI MOSI     MOSI         D7           TX
// SPI MISO     MISO         D6           RX
// SPI SCK      SCK          D5           SCK
#define RST_PIN D3 // Configurable, see typical pin layout above
#define SS_PIN D8  // Configurable, see typical pin layout above
#define NO_OF_PIECES 21

// Due to limited GPIO pins in ESP8266, it cannot accommodate for all the three digital pins required.
#define WHITE_PIECE_WINS_LED D1
// #define WHITE_BLUE_LED  D0
#define BLACK_PIECE_WINS_LED D2
// #define BLACK_BLUE_LED  D4

MFRC522 mfrc522(SS_PIN, RST_PIN);

enum PieceRank
{
  FLAG,
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
  SPY
};

// Use dumpinfo to get the UIDs of the pieces and arrange them according to the pieceRanks array
// For example the index 0 of the whitePiece array should be the UID of the FLAG piece
byte whitePiece[21][7] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

byte blackPiece[21][7] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

PieceRank pieceRanks[21] = {
    FLAG,
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
    GENERAL_FIVE,
    SPY,
    SPY};

void blackPieceWins()
{
  digitalWrite(BLACK_PIECE_WINS_LED, HIGH);
  digitalWrite(WHITE_PIECE_WINS_LED, LOW);
  delay(2000);
};

void whitePieceWins()
{
  digitalWrite(WHITE_PIECE_WINS_LED, HIGH);
  digitalWrite(BLACK_PIECE_WINS_LED, LOW);
  delay(2000);
};

void bothPieceLose()
{
  digitalWrite(BLACK_PIECE_WINS_LED, HIGH);
  digitalWrite(WHITE_PIECE_WINS_LED, HIGH);
  delay(2000);
};

void setup()
{
  Serial.begin(115200); // Initialize serial communication at 9600 baud
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card
  pinMode(WHITE_PIECE_WINS_LED, OUTPUT);
  pinMode(BLACK_PIECE_WINS_LED, OUTPUT);
}

void loop()
{
  digitalWrite(WHITE_PIECE_WINS_LED, LOW);
  digitalWrite(BLACK_PIECE_WINS_LED, LOW);
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // mfrc522.PICC_HaltA();
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Get the rank of the current piece
  PieceRank whitePieceRank = FLAG;
  PieceRank blackPieceRank = FLAG;
  int firstScannedPiece = 0; // 1 for white, 2 for black

  for (int i = 0; i < NO_OF_PIECES; i++)
  {
    if (memcmp(mfrc522.uid.uidByte, whitePiece[i], 7) == 0)
    {
      whitePieceRank = pieceRanks[i];
      Serial.println(whitePieceRank);
      firstScannedPiece = 1;
      // digitalWrite(WHITE_BLUE_LED, LOW);
      break;
    }
    else if (memcmp(mfrc522.uid.uidByte, blackPiece[i], 7) == 0)
    {
      blackPieceRank = pieceRanks[i];
      Serial.println(blackPieceRank);
      firstScannedPiece = 2;
      // digitalWrite(BLACK_BLUE_LED, LOW);
      break;
    }
  }

  while (!mfrc522.PICC_IsNewCardPresent())
  {
    delay(20);
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // mfrc522.PICC_HaltA();
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Get the rank of the second piece
  for (int i = 0; i < NO_OF_PIECES; i++)
  {
    if (memcmp(mfrc522.uid.uidByte, whitePiece[i], 7) == 0)
    {
      whitePieceRank = pieceRanks[i];
      Serial.println(whitePieceRank);
      firstScannedPiece = 1;
      // digitalWrite(WHITE_BLUE_LED, LOW);
      break;
    }
    else if (memcmp(mfrc522.uid.uidByte, blackPiece[i], 7) == 0)
    {
      blackPieceRank = pieceRanks[i];
      Serial.println(blackPieceRank);
      firstScannedPiece = 2;
      // digitalWrite(BLACK_BLUE_LED, LOW);
      break;
    }
  }

  // Decide the winner
  if (whitePieceRank == SPY && blackPieceRank == PRIVATE)
  {
    Serial.println("BLACK piece wins.");
    blackPieceWins();
  }
  else if (blackPieceRank == SPY && whitePieceRank == PRIVATE)
  {
    Serial.println("WHITE piece wins.");
    whitePieceWins();
    // If a flag is captured
  }
  else if (firstScannedPiece == 2 && whitePieceRank == FLAG && blackPieceRank == FLAG)
  {
    Serial.println("WHITE piece wins, black's flag is captured.");
    whitePieceWins();
  }
  else if (firstScannedPiece == 1 && whitePieceRank == FLAG && blackPieceRank == FLAG)
  {
    Serial.println("BLACK piece wins, white's flag is captured.");
    blackPieceWins();
  }
  else if (whitePieceRank > blackPieceRank)
  {
    Serial.println("WHITE piece wins.");
    whitePieceWins();
  }
  else if (blackPieceRank > whitePieceRank)
  {
    Serial.println("BLACK piece wins.");
    blackPieceWins();
  }
  else
  {
    Serial.println("BOTH piece lose.");
    bothPieceLose();
  }
}