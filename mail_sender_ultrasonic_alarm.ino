#include <WiFi.h>
#include <ESP_Mail_Client.h>

/* ---------- PINLER ---------- */
#define LED_PIN     25
#define BUZZER_PIN  26
#define TRIG_PIN    32
#define ECHO_PIN    33

/* ---------- BUZZER ---------- */
#define BUZZER_FREQ 300
#define BUZZER_RES  8

/* ---------- WIFI ---------- */
const char* ssid = "aettel";
const char* password = "12042206";

/* ---------- EMAIL ---------- */
#define AUTHOR_EMAIL "gönderici@gmail.com"
#define AUTHOR_PASS  "göndericiparolasi"
#define RECIPIENT_EMAIL "alici@gmail.com"

SMTPSession smtp;
SMTP_Message message;

/* ---------- AYARLAR ---------- */
float refDistance = 0;
const float threshold = 30.0;   // hassasiyet (cm)
bool alarmSent = false;

/* ---------- MESAFE ---------- */
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 12000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

/* ---------- HIZLI DOĞRULAMA ---------- */
bool motionDetected() {
  float d1 = getDistance();
  delayMicroseconds(8000); // 8 ms
  float d2 = getDistance();

  if (d1 < 0 || d2 < 0) return false;

  return (abs(d1 - refDistance) > threshold &&
          abs(d2 - refDistance) > threshold);
}

/* ---------- BUZZER ---------- */
void buzzerOn()  { ledcWriteTone(BUZZER_PIN, BUZZER_FREQ); }
void buzzerOff() { ledcWriteTone(BUZZER_PIN, 0); }

/* ---------- EMAIL ---------- */
void sendMail() {
  if (alarmSent) return;

  Session_Config config;
  config.server.host_name = "smtp.gmail.com";
  config.server.port = 465;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASS;
  config.login.user_domain = "";

  message.sender.name = "ESP32 Alarm";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP32 ALARMI";
  message.addRecipient("User", RECIPIENT_EMAIL);
  message.text.content = "HAREKET ALGILANDI";
  message.text.charSet = "utf-8";

  smtp.connect(&config);

  if (MailClient.sendMail(&smtp, &message)) {
    Serial.println("MAIL GÖNDERİLDİ");
    alarmSent = true;
  } else {
    Serial.println("MAIL HATASI");
  }
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  ledcAttach(BUZZER_PIN, BUZZER_FREQ, BUZZER_RES);

  WiFi.begin(ssid, password);
  Serial.print("WiFi bağlanıyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK");

  // Referans mesafe (HIZLI)
  float sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += getDistance();
    delay(50);
  }
  refDistance = sum / 5;

  Serial.print("Referans: ");
  Serial.println(refDistance);

  // TEST
  buzzerOn();
  delay(300);
  buzzerOff();
}

/* ---------- LOOP ---------- */
void loop() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 40) return;
  lastCheck = millis();

  if (motionDetected()) {
    digitalWrite(LED_PIN, HIGH);
    buzzerOn();
    sendMail();
  } else {
    digitalWrite(LED_PIN, LOW);
    buzzerOff();
    alarmSent = false;
  }
}
