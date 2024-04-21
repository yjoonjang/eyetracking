import cv2
import dlib
import imutils
from imutils import face_utils


def get_eye_level(frame, detector, predictor):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    rects = detector(gray, 0)

    for rect in rects:
        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        # 눈의 랜드마크 인덱스: 왼쪽 눈 (36 ~ 41), 오른쪽 눈 (42 ~ 47)
        leftEye = shape[36:42]
        rightEye = shape[42:48]

        # 눈의 중심 높이를 계산합니다.
        leftEyeHeight = (leftEye[1][1] + leftEye[5][1]) / 2
        rightEyeHeight = (rightEye[1][1] + rightEye[5][1]) / 2

        # 두 눈의 평균 높이를 반환합니다.
        return (leftEyeHeight + rightEyeHeight) / 2


# 얼굴 감지기 및 랜드마크 예측기 초기화
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("lib/shape_predictor_68_face_landmarks.dat")

# 웹캠 초기화
cap = cv2.VideoCapture(1)

initial_eye_level = None
final_eye_level = None
count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame = imutils.resize(frame, width=450)
    eye_level = get_eye_level(frame, detector, predictor)

    if eye_level:
        cv2.putText(frame, f"Eye Level: {eye_level:.2f}", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

        # 초기 눈 높이 측정
        key = cv2.waitKey(1) & 0xFF
        if (initial_eye_level is None) and (key == ord("s")):
            initial_eye_level = eye_level
            print("Initial eye level set:", initial_eye_level)

        # 실시간으로 높이 조정

    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF
    if eye_level and key == ord("f"):
        final_eye_level = eye_level
        print("Eye level fixed at:", final_eye_level)
        target_height = final_eye_level - 10

        # TODO target_height를 이용해서 아두이노에 전달하는 로직
        # TODO 모터의 rpm 고려하여 height를 기반으로 회전량 계산

    if key == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
