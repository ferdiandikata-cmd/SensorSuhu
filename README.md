# SensorSuhu
# 🌡️ IoT Room Temperature Monitor

![ESP32](https://img.shields.io/badge/ESP32-DevKitC%20V4-blue)
![Blynk](https://img.shields.io/badge/Platform-Blynk-purple)
![Wokwi](https://img.shields.io/badge/Simulator-Wokwi-orange)
![Version](https://img.shields.io/badge/Version-1.0.0-green)

Sistem monitoring suhu dan kelembaban ruangan berbasis IoT dengan notifikasi visual dan alarm.

**Kelompok Manut** - Proyek IoT

---

## 📋 Daftar Isi
- [Fitur](#-fitur)
- [Komponen Hardware](#-komponen-hardware)
- [Diagram Koneksi (Wokwi)](#-diagram-koneksi-wokwi)
- [Instalasi Software](#-instalasi-software)
- [Konfigurasi Blynk](#-konfigurasi-blynk)
- [Upload Program](#-upload-program)
- [Cara Kerja](#-cara-kerja)
- [Troubleshooting](#-troubleshooting)
- [Lisensi](#-lisensi)

---

## ✨ Fitur
- 📊 Monitoring suhu & kelembaban real-time
- 🖥️ Tampilan LCD 1602 I2C 16x2
- 🚦 Indikator LED 3 warna (Hijau, Jingga, Merah)
- 🔊 Alarm buzzer saat suhu berbahaya (>35°C)
- ☁️ Integrasi dengan platform Blynk IoT
- 📱 Monitoring jarak jauh via smartphone
- 🎮 Simulasi penuh di Wokwi
- 📈 Visualisasi data historis di Blynk

---

## 🧩 Komponen Hardware

| Komponen | Jumlah | Spesifikasi | ID di Wokwi |
|----------|--------|-------------|-------------|
| ESP32 DevKitC V4 | 1 | Microcontroller WiFi + Bluetooth | `esp` |
| DHT22 | 1 | Sensor suhu & kelembaban | `dht1` |
| LCD 1602 I2C | 1 | Display 16x2 karakter | `lcd1` |
| Active Buzzer | 1 | 5V | `bz2` |
| LED Merah | 1 | 5mm | `led1` |
| LED Jingga | 1 | 5mm | `led2` |
| LED Hijau | 1 | 5mm | `led3` |
| Breadboard | 1 | 400/830 titik | `bb1` |
| Kabel Jumper | Secukupnya | Male-to-Female, Male-to-Male | - |
| Resistor 220Ω | 3 | Untuk LED | - |

---

## 🔌 Diagram Koneksi (Wokwi)

### Tabel Pinout ESP32

| Komponen | Pin ESP32 | Keterangan | Warna Kabel |
|----------|-----------|------------|-------------|
| **DHT22 (dht1)** | | | |
| VCC | 3.3V | Power | Merah |
| GND | GND | Ground | Hitam |
| SDA | GPIO 13 | Data pin | Hijau |
| **LCD I2C (lcd1)** | | | |
| VCC | 5V | Power | Merah |
| GND | GND | Ground | Hitam |
| SDA | GPIO 21 | I2C Data | Cyan |
| SCL | GPIO 22 | I2C Clock | Cyan |
| **LED** | | | |
| Hijau (led3) | GPIO 14 | Status Normal | Hijau |
| Jingga (led2) | GPIO 27 | Status Panas | Gold |
| Merah (led1) | GPIO 26 | Status Bahaya | Merah |
| **Buzzer (bz2)** | GPIO 12 | Signal input | Ungu |

### Skema Koneksi Lengkap

ESP32 DevKitC V4 (esp)
├── 3.3V ──► dht1:VCC (Merah)
├── GND.2 ──► bb1:bn.5 (Hitam) ──► dht1:GND
├── GPIO13 ──► dht1:SDA (Hijau)
├── 5V ──► bb1:tp.1 (Merah) ──► lcd1:VCC
├── GND.1 ──► bb1:tn.1 (Hitam) ──► lcd1:GND
├── GPIO21 ──► lcd1:SDA (Cyan)
├── GPIO22 ──► lcd1:SCL (Cyan)
├── GPIO14 ──► led3:A (Hijau) ──► led3:C ──► bb1:bn.28 (Hitam) ──► GND
├── GPIO27 ──► led2:A (Gold) ──► led2:C ──► bb1:bn.27 (Hitam) ──► GND
├── GPIO26 ──► led1:A (Merah) ──► led1:C ──► bb1:bn.26 (Hitam) ──► GND
└── GPIO12 ──► bz2:2 (Ungu) ──► bz2:1 ──► bb1:bn.1 (Hitam) ──► GND

### Detail Koneksi di Wokwi

**DHT22 (dht1):**
- Pin 1 (VCC) → Breadboard baris positif (bp.1) → ESP32 3.3V
- Pin 2 (SDA) → ESP32 GPIO 13
- Pin 3 (NC) → Tidak terhubung
- Pin 4 (GND) → Breadboard baris negatif (bn.2) → ESP32 GND

**LCD I2C (lcd1):**
- VCC → Breadboard baris positif (tp.35) → ESP32 5V
- GND → Breadboard baris negatif (tn.34) → ESP32 GND
- SDA → ESP32 GPIO 21
- SCL → ESP32 GPIO 22

**LED:**
- LED Merah (led1): Anoda → ESP32 GPIO 26, Katoda → GND via resistor
- LED Jingga (led2): Anoda → ESP32 GPIO 27, Katoda → GND via resistor
- LED Hijau (led3): Anoda → ESP32 GPIO 14, Katoda → GND via resistor

**Buzzer (bz2):**
- Pin 1 (+) → Breadboard negatif (bn.1) → GND
- Pin 2 (-) → ESP32 GPIO 12

---

## 💻 Instalasi Software

### 1. Install Arduino IDE
Download dan install [Arduino IDE](https://www.arduino.cc/en/software) versi terbaru (1.8.19+ atau 2.x).

### 2. Install Board ESP32
1. Buka Arduino IDE
2. **File** → **Preferences**
3. Tambahkan URL di **Additional Boards Manager URLs**: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. **Tools** → **Board** → **Boards Manager**
5. Cari "esp32" dan install **ESP32 by Espressif Systems** (versi 2.0.14+)

### 3. Install Library
Buka **Sketch** → **Include Library** → **Manage Libraries**, lalu install:

| Library | Versi Minimal | Keterangan |
|---------|---------------|------------|
| **DHT sensor library** | 1.4.4 | Untuk membaca sensor DHT22 |
| **LiquidCrystal I2C** | 1.1.2 | Untuk LCD 1602 I2C |
| **Blynk** | 1.0.0 | Untuk koneksi ke platform Blynk |

**Cara Install Manual (Alternatif):**
1. Download ZIP dari GitHub
2. **Sketch** → **Include Library** → **Add .ZIP Library**
3. Pilih file ZIP yang sudah di-download

### 4. Konfigurasi WiFi
Edit kode di bagian **KONFIGURASI WIFI**:

```cpp
// Untuk simulasi Wokwi (seperti di diagram.json)
char ssid[] = "terimakasihade";
char pass[] = "akusayangbiznet";

// Untuk penggunaan nyata, ganti dengan WiFi Anda
// char ssid[] = "Nama_WiFi_Anda";
// char pass[] = "Password_WiFi_Anda"; 
```

#☁️ Konfigurasi Blynk

1. Buat Akun Blynk
Buka Blynk Console

Register akun baru atau login dengan akun existing

2. Buat Template (Sudah dibuat)
Template yang digunakan dalam proyek ini:

cpp
#define BLYNK_TEMPLATE_ID "TMPL6v_0Uv5Tw"
#define BLYNK_TEMPLATE_NAME "Temperature Monitor"
#define BLYNK_AUTH_TOKEN "gehMFy7l9ZhjPXfeeVl5TnT2XFzteqy8"
Jika ingin membuat template baru:

Klik New Template

Isi data:

Name: Temperature Monitor

Hardware: ESP32

Connection Type: WiFi

Klik Done

3. Tambahkan Virtual Pins
Virtual Pin	Tipe Data	Keterangan
V0	Double	Suhu (°C)
V1	Double	Kelembaban (%)
V2	String	Status Sistem
4. Buat Dashboard
Klik Dashboard → New Dashboard

Tambahkan widget:

Gauge → Pilih V0 (Suhu)

Label: "Suhu Ruangan"

Unit: °C

Range: 0-50

Gauge → Pilih V1 (Kelembaban)

Label: "Kelembaban"

Unit: %

Range: 0-100

Label → Pilih V2 (Status)

Label: "Status"

SuperChart (Opsional) untuk grafik historis

5. Ambil Kredensial (Jika Buat Template Baru)
Template ID: TMPLxxxxxxx (ada di halaman template)

Auth Token: xxxxxxxxxxxxxxxxxxxxxx (ada di halaman device)

📤 Upload Program
Persiapan Arduino IDE
Buka File: src/IoT_Temperature_Monitor.ino

Pilih Board:

Tools → Board → ESP32 Arduino → ESP32 Dev Module

Pilih Port:

Tools → Port → Pilih port COM (Windows) atau /dev/cu.* (Mac)

Set Parameter (Opsional):

Tools → Flash Size: 4MB

Tools → Partition Scheme: Default

Tools → Upload Speed: 115200

Upload ke ESP32
Klik tombol Upload (→) atau Sketch → Upload

Tunggu proses compile dan upload selesai

Lihat output di Serial Monitor (baud rate 115200)

Simulasi di Wokwi
Buka Wokwi

Import file diagram.json

Upload file sketch.ino

Klik Start Simulation

Atur suhu dan kelembaban di slider DHT22

#🎯 Cara Kerja
Logika Sistem
```
┌─────────────────────────────────────────────────────────┐
│                    LOGIKA SISTEM                        │
├─────────────────────────────────────────────────────────┤
│  Suhu < 28°C    → NORMAL ✅                            │
│  └─ LED Hijau ON                                       │
│  └─ LED Jingga OFF                                     │
│  └─ LED Merah OFF                                      │
│  └─ Buzzer OFF                                         │
│                                                         │
│  28°C - 35°C    → PANAS 🔶                            │
│  └─ LED Hijau OFF                                      │
│  └─ LED Jingga ON                                      │
│  └─ LED Merah OFF                                      │
│  └─ Buzzer OFF                                         │
│                                                         │
│  Suhu > 35°C    → BAHAYA! 🔴                          │
│  └─ LED Hijau OFF                                      │
│  └─ LED Jingga OFF                                     │
│  └─ LED Merah ON (BERKEDIP)                            │
│  └─ Buzzer ON (1500Hz) ⚠️                             │
└─────────────────────────────────────────────────────────┘
```

Tampilan LCD
text
Baris 1: T:27.5C H:65.2%
Baris 2: ST: NORMAL ✅
Tampilan Serial Monitor
text
==================================
📟 IoT TEMPERATURE MONITOR - Manut
==================================
🔧 Inisialisasi DHT22... ✅ Selesai
🔧 Inisialisasi LCD I2C... ✅ Selesai
🔧 Setup pin LED & Buzzer... ✅ Selesai
📶 Menghubungkan ke WiFi: Wokwi-GUEST
.....
✅ WiFi Terhubung! IP: 10.10.0.2
📡 Menghubungkan ke Blynk... 
✅ Blynk Terhubung!
==================================
✅ SETUP SELESAI! MASUK KE LOOP...
==================================

🌡️ Suhu: 27.5°C | 💧 Kelembaban: 65.2% | 📊 Status: NORMAL ✅
🌡️ Suhu: 28.1°C | 💧 Kelembaban: 64.8% | 📊 Status: PANAS 🔶
⚠️ PERINGATAN! Suhu melebihi 35°C!
🌡️ Suhu: 36.2°C | 💧 Kelembaban: 60.1% | 📊 Status: BAHAYA! 🔴
🔧 Troubleshooting
❌ Gagal Koneksi WiFi
Gejala: Serial Monitor menampilkan "❌ GAGAL KONEK WIFI!"
Solusi:

Periksa SSID dan password di kode

Untuk Wokwi, gunakan "Wokwi-GUEST" dengan password kosong

Untuk nyata, pastikan WiFi 2.4GHz (bukan 5GHz)

Restart router

Cek jarak ESP32 dengan router

❌ Gagal Baca DHT22
Gejala: LCD menampilkan "ERROR SENSOR"
Solusi:

Periksa kabel koneksi (VCC ke 3.3V, GND ke GND)

Pastikan pin data terhubung ke GPIO 13

Coba ganti sensor DHT22

Tambahkan delay 2 detik sebelum baca sensor

❌ LCD Tidak Menyala
Gejala: LCD kosong/backlight mati
Solusi:

Periksa kabel I2C (SDA ke GPIO21, SCL ke GPIO22)

Coba alamat I2C alternatif (0x27 atau 0x3F)

Jalankan I2C Scanner untuk cek alamat

Pastikan VCC ke 5V (bukan 3.3V)

❌ Blynk Tidak Terhubung
Gejala: "❌ GAGAL KONEK BLYNK!"
Solusi:

Periksa Auth Token (harus sesuai di Blynk Console)

Periksa Template ID

Pastikan WiFi terhubung

Cek kuota internet

Restart perangkat

❌ LED/Buzzer Tidak Aktif
Gejala: LED tidak menyala, buzzer tidak bunyi
Solusi:

Periksa polaritas LED (Anoda ke pin, Katoda ke GND)

Pastikan resistor 220Ω terpasang

Periksa pin yang digunakan (GPIO14, 27, 26, 12)

Cek dengan tester atau LED lain

❌ Data di Blynk Tidak Update
Gejala: Dashboard Blynk tidak menampilkan data
Solusi:

Refresh dashboard Blynk

Periksa koneksi internet

Cek virtual pin (V0, V1, V2)

Restart ESP32

📱 I2C Scanner Code
```cpp
#include <Wire.h>
void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("I2C Scanner");
  for(byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if(Wire.endTransmission() == 0) {
      Serial.print("Found: 0x");
      Serial.println(addr, HEX);
    }
  }
}
void loop() {}
```
