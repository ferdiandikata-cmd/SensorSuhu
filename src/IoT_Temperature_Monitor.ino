// ========== KONFIGURASI BLYNK ==========
#define BLYNK_TEMPLATE_ID "TMPL6v_0Uv5Tw"
#define BLYNK_TEMPLATE_NAME "Temperature Monitor"
#define BLYNK_AUTH_TOKEN "gehMFy7l9ZhjPXfeeVl5TnT2XFzteqy8"

// ========== KONFIGURASI WIFI ==========
char ssid[] = "Wokwi-GUEST";        // Ganti dengan SSID WiFi Anda
char pass[] = "";    // Ganti dengan Password WiFi Anda

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

// ========== INISIALISASI ==========
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  
  // === DEBUG: TAMPILAN AWAL ===
  Serial.println("==================================");
  Serial.println("📟 ESP32 STARTING...");
  Serial.println("==================================");
  
  // === INISIALISASI SENSOR ===
  Serial.print("🔧 Inisialisasi DHT22... ");
  dht.begin();
  Serial.println("✅ Selesai");
  
  // === INISIALISASI LCD ===
  Serial.print("🔧 Inisialisasi LCD... ");
  lcd.init();
  lcd.backlight();
  Serial.println("✅ Selesai");
  
  // === SETUP PIN ===
  Serial.print("🔧 Setup pin LED & Buzzer... ");
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_JINGGA, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);
  Serial.println("✅ Selesai");
  
  // === TAMPILAN DI LCD ===
  lcd.setCursor(0, 0);
  lcd.print("  Connecting..  ");
  lcd.setCursor(0, 1);
  lcd.print("  to Blynk...   ");
  
  // === KONEKSI WIFI ===
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
    Serial.println("");
    Serial.print("✅ WiFi Terhubung! IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("❌ GAGAL KONEK WIFI! Cek SSID/Password!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  WiFi ERROR!   ");
    lcd.setCursor(0, 1);
    lcd.print("  Cek SSID/PW   ");
  }
  
  // === KONEKSI BLYNK ===
  Serial.print("📡 Menghubungkan ke Blynk... ");
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
  
  // Tunggu koneksi Blynk (max 10 detik)
  int timeout = 0;
  while (!Blynk.connected() && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }
  
  if (Blynk.connected()) {
    Serial.println("");
    Serial.println("✅ Blynk Terhubung!");
  } else {
    Serial.println("");
    Serial.println("❌ GAGAL KONEK BLYNK! Cek Auth Token/Template ID!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Blynk ERROR!  ");
    lcd.setCursor(0, 1);
    lcd.print("  Cek Token!    ");
  }
  
  // === TIMER ===
  timer.setInterval(2000L, kirimDataKeBlynk);
  
  // === SELESAI ===
  Serial.println("==================================");
  Serial.println("✅ SETUP SELESAI! MASUK KE LOOP...");
  Serial.println("==================================");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Connected!    ");
  delay(1000);
}

// ========== FUNGSI KIRIM DATA KE BLYNK ==========
void kirimDataKeBlynk() {
  Serial.println("🔄 kirimDataKeBlynk() dipanggil...");
  
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();
  
  Serial.print("   📊 Suhu: "); Serial.print(suhu);
  Serial.print(" | Kelembaban: "); Serial.println(kelembaban);
  
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("❌ Gagal baca DHT22!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  ERROR SENSOR  ");
    lcd.setCursor(0, 1);
    lcd.print("  Cek Kabel!    ");
    
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_JINGGA, LOW);
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER_PIN);
    return;
  }
  
  // Kirim ke Blynk
  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, suhu);
    Blynk.virtualWrite(V1, kelembaban);
  } else {
    Serial.println("⚠️ Blynk tidak terhubung, data tidak dikirim!");
  }
  
  // Tentukan status
  String status;
  if (suhu < 28) {
    status = "NORMAL ✅";
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_JINGGA, LOW);
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER_PIN);
  } 
  else if (suhu >= 28 && suhu <= 35) {
    status = "PANAS 🔶";
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_JINGGA, HIGH);
    digitalWrite(LED_MERAH, LOW);
    noTone(BUZZER_PIN);
  } 
  else {
    status = "BAHAYA! 🔴";
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_JINGGA, LOW);
    digitalWrite(LED_MERAH, HIGH);
    tone(BUZZER_PIN, 1500);
    Serial.println("⚠️ PERINGATAN! Suhu melebihi 35°C!");
  }
  
  if (Blynk.connected()) {
    Blynk.virtualWrite(V2, status);
  }
  
  // LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(suhu, 1); lcd.print("C ");
  lcd.print("H:"); lcd.print(kelembaban, 1); lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("ST: "); lcd.print(status);
  
  // Serial Monitor
  Serial.print("🌡️ Suhu: "); Serial.print(suhu, 1);
  Serial.print("°C | 💧 Kelembaban: "); Serial.print(kelembaban, 1);
  Serial.print("% | 📊 Status: "); Serial.println(status);
}

// ========== LOOP ==========
void loop() {
  Blynk.run();
  timer.run();
  delay(10);
}

// ========== KETIKA BLYNK TERHUBUNG ==========
BLYNK_CONNECTED() {
  Blynk.syncAll();
  Serial.println("✅ BLYNK_CONNECTED() dipanggil!");
}
