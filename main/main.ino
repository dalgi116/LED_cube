const int latchPin = 2;
const int clockPin = 3;
const int dataPin = 4;
const int touchSensorPin = 5;

byte dataArray1[20];
byte dataArray2[20];
int mode = 0;
bool touchDetected;
bool readyToChange = true;
long unsigned int startCountingTime;
int difference;
int j = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  updateRegister(0xFF, 0xFF);
}

void loop() {
  touchDetected = digitalRead(touchSensorPin);

  if (touchDetected) {
    if (readyToChange) {
      startCountingTime = millis();
      readyToChange = false;
      if (mode == 5) {
        mode = 0;
      } else {
        mode++;
      }
    }
  }
  difference = millis() - startCountingTime;
  if (difference > 500) {
    readyToChange = true;
  }
  
  switch (mode) {
    case 0:
      mode0();
      break;
    case 1:
      mode1();
      break;
    case 2:
      mode2();
      break;
    case 3:
      mode3();
      break;
    case 4:
      mode4();
      break;
    case 5:
      mode5();
      break;
  }
}


void mode0() {
  dataArray1[0] = 0xFF;
  dataArray2[0] = 0xFF;
  updateRegister(dataArray1[0], dataArray2[0]);
}

void mode1() {
  dataArray1[0] = 0x00;
  dataArray2[0] = 0x00;
  updateRegister(dataArray1[0], dataArray2[0]);
}

void mode2() {
  dataArray1[0] = 0x06;
  dataArray2[0] = 0x60;
  updateRegister(dataArray1[0], dataArray2[0]);
}

void mode3() {
  dataArray1[0] = 0xF9;
  dataArray1[1] = 0x06;
  
  dataArray2[0] = 0x9F;
  dataArray2[1] = 0x60;

  playSequence(1, 3000);
}

void mode4() {
  dataArray1[0] = 0xFF;
  dataArray1[1] = 0xF9;
  dataArray1[2] = 0x96;
  dataArray1[3] = 0x6F;
  dataArray1[4] = 0x96;
  dataArray1[5] = 0xF9;
  
  dataArray2[0] = 0xFF;
  dataArray2[1] = 0x9F;
  dataArray2[2] = 0x69;
  dataArray2[3] = 0xF6;
  dataArray2[4] = 0x69;
  dataArray2[5] = 0x9F;

  playSequence(5, 300);
}

void mode5() {
  dataArray1[0] = 0x7F;
  dataArray1[1] = 0xBF;
  dataArray1[2] = 0xDF;
  dataArray1[3] = 0xEF;
  dataArray1[4] = 0xFE;
  dataArray1[5] = 0xF7;
  dataArray1[6] = 0x7F;
  dataArray1[7] = 0xBF;
  dataArray1[8] = 0xDF;
  dataArray1[9] = 0xEF;
  dataArray1[10] = 0xFE;
  dataArray1[11] = 0xF7;
  
  dataArray2[0] = 0xFE;
  dataArray2[1] = 0xFD;
  dataArray2[2] = 0xFB;
  dataArray2[3] = 0xF7;
  dataArray2[4] = 0x7F;
  dataArray2[5] = 0xEF;
  dataArray2[6] = 0xFE;
  dataArray2[7] = 0xFD;
  dataArray2[8] = 0xFB;
  dataArray2[9] = 0xF7;
  dataArray2[10] = 0x7F;
  dataArray2[11] = 0xEF;

  playSequence(5, 200);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    } else {
      pinState= 0;
    }
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

void updateRegister(byte data1,byte data2) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, data1);
  shiftOut(dataPin, clockPin, data2);
  digitalWrite(latchPin, HIGH);
}

void playSequence(int indexes, int delayTime) {
  if (millis() % delayTime == 0) {
    delay(10);
    updateRegister(dataArray1[j], dataArray2[j]);
    if (j == indexes) {
      j = 0;
    } else {
      j++;
    }
  }
}
