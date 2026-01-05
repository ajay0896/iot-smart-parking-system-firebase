# 🎫 Panduan Fitur Top-Up Voucher

> Dokumentasi lengkap untuk fitur top-up saldo menggunakan sistem voucher

## 📋 Daftar Isi
- [Pengenalan](#pengenalan)
- [Cara Menggunakan](#cara-menggunakan)
- [Database Structure](#database-structure)
- [FAQ & Troubleshooting](#faq--troubleshooting)

---

## 🎯 Pengenalan

Sistem parkir IoT ini memiliki **2 versi file**:

| File | Deskripsi | Fitur Top-Up |
|------|-----------|--------------|
| **index.html** | Versi asli | ❌ Tidak ada |
| **index-with-topup.html** | Versi dengan top-up | ✅ Ada |

**Anda bebas memilih** file mana yang ingin digunakan:
- Jika tidak butuh fitur top-up → Gunakan `index.html`
- Jika butuh fitur top-up → Gunakan `index-with-topup.html`

---

## 🚀 Cara Menggunakan

### **A. Untuk Admin - Generate Voucher**

#### **Langkah 1: Login Admin**
```
1. Buka index-with-topup.html
2. Klik tombol "Admin" di kanan atas
3. Login:
   - Username: admin
   - Password: admin123
```

#### **Langkah 2: Buka Menu Voucher**
```
1. Setelah login, di Dashboard Admin
2. Klik card "Kelola Voucher"
3. Anda akan masuk ke halaman voucher management
```

#### **Langkah 3: Generate Voucher**
```
┌─────────────────────────────────────┐
│  GENERATE VOUCHER BARU             │
│                                     │
│  Nominal (Rp):    [▼ Rp 50.000]   │
│  Jumlah Voucher:  [____1____]      │
│  Catatan:         [____________]    │
│                                     │
│  [Generate 1 Voucher]               │
└─────────────────────────────────────┘

1. Pilih nominal: Rp 10.000, 25.000, 50.000, 100.000, atau 200.000
2. Tentukan jumlah voucher yang ingin dibuat (1-50)
3. Isi catatan (opsional), misal: "Promo Tahun Baru"
4. Klik "Generate X Voucher"
5. Sistem akan membuat voucher dengan kode unik
```

#### **Langkah 4: Lihat & Copy Kode Voucher**
```
Tabel Voucher akan menampilkan:
┌────────────────────────────────────────────────────────┐
│ Kode Voucher           │ Nominal  │ Status  │ Aksi   │
├────────────────────────────────────────────────────────┤
│ PARKV-AB12-CD34-EF56  │ Rp 50.000│ ACTIVE  │ [Copy] │
│ PARKV-GH78-IJ90-KL12  │ Rp 25.000│ ACTIVE  │ [Copy] │
└────────────────────────────────────────────────────────┘

1. Klik tombol "Copy" untuk copy kode voucher
2. Bagikan kode tersebut ke user (via WA/Email/Print)
```

#### **Langkah 5: Filter Voucher**
```
Tombol filter di atas tabel:
[Semua] [Aktif] [Terpakai]

- Semua: Tampilkan semua voucher
- Aktif: Hanya voucher yang belum digunakan
- Terpakai: Hanya voucher yang sudah digunakan
```

#### **Langkah 6: Delete Voucher (Opsional)**
```
1. Voucher dengan status ACTIVE bisa dihapus
2. Klik tombol [🗑️] di kolom aksi
3. Konfirmasi hapus
4. Voucher USED tidak bisa dihapus (untuk audit trail)
```

---

### **B. Untuk User - Redeem Voucher**

#### **Langkah 1: Akses Menu Top-Up**
```
1. Buka index-with-topup.html
2. Di halaman home, klik card "Top-Up Saldo"
3. Masukkan nomor WhatsApp Anda (yang sudah terdaftar)
4. Klik OK
```

#### **Langkah 2: Cek Saldo Saat Ini**
```
Halaman Top-Up akan menampilkan:
┌─────────────────────────────────────┐
│  INFORMASI USER                    │
│                                     │
│  No. WhatsApp: 089663956717        │
│  Nama: Fajar Ramdani               │
│  Saldo Saat Ini: Rp 47.000         │
└─────────────────────────────────────┘
```

#### **Langkah 3: Input Kode Voucher**
```
┌─────────────────────────────────────┐
│  REDEEM VOUCHER                    │
│                                     │
│  Masukkan Kode Voucher:            │
│  [_____________________]           │
│                                     │
│  Contoh: PARKV-AB12-CD34-EF56      │
│                                     │
│  [Redeem Voucher]                  │
└─────────────────────────────────────┘

1. Ketik/paste kode voucher yang didapat dari admin
2. Format: PARKV-XXXX-XXXX-XXXX
3. Tidak case-sensitive (huruf besar/kecil sama saja)
4. Klik "Redeem Voucher"
```

#### **Langkah 4: Konfirmasi Berhasil**
```
Jika berhasil, akan muncul:
┌─────────────────────────────────────┐
│  ✓ TOP-UP BERHASIL!                │
│                                     │
│  Nominal: Rp 50.000                │
│  Saldo Baru: Rp 97.000             │
│  Waktu: 12:30:45                   │
│                                     │
│  [OK]                               │
└─────────────────────────────────────┘

Saldo Anda langsung bertambah!
```

#### **Langkah 5: Lihat Riwayat Top-Up**
```
Di bawah form redeem, ada tabel riwayat:
┌────────────────────────────────────────────────────────┐
│ Tanggal & Waktu │ Kode Voucher        │ Nominal       │
├────────────────────────────────────────────────────────┤
│ 04/01/2026      │ PARKV-AB12-CD34-EF56│ + Rp 50.000  │
│ 03/01/2026      │ PARKV-GH78-IJ90-KL12│ + Rp 25.000  │
└────────────────────────────────────────────────────────┘

Semua top-up yang pernah Anda lakukan tercatat di sini.
```

---

## 🗂️ Database Structure

### **A. Tabel Vouchers (`/vouchers/{voucherId}`)**
```json
{
  "vouchers": {
    "V1704998400_abc12": {
      "nominal": 50000,
      "kode": "PARKV-AB12-CD34-EF56",
      "status": "ACTIVE",
      "created_by": "admin",
      "created_at": 1704998400,
      "catatan": "Promo Tahun Baru",
      "used_by": null,
      "used_by_name": null,
      "used_at": null
    },
    "V1704998410_def45": {
      "nominal": 50000,
      "kode": "PARKV-GH78-IJ90-KL12",
      "status": "USED",
      "created_by": "admin",
      "created_at": 1704998410,
      "catatan": "Promo Tahun Baru",
      "used_by": "uid_user_123",
      "used_by_name": "Fajar Ramdani",
      "used_at": 1704998500
    }
  }
}
```

**Field Explanation:**
| Field | Type | Deskripsi |
|-------|------|-----------|
| `nominal` | number | Nilai voucher (Rupiah) |
| `kode` | string | Kode unik voucher |
| `status` | string | "ACTIVE" atau "USED" |
| `created_by` | string | Admin yang membuat |
| `created_at` | timestamp | Waktu dibuat |
| `catatan` | string | Catatan voucher |
| `used_by` | string/null | UID user yang pakai |
| `used_by_name` | string/null | Nama user yang pakai |
| `used_at` | timestamp/null | Waktu digunakan |

---

### **B. Tabel Top-Ups (`/top_ups/{timestamp}_{uid}`)**
```json
{
  "top_ups": {
    "1704998500_uid_user_123": {
      "uid": "uid_user_123",
      "nominal": 50000,
      "voucher_kode": "PARKV-AB12-CD34-EF56",
      "timestamp": 1704998500,
      "status": "SUCCESS"
    }
  }
}
```

**Field Explanation:**
| Field | Type | Deskripsi |
|-------|------|-----------|
| `uid` | string | User ID yang top-up |
| `nominal` | number | Nominal yang di-top-up |
| `voucher_kode` | string | Kode voucher yang digunakan |
| `timestamp` | number | Waktu top-up |
| `status` | string | Status (selalu "SUCCESS") |

---

### **C. Update Tabel Users (`/users/{uid}`)**
```json
{
  "users": {
    "uid_user_123": {
      "nama": "Fajar Ramdani",
      "wa": "089663956717",
      "pass": "password123",
      "saldo": 97000,
      "saldo_updated_at": 1704998500,
      "status_terakhir": "KELUAR",
      "timestamp_masuk": 0
    }
  }
}
```

**Field Baru:**
| Field | Type | Deskripsi |
|-------|------|-----------|
| `saldo_updated_at` | timestamp | Waktu terakhir saldo diupdate |

---

## 🔒 Validasi & Keamanan

### **Validasi saat Redeem:**
```javascript
✅ Kode voucher harus ada di database
✅ Status voucher harus "ACTIVE"
✅ Voucher belum pernah digunakan
✅ User harus sudah terdaftar
✅ Transaksi atomic (Firebase transaction)
```

### **Proteksi:**
```javascript
✅ Voucher USED tidak bisa dihapus (audit trail)
✅ Satu voucher hanya bisa dipakai 1x
✅ Semua attempt tercatat di database
✅ Rate limiting (maksimal 5 attempt per menit)
```

---

## 📊 Flow Diagram

### **Admin Generate Voucher:**
```
[Admin Login] 
    → [Dashboard Admin] 
    → [Kelola Voucher]
    → [Input Nominal & Jumlah]
    → [Generate]
    → [Voucher Dibuat di Firebase]
    → [Copy Kode]
    → [Bagikan ke User]
```

### **User Redeem Voucher:**
```
[User] 
    → [Klik Top-Up Saldo]
    → [Input No. WA]
    → [Lihat Saldo Saat Ini]
    → [Input Kode Voucher]
    → [Klik Redeem]
    → [Sistem Validasi]
    → [Saldo Bertambah]
    → [Voucher Status → USED]
    → [Save History]
    → [Sukses!]
```

---

## ❓ FAQ & Troubleshooting

### **Q1: Kode voucher tidak valid?**
```
Penyebab:
✗ Salah ketik kode voucher
✗ Voucher sudah digunakan
✗ Voucher sudah dihapus admin

Solusi:
✓ Periksa kembali kode (copy-paste lebih aman)
✓ Tanya admin untuk voucher baru
✓ Cek riwayat top-up, mungkin sudah berhasil sebelumnya
```

### **Q2: Nomor WA tidak terdaftar?**
```
Penyebab:
✗ Belum registrasi user
✗ Salah input nomor WA

Solusi:
✓ Daftar dulu via menu "Registrasi User"
✓ Pastikan nomor WA sama dengan saat registrasi
✓ Format: 08xxxxxxxxx (tanpa +62)
```

### **Q3: Saldo tidak bertambah setelah redeem?**
```
Penyebab:
✗ Koneksi internet terputus
✗ Firebase error
✗ Voucher sudah dipakai orang lain

Solusi:
✓ Refresh browser (F5)
✓ Cek Firebase Console → Realtime Database
✓ Lihat riwayat top-up, apakah sudah tercatat?
✓ Jika masih gagal, hubungi admin
```

### **Q4: Voucher hilang setelah di-generate?**
```
Penyebab:
✗ Admin menghapus voucher
✗ Filter di "Terpakai" (bukan "Aktif")

Solusi:
✓ Ubah filter ke "Semua" atau "Aktif"
✓ Cek Firebase Console → vouchers
✓ Generate ulang jika memang hilang
```

### **Q5: Cara mengganti nominal voucher?**
```
Tidak bisa diubah setelah di-generate!

Solusi:
✓ Hapus voucher lama (jika masih ACTIVE)
✓ Generate voucher baru dengan nominal yang benar
```

### **Q6: Berapa maksimal voucher yang bisa dibuat?**
```
Maksimal 50 voucher per generate
(untuk menghindari overload database)

Tip:
✓ Jika butuh lebih, lakukan generate berulang kali
✓ Atau ubah limit di kode: max="50" → max="100"
```

---

## 🎨 Kustomisasi

### **Menambah Nominal Baru:**

Edit `index-with-topup.html` baris ~727:
```html
<select v-model="voucherForm.nominal" class="form-select" required>
    <option value="10000">Rp 10.000</option>
    <option value="25000">Rp 25.000</option>
    <option value="50000">Rp 50.000</option>
    <option value="100000">Rp 100.000</option>
    <option value="200000">Rp 200.000</option>
    
    <!-- Tambahkan nominal baru di sini -->
    <option value="500000">Rp 500.000</option>
    <option value="1000000">Rp 1.000.000</option>
</select>
```

### **Mengubah Format Kode Voucher:**

Edit `index-with-topup.html` fungsi `generateVoucher`:
```javascript
// Format default: PARKV-XXXX-XXXX-XXXX
const kode = 'PARKV-' + 
             Math.random().toString(36).substr(2, 4).toUpperCase() + '-' +
             Math.random().toString(36).substr(2, 4).toUpperCase() + '-' +
             Math.random().toString(36).substr(2, 4).toUpperCase();

// Ubah ke format lain, misal: SISTER-XXXXXX
const kode = 'SISTER-' + 
             Math.random().toString(36).substr(2, 6).toUpperCase();
```

### **Menambah Expired Date:**

Tambahkan field di `generateVoucher`:
```javascript
await set(ref(db, `vouchers/${voucherId}`), {
    // ... field lainnya ...
    expired_at: Math.floor(Date.now() / 1000) + (30 * 24 * 60 * 60) // 30 hari
});
```

Tambahkan validasi di `redeemVoucher`:
```javascript
if (foundVoucher.expired_at && foundVoucher.expired_at < Math.floor(Date.now() / 1000)) {
    alert('✗ Voucher sudah expired!');
    return;
}
```

---

## 📝 Best Practices

### **Untuk Admin:**
```
✅ Generate voucher sesuai kebutuhan (jangan berlebihan)
✅ Berikan catatan yang jelas (misal: "Promo Agustus 2026")
✅ Simpan record voucher yang dibagikan (di Excel/Google Sheets)
✅ Hapus voucher ACTIVE yang sudah tidak terpakai
✅ Monitor penggunaan voucher secara berkala
```

### **Untuk User:**
```
✅ Simpan kode voucher dengan aman
✅ Redeem voucher segera setelah dapat
✅ Cek saldo sebelum dan sesudah redeem
✅ Screenshot bukti top-up berhasil
✅ Laporkan ke admin jika ada masalah
```

---

## 🔗 Link Terkait

- [README.md](../README.md) - Dokumentasi utama
- [QUICK_START.md](QUICK_START.md) - Panduan cepat
- [ADMIN_GUIDE.md](ADMIN_GUIDE.md) - Panduan admin lengkap
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Troubleshooting umum

---

## 📞 Support

Jika ada pertanyaan atau masalah:
- 📧 Email: fajarramdani@example.com
- 📱 WhatsApp: +62 896-6395-6717
- 🐛 GitHub Issues: [Report Bug](https://github.com/ajay0896/iot-smart-parking-system-firebase/issues)

---

<p align="center">
  <b>Made with ❤️ for Sistem Terdistribusi</b><br>
  © 2026 Fajar Ramdani
</p>
