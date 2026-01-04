# 🚗 PANDUAN TESTING PROJECT PARKIR IoT

## 📋 DAFTAR ISI
1. [Persiapan Awal](#persiapan-awal)
2. [Testing Flow Registrasi](#testing-flow-registrasi)
3. [Testing Gate Masuk](#testing-gate-masuk)
4. [Testing Gate Keluar](#testing-gate-keluar)
5. [Troubleshooting](#troubleshooting)

---

## 🛠️ PERSIAPAN AWAL

### A. Upload Kode ke ESP32

#### ESP32 Gate MASUK
1. Buka Arduino IDE
2. Buka file `ESP32_GATE_MASUK.ino`
3. Sesuaikan kredensial WiFi dan Firebase:
   ```cpp
   #define WIFI_SSID "Muliakos-Lt2-Utama"
   #define WIFI_PASSWORD "Muliakosan123"
   #define API_KEY "AIzaSyCjRx1OXGSH_uAKDq-7ajVykOtHTNHQUwY"
   #define DATABASE_URL "https://siste-parkir-iot-default-rtdb.firebaseio.com/"
   ```
4. Pilih board: **ESP32 Dev Module**
5. Pilih port yang sesuai
6. Upload kode
7. **Buka Serial Monitor (Baud Rate: 115200)**
8. Catat IP Address yang muncul

#### ESP32 Gate KELUAR
1. Buka file `ESP32_GATE_KELUAR.ino`
2. Upload dengan cara yang sama
3. Buka Serial Monitor di tab terpisah

### B. Buka Website

1. Buka file `index.html` di browser (Chrome/Firefox)
2. Atau gunakan Live Server di VS Code
3. Pastikan terkoneksi ke Firebase (lihat console browser: F12)

### C. Setup Firebase Database

1. Buka Firebase Console: https://console.firebase.google.com
2. Pastikan struktur database seperti ini:
   ```
   ├── system
   │   ├── mode: "NORMAL"
   │   └── temp_rfid: ""
   ├── users
   │   └── (kosong atau ada data dummy)
   ├── logs
   │   └── (kosong atau ada data dummy)
   └── config
       └── notifikasi
   ```

3. **PENTING**: Set Firebase Rules ke mode testing:
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```

---

## 🆕 TESTING FLOW REGISTRASI

### Langkah 1: Persiapan
✅ ESP32 Gate Masuk sudah online (cek Serial Monitor)  
✅ Website sudah terbuka  
✅ Kartu RFID siap (belum terdaftar)  

### Langkah 2: Mulai Registrasi dari Website

1. **Di Website:**
   - Klik menu **"Registrasi User"**
   - Isi form:
     - Nama: `Andi Permana`
     - WhatsApp: `081234567890`
     - Password: `andi123`
   - Klik tombol **"Lanjut Scan Kartu"**

2. **Yang Terjadi:**
   ```
   Website → Firebase: 
     - system/mode = "REGISTER"
     - system/temp_rfid = ""
   ```

3. **Verifikasi di Firebase Console:**
   - Buka path: `/system/mode`
   - Harus tertulis: **"REGISTER"**
   - Jika masih "NORMAL", ada masalah koneksi website ke Firebase

### Langkah 3: Scan Kartu di ESP32

1. **Di Serial Monitor ESP32 Gate Masuk:**
   ```
   [STATUS] Time: 14:23:45 | Mode: REGISTER  ← HARUS MUNCUL INI!
   ```

2. **Tempel kartu RFID ke reader**

3. **Yang Terjadi di Serial Monitor:**
   ```
   -----------------------------
   [RFID] Card Detected! UID: A1B2C3D4
   [LOGIC] Processing REGISTRATION...
   [FIREBASE] Success upload to /system/temp_rfid
   -----------------------------
   ```

4. **Verifikasi di Firebase Console:**
   - Path: `/system/temp_rfid`
   - Harus terisi UID kartu, contoh: `"A1B2C3D4"`

### Langkah 4: Auto-Save oleh Website

1. **Website akan otomatis:**
   - Deteksi UID di `/system/temp_rfid`
   - Simpan data user ke `/users/A1B2C3D4`
   - Reset mode ke "NORMAL"
   - Tampilkan alert sukses

2. **Verifikasi di Firebase:**
   ```
   /users/A1B2C3D4:
     nama: "Andi Permana"
     wa: "081234567890"
     password: "andi123"
     saldo: 50000
     status_terakhir: "BARU"
   ```

3. **Di Serial Monitor:**
   ```
   [STATUS] Time: 14:24:10 | Mode: NORMAL  ← Kembali NORMAL
   ```

### ⚠️ TROUBLESHOOTING REGISTRASI

| Masalah | Penyebab | Solusi |
|---------|----------|--------|
| Serial Monitor tetap "Mode: NORMAL" | ESP32 belum baca Firebase | 1. Cek koneksi WiFi<br>2. Restart ESP32<br>3. Cek Firebase Rules |
| Tombol scan tidak mengubah mode | Website tidak terhubung Firebase | 1. Buka console browser (F12)<br>2. Cek error Firebase<br>3. Cek API Key |
| UID tidak muncul di Firebase | Error upload ESP32 | 1. Lihat error di Serial Monitor<br>2. Cek Firebase Rules |
| Website tidak auto-save | Listener tidak jalan | 1. Refresh halaman<br>2. Cek console browser |

---

## ✅ TESTING GATE MASUK (Mode Normal)

### Skenario 1: Kartu Terdaftar

1. **Pastikan mode NORMAL** (lihat Serial Monitor)
2. **Tempel kartu yang sudah terdaftar**
3. **Yang Terjadi:**

   **Serial Monitor:**
   ```
   -----------------------------
   [RFID] Card Detected! UID: A1B2C3D4
   [LOGIC] Processing CHECK-IN...
   [FIREBASE] Checking /users/A1B2C3D4
   [ACCESS] User FOUND in Database.
   [FIREBASE] Sending Log...
   [FIREBASE] Log Saved Successfully
   [GATE] Opening Gate...
   [GATE] Closing Gate...
   -----------------------------
   ```

   **Hardware:**
   - LED Hijau menyala
   - Buzzer bunyi 1x (beep pendek)
   - Servo buka 90° → tunggu 3 detik → tutup 0°
   - LCD: "Silakan Masuk"

4. **Verifikasi di Firebase:**
   ```
   /logs/1735912345:
     uid: "A1B2C3D4"
     gate: "MASUK"
     status: "Check-In"
     waktu: "14:25:45"
     timestamp: 1735912345
   
   /users/A1B2C3D4:
     status_terakhir: "MASUK"  ← Updated
   ```

### Skenario 2: Kartu Tidak Terdaftar

1. **Tempel kartu baru (belum register)**
2. **Yang Terjadi:**

   **Serial Monitor:**
   ```
   -----------------------------
   [RFID] Card Detected! UID: UNKNOWN123
   [LOGIC] Processing CHECK-IN...
   [FIREBASE] Checking /users/UNKNOWN123
   [ACCESS] User NOT FOUND / Unregistered.
   [GATE] Access DENIED!
   -----------------------------
   ```

   **Hardware:**
   - LED Merah tetap
   - Buzzer bunyi 1x (beep panjang 1 detik)
   - Servo tidak bergerak
   - LCD: "Kartu Tidak Terdaftar!"

---

## 🚪 TESTING GATE KELUAR

### Langkah Testing

1. **Pastikan user sudah CHECK-IN** (`status_terakhir: "MASUK"`)
2. **Tempel kartu di Gate Keluar**
3. **Yang Terjadi:**

   **Serial Monitor Gate Keluar:**
   ```
   [RFID] Card Detected! UID: A1B2C3D4
   [LOGIC] Processing CHECK-OUT...
   [ACCESS] User FOUND. Status: MASUK
   [CALC] Durasi parkir: 120 menit
   [CALC] Biaya: Rp 6000
   [FIREBASE] Saldo sebelum: Rp 50000
   [FIREBASE] Saldo sesudah: Rp 44000
   [GATE] Opening Gate...
   ```

4. **Verifikasi Firebase:**
   ```
   /users/A1B2C3D4:
     saldo: 44000  ← Berkurang
     status_terakhir: "KELUAR"  ← Updated
   
   /logs/1735919200:
     gate: "KELUAR"
     status: "Check-Out"
     biaya: 6000
     durasi: "120 menit"
   ```

---

## 🔧 CHECKLIST TESTING HARIAN

### Sebelum Mulai
- [ ] ESP32 Gate Masuk: Serial Monitor terbuka, mode NORMAL
- [ ] ESP32 Gate Keluar: Serial Monitor terbuka, mode NORMAL
- [ ] Website: Terbuka, console browser (F12) tidak ada error
- [ ] Firebase Console: Terbuka di tab terpisah
- [ ] Kartu RFID: Siap 2-3 kartu (1 terdaftar, 1-2 belum)

### Test Case Utama
1. [ ] **Registrasi User Baru** (15 menit)
   - Isi form → Klik scan → Tempel kartu → Cek Firebase
2. [ ] **Check-In Berhasil** (5 menit)
   - Mode NORMAL → Tempel kartu terdaftar → Gate buka
3. [ ] **Check-In Ditolak** (5 menit)
   - Tempel kartu tidak terdaftar → Gate tetap tutup
4. [ ] **Check-Out Berhasil** (10 menit)
   - User sudah masuk → Tempel di gate keluar → Cek saldo
5. [ ] **Notifikasi** (opsional)
   - Cek notifikasi Telegram/WhatsApp

### Monitoring Real-time
- [ ] Serial Monitor: Update setiap 2 detik
- [ ] Firebase Console: Data berubah real-time
- [ ] Website: Log muncul otomatis

---

## 🐛 TROUBLESHOOTING UMUM

### ESP32 Tidak Konek WiFi
```
[WIFI] Connecting to Muliakos-Lt2-Utama........
```
**Solusi:**
1. Cek password WiFi
2. WiFi 2.4GHz (bukan 5GHz)
3. Dekat router

### Firebase Error "Permission Denied"
**Solusi:**
1. Buka Firebase Console → Realtime Database → Rules
2. Ubah ke:
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```
3. Publish rules

### Mode Tetap NORMAL Padahal Sudah Klik Register
**Solusi:**
1. Buka console browser (F12)
2. Cek ada error Firebase?
3. Cek file `index.html` baris 865:
   ```javascript
   set(ref(db, 'system/mode'), 'REGISTER');  // ← Harus ada ini
   ```
4. Refresh halaman, coba lagi

### Servo Tidak Bergerak
**Solusi:**
1. Cek kabel servo (Signal, VCC, GND)
2. Cek pin: `#define PIN_SERVO 13`
3. Test manual:
   ```cpp
   myServo.write(90);
   delay(2000);
   myServo.write(0);
   ```

### LCD Tidak Tampil
**Solusi:**
1. Cek alamat I2C: `0x27` atau `0x3F`
2. Scan I2C:
   ```cpp
   Wire.begin();
   Wire.beginTransmission(0x27);
   byte error = Wire.endTransmission();
   Serial.println(error == 0 ? "Found 0x27" : "Not found");
   ```

---

## 📊 LOG YANG BENAR

### ESP32 Normal (Idle)
```
[STATUS] Time: 14:30:00 | Mode: NORMAL
[STATUS] Time: 14:30:02 | Mode: NORMAL
[STATUS] Time: 14:30:04 | Mode: NORMAL
```

### ESP32 Mode Registrasi
```
[STATUS] Time: 14:30:10 | Mode: REGISTER  ← Berubah
[STATUS] Time: 14:30:12 | Mode: REGISTER
-----------------------------
[RFID] Card Detected! UID: A1B2C3D4
[LOGIC] Processing REGISTRATION...
[FIREBASE] Success upload to /system/temp_rfid
-----------------------------
[STATUS] Time: 14:30:15 | Mode: NORMAL  ← Kembali normal
```

---

## 📞 BANTUAN LANJUT

Jika masih ada masalah:
1. Screenshot Serial Monitor
2. Screenshot Firebase Console
3. Screenshot console browser (F12)
4. Jelaskan langkah yang sudah dilakukan

**Semoga berhasil! 🚀**
