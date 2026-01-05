# ⚠️ PENTING: Pilihan Versi Website

Project ini memiliki **2 versi file HTML** yang dapat Anda pilih:

## 📁 Pilihan File

### 1️⃣ **index.html** (Versi Asli)
```
✅ Fitur lengkap tanpa top-up voucher
✅ Lebih ringan dan sederhana
✅ Cocok untuk production standard
```

**Fitur:**
- Registrasi user
- Check-in/Check-out parkir
- Dashboard admin
- Monitoring real-time
- Konfigurasi notifikasi

---

### 2️⃣ **index-with-topup.html** (Versi dengan Top-Up)
```
✅ Semua fitur dari index.html
✅ Plus fitur top-up saldo via voucher
✅ Sistem voucher lengkap (generate, redeem, history)
```

**Fitur Tambahan:**
- Admin dapat generate voucher
- User dapat redeem voucher
- Tracking penggunaan voucher
- Riwayat top-up

---

## 🚀 Cara Menggunakan

### **Gunakan index.html jika:**
- ❌ Tidak perlu fitur top-up
- ❌ Ingin sistem yang lebih simple
- ❌ Tidak ingin repot kelola voucher

### **Gunakan index-with-topup.html jika:**
- ✅ Perlu fitur top-up saldo
- ✅ Ingin buat promo dengan voucher
- ✅ Perlu tracking pengisian saldo user

---

## 📝 Setup

### **Opsi 1: Gunakan index.html (Default)**
```bash
# Langsung buka file:
public/index.html

# Atau deploy file index.html ke hosting
```

### **Opsi 2: Gunakan index-with-topup.html**
```bash
# Cara 1: Rename file
mv index-with-topup.html index.html

# Cara 2: Buka langsung
# Buka file index-with-topup.html di browser

# Cara 3: Deploy dengan nama berbeda
# Upload index-with-topup.html sebagai index.html di hosting
```

---

## 🗂️ Database

### **Untuk index.html:**
Database struktur standar:
```
/users/{uid}
/logs/{timestamp}
/config/notifications
```

### **Untuk index-with-topup.html:**
Database struktur + tabel voucher:
```
/users/{uid}
/logs/{timestamp}
/config/notifications
/vouchers/{voucherId}       ← BARU
/top_ups/{timestamp}_{uid}  ← BARU
```

**CATATAN PENTING:**
- Kedua versi menggunakan database yang sama
- Jika pakai index-with-topup.html, tabel `/vouchers` dan `/top_ups` akan otomatis dibuat
- Jika kembali ke index.html, tabel voucher tetap ada (tidak masalah)

---

## 📚 Dokumentasi

| File | Dokumentasi |
|------|-------------|
| `index.html` | [README.md](README.md) |
| `index-with-topup.html` | [docs/TOPUP_VOUCHER_GUIDE.md](docs/TOPUP_VOUCHER_GUIDE.md) |

---

## 🔄 Cara Pindah Versi

### **Dari index.html → index-with-topup.html:**
```bash
1. Backup database dulu (export dari Firebase Console)
2. Ganti file index.html dengan index-with-topup.html
3. Refresh browser
4. Fitur voucher langsung aktif
5. Admin bisa langsung generate voucher
```

### **Dari index-with-topup.html → index.html:**
```bash
1. Backup database dulu
2. Ganti file ke index.html yang asli
3. Refresh browser
4. Fitur voucher hilang, tapi data tetap aman di database
5. Jika suatu saat mau aktifkan lagi, tinggal ganti balik
```

---

## 🎯 Rekomendasi

**Untuk Testing/Development:**
→ Gunakan `index-with-topup.html`
- Bisa test semua fitur
- Bisa eksperimen dengan voucher
- Lebih complete

**Untuk Production Simple:**
→ Gunakan `index.html`
- Lebih fokus ke fitur inti
- Tidak perlu maintain voucher
- Lebih ringan

**Untuk Production Full-Featured:**
→ Gunakan `index-with-topup.html`
- Fitur lengkap
- User bisa top-up sendiri
- Professional

---

## ⚙️ Konfigurasi Firebase

**Keduanya menggunakan Firebase config yang sama:**
```javascript
// Di kedua file, edit bagian ini:
const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    databaseURL: "YOUR_DATABASE_URL",
    // ... config lainnya
};
```

**Database Rules:**
```json
{
  "rules": {
    ".read": "auth != null",
    ".write": "auth != null",
    "vouchers": {
      ".indexOn": ["kode", "status"]
    },
    "top_ups": {
      ".indexOn": ["uid", "timestamp"]
    }
  }
}
```

---

## 🔒 Keamanan

**index.html:**
- Firebase Anonymous Auth
- Database rules standard

**index-with-topup.html:**
- Firebase Anonymous Auth
- Database rules standard + voucher validation
- Atomic transaction untuk redeem voucher
- Prevent double redeem

---

## 📞 Support

Pertanyaan tentang pemilihan versi?
- 📧 Email: fajarramdani@example.com
- 📱 WhatsApp: +62 896-6395-6717
- 📖 Dokumentasi: [docs/](docs/)

---

<p align="center">
  <b>Pilih versi yang sesuai kebutuhan Anda!</b><br>
  Keduanya aman dan sudah tested ✅
</p>
