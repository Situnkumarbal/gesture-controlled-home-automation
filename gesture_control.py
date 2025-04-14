"""
gesture_control.py

Hand Gesture-Based Home Automation System
-----------------------------------------
This script uses OpenCV and MediaPipe to detect hand gestures and sends corresponding
commands to an Arduino over serial communication for appliance control via relays.

Author: Situn Kumar Bal
Year: 2025
"""

import cv2
import mediapipe as mp
import serial
import time

# === Initialize Serial Communication ===
try:
    arduino = serial.Serial(port='COM9', baudrate=9600, timeout=1)
    time.sleep(2)
    print("✅ Connected to Arduino on COM9")
except serial.SerialException:
    print("❌ Failed to connect to Arduino. Check your COM port!")
    arduino = None

# === Initialize Mediapipe Hand Tracking ===
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(
    static_image_mode=False,
    max_num_hands=2,
    min_detection_confidence=0.75,
    min_tracking_confidence=0.75
)
mp_drawing = mp.solutions.drawing_utils

# === Finger Detection Logic ===
def detect_fingers(hand_landmarks, handedness):
    """
    Detects finger states (0 = down, 1 = up) from hand landmarks.
    """
    finger_tips = [8, 12, 16, 20]  # Index to Pinky
    thumb_tip, thumb_ip = 4, 2
    finger_states = [0] * 5

    # Thumb detection (horizontal)
    thumb_x = hand_landmarks.landmark[thumb_tip].x
    base_x = hand_landmarks.landmark[thumb_ip].x
    if handedness == "Right":
        if thumb_x < base_x - 0.05:
            finger_states[0] = 1
    else:  # Left hand
        if thumb_x > base_x + 0.05:
            finger_states[0] = 1

    # Other fingers (vertical)
    for idx, tip in enumerate(finger_tips):
        if hand_landmarks.landmark[tip].y < hand_landmarks.landmark[tip - 2].y:
            finger_states[idx + 1] = 1

    return finger_states

# === Start Webcam Feed ===
cap = cv2.VideoCapture(0)

while cap.isOpened():
    success, image = cap.read()
    if not success:
        print("⚠️ Failed to capture frame.")
        break

    # Flip and convert for Mediapipe
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
    results = hands.process(image)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks:
        for i, hand_landmarks in enumerate(results.multi_hand_landmarks):
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            handedness = results.multi_handedness[i].classification[0].label
            print(f"🖐️ Detected Hand: {handedness}")

            # Detect gesture
            fingers_state = detect_fingers(hand_landmarks, handedness)
            data_to_send = ",".join(map(str, fingers_state)) + "\n"
            print(f"📤 Sending to Arduino: {data_to_send.strip()}")

            if arduino:
                try:
                    arduino.write(data_to_send.encode())
                except serial.SerialException:
                    print("⚠️ Failed to send data to Arduino.")

    cv2.imshow('Hand Tracking', image)
    if cv2.waitKey(5) & 0xFF == 27:  # ESC key to exit
        break

# === Cleanup ===
cap.release()
cv2.destroyAllWindows()
if arduino:
    arduino.close()
print("✅ Program exited successfully.")
