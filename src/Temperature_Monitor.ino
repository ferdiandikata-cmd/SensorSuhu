// ========== KONFIGURASI BLYNK ==========
#define BLYNK_TEMPLATE_ID "TMPL6v_0Uv5Tw"
#define BLYNK_TEMPLATE_NAME "Temperature Monitor"
#define BLYNK_AUTH_TOKEN "gehMFy7l9ZhjPXfeeVl5TnT2XFzteqy8"

// ========== KONFIGURASI WIFI ==========
char ssid[] = "Not";        // Ganti dengan SSID WiFi Anda
char pass[] = "12345567";   // Ganti dengan Password WiFi Anda

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

// ========== DEFINISI PIN I2C UNTUK LCD ==========
#define SDA_PIN 22
#define SCL_PIN 21

// ========== INISIALISASI ==========
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

// ========== SETUP ==========
void setup() {
    Serial.begin(115200);
    
    // === INISIALISASI I2C DENGAN PIN YANG DITENTUKAN ===
    Wire.begin(SDA_PIN, SCL_PIN);
    
    // === DEBUG: TAMPILAN AWAL ===
    Serial.println("==================================");
    Serial.println("📟 ESP32 STARTING...");
    Serial.println("==================================");
    Serial.print("🔧 SDA Pin: "); 
    Serial.println(SDA_PIN);
    Serial.print("🔧 SCL Pin: "); 
    Serial.println(SCL_PIN);
    
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
    
    // === TAMPILAN AWAL DI LCD ===
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("== TEMP MONITOR =");
    lcd.setCursor(0, 1);
    lcd.print("  Booting...   ");
    delay(2000);
    
    // === KONEKSI WIFI ===
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi: Connecting");
    lcd.setCursor(0, 1);
    lcd.print("SSID: ");
    lcd.print(ssid);
    
    Serial.print("📶 Menghubungkan ke WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, pass);
    
    int percobaan = 0;
    while (WiFi.status() != WL_CONNECTED && percobaan < 30) {
        delay(500);
        Serial.print(".");
        percobaan++;
        
        // Animasi titik di LCD
        lcd.setCursor(14, 0);
        for (int i = 0; i < percobaan % 4; i++) {
            lcd.print(".");
        }
        for (int i = percobaan % 4; i < 3; i++) {
            lcd.print(" ");
        }
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("✅ WiFi Terhubung! IP: ");
        Serial.println(WiFi.localIP());
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi: Connected!");
        lcd.setCursor(0, 1);
        lcd.print("IP: ");
        lcd.print(WiFi.localIP());
        delay(2000);
    } else {
        Serial.println("");
        Serial.println("❌ GAGAL KONEK WIFI! Cek SSID/Password!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("!!! WiFi ERROR !!!");
        lcd.setCursor(0, 1);
        lcd.print("Cek SSID/PW");
        delay(3000);
    }
    
    // === KONEKSI BLYNK ===
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Blynk: Connecting");
    lcd.setCursor(0, 1);
    lcd.print("Please wait...");
    
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
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Blynk: Connected!");
        lcd.setCursor(0, 1);
        lcd.print("System Ready!");
        delay(2000);
    } else {
        Serial.println("");
        Serial.println("❌ GAGAL KONEK BLYNK! Cek Auth Token/Template ID!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("! Blynk ERROR !");
        lcd.setCursor(0, 1);
        lcd.print("Cek Token!");
        delay(3000);
    }
    
    // === TIMER ===
    timer.setInterval(2000L, kirimDataKeBlynk);
    
    // === SELESAI ===
    Serial.println("==================================");
    Serial.println("✅ SETUP SELESAI! MASUK KE LOOP...");
    Serial.println("==================================");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" SYSTEM READY  ");
    lcd.setCursor(0, 1);
    lcd.print("  Monitoring..  ");
    delay(1000);
}

// ========== FUNGSI KIRIM DATA KE BLYNK ==========
void kirimDataKeBlynk() {
    Serial.println("🔄 kirimDataKeBlynk() dipanggil...");
    
    float suhu = dht.readTemperature();
    float kelembaban = dht.readHumidity();
    
    Serial.print("   📊 Suhu: "); 
    Serial.print(suhu);
    Serial.print(" | Kelembaban: "); 
    Serial.println(kelembaban);
    
    if (isnan(suhu) || isnan(kelembaban)) {
        Serial.println("❌ Gagal baca DHT22!");
        
        // TAMPILAN LCD ERROR SENSOR
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("!!! SENSOR ERROR !!!");
        lcd.setCursor(0, 1);
        lcd.print("Check DHT22 Cable");
        
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
    String statusIcon;
    if (suhu < 28) {
        status = "NORMAL";
        statusIcon = "✅";
        digitalWrite(LED_HIJAU, HIGH);
        digitalWrite(LED_JINGGA, LOW);
        digitalWrite(LED_MERAH, LOW);
        noTone(BUZZER_PIN);
    } else if (suhu >= 28 && suhu <= 35) {
        status = "PANAS";
        statusIcon = "🔶";
        digitalWrite(LED_HIJAU, LOW);
        digitalWrite(LED_JINGGA, HIGH);
        digitalWrite(LED_MERAH, LOW);
        noTone(BUZZER_PIN);
    } else {
        status = "BAHAYA!";
        statusIcon = "🔴";
        digitalWrite(LED_HIJAU, LOW);
        digitalWrite(LED_JINGGA, LOW);
        digitalWrite(LED_MERAH, HIGH);
        tone(BUZZER_PIN, 1500);
        Serial.println("⚠️ PERINGATAN! Suhu melebihi 35°C!");
    }
    
    if (Blynk.connected()) {
        Blynk.virtualWrite(V2, status);
    }
    
    // ===== TAMPILAN LCD UTAMA =====
    lcd.clear();
    
    // Baris 1: Suhu dan Kelembaban dengan ikon
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(suhu, 1);
    lcd.print("C ");
    lcd.print("H:");
    lcd.print(kelembaban, 1);
    lcd.print("%");
    
    // Baris 2: Status dengan ikon
    lcd.setCursor(0, 1);
    lcd.print(statusIcon);
    lcd.print(" ");
    lcd.print(status);
    
    // Tambahan informasi jika status bahaya
    if (suhu > 35) {
        lcd.setCursor(10, 1);
        lcd.print("!!ALERT!!");
    }
    
    // Serial Monitor
    Serial.print("🌡️ Suhu: "); 
    Serial.print(suhu, 1);
    Serial.print("°C | 💧 Kelembaban: "); 
    Serial.print(kelembaban, 1);
    Serial.print("% | 📊 Status: "); 
    Serial.println(status);
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
