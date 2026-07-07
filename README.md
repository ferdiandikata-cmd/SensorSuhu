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

#☁️ Konfigurasi Blynk
