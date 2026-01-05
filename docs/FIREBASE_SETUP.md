# Panduan Setup Firebase Realtime Database Rules

> Updated dengan fitur Top-Up Voucher (Versi 2.0)

## ⚠️ PENTING: Security Rules

Aplikasi Anda memerlukan rule yang tepat di Firebase. Berikut caranya:

### Langkah 1: Buka Firebase Console
1. Kunjungi https://console.firebase.google.com/
2. Pilih project: **siste-parkir-iot**
3. Klik **Authentication** → Enable **Anonymous Auth**
4. Buka **Realtime Database** dari menu samping

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

Sebelum go live, gunakan rules yang lebih ketat dengan fitur Top-Up:

```json
{
  "rules": {
    "system": {
      ".read": "auth != null",
      ".write": "auth != null"
    },
    "users": {
      ".read": "auth != null",
      "$uid": {
        ".write": "auth != null"
      }
    },
    "logs": {
      ".read": "auth != null",
      ".write": "auth != null",
      ".indexOn": ["uid", "timestamp"]
    },
    "vouchers": {
      ".read": "auth != null",
      ".write": "auth != null",
      ".indexOn": ["kode", "status"]
    },
    "top_ups": {
      ".read": "auth != null",
      ".write": "auth != null",
      ".indexOn": ["uid", "timestamp"]
    },
    "config": {
      ".read": "auth != null",
      ".write": "auth != null"
    }
  }
}
```

**Penjelasan:**
- `auth != null` → Hanya authenticated users
- `.indexOn` → Optimize query performance
- `vouchers` → Untuk fitur top-up (NEW)
- `top_ups` → Untuk tracking top-up history (NEW)

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

// Untuk melihat vouchers (NEW)
firebase.database().ref('vouchers').on('value', (snapshot) => {
    console.log('Data vouchers:', snapshot.val());
});

// Untuk melihat top-ups (NEW)
firebase.database().ref('top_ups').on('value', (snapshot) => {
    console.log('Data top_ups:', snapshot.val());
});
```

### 3. Monitor Real-time
Buka Realtime Database di Firebase Console, semua perubahan data akan terlihat langsung.

---

## Setup untuk Fitur Top-Up Voucher

### Prerequisite
1. Gunakan **`index-with-topup.html`** (bukan `index.html`)
2. Anonymous Auth harus **enabled**
3. Firebase Rules harus include `vouchers` dan `top_ups` path

### Step-by-Step Setup

#### 1. Enable Anonymous Authentication
```
Firebase Console → Authentication → Sign-in method
→ Anonymous → Enable → Save
```

#### 2. Update Database Rules (COPY-PASTE)
```json
{
  "rules": {
    "system": {
      ".read": "auth != null",
      ".write": "auth != null"
    },
    "users": {
      ".read": "auth != null",
      "$uid": {
        ".write": "auth != null"
      }
    },
    "logs": {
      ".read": "auth != null",
      ".write": "auth != null",
      ".indexOn": ["uid", "timestamp"]
    },
    "vouchers": {
      ".read": "auth != null",
      ".write": "auth != null",
      ".indexOn": ["kode", "status"]
    },
    "top_ups": {
      ".read": "auth != null",
      ".write": "auth != null",
      ".indexOn": ["uid", "timestamp"]
    },
    "config": {
      ".read": "auth != null",
      ".write": "auth != null"
    }
  }
}
```
Click "Publish"

#### 3. Test di Browser
```
1. Buka index-with-topup.html
2. Klik "Admin" login
3. Username: admin, Password: admin123
4. Klik "Kelola Voucher"
5. Generate 1 voucher Rp 50.000
6. Jika berhasil → Setup OK ✓
```

### Admin Workflow

#### Generate Voucher
```
1. Login admin (admin/admin123)
2. Dashboard → Kelola Voucher
3. Pilih nominal & jumlah
4. Klik "Generate X Voucher"
5. Voucher otomatis dibuat dengan kode unik
6. Copy kode → bagikan ke user
```

#### Monitor Voucher Usage
```
1. Di tabel voucher, lihat:
   - Kode voucher
   - Nominal
   - Status (ACTIVE/USED)
   - Siapa yang pakai
   - Kapan dipakai
2. Filter: Semua / Aktif / Terpakai
3. Delete voucher ACTIVE jika tidak terpakai
```

### User Workflow

#### Redeem Voucher
```
1. Buka index-with-topup.html
2. Klik "Top-Up Saldo"
3. Input nomor WA (yang sudah terdaftar)
4. Input kode voucher dari admin
5. Klik "Redeem Voucher"
6. Saldo langsung bertambah
7. Lihat riwayat top-up
```

---

## Testing Checklist

- [ ] Anonymous Auth enabled
- [ ] Database Rules published (dengan vouchers & top_ups)
- [ ] Buka index-with-topup.html (bukan index.html)
- [ ] Admin bisa login
- [ ] Admin bisa generate voucher
- [ ] Lihat voucher di tabel (status ACTIVE)
- [ ] User bisa redeem voucher
- [ ] Saldo user bertambah
- [ ] Voucher status berubah jadi USED
- [ ] Riwayat top-up tersimpan di /top_ups

---

## Dokumentasi Lengkap

Untuk info lebih detail tentang fitur Top-Up:
👉 **[Baca: docs/TOPUP_VOUCHER_GUIDE.md](TOPUP_VOUCHER_GUIDE.md)**

Untuk panduan pilih versi website:
👉 **[Baca: FILE_VERSIONS.md](../FILE_VERSIONS.md)**

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
│
├── users/
│   ├── A1B2C3D4/
│   │   ├── nama: "Budi Santoso"
│   │   ├── password: "123"
│   │   ├── saldo: 47000
│   │   ├── saldo_updated_at: 1710002500
│   │   ├── status_terakhir: "KELUAR"
│   │   ├── timestamp_masuk: 0
│   │   └── wa: "08123456789"
│   ├── E5F6G7H8/
│   │   ... (data Siti Aminah)
│   └── 99887766/
│       ... (data Dedi Corbuzier)
│
├── vouchers/  🆕 (NEW - untuk Top-Up)
│   ├── V1704998400_abc12/
│   │   ├── nominal: 50000
│   │   ├── kode: "PARKV-AB12-CD34-EF56"
│   │   ├── status: "ACTIVE"
│   │   ├── created_by: "admin"
│   │   ├── created_at: 1704998400
│   │   ├── catatan: "Promo Tahun Baru"
│   │   ├── used_by: null
│   │   ├── used_by_name: null
│   │   └── used_at: null
│   └── V1704998410_def45/
│       ├── nominal: 50000
│       ├── kode: "PARKV-GH78-IJ90-KL12"
│       ├── status: "USED"
│       ├── created_at: 1704998410
│       ├── used_by: "A1B2C3D4"
│       ├── used_by_name: "Budi Santoso"
│       └── used_at: 1704998500
│
├── top_ups/  🆕 (NEW - untuk Tracking)
│   ├── 1704998500_A1B2C3D4/
│   │   ├── uid: "A1B2C3D4"
│   │   ├── nominal: 50000
│   │   ├── voucher_kode: "PARKV-AB12-CD34-EF56"
│   │   ├── timestamp: 1704998500
│   │   └── status: "SUCCESS"
│   └── ...
│
├── system/
│   ├── mode: "NORMAL"
│   └── temp_rfid: ""
│
└── config/
    └── notifications/
        ├── whatsapp/
        │   ├── enabled: false
        │   ├── token: ""
        │   └── url: ""
        └── telegram/
            ├── enabled: false
            ├── token: ""
            └── chatId: ""
```

---

## Troubleshooting

### Error: "Permission Denied" saat Generate Voucher
✅ Solusi:
1. Pastikan **Anonymous Auth** sudah di-enable di Firebase Console
2. Pastikan Firebase Rules sudah di-publish (terutama bagian `vouchers`)
3. Refresh browser (Ctrl+Shift+R)
4. Buka DevTools (F12) dan lihat error detail di Console
5. Pastikan using `index-with-topup.html` untuk fitur voucher

### Error: "Database operation error"
✅ Solusi:
1. Periksa apakah Firebase Rules sudah "Publish"
2. Pastikan rules mengizinkan `.write: true` untuk path yang diakses
3. Cek apakah path sudah ada di rules (jangan ada typo)

### Data tidak update real-time
✅ Solusi:
1. Refresh browser (Ctrl+F5)
2. Buka DevTools (F12) dan lihat apakah ada error
3. Periksa Firebase Rules masih aktif
4. Cek internet connection

### Tidak bisa register user
✅ Solusi:
1. Pastikan `system/mode` bisa diubah di rules
2. Cek apakah data sudah terwrite ke `users/{uid}`
3. Verify anonymous auth sudah enabled

### Voucher tidak muncul saat redeem
✅ Solusi:
1. Pastikan Firebase Rules sudah include `vouchers` path
2. Periksa database apakah voucher sudah ada
3. Cek kode voucher case-sensitive atau tidak
4. Lihat console browser untuk error message
5. Pastikan status voucher = "ACTIVE"

### Top-Up gagal, saldo tidak bertambah
✅ Solusi:
1. Cek database rules untuk `/top_ups` path
2. Pastikan user sudah terdaftar di `/users`
3. Lihat Firebase Console → Realtime Database untuk error
4. Check browser console (F12) untuk detail error
5. Verify voucher status = "ACTIVE"
