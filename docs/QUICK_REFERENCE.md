# 📋 QUICK REFERENCE CARD - SISTER PARKIR v2.0

## 🔑 Login Credentials

### Admin Login
```
Username: admin
Password: admin123
```
⚠️ **GANTI PASSWORD SEBELUM PRODUCTION!**

---

## 🚀 Quick Actions

### Login Admin
```
1. Klik tombol "Admin" (kuning) di navbar
2. Input: admin / admin123
3. Masuk ke Dashboard
```

### Setup Notifikasi WhatsApp (Fonnte)
```
1. Daftar https://fonnte.com
2. Beli paket → Dapatkan API Token
3. Login Admin → Konfigurasi Notifikasi
4. WhatsApp Tab:
   - Token: [dari Fonnte]
   - URL: https://api.fonnte.com/send
   - ✅ Aktifkan
5. Simpan
```

### Cari Data Per User
```
1. Login Admin
2. Klik "Cari Per User"
3. Input No. WA: 08123456789
4. Klik "Cari"
5. Lihat riwayat lengkap
```

---

## 📊 Menu Admin

| Menu | Fungsi |
|------|--------|
| **Dashboard** | Statistik (Total Users, Transaksi, Parkir) |
| **Kelola Pengguna** | Lihat semua user + saldo |
| **Semua Perjalanan** | Monitor semua aktivitas |
| **Cari Per User** | Riwayat berdasarkan No. WA |
| **Konfigurasi Notifikasi** | Setup WA/Telegram API |

---

## 🔔 Format Notifikasi

### Template Pesan (WhatsApp)
```
Halo *{nama}*!
Anda telah keluar dari parkir pada {waktu}.
Tarif: Rp {biaya}
Sisa saldo: Rp {saldo}

Terima kasih! - SISTER PARKIR
```

### Trigger
- **Automatic** saat user tap kartu di Gate KELUAR
- ESP32 kirim log dengan `status: "Check-Out"`
- Web app detect → kirim notifikasi

---

## 🗂️ Database Structure

```
/users/{uid}/
  - nama: string
  - wa: string (⭐ untuk notifikasi)
  - password: string
  - saldo: number
  - status_terakhir: "MASUK" | "KELUAR" | "BARU"

/logs/{timestamp}/
  - timestamp: number
  - waktu: "HH:MM:SS"
  - nama: string
  - uid: string
  - wa: string (⭐ WAJIB)
  - gate: "MASUK" | "KELUAR"
  - status: "Check-In" | "Check-Out"
  - biaya: number

/config/notifications/
  - whatsapp/
    - enabled: boolean
    - token: string
    - url: string
  - telegram/
    - enabled: boolean
    - token: string
    - chatId: string
```

---

## ⚡ Shortcuts

| Aksi | Shortcut |
|------|----------|
| Buka Console | F12 |
| Refresh Page | F5 |
| Hard Refresh | Ctrl + F5 |
| Clear Cache | Ctrl + Shift + Delete |

---

## 🐛 Quick Troubleshooting

### Notifikasi Tidak Terkirim
```
✅ Cek: Config tersimpan?
✅ Cek: Token benar?
✅ Cek: Balance API cukup?
✅ Cek: ESP32 kirim field "wa"?
```

### Admin Tidak Bisa Login
```
✅ Cek: Username = "admin" (lowercase)
✅ Cek: Password = "admin123"
✅ Cek: Caps Lock off
```

### Data Tidak Muncul
```
✅ Hard refresh: Ctrl + F5
✅ Cek Firebase Console
✅ Cek Firebase Rules
```

---

## 📞 API Endpoints

### WhatsApp (Fonnte)
```http
POST https://api.fonnte.com/send
Headers:
  Authorization: YOUR_TOKEN
Body:
{
  "target": "08123456789",
  "message": "Pesan Anda"
}
```

### Telegram
```http
POST https://api.telegram.org/bot[TOKEN]/sendMessage
Body:
{
  "chat_id": "123456789",
  "text": "Pesan Anda",
  "parse_mode": "Markdown"
}
```

---

## 🔧 Common Commands

### Firebase Query (Console Browser)
```javascript
// Lihat semua users
firebase.database().ref('users').once('value').then(s => console.log(s.val()))

// Lihat semua logs
firebase.database().ref('logs').once('value').then(s => console.log(s.val()))

// Cek config notifikasi
firebase.database().ref('config/notifications').once('value').then(s => console.log(s.val()))
```

---

## 📚 Documentation Files

| File | Baca Jika... |
|------|--------------|
| **UPDATE_SUMMARY.md** | Ingin tahu apa yang baru |
| **ADMIN_GUIDE.md** | Setup admin & notifikasi |
| **ESP32_GUIDE.md** | Integrasi ESP32 |
| **START_HERE.md** | Baru pertama kali |
| **TROUBLESHOOTING.md** | Ada error |

---

## ✅ Pre-Production Checklist

```
Setup:
□ Login admin berhasil
□ Ganti password admin
□ Setup Firebase Rules

Notifikasi:
□ Daftar Fonnte/provider
□ Konfigurasi WA API
□ Test kirim notifikasi
□ Verifikasi user terima pesan

Testing:
□ Registrasi user baru
□ Test check-in
□ Test check-out + notifikasi
□ Cari per user
□ Semua menu admin

ESP32:
□ Upload code
□ WiFi connect
□ RFID detect
□ Field "wa" terkirim di log
```

---

## 🎯 Common Tasks

### Task 1: Tambah User Baru
```
1. Buka app (non-admin)
2. Klik "Registrasi User"
3. Isi form (Nama, WA, Password)
4. Klik "Lanjut Scan Kartu"
5. Tap kartu RFID
6. ✅ User terdaftar
```

### Task 2: Lihat Riwayat User
```
1. Login admin
2. Cari Per User
3. Input No. WA user
4. Lihat semua riwayat
```

### Task 3: Monitoring Real-Time
```
1. Login admin
2. Dashboard → Lihat statistik
3. Atau "Semua Perjalanan" untuk detail
```

---

## 💾 Backup & Recovery

### Backup Database
```
1. Firebase Console
2. Realtime Database
3. ⋮ Menu → Export JSON
4. Save file
```

### Restore Database
```
1. Firebase Console
2. Realtime Database
3. ⋮ Menu → Import JSON
4. Pilih backup file
```

---

## 🆘 Emergency Contacts

### Firebase Console
https://console.firebase.google.com/project/siste-parkir-iot

### Fonnte Dashboard
https://fonnte.com/dashboard

### Telegram BotFather
@BotFather (di Telegram)

---

## 🔒 Security Notes

```
❗ JANGAN commit API keys ke GitHub
❗ GANTI password admin default
❗ Setup Firebase Rules untuk production
❗ Backup database secara berkala
❗ Monitor API usage & costs
```

---

## 📱 Support Resources

| Resource | Link/Info |
|----------|-----------|
| Project Files | File explorer workspace |
| Firebase Console | console.firebase.google.com |
| Fonnte Docs | fonnte.com/docs |
| Telegram Bot API | core.telegram.org/bots/api |
| ESP32 Docs | docs.espressif.com |

---

**Version:** 2.0
**Last Updated:** 26 Dec 2025
**Print this for quick reference!** 📋
