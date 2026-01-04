# ESP32 Integration Guide - Notifikasi Otomatis

## Overview
Panduan ini menjelaskan cara ESP32 mengirim data ke Firebase sehingga web app bisa otomatis kirim notifikasi WhatsApp/Telegram.

---

## Flow Data ESP32 → Firebase → Notifikasi

```
1. User tap kartu di Gate KELUAR
2. ESP32 baca UID dari RFID
3. ESP32 ambil data user dari Firebase (users/{uid})
4. ESP32 hitung biaya parkir
5. ESP32 update saldo user
6. ESP32 kirim log ke Firebase (logs/{timestamp})
7. Web app detect log baru dengan status "Check-Out"
8. Web app kirim notifikasi ke nomor WA user
```

---

## Struktur Data yang Harus Dikirim ESP32

### Saat Check-In (Masuk)
```json
{
  "logs/{timestamp}": {
    "timestamp": 1735210800,
    "waktu": "10:00:00",
    "nama": "Budi Santoso",
    "uid": "A1B2C3D4",
    "wa": "08123456789",
    "gate": "MASUK",
    "status": "Check-In",
    "biaya": 0
  },
  "users/A1B2C3D4/status_terakhir": "MASUK"
}
```

### Saat Check-Out (Keluar) - TRIGGER NOTIFIKASI ⭐
```json
{
  "logs/{timestamp}": {
    "timestamp": 1735214400,
    "waktu": "11:00:00",
    "nama": "Budi Santoso",
    "uid": "A1B2C3D4",
    "wa": "08123456789",
    "gate": "KELUAR",
    "status": "Check-Out",
    "biaya": 3000
  },
  "users/A1B2C3D4/status_terakhir": "KELUAR",
  "users/A1B2C3D4/saldo": 47000
}
```

⚠️ **PENTING:** 
- Field `wa` (nomor WhatsApp) WAJIB ada di log
- Field `status` harus **"Check-Out"** untuk trigger notifikasi
- Timestamp harus unix timestamp (detik)

---

## Contoh Code ESP32 (Arduino)

### Include Libraries
```cpp
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>

// Firebase
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
```

### Konfigurasi
```cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"

#define API_KEY "AIzaSyCjRx1OXGSH_uAKDq-7ajVykOtHTNHQUwY"
#define DATABASE_URL "https://siste-parkir-iot-default-rtdb.firebaseio.com"

// RFID Pins
#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
```

### Setup
```cpp
void setup() {
  Serial.begin(115200);
  
  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  
  // Firebase Config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  // RFID Setup
  SPI.begin();
  rfid.PCD_Init();
  
  // NTP for timestamp
  configTime(7 * 3600, 0, "pool.ntp.org");
  
  Serial.println("System Ready!");
}
```

### Loop - Detect RFID
```cpp
void loop() {
  // Cek kartu
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  // Baca UID
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  
  Serial.println("UID Detected: " + uid);
  
  // Cek mode sistem
  String mode = "";
  if (Firebase.RTDB.getString(&fbdo, "/system/mode")) {
    mode = fbdo.stringData();
  }
  
  if (mode == "REGISTER") {
    // Mode registrasi - simpan UID saja
    Firebase.RTDB.setString(&fbdo, "/system/temp_rfid", uid);
    Serial.println("UID sent for registration");
  } else {
    // Mode normal - proses parkir
    processParking(uid);
  }
  
  rfid.PICC_HaltA();
  delay(1000);
}
```

### Fungsi Process Parking (CORE)
```cpp
void processParking(String uid) {
  // 1. Ambil data user
  String userPath = "/users/" + uid;
  FirebaseJson userData;
  
  if (!Firebase.RTDB.getJSON(&fbdo, userPath)) {
    Serial.println("User tidak ditemukan!");
    return;
  }
  
  FirebaseJsonData jsonData;
  fbdo.jsonObject().get(jsonData, "nama");
  String nama = jsonData.stringValue;
  
  fbdo.jsonObject().get(jsonData, "wa");
  String wa = jsonData.stringValue;
  
  fbdo.jsonObject().get(jsonData, "saldo");
  int saldo = jsonData.intValue;
  
  fbdo.jsonObject().get(jsonData, "status_terakhir");
  String statusTerakhir = jsonData.stringValue;
  
  // 2. Tentukan gate (masuk atau keluar)
  String gate = (statusTerakhir == "KELUAR" || statusTerakhir == "BARU") ? "MASUK" : "KELUAR";
  String status = (gate == "MASUK") ? "Check-In" : "Check-Out";
  int biaya = (gate == "KELUAR") ? 3000 : 0;
  
  // 3. Update saldo jika keluar
  if (gate == "KELUAR") {
    saldo -= biaya;
    Firebase.RTDB.setInt(&fbdo, userPath + "/saldo", saldo);
  }
  
  // 4. Update status user
  String newStatus = (gate == "MASUK") ? "MASUK" : "KELUAR";
  Firebase.RTDB.setString(&fbdo, userPath + "/status_terakhir", newStatus);
  
  // 5. Buat log
  unsigned long timestamp = getTimestamp();
  String waktu = getCurrentTime();
  
  FirebaseJson logData;
  logData.set("timestamp", timestamp);
  logData.set("waktu", waktu);
  logData.set("nama", nama);
  logData.set("uid", uid);
  logData.set("wa", wa);  // ⭐ PENTING untuk notifikasi
  logData.set("gate", gate);
  logData.set("status", status);
  logData.set("biaya", biaya);
  
  // 6. Kirim log ke Firebase
  String logPath = "/logs/" + String(timestamp);
  if (Firebase.RTDB.setJSON(&fbdo, logPath, &logData)) {
    Serial.println("✅ Log berhasil dikirim!");
    Serial.println("Status: " + status);
    
    // Jika Check-Out, web app akan kirim notifikasi otomatis
    if (status == "Check-Out") {
      Serial.println("🔔 Notifikasi akan dikirim ke: " + wa);
    }
  } else {
    Serial.println("❌ Error: " + fbdo.errorReason());
  }
}
```

### Helper Functions
```cpp
unsigned long getTimestamp() {
  time_t now;
  time(&now);
  return now;
}

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "00:00:00";
  }
  
  char timeString[9];
  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
  return String(timeString);
}
```

---

## Testing

### 1. Upload Code ke ESP32
```bash
1. Buka Arduino IDE
2. Copy code di atas
3. Ganti WiFi SSID & Password
4. Upload ke ESP32
5. Buka Serial Monitor (115200 baud)
```

### 2. Test Registrasi
```bash
1. Web app: Klik "Registrasi User"
2. Isi form (Nama, WA, Password)
3. Klik "Lanjut Scan Kartu"
4. Tap kartu di RFID reader
5. Lihat Serial Monitor: "UID sent for registration"
6. Web app: Muncul "Kartu Terhubung!"
```

### 3. Test Check-In
```bash
1. Tap kartu yang sudah terdaftar
2. Serial Monitor: "Status: Check-In"
3. Web app: Live Monitoring muncul log baru
```

### 4. Test Check-Out + Notifikasi ⭐
```bash
1. Tap kartu lagi (yang statusnya MASUK)
2. Serial Monitor: 
   - "Status: Check-Out"
   - "🔔 Notifikasi akan dikirim ke: 08xxx"
3. Web app: 
   - Live Monitoring update
   - Notifikasi terkirim ke WhatsApp/Telegram user
4. Cek WA user: Terima pesan dari sistem
```

---

## Troubleshooting ESP32

### WiFi Tidak Connect
```cpp
// Tambah di setup()
WiFi.setAutoReconnect(true);
WiFi.persistent(true);
```

### Firebase Error
**Cek:**
1. API Key benar?
2. Database URL benar?
3. Firebase Rules allow write?

### RFID Tidak Terbaca
**Cek:**
1. Wiring benar? (SS=5, RST=22)
2. Power supply cukup?
3. Serial Monitor: `rfid.PCD_Init()` sukses?

### Notifikasi Tidak Terkirim
**Kemungkinan:**
1. Field `wa` tidak ada di log → Pastikan ESP32 kirim field `wa`
2. Config notifikasi belum diaktifkan → Admin panel: Aktifkan WA/Telegram
3. Status bukan "Check-Out" → Pastikan `status: "Check-Out"`

---

## Database Structure Reference

```
Firebase RTDB
├── system/
│   ├── mode: "NORMAL" | "REGISTER"
│   └── temp_rfid: "UID_TEMP"
│
├── users/
│   └── {uid}/
│       ├── nama: string
│       ├── wa: string (WAJIB untuk notifikasi)
│       ├── password: string
│       ├── saldo: number
│       └── status_terakhir: "MASUK" | "KELUAR" | "BARU"
│
├── logs/
│   └── {timestamp}/
│       ├── timestamp: number
│       ├── waktu: "HH:MM:SS"
│       ├── nama: string
│       ├── uid: string
│       ├── wa: string ⭐ WAJIB
│       ├── gate: "MASUK" | "KELUAR"
│       ├── status: "Check-In" | "Check-Out"
│       └── biaya: number
│
└── config/
    └── notifications/
        ├── whatsapp/
        │   ├── enabled: boolean
        │   ├── token: string
        │   └── url: string
        └── telegram/
            ├── enabled: boolean
            ├── token: string
            └── chatId: string
```

---

## Advanced: Custom Tarif

Jika ingin tarif berdasarkan durasi:

```cpp
void processParking(String uid) {
  // ... (kode sebelumnya)
  
  int biaya = 0;
  if (gate == "KELUAR") {
    // Hitung durasi
    unsigned long checkInTime = 0;
    
    // Cari log terakhir Check-In user ini
    if (Firebase.RTDB.getJSON(&fbdo, "/logs")) {
      FirebaseJson &json = fbdo.jsonObject();
      // Loop semua logs, cari yang Check-In dengan UID ini
      // ... (implementasi loop)
      
      unsigned long duration = timestamp - checkInTime; // dalam detik
      int hours = duration / 3600;
      
      // Tarif: Rp 3000 per jam
      biaya = (hours > 0 ? hours : 1) * 3000;
    }
  }
  
  // ... (kirim log dengan biaya calculated)
}
```

---

## Monitoring ESP32

### Serial Monitor Output
```
WiFi Connected
System Ready!
UID Detected: A1B2C3D4
✅ Log berhasil dikirim!
Status: Check-Out
🔔 Notifikasi akan dikirim ke: 08123456789
```

### Firebase Console
1. Buka https://console.firebase.google.com/
2. Project: siste-parkir-iot
3. Realtime Database
4. Lihat data real-time update

---

## Production Checklist

- [ ] WiFi credentials benar
- [ ] Firebase config benar
- [ ] RFID reader terhubung dengan benar
- [ ] NTP time sync berhasil
- [ ] Field `wa` selalu dikirim di log
- [ ] Admin panel: Notifikasi sudah dikonfigurasi
- [ ] Test Check-In: OK
- [ ] Test Check-Out: OK
- [ ] Test Notifikasi terkirim: OK

---

## Support

Jika ada error pada ESP32:
1. Cek Serial Monitor
2. Cek Firebase Rules
3. Cek format data yang dikirim
4. Lihat Firebase Console untuk verifikasi data

Jika notifikasi tidak terkirim:
1. Lihat ADMIN_GUIDE.md
2. Cek konfigurasi API di admin panel
3. Test API manual via Postman

---

**File ini:** Panduan ESP32 Integration
**File terkait:** ADMIN_GUIDE.md (untuk konfigurasi web app)
**Status:** Ready for implementation
