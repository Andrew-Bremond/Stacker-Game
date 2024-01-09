#include "Timer.h"

enum STATE {INIT, l2, l2p, l2r, l1, l1p, l1r, mid, midp, midr, r1, r1p, r1r, r2, r2p, r2r} gState = INIT;
// l2 = leftmost, l1 = left, mid = middle, r1 = right, r2 = rightmost // p = pressed, r = released
enum otherSTATE {ones, tens} cState = ones;
int button = LOW;
int i = 0;
int tick_timer = 0;
int counter = 0;
char gSegPins[] = {2, 3, 4, 5, 6, 7, 8};


void displayNumTo7Seg(unsigned int targetNum, int digitPin) {
    unsigned char encodeInt[10] = {
        // 0     1     2     3     4     5     6     7     8     9
        0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67,
    };
    digitalWrite(digitPin, HIGH);
    for (int k = 0; k < 7; ++k) {
        digitalWrite(gSegPins[k], encodeInt[targetNum] & (1 << k));
    }
    digitalWrite(digitPin, LOW);
}


void display(void) {
  int one_digit = 0, ten_digit = 0;

  //transitions
  switch (cState) {
    case ones:
      cState = tens;
    break;
    case tens:
      cState = ones;
    break;
  }

  //actions
  switch (cState) {
    case ones:
      one_digit = counter % 10;
      ten_digit = counter / 10;
      digitalWrite(A1, HIGH);
      displayNumTo7Seg(one_digit, A0);
    break;
    case tens:
      one_digit = counter % 10;
      ten_digit = counter / 10;
      digitalWrite(A0, HIGH);
      displayNumTo7Seg(ten_digit, A1);
    break;
  }
}


void tick(void) {
  //transitions
  switch (gState) {
    case INIT:
      gState = l2;
    break;

    case l2:
      i = 0;
      if (button) {
        gState = l2p;
      }
      else if (tick_timer >= 80) {
        tick_timer = 0;
      }
      else if (i == 0 && tick_timer >= 10) {
        gState = l1;
      }
      else {
        tick_timer++;
        gState = l2;
      }
    break;
    case l2p:
      if (!button) {
        gState = l2r;
      }
      else {
        gState = l2p;
      }
    break;
    case l2r:
      if (!button) {
        counter = 0;
      }
      if (tick_timer >= 80) {
        tick_timer = 0;
      }
      else if (i == 0 && tick_timer >= 10) {
        gState = l1;
      }
      else {
        tick_timer++;
        gState = l2r;
      }
    break;


    case l1:
      if (button) {
        gState = l1p;
      }
      else if (i == 0 && tick_timer >= 20) {
        gState = mid;
      }
      else if (i == 1 && tick_timer >= 80) {
        gState = l2;
      }
      else {
        tick_timer++;
        gState = l1;
      }
    break;
    case l1p:
      if (!button) {
        gState = l1r;
      }
      else {
        gState = l1p;
      }
    break;
    case l1r:
      if (!button) {
        counter = 0;
      }
      if (i == 0 && tick_timer >= 20) {
        gState = mid;
      }
      else if (i == 1 && tick_timer >= 80) {
        gState = l2;
      }
      else {
        tick_timer++;
        gState = l1r;
      }
    break;


    case mid:
      if (button) {
        gState = midp;
      }
      else if (i == 0 && tick_timer >= 30) {
        gState = r1;
      }
      else if (i == 1 && tick_timer >= 70) {
        gState = l1;
      }
      else {
        tick_timer++;
        gState = mid;
      }
    break;
    case midp:
      if (!button) {
        counter++;
        gState = midr;
      }
      else {
        gState = midp;
      }
    break;
    case midr:
      if (i == 0 && tick_timer >= 30) {
        gState = r1;
      }
      else if (i == 1 && tick_timer >= 70) {
        gState = l1;
      }
      else {
        tick_timer++;
        gState = midr;
      }
    break;


    case r1:
      if (button) {
        gState = r1p;
      }
      else if (i == 0 && tick_timer >= 40) {
        gState = r2;
      }
      else if (i == 1 && tick_timer >= 60) {
        gState = mid;
      }
      else {
        tick_timer++;
        gState = r1;
      }
    break;
    case r1p:
      if (!button) {
        gState = r1r;
      }
      else {
        gState = r1p;
      }
    break;
    case r1r:
      if (!button) {
        counter = 0;
      }
      if (i == 0 && tick_timer >= 40) {
        gState = r2;
      }
      else if (i == 1 && tick_timer >= 60) {
        gState = mid;
      }
      else {
        tick_timer++;
        gState = r1r;
      }
    break;


    case r2:
      i = 1;
      if (button) {
        gState = r2p;
      }
      else if (i == 1 && tick_timer >= 50) {
        gState = r1;
      }
      else {
        tick_timer++;
        gState = r2;
      }
    break;
    case r2p:
      if (!button) {
        gState = r2r;
      }
      else {
        gState = r2p;
      }
    break;
    case r2r:
      if (!button) {
        counter = 0;
      }
      if (i == 1 && tick_timer >= 50) {
        gState = r1;
      }
      else {
        tick_timer++;
        gState = r2r;
      }
    break;
  }

  //actions
  switch (gState) {
    case l2:
    case l2p:
    case l2r:
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    break;
    case l1:
    case l1p:
    case l1r:
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
    break;
    case mid:
    case midp:
    case midr:
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
    break;
    case r1:
    case r1p:
    case r1r:
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
    break;
    case r2:
    case r2p:
    case r2r:
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
    break;
  }
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(19, INPUT);
  TimerSet(10);
  TimerOn();
}

void loop() {
  //button = digitalRead(19);
  //tick();
  display();
  while(!TimerFlag){}
  TimerFlag = 0;
}
