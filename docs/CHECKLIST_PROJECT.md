# ✅ CHECKLIST PROJECT PARKIR IoT

## 📌 DAFTAR FILE YANG DIBUTUHKAN

### Hardware (ESP32)
- [x] `ESP32_GATE_MASUK.ino` - Kode untuk ESP32 gate masuk (sudah dibuat)
- [x] `ESP32_GATE_KELUAR.ino` - Kode untuk ESP32 gate keluar (sudah dibuat)

### Website
- [x] `index.html` - Web aplikasi utama
- [ ] `database_dummy.json` - Data dummy untuk testing (optional)
- [ ] `upload-data.js` - Script upload data dummy (optional)

### Dokumentasi
- [x] `PANDUAN_TESTING.md` - Panduan lengkap cara testing
- [x] `CHECKLIST.md` - File ini
- [ ] `README.md` - Dokumentasi utama project
- [ ] `FIREBASE_SETUP.md` - Setup Firebase
- [ ] `ESP32_GUIDE.md` - Panduan ESP32

---

## 🔧 PERSIAPAN HARDWARE

### ESP32 Gate MASUK
- [ ] ESP32 Dev Module
- [ ] RFID Reader (MFRC522)
- [ ] LCD I2C (16x2, alamat 0x27)
- [ ] Servo Motor (SG90 atau sejenis)
- [ ] Buzzer
- [ ] LED Hijau
- [ ] LED Merah
- [ ] Resistor 220Ω (untuk LED)
- [ ] Kabel jumper
- [ ] Breadboard

### ESP32 Gate KELUAR
- [ ] ESP32 Dev Module
- [ ] RFID Reader (MFRC522)
- [ ] LCD I2C (16x2, alamat 0x27)
- [ ] Servo Motor (SG90 atau sejenis)
- [ ] Buzzer
- [ ] LED Hijau
- [ ] LED Merah
- [ ] Resistor 220Ω (untuk LED)
- [ ] Kabel jumper
- [ ] Breadboard

### Koneksi Pin (Kedua ESP32)

#### RFID MFRC522
```
ESP32 Pin → RFID Pin
-----------------------
3.3V      → 3.3V
GND       → GND
GPIO 5    → SDA (SS)
GPIO 4    → RST
GPIO 18   → SCK
GPIO 23   → MOSI
GPIO 19   → MISO
```

#### LCD I2C
```
ESP32 Pin → LCD Pin
-----------------------
3.3V      → VCC
GND       → GND
GPIO 21   → SDA
GPIO 22   → SCL
```

#### Komponen Lain
```
GPIO 13   → Servo (Signal)
GPIO 27   → Buzzer (+)
GPIO 25   → LED Hijau (+)
GPIO 26   → LED Merah (+)
GND       → Servo (GND), Buzzer (-), LED (-)
5V        → Servo (VCC)
```

---

## 📱 SETUP FIREBASE

### 1. Buat Project Firebase
- [ ] Buka https://console.firebase.google.com
- [ ] Klik "Add Project" / "Tambah Project"
- [ ] Nama project: `siste-parkir-iot` (atau sesuai keinginan)
- [ ] Disable Google Analytics (tidak wajib)
- [ ] Create Project

### 2. Setup Realtime Database
- [ ] Di menu sebelah kiri, pilih **Build → Realtime Database**
- [ ] Klik "Create Database"
- [ ] Pilih lokasi: **asia-southeast1** (Singapore)
- [ ] Start in **test mode** (untuk development)
- [ ] Database URL akan muncul, contoh:
      ```
      https://siste-parkir-iot-default-rtdb.firebaseio.com/
      ```

### 3. Setup Database Rules (Development)
- [ ] Klik tab **Rules**
- [ ] Paste rules berikut:
      ```json
      {
        "rules": {
          ".read": true,
          ".write": true
        }
      }
      ```
- [ ] Klik **Publish**

### 4. Ambil API Key
- [ ] Klik ⚙️ (Settings) → Project Settings
- [ ] Tab **General**
- [ ] Scroll ke **Your apps** → Web apps
- [ ] Jika belum ada, klik **Add app** → pilih **Web** (ikon </>)
- [ ] Register app, nama: `parkir-web`
- [ ] Copy **API Key**, contoh:
      ```
      AIzaSyCjRx1OXGSH_uAKDq-7ajVykOtHTNHQUwY
      ```

### 5. Setup Data Awal (Opsional)
- [ ] Di Realtime Database, klik **+** di root
- [ ] Tambahkan struktur:
      ```json
      {
        "system": {
          "mode": "NORMAL",
          "temp_rfid": ""
        },
        "users": {},
        "logs": {}
      }
      ```

---

## 💻 SETUP SOFTWARE

### Arduino IDE
- [ ] Download Arduino IDE 2.x dari https://www.arduino.cc/en/software
- [ ] Install Arduino IDE
- [ ] Buka **File → Preferences**
- [ ] Di **Additional Board Manager URLs**, tambahkan:
      ```
      https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
      ```
- [ ] Klik **OK**

### Install ESP32 Board
- [ ] Buka **Tools → Board → Board Manager**
- [ ] Cari: `esp32`
- [ ] Install: **esp32 by Espressif Systems** (versi terbaru)

### Install Library yang Dibutuhkan
Buka **Sketch → Include Library → Manage Libraries**, install:

- [ ] **MFRC522** by GithubCommunity (v1.4.11 atau lebih baru)
- [ ] **Firebase Arduino Client Library for ESP8266 and ESP32** by Mobizt (v4.4.x)
- [ ] **LiquidCrystal I2C** by Frank de Brabander (v1.1.2)
- [ ] **ESP32Servo** by Kevin Harrington (v3.0.x)
- [ ] **NTPClient** by Fabrice Weinberg (v3.2.1)

### Verifikasi Library
- [ ] Buka **Sketch → Include Library**
- [ ] Pastikan semua library di atas ada dalam daftar

---

## 🌐 SETUP WEBSITE

### 1. File Website
- [ ] Pastikan file `index.html` ada
- [ ] Buka dengan text editor (VS Code recommended)

### 2. Update Konfigurasi Firebase
Cari bagian ini di `index.html` (sekitar baris 650-660):

```javascript
const firebaseConfig = {
    apiKey: "AIzaSyCjRx1OXGSH_uAKDq-7ajVykOtHTNHQUwY",
    authDomain: "siste-parkir-iot.firebaseapp.com",
    databaseURL: "https://siste-parkir-iot-default-rtdb.firebaseio.com",
    projectId: "siste-parkir-iot",
    storageBucket: "siste-parkir-iot.appspot.com",
    messagingSenderId: "123456789",
    appId: "1:123456789:web:abcdef123456"
};
```

- [ ] Ganti `apiKey` dengan API Key Anda
- [ ] Ganti `databaseURL` dengan Database URL Anda
- [ ] Ganti `projectId` sesuai project Anda

### 3. Buka Website
Pilih salah satu:

**Opsi A: Langsung buka file**
- [ ] Klik kanan `index.html` → Open with → Browser (Chrome/Firefox)

**Opsi B: Gunakan Live Server (Recommended)**
- [ ] Install VS Code
- [ ] Install extension: **Live Server**
- [ ] Klik kanan `index.html` → **Open with Live Server**
- [ ] Website otomatis buka di `http://localhost:5500`

### 4. Test Koneksi Firebase
- [ ] Buka website
- [ ] Tekan F12 (buka Console)
- [ ] Tidak ada error merah tentang Firebase
- [ ] Coba klik menu "Registrasi User"
- [ ] Jika tidak ada error, koneksi OK ✅

---

## 📤 UPLOAD KODE KE ESP32

### ESP32 #1 - GATE MASUK

#### 1. Persiapan
- [ ] Buka file `ESP32_GATE_MASUK.ino`
- [ ] Update kredensial WiFi (baris 18-19):
      ```cpp
      #define WIFI_SSID "NAMA_WIFI_ANDA"
      #define WIFI_PASSWORD "PASSWORD_WIFI_ANDA"
      ```
- [ ] Update Firebase (baris 20-21):
      ```cpp
      #define API_KEY "YOUR_API_KEY"
      #define DATABASE_URL "https://YOUR_PROJECT.firebaseio.com/"
      ```

#### 2. Upload
- [ ] Hubungkan ESP32 ke komputer via USB
- [ ] Pilih **Tools → Board → ESP32 Dev Module**
- [ ] Pilih **Tools → Port → COM X** (sesuai ESP32 Anda)
- [ ] Klik tombol **Upload** (→)
- [ ] Tunggu hingga "Done uploading"

#### 3. Monitor
- [ ] Klik **Tools → Serial Monitor**
- [ ] Set baud rate: **115200**
- [ ] ESP32 akan reboot, tunggu hingga muncul:
      ```
      ========================================
        GATE MASUK + REGISTRASI - v2.0
      ========================================
      [WIFI] ✓ Connected!
      [WIFI] IP Address: 192.168.x.x
      [FIREBASE] ✓ Connected
      [FIREBASE] ✓ Initial mode: NORMAL
      ========================================
        SYSTEM READY - Waiting for cards...
      ========================================
      ```
- [ ] Catat IP Address

### ESP32 #2 - GATE KELUAR

#### 1. Persiapan
- [ ] Buka file `ESP32_GATE_KELUAR.ino`
- [ ] Update kredensial WiFi (sama seperti Gate Masuk)
- [ ] Update Firebase (sama seperti Gate Masuk)

#### 2. Upload
- [ ] Lepas ESP32 Gate Masuk, hubungkan ESP32 Gate Keluar
- [ ] Atau gunakan komputer kedua / port USB berbeda
- [ ] Pilih **Tools → Board → ESP32 Dev Module**
- [ ] Pilih **Tools → Port → COM Y** (port ESP32 Gate Keluar)
- [ ] Klik **Upload**

#### 3. Monitor
- [ ] Buka Serial Monitor (baud rate: 115200)
- [ ] Tunggu hingga:
      ```
      --- GATE KELUAR STARTING ---
      [WIFI] ✓ Connected!
      [FIREBASE] ✓ Connected
      --- SYSTEM READY ---
      ```

---

## 🧪 TESTING RUNTUTAN

### Test 1: Koneksi Dasar
- [ ] **ESP32 Gate Masuk**: Serial Monitor menunjukkan "SYSTEM READY"
- [ ] **ESP32 Gate Keluar**: Serial Monitor menunjukkan "SYSTEM READY"
- [ ] **Website**: Buka tanpa error (cek Console browser)
- [ ] **Firebase**: Buka Realtime Database, terlihat `/system/mode: "NORMAL"`

### Test 2: Registrasi User Baru
- [ ] Website: Klik "Registrasi User"
- [ ] Isi form (Nama, WA, Password)
- [ ] Klik "Lanjut Scan Kartu"
- [ ] **Cek Firebase**: `/system/mode` berubah jadi `"REGISTER"`
- [ ] **Cek Serial Monitor Gate Masuk**: Muncul `Mode: REGISTER`
- [ ] Tempel kartu RFID ke reader Gate Masuk
- [ ] **Cek Serial Monitor**: Muncul "Processing REGISTRATION..."
- [ ] **Cek Firebase**: `/system/temp_rfid` terisi UID kartu
- [ ] Website otomatis save, muncul alert "User berhasil didaftarkan!"
- [ ] **Cek Firebase**: Ada data baru di `/users/[UID]`
- [ ] Mode kembali ke "NORMAL"

### Test 3: Check-In (Gate Masuk)
- [ ] Pastikan mode "NORMAL"
- [ ] Tempel kartu yang sudah terdaftar
- [ ] **Hardware**: LED Hijau nyala, Buzzer bunyi, Servo buka-tutup
- [ ] **LCD**: Tampil "Selamat Datang!"
- [ ] **Cek Firebase**: Ada log baru di `/logs/[timestamp]`
- [ ] **Cek Firebase**: `/users/[UID]/status_terakhir: "MASUK"`

### Test 4: Akses Ditolak (Gate Masuk)
- [ ] Tempel kartu yang BELUM terdaftar
- [ ] **Hardware**: LED Merah tetap, Buzzer bunyi panjang, Servo tidak gerak
- [ ] **LCD**: Tampil "Kartu Tidak Terdaftar!"
- [ ] **Serial Monitor**: "ACCESS DENIED"

### Test 5: Check-Out (Gate Keluar)
- [ ] Pastikan user sudah check-in (status "MASUK")
- [ ] Tempel kartu di Gate Keluar
- [ ] **Serial Monitor**: Hitung durasi & biaya
- [ ] **Hardware**: LED Hijau nyala, Buzzer bunyi, Servo buka-tutup
- [ ] **LCD**: Tampil biaya & sisa saldo
- [ ] **Cek Firebase**: Saldo berkurang
- [ ] **Cek Firebase**: Status berubah jadi "KELUAR"
- [ ] **Cek Firebase**: Ada log check-out dengan biaya

### Test 6: Saldo Kurang (Gate Keluar)
- [ ] Edit saldo user di Firebase jadi Rp 1000
- [ ] Check-in user tersebut
- [ ] Tunggu beberapa jam (atau ubah timestamp log manual)
- [ ] Coba check-out
- [ ] **LCD**: Tampil "Saldo Kurang!"
- [ ] Gate tidak terbuka

### Test 7: Monitoring Website (Admin)
- [ ] Klik tombol "Admin"
- [ ] Login: `admin` / `admin123` (sesuaikan jika beda)
- [ ] Dashboard admin terbuka
- [ ] Lihat daftar user
- [ ] Lihat log real-time
- [ ] Coba top-up saldo user
- [ ] Lihat grafik statistik

---

## ⚙️ KONFIGURASI LANJUTAN

### Ubah Tarif Parkir
File: `ESP32_GATE_KELUAR.ino`
```cpp
#define TARIF_PER_JAM 3000  // Ubah sesuai kebutuhan
```

### Ubah Durasi Gate Terbuka
File: `ESP32_GATE_MASUK.ino` & `ESP32_GATE_KELUAR.ino`
```cpp
delay(3000);  // Gate terbuka 3 detik, ubah sesuai kebutuhan
```

### Ubah Alamat LCD I2C
Jika LCD tidak muncul, coba ubah alamat:
```cpp
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Dari 0x27 ke 0x3F
```

### Ubah Timezone
Default: GMT+7 (WIB)
```cpp
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200);  // 25200 = GMT+7
```
Untuk WIT (GMT+8): `28800`
Untuk WITA (GMT+8): `28800`

---

## 🔒 KEAMANAN (PRODUCTION)

### Firebase Rules (Production)
Sebelum deploy ke production, ubah rules:

```json
{
  "rules": {
    "system": {
      ".read": true,
      ".write": true
    },
    "users": {
      ".read": "auth != null",
      "$uid": {
        ".write": "auth != null"
      }
    },
    "logs": {
      ".read": "auth != null",
      ".write": true
    },
    "config": {
      ".read": "auth != null",
      ".write": "auth != null"
    }
  }
}
```

### Password Admin
Ubah password default di website (cari "admin123")

### WiFi
Gunakan WiFi dengan password kuat, WPA2

---

## 📊 MONITORING & MAINTENANCE

### Cek Rutin
- [ ] Setiap hari: Cek Serial Monitor untuk error
- [ ] Setiap minggu: Backup data Firebase (Export JSON)
- [ ] Setiap bulan: Cek Firebase usage (quota)

### Troubleshooting
Lihat file: `PANDUAN_TESTING.md` bagian Troubleshooting

### Update Kode
Jika ada perubahan:
- [ ] Update kode di Arduino IDE
- [ ] Upload ulang ke ESP32
- [ ] Test semua fungsi

---

## ✅ FINAL CHECKLIST

### Hardware
- [ ] ESP32 Gate Masuk berfungsi 100%
- [ ] ESP32 Gate Keluar berfungsi 100%
- [ ] Semua sensor & aktuator terhubung dengan benar
- [ ] Power supply stabil (5V untuk Servo, 3.3V untuk ESP32)

### Software
- [ ] Kode ESP32 uploaded tanpa error
- [ ] Website buka tanpa error
- [ ] Firebase terhubung
- [ ] Semua library terinstall

### Testing
- [ ] Registrasi user berhasil
- [ ] Check-in berhasil
- [ ] Check-out berhasil
- [ ] Perhitungan biaya benar
- [ ] Notifikasi jalan (jika diaktifkan)

### Dokumentasi
- [ ] README.md lengkap
- [ ] Panduan testing tersedia
- [ ] Skema hardware terdokumentasi
- [ ] Kredensial tersimpan aman

---

## 🚀 PROJECT READY!

Jika semua checklist di atas sudah ✅, project Anda siap digunakan!

**Selamat! 🎉**

---

## 📞 SUPPORT

Jika ada pertanyaan atau masalah:
1. Baca ulang `PANDUAN_TESTING.md`
2. Cek Serial Monitor untuk error log
3. Cek Firebase Console untuk data
4. Cek Console Browser (F12) untuk error JavaScript

**Good luck with your project! 🚗🎯**
