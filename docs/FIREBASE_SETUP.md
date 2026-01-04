# Panduan Setup Firebase Realtime Database Rules

## ⚠️ PENTING: Security Rules

Aplikasi Anda memerlukan rule yang tepat di Firebase. Berikut caranya:

### Langkah 1: Buka Firebase Console
1. Kunjungi https://console.firebase.google.com/
2. Pilih project: **siste-parkir-iot**
3. Buka **Realtime Database** dari menu samping

### Langkah 2: Buka Tab "Rules"
1. Klik tab **"Rules"** di atas
2. Hapus rules yang ada (atau gunakan template di bawah)

### Langkah 3: Paste Rules Development (UNTUK TESTING)

**⚠️ HANYA UNTUK DEVELOPMENT/TESTING:**

```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

Klik "Publish"

### Langkah 4: Rules Production (UNTUK LIVE/PRODUCTION)

Sebelum go live, gunakan rules yang lebih ketat:

```json
{
  "rules": {
    "system": {
      ".read": true,
      ".write": true
    },
    "users": {
      "$uid": {
        ".read": true,
        ".write": true
      }
    },
    "logs": {
      "$key": {
        ".read": true,
        ".write": true
      }
    }
  }
}
```

---

## Debugging Database

### 1. Cek Data via Firebase Console
1. Buka Realtime Database
2. Tab **"Data"**
3. Lihat structure JSON-nya

### 2. Cek di Browser Console
Buka aplikasi web Anda, tekan F12, buka tab "Console":

```javascript
// Untuk melihat data dari Firebase
firebase.database().ref('logs').on('value', (snapshot) => {
    console.log('Data logs:', snapshot.val());
});
```

### 3. Monitor Real-time
Buka Realtime Database di Firebase Console, semua perubahan data akan terlihat langsung.

---

## Struktur Database Expected

Setelah menggunakan tombol "Initialize Data", database Anda seharusnya seperti ini:

```
siste-parkir-iot/
├── logs/
│   ├── 1710001000/
│   │   ├── biaya: 0
│   │   ├── gate: "MASUK"
│   │   ├── nama: "Budi Santoso"
│   │   ├── status: "Check-In"
│   │   ├── timestamp: 1710001000
│   │   ├── uid: "A1B2C3D4"
│   │   └── waktu: "08:00:00"
│   ├── 1710002500/
│   │   ... (data Siti Aminah)
│   └── ... (lebih banyak log)
├── system/
│   ├── mode: "NORMAL"
│   └── temp_rfid: ""
└── users/
    ├── A1B2C3D4/
    │   ├── nama: "Budi Santoso"
    │   ├── password: "123"
    │   ├── saldo: 47000
    │   ├── status_terakhir: "KELUAR"
    │   └── wa: "08123456789"
    ├── E5F6G7H8/
    │   ... (data Siti Aminah)
    └── 99887766/
        ... (data Dedi Corbuzier)
```

---

## Troubleshooting

### Error: "Database operation error"
✅ Solusi:
1. Periksa apakah Firebase Rules sudah "Publish"
2. Pastikan rules mengizinkan `.write: true`

### Data tidak update real-time
✅ Solusi:
1. Refresh browser (Ctrl+F5)
2. Buka DevTools (F12) dan lihat apakah ada error
3. Periksa Firebase Rules masih aktif

### Tidak bisa register user
✅ Solusi:
1. Pastikan `system/mode` bisa diubah di rules
2. Cek apakah data sudah terwrite ke `users/{uid}`

---

## Testing Offline

Jika tidak ada koneksi internet/Firebase:
1. Data tidak akan tersimpan
2. Live Monitoring akan menampilkan pesan "Belum ada data"
3. Semua perubahan akan hilang saat refresh

Untuk testing offline, gunakan mock data atau local storage sebagai alternatif.
