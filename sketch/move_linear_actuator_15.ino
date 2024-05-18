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

const int maxPosition = 1023;  // 최대 위치 (Potentiometer의 최대 값)
const int minPosition = 0;     // 최소 위치 (Potentiometer의 최소 값)
int currentPosition = 0;       // 현재 위치
int targetPosition = maxPosition;  // 목표 위치
int spd = 100;  // 기본 속도 설정

void setup() {
    // 초기 설정이 필요하면 여기에 추가
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        int newPosition = input.toInt(); // 문자열을 정수로 변환
        if (newPosition >= minPosition && newPosition <= maxPosition) {
            targetPosition = newPosition; // 유효한 범위 내의 경우 목표 위치로 설정
        }
    }

    if (myShield.button1Clicked()) {
        targetPosition = maxPosition;  // 버튼 클릭 시 목표 위치를 최대값으로 설정
    } else if (myShield.button2Clicked()) {
        targetPosition = minPosition;  // 버튼2 클릭 시 목표 위치를 최소값으로 설정
    }

    currentPosition = myShield.getPot();  // 현재 위치를 Potentiometer에서 읽기

    if (currentPosition < targetPosition) {
        spd = map(targetPosition - currentPosition, 0, maxPosition, 0, 255);
        myMotor.rotation(spd);
    } else if (currentPosition > targetPosition) {
        spd = map(currentPosition - targetPosition, 0, maxPosition, 0, -255);
        myMotor.rotation(spd);
    } else {
        myMotor.floating();  // 목표 위치에 도달하면 멈춤
    }

    // 상태 출력
    Serial.print("Current Position: ");
    Serial.print(currentPosition);
    Serial.print(" Target Position: ");
    Serial.println(targetPosition);
    delay(100);  // 필요에 따라 딜레이 시간을 조절
}