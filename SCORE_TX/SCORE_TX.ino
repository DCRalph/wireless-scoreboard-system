#include <EasyTransfer.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

EasyTransfer ETin, ETout;

struct SEND_DATA_STRUCTURE {
  unsigned long ms = 0;
  int team1 = 0;
  int team2 = 0;
  int goal = 20;
};
struct RECEIVE_DATA_STRUCTURE {
  unsigned long ms = 0;
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
byte arrow_left[8] = {
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b01000,
  0b00100,
  0b00010,
  0b00000
};
byte arrow_right[8] = {
  0b01000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b00000
};
byte arrow_up[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00000,
  0b00000
};
byte arrow_down[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

unsigned long ms = -9999;
unsigned long ms2 = -9999;
bool conn = false;
char buf[16];

int mode = 0;
int mode_1 = 1;

int settings_1 = 1;
int settings = 1;
int brightness = 10;

int game = 0;
int game_1 = 10;

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, arrow_left);
  lcd.createChar(1, arrow_right);
  lcd.createChar(2, char0);
  lcd.createChar(3, char1);
  lcd.createChar(4, arrow_up);
  lcd.createChar(5, arrow_down);

  pinMode(A0, INPUT);
  pinMode(10, OUTPUT);
  analogWrite(10, map(brightness, 0, 10, 0, 255));

  Serial1.begin(9600);
  ETout.begin(details(TX), &Serial1);
  ETin.begin(details(RX), &Serial1);

}

void loop() {

  mode_1 = constrain(mode_1, 1, 2);
  settings_1 = constrain(settings_1, 1, 4);
  brightness = constrain(brightness, 0, 10);
  analogWrite(10, map(brightness, 0, 10, 0, 255));
  TX.goal = constrain(TX.goal, 1, 50);
  game_1 = constrain(game_1, 7, 10);

  TX.team1 = constrain(TX.team1, 0, TX.goal);
  TX.team2 = constrain(TX.team2, 0, TX.goal);

  sprintf(buf, "1:%d:%d:%d", TX.goal, TX.team1, TX.team2);
  //Serial.println(buf);

  if (millis() - ms > 200) {
    ms = millis();
    TX.ms = ms;
    ETout.sendData();
  } else if (ETin.receiveData()) {
    ms2 = millis();
    ms = 0;
  }
  conn = (millis() - ms2 < 500);

  if (conn) {
    lcd.setCursor(15, 0);
    lcd.write(byte(3));
  } else {
    lcd.setCursor(15, 0);
    lcd.write(byte(2));
  }

  switch (mode) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("GAME ");
      lcd.setCursor(0, 1);
      lcd.print("SETTINGS ");

      if (mode_1 == 1) {
        lcd.setCursor(5, 0);
        lcd.write(byte(0));
        lcd.setCursor(9, 1);
        lcd.print("         ");
      }
      if (mode_1 == 2) {
        lcd.setCursor(9, 1);
        lcd.write(byte(0));
        lcd.setCursor(5, 0);
        lcd.print("      ");
      }

      if (button() == "U") {
        wait();
        mode_1 --;
      }
      if (button() == "D") {
        wait();
        mode_1 ++;
      }

//      if (mode_1 == 3) {
//        mode = 1;
//      }

      if (button() == "S" && mode_1 == 1) {
        wait();
        mode = 10;
      }
      if (button() == "S" && mode_1 == 2) {
        wait();
        mode = 100;
      }
      break;
    //game============================
    case 10:
      switch (game) {
        case 0:
          lcd.setCursor(0, 0);
          lcd.print("SET GOAL");
          lcd.setCursor(0, 1);
          lcd.print("GOAL:");
          lcd.print(TX.goal);

          if (button() == "U") {
            wait();
            TX.goal ++;
          }
          if (button() == "D") {
            wait();
            TX.goal --;
          }
          if (button() == "S") {
            wait();
            game = 1;
          }

          break;
        case 1:

          lcd.setCursor(0, 0);
          lcd.print("RED");
          lcd.setCursor(10, 0);
          lcd.print("BLUE");

          if (game_1 == 9) {
            lcd.setCursor(6, 0);
            lcd.write(byte(5));
          }
          if (game_1 == 10) {
            lcd.setCursor(6, 0);
            lcd.write(byte(4));
          }

          lcd.setCursor(0, 1);
          lcd.print("P:");
          lcd.print(TX.team1);
          lcd.setCursor(10, 1);
          lcd.print("P:");
          lcd.print(TX.team2);


          if (button() == "U") {
            wait();
            game_1++;
          }
          if (button() == "D") {
            wait();
            game_1--;
          }

          if (button() == "L" && game_1 == 10) {
            wait();
            TX.team1 ++;
          }
          if (button() == "R" && game_1 == 10) {
            wait();
            TX.team2 ++;
          }
          if (button() == "L" && game_1 == 9) {
            wait();
            TX.team1 --;
          }
          if (button() == "R" && game_1 == 9) {
            wait();
            TX.team2 --;
          }
          if (game_1 == 8) game = 2;

          break;
        case 2:

          lcd.setCursor(0, 0);
          lcd.print("RESET");
          lcd.setCursor(0, 1);
          lcd.print("HOME ");

          if (game_1 == 8) {
            lcd.setCursor(6, 0);
            lcd.write(byte(0));
            lcd.setCursor(5, 1);
            lcd.print(" ");
          }
          if (game_1 == 7) {
            lcd.setCursor(5, 1);
            lcd.write(byte(0));
            lcd.setCursor(6, 0);
            lcd.print(" ");
          }


          if (button() == "U") {
            wait();
            game_1++;
          }
          if (button() == "D") {
            wait();
            game_1--;
          }

          if (button() == "S" && game_1 == 8) {
            wait();
            TX.team1 = 0;
            TX.team2 = 0;
            game = 1;
            game_1 = 10;
          }
          if (button() == "S" && game_1 == 7) {
            wait();
            mode = 0;
            game = 0;
            game_1 = 10;
          }

          if (game_1 == 9) game = 1;
          break;
      }
      break;
    //game============================
    //settings========================
    case 100:
      switch (settings) {
        case 1:
          lcd.setCursor(0, 0);
          lcd.print("BACK");
          lcd.setCursor(0, 1);
          lcd.print("BRIGHTNESS");

          if (settings_1 == 1) {
            lcd.setCursor(5, 0);
            lcd.write(byte(0));
            lcd.setCursor(11, 1);
            lcd.print(" ");
          }
          if (settings_1 == 2) {
            lcd.setCursor(11, 1);
            lcd.write(byte(0));
            lcd.setCursor(5, 0);
            lcd.print(" ");
          }
          if (button() == "U") {
            wait();
            settings_1 --;
          }
          if (button() == "D") {
            wait();
            settings_1 ++;
          }

          if (button() == "S" && settings_1 == 1) {
            wait();
            mode = 0;
          }
          if (button() == "S" && settings_1 == 2) {
            wait();
            settings = 10;
          }

          if (settings_1 == 3) {
            settings = 2;
          }
          break;
        case 2:
          lcd.setCursor(0, 0);
          lcd.print("BUTTONS");
          lcd.setCursor(0, 1);
          lcd.print("TIME ON");

          if (settings_1 == 3) {
            lcd.setCursor(8, 0);
            lcd.write(byte(0));
            lcd.setCursor(8, 1);
            lcd.print("      ");
          }
          if (settings_1 == 4) {
            lcd.setCursor(8, 1);
            lcd.write(byte(0));
            lcd.setCursor(8, 0);
            lcd.print("      ");
          }
          if (button() == "U") {
            wait();
            settings_1 --;
          }
          if (button() == "D") {
            wait();
            settings_1 ++;
          }

          if (button() == "S" && settings_1 == 3) {
            wait();
            settings = 11;
          }
          if (button() == "S" && settings_1 == 4) {
            wait();
            settings = 12;
          }

          if (settings_1 == 2) {
            settings = 1;
          }
          break;
        case 10:
          lcd.setCursor(0, 0);
          lcd.print("BRIGHTNESS:");
          lcd.setCursor(11, 0);
          lcd.print(brightness);
          lcd.setCursor(0, 1);
          lcd.print("LB:MIN    RB:MAX");

          if (button() == "U") {
            wait();
            brightness += 1;
          }
          if (button() == "D") {
            wait();
            brightness -= 1;
          }
          if (button() == "L") {
            wait();
            brightness = 0;
          }
          if (button() == "R") {
            wait();
            brightness = 10;
          }
          if (button() == "S") {
            wait();
            settings = 1;
          }
          break;
        case 11:
          if (button() != "N") {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(button());
            lcd.setCursor(0, 1);
            lcd.print(analogRead(A0));
            while (button() != "N");
            lcd.clear();
          } else {
            lcd.setCursor(0, 0);
            lcd.print("PUSH BUTTON");
          }
          break;
        case 12:
          lcd.setCursor(0, 0);
          lcd.print("TIME ON");
          lcd.setCursor(0, 1);
          lcd.print(millis() / 1000);
          lcd.print(":S");

          if (button() == "D") {
            wait();
            settings = 2;
          }
          break;
      }
      break;
      //settings========================
  }
}

void wait() {
  while (button() != "N")
    lcd.clear();
    delay(50);
}
String button() {
  int x = analogRead(A0);
  if (x < 50)  return "R";
  else if (x < 150)  return "U";
  else if (x < 400)  return "D";
  else if (x < 500)  return "L";
  else if (x < 800)  return "S";
  else   return "N";
}
