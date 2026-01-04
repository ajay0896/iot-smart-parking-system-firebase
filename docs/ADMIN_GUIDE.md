# 🎉 SISTER PARKIR IoT - Update v2.0

## ✨ Fitur Baru yang Ditambahkan

### 1. ✅ Tombol "Initialize Data" DIHAPUS
Tombol untuk populate database sudah dihapus sesuai permintaan.

---

## 🔐 HALAMAN ADMIN

### Login Admin
**URL:** Klik tombol "Admin" di navbar

**Kredensial Default:**
- Username: `admin`
- Password: `admin123`

⚠️ **PENTING:** Ganti password ini di production!

---

## 📊 Fitur Admin Dashboard

### 1. **Dashboard Statistik**
Menampilkan:
- Total Users terdaftar
- Total Transaksi
- Kendaraan yang sedang parkir

### 2. **Kelola Pengguna**
- Lihat semua user terdaftar
- Informasi: UID, Nama, No. WA, Saldo, Status
- Tombol "Lihat" untuk melihat riwayat per user

### 3. **Semua Data Perjalanan**
- Monitor semua aktivitas masuk/keluar
- Terurut dari terbaru
- Tampilkan: Waktu, Nama, No. WA, Gate, Status, Tarif

### 4. **Cari Per User (Berdasarkan No. WA)** ⭐
Fitur untuk monitoring per user:
1. Input nomor WhatsApp user
2. Klik "Cari"
3. Tampil:
   - Informasi user (Nama, UID, Saldo, Status)
   - Riwayat perjalanan user tersebut
   - Total transaksi

**Cara Pakai:**
```
1. Login sebagai admin
2. Klik "Cari Per User"
3. Masukkan No. WA: 08123456789
4. Klik "Cari"
5. Lihat riwayat lengkap user
```

### 5. **Konfigurasi Notifikasi** ⭐

#### WhatsApp Notification
Fitur kirim notifikasi WA saat user keluar parkir:

**Setup:**
1. Login admin → Klik "Konfigurasi Notifikasi"
2. Pilih tab WhatsApp
3. Isi:
   - **API Token:** Token dari provider (Fonnte/Wablas/WA Business)
   - **API URL:** Endpoint API provider
   - Centang "Aktifkan notifikasi WhatsApp"
4. Klik "Simpan Konfigurasi"

**Provider yang Direkomendasikan:**
- **Fonnte:** https://fonnte.com (Mudah, murah)
- **Wablas:** https://wablas.com
- **WA Business API:** https://business.whatsapp.com

**Contoh Integration (Fonnte):**
```
API URL: https://api.fonnte.com/send
API Token: [Dapatkan dari dashboard Fonnte]
```

#### Telegram Notification
Alternatif kirim notifikasi via Telegram:

**Setup:**
1. Buat bot di Telegram via @BotFather
2. Dapatkan Bot Token
3. Dapatkan Chat ID (bisa pakai @userinfobot)
4. Isi konfigurasi:
   - **Bot Token:** Token dari BotFather
   - **Chat ID:** ID user/group yang akan menerima notif
   - Centang "Aktifkan notifikasi Telegram"
5. Klik "Simpan Konfigurasi"

**Template Pesan yang Dikirim:**
```
Halo [Nama User]!
Anda telah keluar dari parkir pada [Waktu].
Tarif: Rp [Biaya]
Sisa saldo: Rp [Saldo]

Terima kasih! - SISTER PARKIR
```

---

## 🔔 Cara Kerja Notifikasi

### Automatic Trigger
Notifikasi akan **otomatis terkirim** saat:
1. User tap kartu di **Gate KELUAR**
2. ESP32 mengirim log dengan `status: "Check-Out"`
3. Sistem membaca konfigurasi notifikasi
4. Jika enabled, kirim pesan ke nomor WA user

### Flow Diagram
```
User Tap Kartu (Gate Keluar)
        ↓
ESP32 kirim data ke Firebase
        ↓
Web app detect log baru (status: Check-Out)
        ↓
Ambil data user (nama, wa, saldo)
        ↓
Cek config notifikasi
        ↓
Kirim pesan via WhatsApp/Telegram API
        ↓
User terima notifikasi ✅
```

---

## 📱 Format Notifikasi

### WhatsApp (dengan formatting)
```
Halo *Budi Santoso*!
Anda telah keluar dari parkir pada 10:30:00.
Tarif: Rp 3.000
Sisa saldo: Rp 47.000

Terima kasih! - SISTER PARKIR
```

### Telegram (dengan emoji)
```
🚗 *SISTER PARKIR*

Halo Budi Santoso!
Keluar: 10:30:00
Tarif: Rp 3.000
Saldo: Rp 47.000
```

---

## 🎯 Use Cases

### Use Case 1: Admin Monitoring User Tertentu
**Scenario:** Admin ingin lihat riwayat parkir "Budi Santoso"

**Langkah:**
1. Login admin
2. Klik "Cari Per User"
3. Input No. WA Budi: `08123456789`
4. Klik "Cari"
5. Lihat semua riwayat perjalanan Budi

### Use Case 2: Setup Notifikasi WhatsApp
**Scenario:** Admin ingin user otomatis dapat notif WA saat keluar

**Langkah:**
1. Daftar akun di Fonnte.com
2. Dapatkan API Token
3. Login admin → Konfigurasi Notifikasi
4. Isi token & URL Fonnte
5. Aktifkan checkbox
6. Simpan
7. Test: Tap kartu keluar → User terima WA

### Use Case 3: Lihat Siapa Saja yang Masih Parkir
**Langkah:**
1. Login admin
2. Dashboard menampilkan "Kendaraan Parkir: X"
3. Klik "Kelola Pengguna"
4. Filter yang statusnya "MASUK"

---

## 🔧 Setup Guide Lengkap

### A. Setup WhatsApp Notification (Fonnte)

1. **Daftar Fonnte**
   - Kunjungi https://fonnte.com
   - Register akun
   - Beli paket (mulai dari Rp 100.000)

2. **Dapatkan Token**
   - Login dashboard Fonnte
   - Copy API Token

3. **Konfigurasi di Admin Panel**
   ```
   API Token: xxxxx-yyyy-zzzz
   API URL: https://api.fonnte.com/send
   ✅ Aktifkan notifikasi WhatsApp
   ```

4. **Testing**
   - Tap kartu keluar di gate
   - Cek WA user, seharusnya terima pesan

### B. Setup Telegram Bot

1. **Buat Bot**
   - Buka Telegram, cari @BotFather
   - Ketik `/newbot`
   - Ikuti instruksi
   - Copy Bot Token

2. **Dapatkan Chat ID**
   - Cari bot Anda di Telegram
   - Kirim pesan `/start`
   - Buka https://api.telegram.org/bot[TOKEN]/getUpdates
   - Copy `chat.id`

3. **Konfigurasi di Admin Panel**
   ```
   Bot Token: 123456:ABCdefGHIjklMNOpqrsTUVwxyz
   Chat ID: 123456789
   ✅ Aktifkan notifikasi Telegram
   ```

---

## 🚀 Quick Start

### Untuk Admin:
1. Buka aplikasi
2. Klik tombol "Admin" (kuning)
3. Login: `admin` / `admin123`
4. Explore dashboard!

### Untuk User (tetap sama):
1. Registrasi User
2. Data Perjalanan (Live Monitoring)

---

## 🔐 Keamanan

### Admin Password
**WAJIB GANTI PASSWORD!**

Edit di line ~615 file `index.html`:
```javascript
if (adminForm.value.username === 'admin' && adminForm.value.password === 'admin123') {
```

Ganti `admin123` dengan password kuat Anda.

### Firebase Rules
Tambahkan rules untuk admin:
```json
{
  "rules": {
    "config": {
      ".read": "auth != null",
      ".write": "auth != null"
    },
    "users": {
      ".read": true,
      ".write": true
    },
    "logs": {
      ".read": true,
      ".write": true
    },
    "system": {
      ".read": true,
      ".write": true
    }
  }
}
```

---

## 📋 Checklist Implementasi

### Setup Awal
- [x] Hapus tombol "Initialize Data"
- [x] Buat halaman login admin
- [x] Buat dashboard admin
- [x] Buat fitur kelola users
- [x] Buat fitur cari per user (by No. WA)
- [x] Buat konfigurasi notifikasi

### Yang Perlu Dilakukan User
- [ ] Login sebagai admin
- [ ] Ganti password default
- [ ] Setup WhatsApp API (daftar Fonnte)
- [ ] Konfigurasi notifikasi
- [ ] Test kirim notifikasi
- [ ] Setup Firebase Rules (optional)

---

## 💡 Tips & Tricks

### Tip 1: Testing Notifikasi
Gunakan nomor WA Anda sendiri untuk testing sebelum production.

### Tip 2: Multiple Admins
Edit fungsi `loginAdmin()` untuk menambah admin lain:
```javascript
const admins = [
    { username: 'admin', password: 'admin123' },
    { username: 'superadmin', password: 'super456' }
];
```

### Tip 3: Custom Template Pesan
Edit fungsi `sendWhatsAppNotif()` dan `sendTelegramNotif()` untuk customize pesan.

### Tip 4: Batasi Akses Public
Jika ingin "Data Perjalanan" publik hanya bisa diakses admin, tambahkan kondisi:
```javascript
<div v-if="view === 'logs' && !isAdminLoggedIn">
    <p class="alert alert-warning">Hanya admin yang bisa melihat data ini</p>
</div>
```

---

## 🐛 Troubleshooting

### Notifikasi Tidak Terkirim
**Kemungkinan:**
1. Config belum disimpan → Simpan ulang
2. API Token salah → Cek di dashboard provider
3. Nomor WA salah format → Pastikan 08xxx atau 628xxx
4. Balance API habis → Top up di provider

**Fix:**
- Buka Console browser (F12)
- Cek error message
- Verifikasi API di Postman dulu

### Admin Tidak Bisa Login
**Kemungkinan:**
1. Password typo → Coba lagi pelan-pelan
2. Caps Lock aktif

**Fix:**
- Pastikan `admin` / `admin123` (lowercase semua)

### Data User Tidak Muncul di Search
**Kemungkinan:**
1. Nomor WA tidak match → Cek di database Firebase
2. Format WA berbeda (08xxx vs 628xxx)

**Fix:**
- Pastikan format WA konsisten
- Cek di "Kelola Pengguna" dulu

---

## 📞 API Documentation

### WhatsApp (Fonnte)
```javascript
POST https://api.fonnte.com/send
Headers:
  Authorization: YOUR_TOKEN
Body:
{
  "target": "08123456789",
  "message": "Your message here"
}
```

### Telegram
```javascript
POST https://api.telegram.org/bot[TOKEN]/sendMessage
Body:
{
  "chat_id": "123456789",
  "text": "Your message",
  "parse_mode": "Markdown"
}
```

---

## 🎨 Customization

### Ganti Warna Admin Dashboard
Edit CSS di `<style>`:
```css
.stat-card { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); }
.stat-card-success { background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); }
.stat-card-warning { background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%); }
```

### Tambah Statistik Baru
Contoh: Total pendapatan hari ini
```javascript
const totalRevenue = computed(() => {
    return logs.value
        .filter(log => log.status === 'Check-Out')
        .reduce((sum, log) => sum + (log.biaya || 0), 0);
});
```

---

## 🚀 Fitur Masa Depan (Roadmap)

- [ ] Export data ke Excel/PDF
- [ ] Grafik statistik (Chart.js)
- [ ] Multi-language support
- [ ] Email notification
- [ ] SMS notification
- [ ] Top-up saldo via payment gateway
- [ ] Laporan bulanan otomatis

---

## 📝 Changelog

### Version 2.0 (26 Dec 2025)
- ✅ Hapus tombol "Initialize Data"
- ✅ Tambah halaman admin dengan login
- ✅ Dashboard admin dengan statistik
- ✅ Kelola pengguna
- ✅ Cari data per user (by No. WA)
- ✅ Konfigurasi notifikasi WhatsApp/Telegram
- ✅ Auto-send notifikasi saat checkout

### Version 1.0
- Registrasi user
- Live monitoring
- Data perjalanan realtime

---

**Status:** ✅ READY FOR PRODUCTION
**Dokumentasi Lengkap:** File ini
**Support:** Baca troubleshooting di atas

Selamat menggunakan SISTER PARKIR IoT v2.0! 🎉
