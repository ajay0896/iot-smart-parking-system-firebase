# 🔧 DEVELOPMENT & MAINTENANCE GUIDE

Panduan untuk development dan maintenance project IoT Smart Parking System.

## 📁 Struktur Project

```
iot-smart-parking-system-firebase/
│
├── arduino/                      # ESP32 Source Code
│   ├── ESP32_GATE_MASUK.ino     # Gate masuk controller
│   ├── ESP32_GATE_KELUAR.ino    # Gate keluar controller
│   └── ESP32_HARDWARE_TEST.ino  # Hardware testing utility
│
├── public/                       # Website (Frontend)
│   ├── index.html               # Main HTML (Vue.js app inline)
│   ├── css/
│   │   └── style.css            # Custom styles
│   └── js/
│       └── firebase-config.js   # Firebase configuration
│
├── docs/                         # Documentation
│   ├── QUICK_START.md           # Quick start guide
│   ├── FIREBASE_SETUP.md        # Firebase setup
│   ├── ESP32_GUIDE.md           # ESP32 guide
│   ├── TROUBLESHOOTING.md       # Troubleshooting
│   └── ... (25+ documentation files)
│
├── firebase/                     # Firebase Configuration
│   ├── database-rules.json      # Security rules
│   ├── database-structure.json  # DB structure
│   └── database-dummy.json      # Sample data
│
├── .gitignore                    # Git ignore rules
├── LICENSE                       # MIT License
├── README.md                     # Main documentation
└── PROJECT_CLEANUP_SUMMARY.md    # Cleanup summary
```

## 🔄 Workflow Development

### 1. Update Kode ESP32

**File**: `arduino/ESP32_GATE_MASUK.ino` atau `arduino/ESP32_GATE_KELUAR.ino`

```bash
# 1. Edit kode di Arduino IDE
# 2. Test di Serial Monitor
# 3. Jika OK, commit changes

git add arduino/
git commit -m "Update: [deskripsi perubahan]"
git push
```

**Tips:**
- Selalu test di Serial Monitor sebelum commit
- Gunakan `ESP32_HARDWARE_TEST.ino` untuk test komponen
- Backup kode lama sebelum major changes

### 2. Update Website

**File**: `public/index.html`, `public/css/style.css`

```bash
# 1. Edit file
# 2. Test di browser (Live Server)
# 3. Test semua fitur:
#    - Registrasi
#    - Check-in/out simulation
#    - Admin dashboard
# 4. Commit

git add public/
git commit -m "Update: [deskripsi perubahan]"
git push
```

**Best Practices:**
- Test di multiple browsers (Chrome, Firefox, Safari)
- Check responsive design (mobile, tablet, desktop)
- Validate Firebase connection
- Check console untuk errors

### 3. Update Dokumentasi

**Folder**: `docs/`

```bash
# Update dokumentasi yang relevan
git add docs/
git commit -m "Docs: [deskripsi update]"
git push
```

## 🎨 Customization Guide

### Mengubah Tarif Parkir

**File**: `arduino/ESP32_GATE_KELUAR.ino`

```cpp
// Baris 36
#define TARIF_PER_JAM 3000  // Ubah nilai ini (dalam Rupiah)
```

### Mengubah Saldo Awal

**File**: `public/index.html`

```javascript
// Cari fungsi saveFinalData() (sekitar baris 920)
const newUser = {
    nama: form.value.nama,
    wa: form.value.wa,
    password: form.value.pass,
    saldo: 50000,  // <-- Ubah nilai ini
    status_terakhir: 'BARU'
};
```

### Mengubah WiFi Credentials

**File**: `arduino/ESP32_GATE_MASUK.ino` & `arduino/ESP32_GATE_KELUAR.ino`

```cpp
// Baris 20-21
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

### Mengubah Firebase Config

**File**: `public/js/firebase-config.js`

```javascript
export const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    databaseURL: "YOUR_DATABASE_URL",
    projectId: "YOUR_PROJECT_ID",
    // ... config lainnya
};
```

### Mengubah Pin ESP32

**File**: `arduino/ESP32_GATE_MASUK.ino` & `arduino/ESP32_GATE_KELUAR.ino`

```cpp
// Baris 27-32
#define PIN_RFID_SDA  5    // Ubah sesuai wiring
#define PIN_RFID_RST  4
#define PIN_SERVO     13
#define PIN_BUZZER    27
#define PIN_LED_HIJAU 25
#define PIN_LED_MERAH 26
```

### Mengubah Warna Theme

**File**: `public/css/style.css`

```css
/* Gradient colors */
.stat-card {
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    /* Ubah kode warna di sini */
}

.stat-card-success {
    background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%);
}

.stat-card-warning {
    background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
}
```

### Mengubah Admin Credentials

**File**: `public/index.html`

```javascript
// Cari fungsi loginAdmin() (sekitar baris 726)
const loginAdmin = () => {
    if (adminForm.value.username === 'admin' && 
        adminForm.value.password === 'admin123') {  // <-- Ubah di sini
        isAdminLoggedIn.value = true;
        view.value = 'admin-dashboard';
    }
};
```

**Security Note**: Untuk production, implement proper authentication dengan Firebase Auth.

## 🧪 Testing Checklist

### Before Commit

- [ ] Kode compile tanpa error
- [ ] Test di Serial Monitor (untuk ESP32)
- [ ] Test di browser (untuk website)
- [ ] No console errors
- [ ] Semua fitur berfungsi
- [ ] Responsive design OK
- [ ] Documentation updated

### Before Push

- [ ] Commit message jelas
- [ ] `.gitignore` updated
- [ ] No sensitive data (API keys, passwords)
- [ ] README updated (jika perlu)
- [ ] CHANGELOG updated (optional)

### Before Release

- [ ] Full end-to-end testing
- [ ] Multiple browser testing
- [ ] Multiple device testing
- [ ] Documentation review
- [ ] Version bump
- [ ] Tag release di GitHub

## 📝 Commit Message Convention

Gunakan format:

```
<type>: <description>

<body> (optional)

<footer> (optional)
```

**Types:**
- `feat`: Fitur baru
- `fix`: Bug fix
- `docs`: Dokumentasi
- `style`: Formatting, styling
- `refactor`: Code refactoring
- `test`: Testing
- `chore`: Maintenance

**Examples:**

```bash
git commit -m "feat: Add notification feature"
git commit -m "fix: Resolve RFID detection issue"
git commit -m "docs: Update installation guide"
git commit -m "refactor: Optimize check-out process"
```

## 🐛 Debugging Tips

### ESP32 Not Connecting

```
1. Check Serial Monitor (115200 baud)
2. Look for: "[WIFI] Connecting..."
3. Verify SSID/Password
4. Check WiFi signal strength
5. Try upload ESP32_HARDWARE_TEST.ino
```

### Website Not Loading

```
1. Check browser console (F12)
2. Look for Firebase errors
3. Verify firebase-config.js
4. Check network tab for failed requests
5. Clear browser cache (Ctrl+Shift+R)
```

### Mode Not Changing

```
1. Check Firebase Console → Database
2. Verify /system/mode value
3. Check ESP32 Serial Monitor
4. Verify Anonymous Auth enabled
5. Check Database Rules
```

### RFID Not Reading

```
1. Upload ESP32_HARDWARE_TEST.ino
2. Check RFID firmware version
3. Verify wiring (use multimeter)
4. Check power supply (3.3V)
5. Distance kartu < 3cm
```

## 🔐 Security Best Practices

### For Production

1. **Firebase Rules**
   - Jangan gunakan `.read: true`, `.write: true`
   - Implement proper user-based rules
   - Use Firebase Auth tokens

2. **API Keys**
   - Jangan commit API keys ke public repo
   - Use environment variables
   - Implement rate limiting

3. **Passwords**
   - Hash passwords (jangan plain text)
   - Use Firebase Auth untuk user auth
   - Implement password strength requirements

4. **Admin Access**
   - Ganti default admin password
   - Implement session timeout
   - Add 2FA (optional)

## 📊 Performance Optimization

### ESP32

```cpp
// Use delay() wisely
delay(1000);  // 1 second OK
delay(10000); // 10 seconds TOO LONG

// Prefer non-blocking code
unsigned long lastCheck = 0;
if (millis() - lastCheck > 1000) {
    // Do something
    lastCheck = millis();
}
```

### Website

```javascript
// Debounce Firebase writes
let timeout;
function updateData(value) {
    clearTimeout(timeout);
    timeout = setTimeout(() => {
        set(ref(db, 'path'), value);
    }, 500);
}
```

### Firebase

- Use `.indexOn` for queries
- Limit data dengan `.limitToLast(50)`
- Denormalize data jika perlu

## 🚀 Deployment

### Website to Firebase Hosting

```bash
npm install -g firebase-tools
firebase login
firebase init hosting
# Select 'public' folder
firebase deploy
```

### Website to Netlify

1. Buka netlify.com
2. Drag & drop folder `public/`
3. Custom domain (optional)

### Auto-Deploy with GitHub Actions

Create `.github/workflows/deploy.yml`:

```yaml
name: Deploy
on:
  push:
    branches: [ main ]
jobs:
  deploy:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: FirebaseExtended/action-hosting-deploy@v0
        with:
          repoToken: '${{ secrets.GITHUB_TOKEN }}'
          firebaseServiceAccount: '${{ secrets.FIREBASE_SERVICE_ACCOUNT }}'
```

## 📚 Resources

### Documentation
- [Firebase Docs](https://firebase.google.com/docs)
- [ESP32 Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Vue.js Guide](https://vuejs.org/guide/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

### Libraries
- [Firebase ESP Client](https://github.com/mobizt/Firebase-ESP-Client)
- [MFRC522](https://github.com/miguelbalboa/rfid)
- [LiquidCrystal I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

### Tools
- [VS Code](https://code.visualstudio.com/)
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Firebase Console](https://console.firebase.google.com)

## 📞 Support

- **GitHub Issues**: [Create Issue](https://github.com/ajay0896/iot-smart-parking-system-firebase/issues)
- **Discussions**: [GitHub Discussions](https://github.com/ajay0896/iot-smart-parking-system-firebase/discussions)
- **WhatsApp**: +62 896-6395-6717

---

**Last Updated**: 4 Januari 2026  
**Maintainer**: Fajar Ramdani
