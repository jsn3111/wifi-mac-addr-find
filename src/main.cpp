#include <Arduino.h>
#include "WiFi.h"

/* 와이파이 관련 변수들 정의*/
//board macAddress to send data (내가 board A이면 보드 B가 dest가 되고, 또 그 반대)
byte boardA[] = {0xE8,0x68,0xE7,0x0B,0xBA,0xD0};
byte boardB[] = {0x24,0x6F,0x28,0xB3,0xF5,0x60};

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  Serial.begin(115200);
  Serial.print(" macAddress of board B : ");
  Serial.println(WiFi.macAddress());

  return x + y;
}