# 🖐️ Hand Gesture-Based Home Automation System

This project enables users to control home appliances using intuitive hand gestures, combining computer vision with embedded electronics for a contactless and smart home experience.

## 🚀 Overview

The system uses a webcam to capture hand gestures in real time and leverages **OpenCV** and **MediaPipe** to detect and analyze finger positions. Detected gestures are transmitted to an **Arduino** via serial communication, which then triggers **relays** to control connected appliances like lights or fans.

### 🔧 Key Features

- Real-time gesture detection using OpenCV + MediaPipe  
- Touchless control of household devices  
- Arduino-based hardware interface with relay modules  
- LED feedback for individual finger states  
- Reliable serial communication between PC and microcontroller  

---

## 🛠️ Technologies Used

| Component         | Purpose                          |
|------------------|----------------------------------|
| Python            | Main logic and vision processing |
| OpenCV + MediaPipe | Hand and finger tracking        |
| Arduino UNO/Nano  | Hardware control unit            |
| Relay Module      | Appliance switching              |
| pyserial          | Serial communication             |

---

## 🖥️ Software Setup

### 1. Install Python Dependencies
pip install opencv-python mediapipe pyserial

### 2. Upload Arduino Code
Upload the arduino_relay_control.ino file to your Arduino board using the Arduino IDE or PlatformIO.

### 3. Run Python Script

python gesture_control.py

Make sure your Arduino is connected to the correct COM port (update it in the Python script if needed).

