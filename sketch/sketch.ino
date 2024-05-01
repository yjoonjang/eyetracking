#include <Arduino.h>
// 모터 제어를 위한 핀 정의
const int motorPin1 = 3; // PWM 핀을 사용하여 모터 속도 조절
const int motorPin2 = 4; // 방향 제어 핀

void setup() {
  // 핀 모드 설정
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  Serial.begin(9600); // 시리얼 통신 시작
  Serial.println("Arduino is ready and waiting for data...");
}

void loop() {
  while (Serial.available() == 0) {
    pinMode(13, OUTPUT);
  }
  if (Serial.available() > 0) {
    int targetHeight = Serial.parseInt();
    if (Serial.read() == '\n') {
      Serial.print("Received target height: ");
      Serial.println(targetHeight);
      
      // targetHeight가 양수이면 한 방향, 음수이면 반대 방향으로 회전
      if (targetHeight > 0) {
        digitalWrite(motorPin2, HIGH); // 한 방향으로 설정
      } else {
        digitalWrite(motorPin2, LOW);  // 반대 방향으로 설정
        targetHeight = -targetHeight;  // 속도 값은 양수가 되어야 함
      }

      analogWrite(motorPin1, map(targetHeight, 0, 100, 0, 255)); // 속도 조절
    }
  }
}
