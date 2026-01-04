# 🔐 FIX: Website Tidak Bisa Ubah Mode

## ❌ MASALAH
Website tidak bisa mengubah mode saat klik "Registrasi User" karena Firebase Rules memblokir akses dari user yang tidak terautentikasi.

## ✅ SOLUSI

### Step 1: Enable Anonymous Authentication (5 menit)

1. **Buka Firebase Console**
   ```
   https://console.firebase.google.com
   ```

2. **Pilih Project**: `siste-parkir-iot`

3. **Klik**: Build → Authentication

4. **Tab**: Sign-in method

5. **Cari**: Anonymous
   - Klik "Anonymous"
   - Toggle: **Enable**
   - Klik: **Save**

### Step 2: Refresh Website (1 menit)

1. **Refresh halaman website** (Ctrl + F5 atau Cmd + Shift + R)

2. **Buka Console Browser** (F12)

3. **Lihat Console**, harus muncul:
   ```
   ✓ Website authenticated to Firebase (Anonymous)
   ```

### Step 3: Test Registrasi (2 menit)

1. **Klik "Registrasi User"**

2. **Isi form**

3. **Klik "Lanjut Scan Kartu"**

4. **Cek Console Browser**, harus muncul:
   ```
   Setting mode to REGISTER...
   ✓ Mode set to REGISTER
   ✓ temp_rfid cleared
   ```

5. **Cek Serial Monitor ESP32**, harus muncul:
   ```
   ╔════════════════════════════════════╗
   ║  MODE CHANGED → REGISTER           ║
   ╚════════════════════════════════════╝
   ```

**Jika semua muncul → BERHASIL!** ✅

---

## 🐛 TROUBLESHOOTING

### Console Browser: "Auth error: ..."

**Solusi:**
1. Pastikan Anonymous Auth sudah di-enable
2. Refresh halaman dengan Ctrl + Shift + R (hard refresh)
3. Clear cache browser

### Console Browser: "Permission denied"

**Solusi:**
1. Pastikan Firebase Rules sudah benar (auth != null)
2. Pastikan Anonymous Auth enabled
3. Cek console muncul "Website authenticated"

### Serial Monitor Tidak Berubah

**Solusi:**
1. Cek Console Browser dulu (F12)
2. Lihat ada error merah?
3. Pastikan muncul "Mode set to REGISTER"
4. Jika muncul tapi ESP32 tidak berubah, restart ESP32

---

## 📊 FLOW LENGKAP

```
User klik "Lanjut Scan Kartu"
  ↓
Website: signInAnonymously() → Success
  ↓
Website: set /system/mode = "REGISTER"
  ↓
Firebase: Auth check → Anonymous user OK ✓
  ↓
Firebase: Write allowed ✓
  ↓
ESP32: Polling deteksi mode berubah
  ↓
ESP32 Serial Monitor: "MODE CHANGED → REGISTER" ✓
  ↓
User tap kartu → Registrasi berhasil ✓
```

---

## 🔑 YANG SUDAH DIPERBAIKI

### 1. **Website sekarang menggunakan Anonymous Auth**
```javascript
import { getAuth, signInAnonymously } from "firebase/auth";

const auth = getAuth(app);
signInAnonymously(auth)
    .then(() => console.log('✓ Website authenticated'))
```

### 2. **Validasi auth sebelum set mode**
```javascript
const triggerScan = () => {
    if (!isAuthReady) {
        alert('Please wait, connecting to Firebase...');
        return;
    }
    // ...set mode...
}
```

### 3. **Console logging untuk debugging**
```javascript
console.log('Setting mode to REGISTER...');
set(ref(db, 'system/mode'), 'REGISTER')
    .then(() => console.log('✓ Mode set to REGISTER'))
    .catch(err => console.error('✗ Error:', err));
```

---

## ✅ CHECKLIST

- [ ] Enable Anonymous Auth di Firebase Console
- [ ] Refresh website (Ctrl + Shift + R)
- [ ] Cek Console Browser: "Website authenticated"
- [ ] Klik Registrasi User → Isi form → Lanjut Scan Kartu
- [ ] Cek Console Browser: "Mode set to REGISTER"
- [ ] Cek Serial Monitor ESP32: "MODE CHANGED → REGISTER"
- [ ] Tap kartu → Cek berhasil registrasi

---

## 📞 JIKA MASIH ERROR

Screenshot:
1. Console Browser (F12 → Console tab)
2. Serial Monitor ESP32
3. Firebase Console (Authentication & Rules)

Dan jelaskan di step mana error terjadi.

---

**Setelah enable Anonymous Auth, website akan berfungsi normal! 🚀**
