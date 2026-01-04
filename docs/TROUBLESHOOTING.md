# 🔧 Troubleshooting Checklist

## Symptom: "Belum ada data perjalanan hari ini."

### ✓ Langkah 1: Pastikan Firebase Connected (2 min)
- [ ] Buka `index.html` di browser (HTTP/HTTPS)
- [ ] Buka Developer Console (F12)
- [ ] Tab "Console" - tidak ada error merah?
- [ ] Jika ada error firebase, config mungkin salah

### ✓ Langkah 2: Pastikan Data Ada di Firebase (3 min)
- [ ] Buka Firebase Console
- [ ] Project: siste-parkir-iot
- [ ] Realtime Database → Tab "Data"
- [ ] Apakah ada folder `logs/`? 
  - Jika tidak → Lanjut ke Langkah 3
  - Jika ya tapi kosong → Klik Initialize Data button

### ✓ Langkah 3: Initialize Data (1 min)
**Cara A - Web Button:**
- [ ] Di halaman utama, klik tombol "Initialize Data" (ikon database kuning)
- [ ] Klik OK pada popup
- [ ] Tunggu notifikasi sukses
- [ ] Refresh halaman (Ctrl+F5)
- [ ] Data seharusnya sudah muncul

**Cara B - Firebase Console:**
- [ ] Realtime Database → Klik ⋮ → Import JSON
- [ ] Pilih file: `database_dummy.json`
- [ ] OK, tunggu selesai
- [ ] Data seharusnya sudah ada

### ✓ Langkah 4: Verifikasi Database Rules (2 min)
- [ ] Firebase Console → Realtime Database
- [ ] Tab "Rules"
- [ ] Paste rules ini:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```
- [ ] Klik "Publish"

### ✓ Langkah 5: Cek Data di Firebase
- [ ] Firebase Console → Realtime Database → Data tab
- [ ] Expand folder `logs`
- [ ] Apakah ada entry seperti:
  - 1710001000 (Budi Check-In)
  - 1710002500 (Siti Check-In)
  - 1710005000 (Budi Check-Out)
  - dll

### ✓ Langkah 6: Refresh & Verifikasi di App
- [ ] Refresh browser (Ctrl+F5)
- [ ] Klik "Data Perjalanan"
- [ ] Apakah data sudah muncul di tabel?

---

## Advanced Debugging

### Jika masih belum muncul:

#### Check Browser Console (F12)
```javascript
// Jalankan di console browser:
db.ref('logs').once('value').then(snap => console.log('Logs:', snap.val()))
```

Lihat apakah ada output atau error.

#### Check Network Tab
- [ ] Buka DevTools (F12)
- [ ] Tab "Network"
- [ ] Refresh halaman
- [ ] Ada request ke Firebase? (firebaseio.com)
- [ ] Response status 200/OK?

#### Clear Cache & Cookies
Jika masih tidak berubah:
1. Ctrl+Shift+Delete (Clear Browsing Data)
2. Hapus semua cookies dan cached images
3. Refresh browser
4. Coba lagi

---

## Kondisi Tertentu

### Jika Initialize Data Error: "Permission Denied"
**Penyebab:** Firebase Rules belum diupdate
**Solusi:**
1. Firebase Console → Realtime Database → Rules
2. Ubah ke:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```
3. Klik "Publish"
4. Tunggu ~1 menit
5. Coba Initialize Data lagi

### Jika Data Upload Tapi Tidak Muncul
**Penyebab:** Browser cache atau listener belum refresh
**Solusi:**
1. Hard refresh: Ctrl+F5 (Windows) atau Cmd+Shift+R (Mac)
2. Buka Inspector (F12) → Settings → disable cache
3. Refresh lagi

### Jika Live Monitoring Lambat Update
**Penyebab:** Internet lambat atau Firebase listener lag
**Solusi:**
1. Periksa koneksi internet
2. Tunggu 2-3 detik sebelum masuk halaman logs
3. Jika tetap lambat, mungkin server Firebase sedang sibuk

---

## Rollback/Reset Database

### Jika mau hapus semua data dan mulai fresh:

**Warning:** Ini akan menghapus SEMUA data di Firebase!

1. Firebase Console → Realtime Database → Data tab
2. Pilih root (/) 
3. Klik ⋮ → Delete
4. Konfirmasi "DELETE THIS LOCATION"
5. Database sekarang kosong
6. Upload dummy data lagi

---

## Testing Data Structure

Untuk menambah data testing baru, edit `database_dummy.json`:

```json
"logs": {
  "1710008000": {
    "biaya": 0,
    "gate": "MASUK",
    "nama": "User Baru",
    "status": "Check-In",
    "timestamp": 1710008000,
    "uid": "NEW_UID_123",
    "waktu": "10:30:00"
  }
}
```

Kemudian:
1. Klik "Initialize Data" button, atau
2. Manual upload ke Firebase

---

## Checklist Akhir

- [ ] Firebase dapat diakses (tidak error)
- [ ] Database Rules sudah diupdate
- [ ] Data sudah di Firebase (verified di console)
- [ ] Live Monitoring menampilkan minimal 1 log
- [ ] Refresh tidak menghilangkan data
- [ ] Button "Initialize Data" berfungsi

Jika semua sudah ✓, maka setup Anda sudah benar!

---

## Kontakt Support

Jika masih error setelah semua steps:
1. Screenshot error console (F12)
2. Screenshot Firebase Rules yang Anda pakai
3. Cek apakah Firebase config sudah benar di `index.html`
4. Tanyakan ke developer/maintainer project
