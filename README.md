# Mail-Sender-Ultrasonic-Sensor-Alarm
A fast and reliable ESP32-based alarm system using an ultrasonic distance sensor for motion detection.
When motion is detected, the system immediately triggers a buzzer and LED and sends an email alert via SMTP.

This project is designed to be low-latency, stable, and compatible with ESP32 Arduino Core 3.x.

 Features

 Ultra-fast motion detection (~50 ms reaction time)

 Ultrasonic sensor (HC-SR04 compatible)

 Passive buzzer with adjustable tone (PWM)

 Status LED indicator

 Instant email notification (SMTP – Gmail supported)

 False alarm reduction using double-measure validation

 Non-blocking loop (millis-based)

 Compatible with ESP32 Arduino Core v3.x

 Hardware Requirements

ESP32 (ESP32-Dev Module / ESP32-WROOM / ESP32-32U)

Ultrasonic Sensor (HC-SR04 or compatible)

Passive Buzzer

LED + 220Ω resistor

Power module (e.g. HW-131)

Jumper wires

 Pin Configuration
Component	ESP32 Pin
LED	GPIO 25
Buzzer	GPIO 26
Ultrasonic TRIG	GPIO 32
Ultrasonic ECHO	GPIO 33
 Email Configuration

This project uses SMTP (Gmail) to send alarm notifications.

 Important:

Enable 2-Step Verification on your Google account

Create a Google App Password

Do NOT use your normal Gmail password

#define AUTHOR_EMAIL     "your_email@gmail.com"
#define AUTHOR_PASS      "YOUR_APP_PASSWORD"
#define RECIPIENT_EMAIL  "recipient@gmail.com"

 How It Works

ESP32 connects to WiFi

A reference distance is measured at startup

Ultrasonic sensor continuously monitors distance

If two consecutive measurements exceed the threshold:

LED turns ON

Buzzer sounds

Email alert is sent once per event

System resets when motion disappears

 Libraries Used

WiFi.h

ESP_Mail_Client

ESP32 Arduino Core v3.x

Install ESP Mail Client from Arduino Library Manager.

 Performance

Detection latency: ~40–60 ms

Email trigger: Instant on detection

Stable operation without blocking delays


Possible Improvements

Telegram / MQTT notifications

Web dashboard

Deep sleep for low power usage

Adjustable sensitivity via web interface

Siren pattern / multi-tone alarm

MIT License
Free to use, modify, and distribute.

Regards.
Arda Türkmen
