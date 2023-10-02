#include <Encoder.h>
#include <EasyTransfer.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ClickButton.h>

Encoder myEnc(5, 6);

EasyTransfer ETin, ETout;
struct SEND_DATA_STRUCTURE {
  unsigned long ms = 0;
};
struct RECEIVE_DATA_STRUCTURE {
  unsigned long ms = 0;
  int team1 = 0;
  int team2 = 0;
  int goal = 20;
};
RECEIVE_DATA_STRUCTURE RX;
SEND_DATA_STRUCTURE TX;

byte char0[8] = {
  0b00000,
  0b11011,
  0b01110,
  0b00100,
  0b01110,
  0b11011,
  0b00000,
  0b00000
};
byte char1[8] = {
  0b00000,
  0b00011,
  0b10110,
  0b11100,
  0b01000,
  0b00000,
  0b00000,
  0b00000
};

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned long ms = -9999;
bool conn;
char buf[16];

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  ETout.begin(details(TX), &Serial1);
  ETin.begin(details(RX), &Serial1);

  lcd.begin(20, 4);
  lcd.createChar(0, char0);
  lcd.createChar(1, char1);
}

void loop() {
  if (ETin.receiveData()) {
    ms = millis();
    TX.ms = ms;
    ETout.sendData();
  }
  conn = (millis() - ms < 500);

  if (conn) {
    lcd.setCursor(19, 0);
    lcd.write(byte(1));
  } else {
    lcd.setCursor(19, 0);
    lcd.write(byte(0));
  }

  lcd.setCursor(0, 0);
  lcd.print("GOAL:");
  lcd.print(RX.goal);

  lcd.setCursor(0, 1);
  lcd.print("TEAM 1:");
  lcd.print(RX.team1);
  lcd.setCursor(0, 2);
  lcd.print("TEAM 2:");
  lcd.print(RX.team2);

  sprintf(buf, "%d:%d:%d:%d",conn, RX.goal, RX.team1, RX.team2);
  Serial.println(buf);

  lcd.setCursor(0, 3);
  lcd.print("OUTPUT:");
  lcd.print(buf);

  lcd.setCursor(16, 1);
  lcd.print(myEnc.read());
}
