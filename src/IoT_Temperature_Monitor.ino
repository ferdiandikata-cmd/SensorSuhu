/*
 * IoT Room Temperature Monitor
 * Kelompok: Manut
 * Mikrokontroler: ESP32 DevKitC V4
 * Sensor: DHT22
 * Output: LCD 1602 I2C, Buzzer, 3x LED
 * Platform: Blynk IoT
 */

// ========== KONFIGURASI BLYNK ==========
#define BLYNK_TEMPLATE_ID "TMPL6v_0Uv5Tw"
#define BLYNK_TEMPLATE_NAME "Temperature Monitor"
#define BLYNK_AUTH_TOKEN "gehMFy7l9ZhjPXfeeVl5TnT2XFzteqy8"

// ========== KONFIGURASI WIFI ==========
char ssid[] = "Wokwi-GUEST"; //Ini adalah jaringan virtual, jadi sesuaikan dengan wifi yang ada
char pass[] = "";

// ========== LIBRARY ==========
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// ========== DEFINISI PIN ==========
#define DHTPIN 13
#define DHTTYPE DHT22
#define BUZZER_PIN 12
#define LED_MERAH 26
#define LED_JINGGA 27
#define LED_HIJAU 14

// ========== KONSTANTA SUHU ==========
const float BATAS_NORMAL = 28.0;
const float BATAS_BAHAYA = 35.0;

// ========== INISIALISASI ==========
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

// ========== VARIABEL GLOBAL ==========
float suhu = 0;
float kelembaban = 0;
String statusSistem = "NORMAL ✅";

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  Serial.println("\n==================================");
  Serial.println("📟 IoT TEMPERATURE MONITOR - Manut");
  Serial.println("==================================");

  // Inisialisasi Sensor
  Serial.print("🔧 Inisialisasi DHT22... ");
  dht.begin();
  Serial.println("✅ Selesai");

  // Inisialisasi LCD
  Serial.print("🔧 Inisialisasi LCD I2C... ");
  lcd.init();
  lcd.backlight();
  Serial.println("✅ Selesai");

  // Setup Pin Output
  Serial.print("🔧 Setup pin LED & Buzzer... ");
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_JINGGA, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);
  Serial.println("✅ Selesai");

  // Tampilan Awal LCD
  lcd.setCursor(0, 0);
  lcd.print("  Connecting..  ");
  lcd.setCursor(0, 1);
  lcd.print("  to Blynk...   ");

  // Koneksi WiFi
  Serial.print("📶 Menghubungkan ke WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int percobaan = 0;
  while (WiFi.status() != WL_CONNECTED && percobaan < 30) {
    delay(500);
    Serial.print(".");
    percobaan++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Terhubung!");
    Serial.print("📡 IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ GAGAL KONEK WIFI! Cek SSID/Password!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  WiFi ERROR!   ");
    lcd.setCursor(0, 1);
    lcd.print("  Cek SSID/PW   ");
  }

  // Koneksi Blynk
  Serial.print("📡 Menghubungkan ke Blynk... ");
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  int timeout = 0;
  while (!Blynk.connected() && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (Blynk.connected()) {
    Serial.println("\n✅ Blynk Terhubung!");
  } else {
    Serial.println("\n❌ GAGAL KONEK BLYNK! Cek Auth Token/Template ID!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Blynk ERROR!  ");
    lcd.setCursor(0, 1);
    lcd.print("  Cek Token!    ");
  }

  // Setup Timer (kirim data setiap 2 detik)
  timer.setInterval(2000L, kirimDataKeBlynk);

  Serial.println("==================================");
  Serial.println("✅ SETUP SELESAI! MASUK KE LOOP...");
  Serial.println("==================================\n");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Connected!    ");
  delay(1000);
}

// ========== FUNGSI KIRIM DATA KE BLYNK ==========
void kirimDataKeBlynk() {
  // Baca data dari sensor
  suhu = dht.readTemperature();
  kelembaban = dht.readHumidity();

  // Cek pembacaan sensor
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("❌ ERROR: Gagal membaca DHT22!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  ERROR SENSOR  ");
    lcd.setCursor(0, 1);
    lcd.print("  Cek Kabel!    ");
    
    // Matikan semua output
    matikanSemuaOutput();
    return;
  }

  // Update status berdasarkan suhu
  updateStatusSuhu();

  // Kirim data ke Blynk
  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, suhu);
    Blynk.virtualWrite(V1, kelembaban);
    Blynk.virtualWrite(V2, statusSistem);
  } else {
    Serial.println("⚠️ Blynk tidak terhubung!");
  }

  // Update LCD
  updateLCD();

  // Tampilkan di Serial Monitor
  Serial.print("🌡️ Suhu: ");
  Serial.print(suhu, 1);
  Serial.print("°C | 💧 Kelembaban: ");
  Serial.print(kelembaban, 1);
  Serial.print("% | 📊 Status: ");
  Serial.println(statusSistem);
}

// ========== UPDATE STATUS SUHU ==========
void updateStatusSuhu() {
  if (suhu < BATAS_NORMAL) {
    statusSistem = "NORMAL ✅";
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_JINGGA, LOW);
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER_PIN);
  }
  else if (suhu >= BATAS_NORMAL && suhu <= BATAS_BAHAYA) {
    statusSistem = "PANAS 🔶";
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_JINGGA, HIGH);
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER_PIN);
  }
  else { // suhu > 35°C
    statusSistem = "BAHAYA! 🔴";
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_JINGGA, LOW);
    digitalWrite(LED_MERAH, HIGH);
    tone(BUZZER_PIN, 1500); // Buzzer bunyi 1500Hz
    Serial.println("⚠️ PERINGATAN! Suhu melebihi 35°C!");
  }
}

// ========== UPDATE LCD ==========
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(suhu, 1);
  lcd.print("C ");
  lcd.print("H:");
  lcd.print(kelembaban, 1);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("ST: ");
  lcd.print(statusSistem);
}

// ========== MATIKAN SEMUA OUTPUT ==========
void matikanSemuaOutput() {
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(LED_JINGGA, LOW);
  digitalWrite(LED_MERAH, LOW);
  noTone(BUZZER_PIN);
}

// ========== LOOP ==========
void loop() {
  Blynk.run();
  timer.run();
  delay(10);
}

// ========== EVENT BLYNK TERHUBUNG ==========
BLYNK_CONNECTED() {
  Blynk.syncAll();
  Serial.println("✅ BLYNK_CONNECTED() - Sinkronisasi selesai!");
}
