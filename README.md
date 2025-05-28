AUTOMATED RESERVATION AND GUIDANCE SYSTEM FOR CAR PARKING

This project presents a smart IoT-based car parking system divided into two main modules: Parking Slot Area and Parking Gate. It aims to simplify and automate the parking experience by guiding vehicles to available slots and managing entry/exit based on real-time occupancy and reservations.

Project Structure
1. Parking Slot Area

Built using an ESP32 (30-pin) microcontroller connected to a breadboard at the center.

2 Ultrasonic sensors are placed on opposite sides to monitor the status of 2 individual parking slots.

Each slot includes:

1 RGB LED to indicate status:

Green: Slot is Free

Blue: Slot is Booked

Red: Slot is Occupied

1 Buzzer for audio feedback during parking assistance.

Slot status is monitored and updated in real-time.

2. Parking Gate Module

Includes:

1 Ultrasonic sensor to detect incoming vehicles.

1 RGB LED to indicate gate status.

1 Buzzer for alerts.

1 Servo motor acting as a barricade.

1 I2C 16x2 LCD Display showing live status of both parking slots.

The gate logic ensures smooth entry when a slot is available. The servo opens/closes automatically, and the LCD guides the user with real-time updates such as "All slots booked", "Welcome", etc.

Features
Real-time monitoring of slot availability.

Visual (RGB LED) and audio (buzzer) feedback for parking assistance.

Servo-based automated gate access.

LCD-based dynamic status updates.

Efficient use of ESP32 GPIOs and peripherals.

Modular hardware layout for clear demonstration.

