# 🚀 QUICK START - Fix Data Tidak Muncul

## Masalah Anda:
Data tidak muncul di **Live Monitoring**

---

## ✅ SOLUSI CEPAT (2 MENIT)

### Pilihan A: Tombol di Web App (RECOMMENDED)

1. **Buka aplikasi** di browser
   - File: `index.html`
   - Buka dengan live server atau akses HTTP

2. **Klik tombol "Initialize Data"**
   - Ada di menu utama (tombol ikon database kuning)
   - Konfirmasi dengan "OK"

3. **Tunggu notifikasi "Berhasil"**
   - Jika error, lihat bagian Troubleshooting di bawah

4. **Pergi ke "Data Perjalanan"**
   - Sekarang data sudah muncul!

---

## ✅ SOLUSI B: Firebase Console (Manual)

1. Buka https://console.firebase.google.com/
2. Pilih project: `siste-parkir-iot`
3. Klik **Realtime Database**
4. Klik tombol **"⋮"** di atas, pilih **"Import JSON"**
5. Pilih file: `database_dummy.json`
6. Selesai! Data sudah masuk

---

## ⚠️ Jika Masih Tidak Muncul

### Check 1: Firebase Rules
Firebase Console → Realtime Database → **Rules** tab

Pastikan rules adalah:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

Klik **"Publish"** jika belum.

### Check 2: Error Console
1. Buka halaman di browser
2. Tekan **F12** (buka Developer Tools)
3. Klik tab **"Console"**
4. Lihat apakah ada error merah (biasanya tentang Firebase)

### Check 3: Firebase Config
Di file `index.html` baris ~169, pastikan config sudah benar:
```javascript
const firebaseConfig = {
    apiKey: "AIzaSyCjRx1OXGSH_uAKDq-7ajVykOtHTNHQUwY",
    authDomain: "siste-parkir-iot.firebaseapp.com",
    databaseURL: "https://siste-parkir-iot-default-rtdb.firebaseio.com",
    projectId: "siste-parkir-iot",
    ...
};
```

---

## 📊 Verifikasi Data di Firebase

1. Firebase Console → Realtime Database
2. Tab **"Data"**
3. Lihat apakah ada folder:
   - `logs/` (berisi data perjalanan)
   - `users/` (berisi user)
   - `system/` (settings)

Jika kosong → Klik "Initialize Data" atau upload manual

---

## 🎯 Testing Lengkap

### Data yang Seharusnya Ada:
- 3 users (Budi, Siti, Dedi)
- 5 logs (3 Check-In, 2 Check-Out)

### Live Monitoring Seharusnya Menampilkan:
```
Waktu       | Nama                      | Gate   | Status    | Tarif
08:00:00    | Budi Santoso (A1B2C3D4)  | MASUK  | Check-In  | -
08:25:00    | Siti Aminah (E5F6G7H8)   | MASUK  | Check-In  | -
09:05:00    | Budi Santoso (A1B2C3D4)  | KELUAR | Check-Out | Rp 3.000
09:30:00    | Dedi Corbuzier (99887766)| MASUK  | Check-In  | -
10:00:00    | Dedi Corbuzier (99887766)| KELUAR | Check-Out | Rp 3.000
```

---

## 📝 File-File Penting

- ✅ `index.html` - Aplikasi utama (sudah diupdate)
- ✅ `database_dummy.json` - Data dummy (gunakan untuk upload)
- 📋 `README.md` - Dokumentasi lengkap
- 📋 `FIREBASE_SETUP.md` - Panduan Firebase
- 📋 `QUICK_START.md` - File ini

---

## ❓ FAQ

**Q: Tombol "Initialize Data" tidak muncul?**
- A: Pastikan Anda sudah mengupdate `index.html` (cek versi terbaru)

**Q: Data upload berhasil tapi tidak muncul di Live Monitoring?**
- A: Refresh browser (Ctrl+F5), bersihkan cache

**Q: Cara menambah data testing baru?**
- A: Edit `database_dummy.json`, tambah entry ke `logs` dengan timestamp baru

**Q: Berapa tarif parkir?**
- A: Rp 3.000 untuk Check-Out (keluar), Check-In gratis

---

## 🎉 Selesai!
Data Anda sekarang seharusnya sudah muncul di Live Monitoring.
Jika ada pertanyaan, lihat `README.md` untuk dokumentasi lengkap.
