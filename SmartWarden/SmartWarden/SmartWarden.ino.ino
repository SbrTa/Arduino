#include <UTFT.h>
#include <Wire.h>
#include <Keypad.h>
#include "PrintTimeDate.h"
#include "Ultrasonic.h"
#include "TemperatureHead.h"


extern uint8_t BigFont[];


// // Initialize Keypad ******************************************************************
const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9, 8, 7, 6}; //Rows 0 to 3
byte colPins[numCols] = {5, 4, 3, 2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
// // Initialize Keypad End ###################################################################

UTFT myGLCD(CTE32HR, 38, 39, 40, 41);

//Define pins ultrasonic(trig,echo)*********************************************
Ultrasonic ultrasonic(A0, A1);
int buzzer = 12;
int distance;
int safetyDistance;
//Ultrasonic sensor defination End ###########################################

// PIR Defination
int PIRoutput = 15;                // pin for the LED
int PIRinputPin = 14;               // choose the input pin (for PIR sensor)
int PIRstate = LOW;             // we start, assuming no motion detected
int PIRval = 0;                    // variable for reading the pin status
// PIR Defination End


// All Used Function ********************************************
void FirstPage();
void MainManuSecurity();
void HomePageBackground();
void HomePage();
void ShowTimeDate();
void SecurityModeI();
void SecurityModeII();
void Credit();
void BackButton(int, int);
void AlarmButton(int, int);
void ResetButton(int, int);
void Instruction();
void ClapLight();
void Calculator();
void TemperatureHumidity();
// All Used Function Ends ########################################
int AlarmStatus = 0;
char AlarmTime[100];

void setup()
{
  // Setup LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  // Setup LCD ENd

  // Setup Clock
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  memset(recv, 0, BUFF_MAX);
  //parse_cmd("T201914728022017", 16);
  //         TssmmhhWDDMMYYYY
  // Setup Clock End

  // Setup UltraSonic
  pinMode(buzzer, OUTPUT);
  // Setup UltraSonic End

  // Setup PIR
  pinMode(PIRoutput, OUTPUT);
  pinMode(PIRinputPin, INPUT);
  // Setup PIR End

}



void loop()
{
  myGLCD.clrScr();


  
  strcpy(AlarmTime, "Not Set");
  //Instruction();
  //Credit();
  //Calculator();
  //SecurityModeI();
  //SecurityModeII();
  //ShowTimeDate();
  //TemperatureHumidity();
  //delay(1000000);

  FirstPage();
  //delay(1000000);
  MainManuSecurity();
  HomePageBackground();

  int now = 1;
  int pointstart = 45;
  int pointy;
  int pointx = 20;

  while (1) {
    HomePage();
    digitalWrite(buzzer, LOW);
    digitalWrite(PIRoutput, LOW);
    PTD();
    myGLCD.print(HomeTime, 342, 280);
    if (strcmp(HHMM, AlarmTime) == 0) {
      strcpy(AlarmTime, "Not Set");
      digitalWrite(buzzer, HIGH);
      delay(5000);
      digitalWrite(buzzer, LOW);
    }
    myGLCD.setColor(255, 0, 0);
    pointy = pointstart + (now * 25);
    myGLCD.print(">>", pointx, pointy);

    char keypressed = myKeypad.getKey();
    if (keypressed == '2') {
      now--;
      if (now == 0) now = 7;

      myGLCD.setColor (0, 0, 0);
      myGLCD.fillRect(10, 10, 50, 310);
      myGLCD.setColor(255, 0, 0);
      pointy = pointstart + (now * 25);
      myGLCD.print(">>", pointx, pointy);
    }
    else if (keypressed == '8') {
      now++;
      if (now == 8) now = 1;

      myGLCD.setColor (0, 0, 0);
      myGLCD.fillRect(10, 10, 50, 310);
      myGLCD.setColor(255, 0, 0);
      pointy = pointstart + (now * 25);
      myGLCD.print(">>", pointx, pointy);
    }
    else if (keypressed == '*') {

      if (now == 1) {
        ShowTimeDate();
        myGLCD.clrScr(); HomePageBackground();
      }
      else if (now == 2) {
        TemperatureHumidity();
        myGLCD.clrScr(); HomePageBackground();
      }
      else if (now == 3) {
        Calculator();
        myGLCD.clrScr(); HomePageBackground();
      }
      else if (now == 4) {
        SecurityModeI();
        myGLCD.clrScr(); HomePageBackground();
      }
      else if (now == 5) {
        SecurityModeII();
        myGLCD.clrScr(); HomePageBackground();
      }
      else if (now == 6) {
        Instruction();
        myGLCD.clrScr(); HomePageBackground();
      }
      else if (now == 7) {
        Credit();
        myGLCD.clrScr(); HomePageBackground();
      }
    }

  }
}




// // First Page : Show Project Name ***************************
void FirstPage()
{
  myGLCD.setColor (255, 0, 0);
  for (int i = 15; i < 304; i += 5)
  {
    myGLCD.drawLine(1, i, (i * 1.6) - 10, 304);
  }
  myGLCD.setColor (255, 0, 0);
  for (int i = 304; i > 15; i -= 5)
  {
    myGLCD.drawLine(478, i, (i * 1.6) - 11, 15);
  }
  myGLCD.setColor (0, 255, 255);
  for (int i = 304; i > 15; i -= 5)
  {
    myGLCD.drawLine(1, i, 491 - (i * 1.6), 15);
  }
  myGLCD.setColor (0, 255, 255);
  for (int i = 15; i < 304; i += 5)
  {
    myGLCD.drawLine(478, i, 490 - (i * 1.6), 304);
  }
  myGLCD.setColor (255, 0, 0);
  myGLCD.print("SMART WARDEN", CENTER, 150);
  delay(2000);
}
// // First Look End ########################################


// // Main Manu Security ************************************
void MainManuSecurity()
{
  myGLCD.clrScr();
  int x, y, x2, y2;
  // Draw some random rounded rectangles
  for (int i = 0; i < 100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x = 2 + random(476);
    y = 16 + random(289);
    x2 = 2 + random(476);
    y2 = 16 + random(289);
    myGLCD.drawRoundRect(x, y, x2, y2);
  }
  // END

  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(90, 140, 380, 180);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print("Password: ", 95, 152);

  int pass = 1234;
  int n = 0;
  int in = 0;
  int xx = 250, yy = 152, xy = 15;

  while (1) {
    char keypressed = myKeypad.getKey();
    if (keypressed == '1') {
      n = (n * 10) + ('1' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '2') {
      n = (n * 10) + ('2' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '3') {
      n = (n * 10) + ('3' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '4') {
      n = (n * 10) + ('4' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '5') {
      n = (n * 10) + ('5' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '6') {
      n = (n * 10) + ('6' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '7') {
      n = (n * 10) + ('7' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '8') {
      n = (n * 10) + ('8' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '9') {
      n = (n * 10) + ('9' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    else if (keypressed == '0') {
      n = (n * 10) + ('0' - '0');
      myGLCD.print("*", xx, yy);
      xx += xy;
      in++;
    }
    if (in == 4) {
      if (n == pass) break;
      else {
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(90, 140, 380, 180);
        myGLCD.setColor(255, 0, 0);
        myGLCD.print("Password: ", 95, 152);
      }
      n = 0;
      in = 0;
      xx = 250;
      yy = 152;
    }
  }
}
// // Main Manu Security End ############################


// // Home Page BackGround ******************************
void HomePageBackground()
{
  myGLCD.clrScr();
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(10, 10, 470, 310);
}
// // Home Page BackGround End  #########################



// // Home Page *****************************************
void HomePage()
{

  myGLCD.setColor(255, 255, 255);
  myGLCD.print("PROJECT MENU", CENTER, 30);
  myGLCD.drawLine(145, 48, 335, 48);
  myGLCD.drawLine(145, 50, 335, 50);

  myGLCD.print("Clock", 55, 70);
  myGLCD.print("Temperature & Humidity", 55, 95);
  myGLCD.print("Calculator", 55, 120);
  myGLCD.print("Security Mode I", 55, 145);
  myGLCD.print("Security Mode II", 55, 170);
  myGLCD.print("Instruction", 55, 195);
  myGLCD.print("Credit", 55, 220);

}
// // Home Page End ####################################


// // Print Temperature & Humidity *********************
void TemperatureHumidity()
{
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillRect(10, 10, 470, 310);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(20, 45, 460, 290);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("TEMPERATURE & HUMIDITY", CENTER, 20);
  BackButton(360, 248);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Temperature:", 45, 100);
  myGLCD.print("Humidity : ", 80, 150);
  while (1) {
    char keypressed = myKeypad.getKey();
    if (keypressed == '#') {
      return;
    }
    Tempu();
    myGLCD.print(TTT, 250, 100);
    myGLCD.print("|", 320, 100);
    myGLCD.print(TTTT, 345, 100);
    myGLCD.print(HHH, 250, 150);
    delay(500);
  }
}
// // Print Temperature & Humidity End ##################



// Print Time & Date if pressed 1 ***********************
void ShowTimeDate()
{
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillRect(10, 10, 470, 310);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(20, 45, 460, 290);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("CLOCK", CENTER, 20);

  BackButton(360, 248);
  AlarmButton(35, 248);

  while (1) {
    char keypressed = myKeypad.getKey();
    if (keypressed == '#') {
      return;
    }

    // Set Alarm *************************
    else if (keypressed == '*') {
      myGLCD.setColor(255, 255, 0);
      myGLCD.fillRect(10, 10, 470, 310);
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(20, 45, 460, 290);
      myGLCD.setColor(255, 255, 255);
      myGLCD.print("ALARM", CENTER, 20);
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("[Plz use 24 hour format]", CENTER, 50);
      myGLCD.setColor(255, 255, 255);
      BackButton(345, 243);
      ResetButton(50, 243);
      myGLCD.print("Alarm Time:  ", 85, 110);
      myGLCD.print(AlarmTime, 275, 110);
      while (1) {
        keypressed = myKeypad.getKey();
        if (keypressed == '#') {
          myGLCD.setColor(255, 255, 0);
          myGLCD.fillRect(10, 10, 470, 310);
          myGLCD.setColor(0, 0, 0);
          myGLCD.fillRect(20, 45, 460, 290);
          myGLCD.setColor(255, 255, 255);
          myGLCD.print("CLOCK", CENTER, 20);
          BackButton(345, 243);
          AlarmButton(50, 243);
          break;
        }
        else if (keypressed == '*') {
          *AlarmTime = '\0';
          myGLCD.print("New Alarm Time:  ", 55, 140);
          int in = 0;
          int xx = 300, yy = 140, xy = 15;
          while (1) {
            keypressed = myKeypad.getKey();
            if (keypressed == '1') {
              strcat(AlarmTime, "1");
              myGLCD.print("1", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '2') {
              strcat(AlarmTime, "2");
              myGLCD.print("2", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '3') {
              strcat(AlarmTime, "3");
              myGLCD.print("3", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '4') {
              strcat(AlarmTime, "4");
              myGLCD.print("4", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '5') {
              strcat(AlarmTime, "5");
              myGLCD.print("5", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '6') {
              strcat(AlarmTime, "6");
              myGLCD.print("6", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '7') {
              strcat(AlarmTime, "7");
              myGLCD.print("7", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '8') {
              strcat(AlarmTime, "8");
              myGLCD.print("8", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '9') {
              strcat(AlarmTime, "9");
              myGLCD.print("9", xx, yy);
              xx += xy;
              in++;
            }
            else if (keypressed == '0') {
              strcat(AlarmTime, "0");
              myGLCD.print("0", xx, yy);
              xx += xy;
              in++;
            }
            if (in == 2) {
              strcat(AlarmTime, ":");
              myGLCD.print(":", xx, yy);
              xx += xy;
              in++;
            }
            if (in == 5) {
              delay(1500);
              AlarmStatus = 1;
              myGLCD.setColor(255, 255, 0);
              myGLCD.fillRect(10, 10, 470, 310);
              myGLCD.setColor(0, 0, 0);
              myGLCD.fillRect(20, 45, 460, 290);
              myGLCD.setColor(255, 255, 255);
              myGLCD.print("ALARM", CENTER, 20);
              myGLCD.setColor(255, 0, 0);
              myGLCD.print("[Plz use 24 hour format]", CENTER, 50);
              myGLCD.setColor(255, 255, 255);
              myGLCD.print("Alarm Time:  ", 85, 110);
              myGLCD.print(AlarmTime, 275, 110);
              BackButton(345, 243);
              ResetButton(50, 243);
              break;
            }

          }
          //myGLCD.print(AlarmTime,275,110);
        }
      }
    }
    // Set Alarm End ###################

    myGLCD.setColor(255, 255, 255);
    PTD();
    myGLCD.print("Time", CENTER, 90);
    myGLCD.drawLine(205, 108, 273, 108);
    myGLCD.drawLine(205, 110, 273, 110);
    //Serial.print("Time"); Serial.println(PrintTime);
    myGLCD.print(PrintTime, CENTER, 120);

    myGLCD.print("Date", CENTER, 160);
    myGLCD.drawLine(205, 178, 273, 178);
    myGLCD.drawLine(205, 180, 273, 180);
    myGLCD.print(PrintDate, CENTER, 190);

// // Day 
    

// // Day End

    if (strcmp(HHMM, AlarmTime) == 0) {
      strcpy(AlarmTime, "Not Set");
      digitalWrite(buzzer, HIGH);
      delay(5000);
      digitalWrite(buzzer, LOW);
    }
    //delay(1000);
  }
}
// //Print Time & Date End ##############################


// //Calculator ******************************************
void Calculator()
{
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillRect(10, 10, 470, 310);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(15, 40, 465, 305);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("CALCULATOR", 152, 17);
  BackButton(367, 267);


  myGLCD.setColor(166, 255, 166);
  myGLCD.fillRect(45, 75, 435, 130);
  myGLCD.fillRect(45, 155, 435, 240);

  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(50, 80, 430, 125);
  myGLCD.fillRect(50, 160, 430, 235);
  myGLCD.setColor(166, 255, 166);
  int a = 0, b = 0, c = 0, n = 0;
  int xx = 100, yy = 93, xy = 15;
  while (1) {
    char keypressed = myKeypad.getKey();
    if (keypressed == '#') {
      return;
    }
    else if (keypressed == '1') {
      n = (n * 10) + ('1' - '0');
      myGLCD.print("1", xx, yy);
      xx += xy;
    }
    else if (keypressed == '2') {
      n = (n * 10) + ('2' - '0');
      myGLCD.print("2", xx, yy);
      xx += xy;
    }
    else if (keypressed == '3') {
      n = (n * 10) + ('3' - '0');
      myGLCD.print("3", xx, yy);
      xx += xy;
    }
    else if (keypressed == '4') {
      n = (n * 10) + ('4' - '0');
      myGLCD.print("4", xx, yy);
      xx += xy;
    }
    else if (keypressed == '5') {
      n = (n * 10) + ('5' - '0');
      myGLCD.print("5", xx, yy);
      xx += xy;
    }
    else if (keypressed == '6') {
      n = (n * 10) + ('6' - '0');
      myGLCD.print("6", xx, yy);
      xx += xy;
    }
    else if (keypressed == '7') {
      n = (n * 10) + ('7' - '0');
      myGLCD.print("7", xx, yy);
      xx += xy;
    }
    else if (keypressed == '8') {
      n = (n * 10) + ('8' - '0');
      myGLCD.print("8", xx, yy);
      xx += xy;
    }
    else if (keypressed == '9') {
      n = (n * 10) + ('9' - '0');
      myGLCD.print("9", xx, yy);
      xx += xy;
    }
    else if (keypressed == '0') {
      n = (n * 10) + ('0' - '0');
      myGLCD.print("0", xx, yy);
      xx += xy;
    }
    else if (keypressed == 'A' || keypressed == 'B' ||
             keypressed == 'C' || keypressed == 'D') {
      xx += xy;
      a = n;
      n = 0;
      if (keypressed == 'A') {
        c = 1;
        myGLCD.print("+", xx, yy);
      }
      if (keypressed == 'B') {
        c = 2;
        myGLCD.print("-", xx, yy);
      }
      if (keypressed == 'C') {
        c = 3;
        myGLCD.print("*", xx, yy);
      }
      if (keypressed == 'D') {
        c = 4;
        myGLCD.print("/", xx, yy);
      }
      xx += (2 * xy);
    }
    else if (keypressed == '*') {
      char nt[100];
      char nn[100];
      *nt = '\0';
      *nn = '\0';
      sprintf(nn, "%d", a);
      strcat(nt, nn);

      myGLCD.print("", 70, 160);
      myGLCD.setColor(166, 255, 166);
      myGLCD.fillRect(45, 75, 435, 130);
      myGLCD.fillRect(45, 155, 435, 240);
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(50, 80, 430, 125);
      myGLCD.fillRect(50, 160, 430, 235);
      myGLCD.setColor(166, 255, 166);
      int nnn = 0;
      if (c == 1) {
        nnn = a + n;
        strcat(nt, " + ");
      }
      if (c == 2) {
        nnn = a - n;
        strcat(nt, " - ");
      }
      if (c == 3) {
        nnn = a * n;
        strcat(nt, " * ");
      }
      if (c == 4) {
        nnn = a / n;
        strcat(nt, " / ");
      }
      *nn = '\0';
      sprintf(nn, "%d", n);
      strcat(nt, nn);
      myGLCD.print(nt, 100, 175);

      *nn = '\0';
      sprintf(nn, "%d", nnn);
      myGLCD.print("=", 100, 200);
      myGLCD.print(nn, 130, 200);
      n = 0;
      a = 0;
      c = 0;
      xx = 100;
      yy = 93;
    }
  }
}
// //Calculator End ###########################################


// // Instruction *********************************************
void Instruction()
{
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillRect(10, 10, 470, 310);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(15, 40, 465, 300);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("INSTRUCTION", CENTER, 18);
  BackButton(375, 265);
  InstrumentButton(20, 265);

  while (1) {
    char keypressed = myKeypad.getKey();
    if (keypressed == '#') {
      return;
    }
    // // Instrument Page ****************************
    else if (keypressed == '*') {
      myGLCD.setColor(255, 255, 0);
      myGLCD.fillRect(5, 10, 475, 310);
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(10, 40, 470, 300);
      myGLCD.setColor(255, 255, 255);
      myGLCD.print("INSTRUMENT", CENTER, 18);
      BackButton(375, 265);

      myGLCD.print(">Arduino MEGA 2560", 20, 60);
      myGLCD.print(">3.2\" TFT Display: QD320", 20, 80);
      myGLCD.print(">RTC-DS3231", 20, 100);
      myGLCD.print(">Ultrasonic Ranging ", 20, 120);
      myGLCD.print(" Module: HC SR04", 20, 140);
      myGLCD.print(">PIR Motion Sensor: HC SR501", 20, 160);
      myGLCD.print(">Temperature Sensor: DHT11", 20, 180);
      myGLCD.print(">4*4 Membrane Keyboard", 20, 200);
      myGLCD.print(">Jumper Wire, Breadboard,", 20, 220);
      myGLCD.print(" LED, Buzzer", 20, 240);
      
      while (1) {
        char keypressed = myKeypad.getKey();
        if (keypressed == '#') {
          myGLCD.setColor(255, 255, 0);
          myGLCD.fillRect(10, 10, 470, 310);
          myGLCD.setColor(0, 0, 0);
          myGLCD.fillRect(15, 40, 465, 300);
          myGLCD.setColor(255, 255, 255);
          myGLCD.print("INSTRUCTION", CENTER, 18);
          BackButton(375, 265);
          InstrumentButton(20, 265);
          break;
        }
      }
    }
    // // Instrument Page ############################

    myGLCD.print(">Press 2 : Move Up", 45, 80);
    myGLCD.print(">Press 8 : Move Down", 45, 105);
    myGLCD.print(">Press 4 : Previous", 45, 130);
    myGLCD.print(">Press 6 : Next", 45, 155);
  }
}
// // Instruction End ############################################



// // Credit *****************************************************
void Credit()
{
  myGLCD.clrScr();

  myGLCD.setColor(0, 255, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(255, 128, 128);
  myGLCD.fillRect(3, 7, 477, 315);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(8, 33, 472, 307);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("CREDIT", 188 , 13);
  BackButton(380, 275);
  while (1) {
    char keypressed = myKeypad.getKey();
    if (keypressed == '#') {
      return;
    }
    myGLCD.setColor(255, 255, 255);
    myGLCD.print("Created By:", 13 , 45);
    myGLCD.print(">Fahad Ahmed (IT-13021)", 25, 70);
    myGLCD.print(">Subrata Roy (IT-13019)", 25, 90);
    myGLCD.print(">Autish Moulik (IT-13033)", 25, 110);
    myGLCD.print(">Sunjida Naznin (IT-13007)", 25, 130);
    myGLCD.print(">Md.Asraful Alam (IT-13001)", 25, 150);
    myGLCD.print(">Md.Shariful Islam(IT-13047)", 25, 170);

    myGLCD.print("Special Thanks to:", 13, 205);
    myGLCD.print("Ali Newaz Bahar", 55, 230);
    myGLCD.print("Assistant Professor", 55, 250);
    myGLCD.print("Dept of ICT, MBSTU", 55, 270);
  }
}

// // Credit End ############################################


// // PIR Security Mode *************************************
void SecurityModeII() {
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillRect(20, 20, 460, 300);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(35, 55, 445, 285);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("PIR MOTION SECURITY", 85, 30);
  BackButton(345, 243);
  myGLCD.print("Status : ", 100, 140);

  while (1) {
    delay(300);
    char keypressed = myKeypad.getKey();
    if (keypressed == '#') {
      return;
    }
    PIRval = digitalRead(PIRinputPin);  // read input value

    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(225, 130, 340, 180);

    if (PIRval == HIGH) {            // check if the input is HIGH
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Danger", 238, 140);
      digitalWrite(PIRoutput, HIGH);  // turn LED ON
      digitalWrite(buzzer, HIGH);

      if (PIRstate == LOW) {
        // we have just turned on
        Serial.println("Motion detected!");
        // We only want to print on the output change, not state
        PIRstate = HIGH;
      }
    }
    else {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Safe", 238, 140);
      digitalWrite(PIRoutput, LOW); // turn LED OFF
      digitalWrite(buzzer, LOW);
      if (PIRstate == HIGH) {
        // we have just turned of
        Serial.println("Motion ended!");
        // We only want to print on the output change, not state
        PIRstate = LOW;
      }
    }
  }
}
// // PIR Security Mode End #################################

// // Security Mode *****************************************
void SecurityModeI()
{
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 480, 320);
  myGLCD.setColor(255, 255, 0);
  myGLCD.fillRect(10, 15, 470, 300);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(20, 50, 460, 285);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("ULTRASONIC DISTANCE SECURITY", 17, 25);
  BackButton(360, 245);

  myGLCD.print("Object Distance : ", 50, 100);
  myGLCD.print("Status : ", 100, 160);
  while (1)
  {
    char keypressed = myKeypad.getKey();
    if (keypressed == '#') {
      return;
    }
    distance = ultrasonic.Ranging(CM);
    //Use 'CM' for centimeters or 'INC' for inches
    char dist[100], pdist[100];
    *pdist = '\0';
    if (distance < 10) {
      strcat(pdist, "00");
    }
    else if (distance < 100) {
      strcat(pdist, "0");
    }
    else if (distance > 999) distance = 999;
    sprintf(dist, "%d", distance);
    strcat(dist, " CM");
    strcat(pdist, dist);
    myGLCD.print(pdist, 340, 100);
    //every 1sec.
    delay(300);

    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(225, 150, 340, 180);
    safetyDistance = distance;
    if (safetyDistance <= 20) {
      digitalWrite(buzzer, HIGH);
      myGLCD.setColor(255, 0, 0);
      myGLCD.print("Danger", 238, 160);
    }
    else {
      digitalWrite(buzzer, LOW);

      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Safe", 238, 160);
    }
    myGLCD.setColor(255, 255, 255);
  }
}
// // Security Mode End ##########################


// Back Button **********************************
void BackButton(int x1, int y1)
{
  int x2 = x1 + 85;
  int y2 = y1 + 27;
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(x1, y1, x2, y2);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(x1 + 3, y1 + 3, x2 - 3, y2 - 3);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Back", x1 + 10, y1 + 5);
}
// Back Button End ###############################

// Alarm Button **********************************
void AlarmButton(int x1, int y1)
{
  int x2 = x1 + 100;
  int y2 = y1 + 27;
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(x1, y1, x2, y2);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(x1 + 3, y1 + 3, x2 - 3, y2 - 3);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Alarm", x1 + 10, y1 + 5);
}
// Alarm Button End ###############################

// Reset Button **********************************
void InstrumentButton(int x1, int y1)
{
  int x2 = x1 + 170;
  int y2 = y1 + 27;
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(x1, y1, x2, y2);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(x1 + 3, y1 + 3, x2 - 3, y2 - 3);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Instrument", x1 + 5, y1 + 5);
}
// Reset Button End ###############################


// Reset Button **********************************
void ResetButton(int x1, int y1)
{
  int x2 = x1 + 100;
  int y2 = y1 + 27;
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(x1, y1, x2, y2);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(x1 + 3, y1 + 3, x2 - 3, y2 - 3);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Reset", x1 + 10, y1 + 5);
}
// Reset Button End ###############################


