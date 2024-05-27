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
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        int newPosition = input.toInt(); // 문자열을 정수로 변환
        if (newPosition >= minPosition) {
            targetPosition = newPosition;
            moveToTarget = true; // 목표 위치로 이동하도록 설정
            Serial.print("Received new target position: ");
            Serial.println(targetPosition);
        }
    }

    if (moveToTarget) {
        currentPosition = myShield.getPot();
        Serial.print("Current Position: ");
        Serial.print(currentPosition);
        Serial.print(" Target Position: ");
        Serial.println(targetPosition);

        if (currentPosition < targetPosition) {
            spd = map(targetPosition - currentPosition, 0, maxPosition, 0, 255);
            myMotor.rotation(spd);
        } else if (currentPosition > targetPosition) {
            spd = map(currentPosition - targetPosition, 0, maxPosition, 0, -255);
            myMotor.rotation(spd);
        } else {
            myMotor.floating();  // 목표 위치에 도달하면 멈춤
            moveToTarget = false; // 목표 위치로 이동 완료
            Serial.println("Reached target position");
        }

        delay(100);  // 필요에 따라 딜레이 시간을 조절
    }
}