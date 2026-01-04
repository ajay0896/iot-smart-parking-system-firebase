# 🚀 QUICK START - Project Parkir IoT

## ⚡ Mulai dalam 5 Langkah

### 1️⃣ UPLOAD KODE KE ESP32 (10 menit)

#### ESP32 #1 - Gate Masuk
```
1. Buka: ESP32_GATE_MASUK.ino
2. Edit baris 18-21:
   - WIFI_SSID: "Muliakos-Lt2-Utama"
   - WIFI_PASSWORD: "Muliakosan123"
   - API_KEY: "AIzaSyCjRx1OXGSH_uAKDq-7ajVykOtHTNHQUwY"
   - DATABASE_URL: "https://siste-parkir-iot-default-rtdb.firebaseio.com/"

3. Tools → Board → ESP32 Dev Module
4. Tools → Port → (pilih port ESP32)
5. Upload (→)
6. Serial Monitor (Ctrl+Shift+M) → Baud: 115200
```

**Sukses jika muncul:**
```
========================================
  GATE MASUK + REGISTRASI - v2.0
========================================
[WIFI] ✓ Connected!
[FIREBASE] ✓ Connected
[FIREBASE] ✓ Initial mode: NORMAL
========================================
  SYSTEM READY - Waiting for cards...
========================================
```

#### ESP32 #2 - Gate Keluar
```
Ulangi langkah yang sama dengan file:
ESP32_GATE_KELUAR.ino
```

---

### 2️⃣ BUKA WEBSITE (2 menit)

```
1. Buka index.html dengan browser
   atau
2. VS Code → Install "Live Server" → Klik kanan index.html → Open with Live Server
3. Tekan F12 → Lihat Console (tidak boleh ada error merah)
```

---

### 3️⃣ CEK FIREBASE (1 menit)

```
1. Buka: https://console.firebase.google.com
2. Pilih project: siste-parkir-iot
3. Klik: Realtime Database
4. Pastikan ada struktur:
   /system
     ├─ mode: "NORMAL"
     └─ temp_rfid: ""
```

---

### 4️⃣ TEST REGISTRASI (5 menit)

#### Di Website:
```
1. Klik "Registrasi User"
2. Isi:
   - Nama: Andi Permana
   - WA: 081234567890
   - Password: andi123
3. Klik "Lanjut Scan Kartu"
```

#### Di Firebase Console (refresh):
```
✅ /system/mode harus berubah jadi: "REGISTER"
```

#### Di Serial Monitor ESP32 Gate Masuk:
```
╔════════════════════════════════════╗
║  MODE CHANGED → REGISTER           ║  ← HARUS MUNCUL INI!
╚════════════════════════════════════╝
```

**Jika sudah muncul → PERBAIKAN BERHASIL! ✅**

#### Tempel Kartu RFID:
```
Serial Monitor akan menampilkan:
┌────────────────────────────────────┐
│ CARD DETECTED: A1B2C3D4            │
│ MODE: REGISTER                     │
└────────────────────────────────────┘
► Processing: REGISTRATION MODE
  ✓ Upload SUCCESS!
```

#### Website akan auto-save:
```
- Muncul alert: "User berhasil didaftarkan!"
- Mode kembali ke "NORMAL"
```

---

### 5️⃣ TEST CHECK-IN & CHECK-OUT (5 menit)

#### Test Check-In:
```
1. Pastikan mode: NORMAL (lihat Serial Monitor)
2. Tempel kartu yang sudah terdaftar (A1B2C3D4)
3. Gate buka? ✅ Berhasil!
4. Cek Firebase → /users/A1B2C3D4/status_terakhir: "MASUK"
```

#### Test Check-Out:
```
1. Tempel kartu yang sama di ESP32 Gate Keluar
2. Serial Monitor menampilkan perhitungan biaya
3. Gate buka? ✅ Berhasil!
4. Cek Firebase → saldo berkurang, status: "KELUAR"
```

---

## ✅ CHECKLIST CEPAT

Sebelum mulai testing:
- [ ] ESP32 #1 (Gate Masuk): Serial Monitor terbuka, "SYSTEM READY"
- [ ] ESP32 #2 (Gate Keluar): Serial Monitor terbuka, "SYSTEM READY"
- [ ] Website: Terbuka, console browser (F12) tidak ada error
- [ ] Firebase Console: Terbuka, `/system/mode: "NORMAL"`
- [ ] Kartu RFID: Siap 2-3 kartu

---

## 🐛 TROUBLESHOOTING CEPAT

### Serial Monitor tetap "Mode: NORMAL" padahal sudah klik register?

**Cek 1: Firebase Rules**
```
Firebase Console → Database → Rules tab → Pastikan:
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
Klik "Publish"
```

**Cek 2: Test Manual**
```
1. Firebase Console → Realtime Database
2. Klik /system/mode
3. Edit manual jadi "REGISTER"
4. Dalam 1-2 detik, Serial Monitor harus muncul:
   "MODE CHANGED → REGISTER"
   
Jika muncul → ESP32 OK, masalah di website
Jika tidak muncul → Cek koneksi ESP32 ke Firebase
```

**Cek 3: Console Browser**
```
1. Website → Tekan F12
2. Klik tab "Console"
3. Ada error merah tentang Firebase?
4. Jika ada, cek API Key dan Database URL di index.html
```

---

## 📚 DOKUMENTASI LENGKAP

Untuk panduan lebih detail, baca:

1. **SUMMARY_PERBAIKAN.md** - Penjelasan masalah & perbaikan
2. **PANDUAN_TESTING.md** - Panduan testing lengkap step-by-step
3. **CHECKLIST_PROJECT.md** - Checklist setup hardware & software lengkap

---

## 🎯 FLOW SINGKAT

```
REGISTRASI:
Website (klik register) 
  → Firebase (mode = "REGISTER") 
  → ESP32 (deteksi mode, LCD berubah)
  → User tap kartu
  → ESP32 (upload UID)
  → Website (auto-save user)
  → DONE ✅

CHECK-IN:
User tap kartu 
  → ESP32 (cek database) 
  → Terdaftar? 
  → Buka gate + simpan log ✅

CHECK-OUT:
User tap kartu 
  → ESP32 (cek database + status)
  → Hitung durasi & biaya
  → Saldo cukup?
  → Potong saldo + buka gate ✅
```

---

## 🚨 PENTING!

### SEBELUM UPLOAD KODE:
✅ Pastikan WiFi SSID & Password benar  
✅ Pastikan Firebase API Key & Database URL benar  
✅ Pilih board: ESP32 Dev Module  
✅ Pilih port yang benar  

### SETELAH UPLOAD:
✅ WAJIB buka Serial Monitor (Baud: 115200)  
✅ Tunggu hingga "SYSTEM READY"  
✅ Catat IP Address yang muncul  

### SAAT TESTING:
✅ Selalu pantau Serial Monitor untuk log real-time  
✅ Buka Firebase Console untuk verifikasi data  
✅ Buka Console browser (F12) untuk cek error website  

---

## 💡 TIPS

1. **Gunakan 2 monitor/layar** untuk melihat Serial Monitor + Website + Firebase secara bersamaan

2. **Serial Monitor harus selalu terbuka** - ini "mata" Anda untuk debugging

3. **Jika ada error** - selalu cek 3 tempat:
   - Serial Monitor (error ESP32)
   - Console Browser / F12 (error website)
   - Firebase Console (data)

4. **Mode tidak berubah?** - Test manual ubah di Firebase Console dulu

5. **Gate tidak buka?** - Cek koneksi servo & Serial Monitor untuk error

---

## 🎉 SELAMAT!

Jika semua langkah di atas berhasil, project Anda sudah siap digunakan!

**Selamat mencoba! 🚀**

---

**Butuh bantuan?** Baca file dokumentasi lainnya atau cek bagian troubleshooting.
