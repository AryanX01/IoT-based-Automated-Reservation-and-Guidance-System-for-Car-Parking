# IoT-Based Automated Reservation and Guidance System for Car Parking 🚗📶

This project is a smart IoT-based car parking system developed using an ESP32 microcontroller. It allows users to **automatically detect vehicles**, **monitor parking slot availability**, **reserve slots**, and **guide vehicles** to park efficiently using sensors, LEDs, buzzers, a servo gate, and an LCD display.

## 🔧 Features

- 🔄 **Automated Gate System**: Uses an ultrasonic sensor to detect vehicles at the entrance and operate a servo motor to open the barricade.
- 📍 **Slot Monitoring**: Two parking slots monitored using ultrasonic sensors.
- 🔔 **Parking Assist System**: Buzzers beep at varying intervals based on vehicle proximity to assist in parking.
- 🟢🔴🔵 **RGB LED Indicators**: Shows slot status — Free (Green), Occupied (Red), Booked (Blue).
- 💬 **LCD Display (16x2)**: Displays real-time messages like "Welcome", "Slot Booked", "All Booked", or "Stop".
- 📦 **Slot Reservation Logic**: Slots can be pre-booked and their status is updated in real-time.
- 🧠 **Efficient Resource Handling**: Timer-based buzzer beeping and gate control using millis().

## 📚 What I Learned

- Interfacing multiple sensors and actuators with ESP32.
- Using `millis()` for non-blocking delays and timing logic.
- Real-time distance measurement using ultrasonic sensors.
- PWM-based servo control and buzzer feedback systems.
- RGB LED control for status indication.
- I2C communication with LCD display.
- Logical handling of booking, status updates, and vehicle detection.
- Clean and modular coding practices using enums and functions.

## 🧰 Hardware Used

| Component             | Quantity |
|----------------------|----------|
| ESP32 Dev Board (30 pin) | 1 |
| Ultrasonic Sensors (HC-SR04) | 3 |
| RGB LEDs             | 3 |
| Buzzer Modules       | 3 |
| Servo Motor (Gate)   | 1 |
| 16x2 LCD Display with I2C | 1 |
| Jumper Wires         | Several |
| Breadboard           | 2 small |

## 🔌 Circuit Overview

- **Slot Sensors**: Trig/Echo pins monitor slot distance.
- **Gate Sensor**: Trig/Echo detects vehicle at gate.
- **Servo Motor**: Connected to gate and opens when a vehicle is detected.
- **RGB LEDs and Buzzers**: Mounted per slot and gate to indicate status and help with parking and with gate operations.
- **LCD Display**: Shows system messages for user feedback.

## 🚦 Slot Status Logic

| Condition            | LED Color | Buzzer | LCD Message  |
|---------------------|-----------|--------|--------------|
| Free                | Green     | No     | "Free"       |
| Booked              | Blue      | No     | "Slot Booked"|
| Occupied            | Red       | Yes    | "Occupied"   |
| All Booked          | -         | -      | "All Booked" |
| Vehicle at Gate     | Green     | Beeps  | "Welcome"    |

## 🛠️ How to Set Up

1. Connect all components as per the circuit diagram.
2. Flash the provided `.ino` file to the ESP32 using Arduino IDE.
3. Power the ESP32 and observe the behavior.
4. Modify the `slot1Booked` and `slot2Booked` flags to simulate online bookings.

## 🔮 Future Enhancements

- 🔗 **Website Integration**: Add web-based interface for booking and monitoring.
- ☁️ **Cloud Storage**: Log data to Firebase or Thingspeak.
- 📱 **Mobile App**: Real-time slot booking and navigation.
- 🎯 **More Slots**: Scale to 4–8 slots with dynamic allocation.

## 📷 Screenshots / Demo

[Images](https://github.com/AryanX01/IoT-based-Automated-Reservation-and-Guidance-System-for-Car-Parking/tree/main/Images)

## 📄 License

This project is open-source and available under the MIT License.

---

Made with ❤️ using ESP32, sensors, and code.
