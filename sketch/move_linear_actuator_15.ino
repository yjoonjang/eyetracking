// #include "mas001.h"
// #include "dmd150.h"

// MAS001 myShield;
// DMD150 myMotor;

// void setup() {
//     // 초기 설정이 필요하면 여기에 추가
// }

// uint8_t upspd;
// int spd;
// void loop() {
//     if (myShield.button1Clicked()) {
//         myMotor.floating();
//         spd = 0;
//     } else {
//         if (upspd) {
//             spd += 1;  // 증가 속도를 5로 변경
//             if (spd >= 255) upspd = 0;  
//         } else {
//             spd -= 1;  // 감소 속도를 5로 변경
//             if (spd <= -255) upspd = 1;  
//         }
//         myMotor.rotation(spd);    
//     }
//     delay(5);  // 딜레이 시간을 5ms로 변경
// }

#include "mas001.h"
#include "dmd150.h"

MAS001 myShield;
DMD150 myMotor;

const int maxPosition = 10;  // 최대 위치 (Potentiometer의 최대 값)
const int minPosition = 0;     // 최소 위치 (Potentiometer의 최소 값)
int currentPosition = 0;       // 현재 위치
int targetPosition = maxPosition;  // 목표 위치
int spd = 100;  // 기본 속도 설정
bool moveToTarget = false;  // 목표 위치로 이동 플래그
const int threshold = 1;  // 목표 위치 근처에서 멈추기 위한 오차 범위

void setup() {
    Serial.begin(9600);
    Serial.println("Setup completed");
}

void loop() {
  // spd=255;
  // myMotor.rotation(spd);
  // delay(500);
  // myMotor.brake();
  //  if (Serial.available() > 0) {
  //       String input = Serial.readStringUntil('\n');
  //       int newPosition = input.toInt(); // 문자열을 정수로 변환
  //       if (newPosition >= minPosition) {
  //           targetPosition = newPosition;
  //           moveToTarget = true; // 목표 위치로 이동하도록 설정
  //           Serial.print("Received new target position: ");
  //           Serial.println(targetPosition);
  //         } 
  //     }

   if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        // Serial.print(input);
        // input.trim(); // 양 끝 공백 제거

        if (input.startsWith("init")) {
            Serial.println("Received initialization command");
            input.remove(0, 4); // "init"을 제거하고 나머지 값만 남김
            int initPosition = input.toInt();
            initializeSystem(initPosition);
        } else {
            int newPosition = input.toInt(); // 문자열을 정수로 변환
            if (newPosition >= minPosition) {
                targetPosition = newPosition;
                moveToTarget = true; // 목표 위치로 이동하도록 설정
                Serial.print("Received new target position: ");
                Serial.println(targetPosition);
            }
        }
    }

    if (moveToTarget) {
        Serial.print(" Target Position: ");
        Serial.println(targetPosition);

        spd = -255;
        Serial.println(targetPosition);
        myMotor.rotation(spd);
        int rotate_time = targetPosition * 1000 / 0.5;
        delay(rotate_time);
        myMotor.brake();
        moveToTarget=false;
    }
}

void initializeSystem(int targetPosition) {
    spd = 255;
    myMotor.rotation(spd);
    int rotate_time = (targetPosition-4) * 1000 / 0.6;
    delay(rotate_time);
    myMotor.brake();
    Serial.println("Initialization completed");
}