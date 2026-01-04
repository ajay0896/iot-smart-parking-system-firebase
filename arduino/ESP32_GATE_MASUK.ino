/*
 * GATE MASUK + REGISTRASI SCANNER (Firebase) - IMPROVED VERSION
 * Fungsi: Check-in dan Registrasi kartu baru
 */

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// --- ADDONS FIREBASE ---
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// --- KREDENSIAL FIREBASE & WIFI ---
#define WIFI_SSID "Muliakos-Lt2-Utama"
#define WIFI_PASSWORD "Muliakosan123"
#define API_KEY "AIzaSyCjRx1OXGSH_uAKDq-7ajVykOtHTNHQUwY"
#define DATABASE_URL "https://siste-parkir-iot-default-rtdb.firebaseio.com/"
#define USER_EMAIL "esp32-gate-masuk@sisterparkir.com"  // Email untuk autentikasi
#define USER_PASSWORD "ESP32GateMasuk2024"              // Password untuk autentikasi

// --- PINOUT ---
#define PIN_RFID_SDA  5
#define PIN_RFID_RST  4
#define PIN_SERVO     13
#define PIN_BUZZER    27
#define PIN_LED_HIJAU 25
#define PIN_LED_MERAH 26

// --- OBJEK ---
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
MFRC522 rfid(PIN_RFID_SDA, PIN_RFID_RST);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200); // GMT+7

String uidString;
String currentMode = "NORMAL"; // Cache mode lokal
unsigned long lastModeCheck = 0;
const unsigned long MODE_CHECK_INTERVAL = 1000; // Cek mode setiap 1 detik

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n========================================");
  Serial.println("  GATE MASUK + REGISTRASI - v2.0");
  Serial.println("========================================");
  
  // Setup Hardware
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_MERAH, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_LED_MERAH, HIGH);
  Serial.println("[INIT] ✓ GPIO Configured");

  myServo.setPeriodHertz(50);
  myServo.attach(PIN_SERVO, 500, 2400);
  myServo.write(0);
  Serial.println("[INIT] ✓ Servo Attached (Position: 0°)");

  lcd.init(); lcd.backlight();
  SPI.begin(); rfid.PCD_Init();
  Serial.println("[INIT] ✓ LCD & RFID Ready");

  // Koneksi WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.clear();
  lcd.print("Connecting WiFi");
  Serial.print("[WIFI] Connecting to: "); Serial.println(WIFI_SSID);
  
  int dots = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("Wait");
    for(int i=0; i<dots; i++) lcd.print(".");
    dots = (dots + 1) % 4;
  }
  
  Serial.println("\n[WIFI] ✓ Connected!");
  Serial.print("[WIFI] IP Address: "); Serial.println(WiFi.localIP());
  
  // Koneksi Firebase dengan Autentikasi
  Serial.println("[FIREBASE] Initializing...");
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  // Set user credentials untuk autentikasi
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  
  Serial.println("[FIREBASE] ⏳ Waiting for authentication...");
  
  // Tunggu autentikasi berhasil (max 30 detik)
  unsigned long authTimeout = millis();
  while (!Firebase.ready() && (millis() - authTimeout < 30000)) {
    delay(500);
    Serial.print(".");
  }
  
  if (Firebase.ready()) {
    Serial.println("\n[FIREBASE] ✓ Authenticated & Connected!");
    Serial.print("[FIREBASE] ✓ Device ID: ESP32-GATE-MASUK");
    Serial.print(" | UID: "); Serial.println(auth.token.uid.c_str());
    
    // Test koneksi Firebase
    if (Firebase.RTDB.getString(&fbdo, "/system/mode")) {
      currentMode = fbdo.stringData();
      Serial.print("[FIREBASE] ✓ Initial mode: "); Serial.println(currentMode);
    } else {
      Serial.print("[FIREBASE] ✗ Read error: "); Serial.println(fbdo.errorReason());
    }
  } else {
    Serial.println("\n[FIREBASE] ✗ Authentication FAILED!");
    Serial.println("[ERROR] Please check:");
    Serial.println("  1. API Key is correct");
    Serial.println("  2. Database URL is correct");
    Serial.println("  3. User email/password is created in Firebase Auth");
    Serial.println("  4. Firebase Auth is enabled in console");
    lcd.clear();
    lcd.print("Auth Failed!");
    lcd.setCursor(0,1);
    lcd.print("Check Serial Mon");
  }

  timeClient.begin();
  Serial.println("[NTP] ✓ Time Client Started");
  
  lcd.clear();
  lcd.print("System Online");
  lcd.setCursor(0,1);
  lcd.print("Gate MASUK");
  delay(2000);
  
  Serial.println("========================================");
  Serial.println("  SYSTEM READY - Waiting for cards...");
  Serial.println("========================================\n");
}

void loop() {
  timeClient.update();
  
  // Update mode dari Firebase secara berkala
  if (millis() - lastModeCheck >= MODE_CHECK_INTERVAL) {
    updateSystemMode();
    lastModeCheck = millis();
  }
  
  // Tampilan LCD berdasarkan mode
  static unsigned long lastLcdUpdate = 0;
  if (millis() - lastLcdUpdate > 2000) {
    updateLcdDisplay();
    lastLcdUpdate = millis();
  }

  // --- SCAN RFID ---
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Baca UID
  uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.println("\n┌────────────────────────────────────┐");
  Serial.print("│ CARD DETECTED: "); Serial.print(uidString); 
  for(int i=uidString.length(); i<20; i++) Serial.print(" ");
  Serial.println("│");
  Serial.print("│ MODE: "); Serial.print(currentMode);
  for(int i=currentMode.length(); i<28; i++) Serial.print(" ");
  Serial.println("│");
  Serial.println("└────────────────────────────────────┘");

  // Logika berdasarkan mode
  if (currentMode == "REGISTER") {
    Serial.println("► Processing: REGISTRATION MODE");
    prosesRegistrasi(uidString);
  } else {
    Serial.println("► Processing: CHECK-IN MODE");
    prosesCheckIn(uidString);
  }

  // Cleanup RFID
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  
  delay(500); // Debounce
}

void updateSystemMode() {
  if (Firebase.ready()) {
    if (Firebase.RTDB.getString(&fbdo, "/system/mode")) {
      String newMode = fbdo.stringData();
      if (newMode != currentMode) {
        currentMode = newMode;
        Serial.println("\n╔════════════════════════════════════╗");
        Serial.print("║  MODE CHANGED → "); Serial.print(currentMode);
        for(int i=currentMode.length(); i<19; i++) Serial.print(" ");
        Serial.println("║");
        Serial.println("╚════════════════════════════════════╝\n");
        
        // Beep notification
        digitalWrite(PIN_BUZZER, HIGH);
        delay(100);
        digitalWrite(PIN_BUZZER, LOW);
      }
    } else {
      // Hanya log error jika pertama kali atau error berubah
      static String lastError = "";
      String currentError = fbdo.errorReason();
      if (currentError != lastError) {
        Serial.print("[WARNING] Mode read error: "); Serial.println(currentError);
        lastError = currentError;
      }
    }
  }
}

void updateLcdDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (currentMode == "REGISTER") {
    lcd.print("** REGISTRASI **");
    lcd.setCursor(0, 1);
    lcd.print("Tempel Kartu..");
  } else {
    lcd.print("Gate MASUK");
    lcd.setCursor(0, 1);
    lcd.print("Silakan Tap..");
  }
  
  // Log status (hanya setiap 10 detik untuk tidak spam)
  static unsigned long lastStatusLog = 0;
  if (millis() - lastStatusLog > 10000) {
    Serial.print("[STATUS] "); 
    Serial.print(timeClient.getFormattedTime());
    Serial.print(" | Mode: "); 
    Serial.print(currentMode);
    Serial.print(" | WiFi: ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "OK" : "LOST");
    lastStatusLog = millis();
  }
}

void prosesRegistrasi(String uid) {
  lcd.clear(); 
  lcd.print("Reading UID...");
  lcd.setCursor(0, 1);
  lcd.print(uid.substring(0, 16));
  
  Serial.println("  ↳ Uploading to /system/temp_rfid...");
  
  // Upload UID ke Firebase
  if (Firebase.RTDB.setString(&fbdo, "/system/temp_rfid", uid)) {
    Serial.println("  ✓ Upload SUCCESS!");
    Serial.println("  ✓ Website will auto-save the user");
    
    lcd.clear();
    lcd.print("UID Tersimpan!");
    lcd.setCursor(0, 1);
    lcd.print("Cek Website");
    
    // Beep sukses
    for(int i=0; i<3; i++) {
      digitalWrite(PIN_BUZZER, HIGH); delay(100);
      digitalWrite(PIN_BUZZER, LOW); delay(100);
    }
    
    delay(3000);
    
  } else {
    Serial.print("  ✗ Upload FAILED: "); Serial.println(fbdo.errorReason());
    
    lcd.clear();
    lcd.print("Upload Gagal!");
    lcd.setCursor(0, 1);
    lcd.print("Coba Lagi");
    
    // Beep error
    digitalWrite(PIN_BUZZER, HIGH); delay(800);
    digitalWrite(PIN_BUZZER, LOW);
    
    delay(2000);
  }
}

void prosesCheckIn(String uid) {
  lcd.clear(); 
  lcd.print("Verifying...");
  
  Serial.println("  ↳ Checking user in database...");
  
  // Cek apakah user terdaftar
  String userPath = "/users/" + uid;
  
  if (Firebase.RTDB.get(&fbdo, userPath)) {
    
    if (fbdo.dataType() == "null" || fbdo.dataType() == "undefined") {
      // User TIDAK DITEMUKAN
      Serial.println("  ✗ User NOT FOUND in database");
      aksesDitolak();
      return;
    }
    
    // User DITEMUKAN
    Serial.println("  ✓ User FOUND!");
    
    // Ambil nama user (opsional untuk log)
    String nama = "";
    if (Firebase.RTDB.getString(&fbdo, userPath + "/nama")) {
      nama = fbdo.stringData();
      Serial.print("  ↳ Name: "); Serial.println(nama);
    }
    
    // Simpan log check-in
    unsigned long timestamp = timeClient.getEpochTime();
    String pathLog = "/logs/" + String(timestamp);
    
    FirebaseJson json;
    json.set("uid", uid);
    json.set("gate", "MASUK");
    json.set("status", "Check-In");
    json.set("waktu", timeClient.getFormattedTime());
    json.set("timestamp", timestamp);
    if (nama != "") json.set("nama", nama);

    Serial.println("  ↳ Saving check-in log...");
    if (Firebase.RTDB.setJSON(&fbdo, pathLog, &json)) {
      Serial.println("  ✓ Log saved");
    } else {
      Serial.print("  ✗ Log error: "); Serial.println(fbdo.errorReason());
    }
    
    // Update status terakhir DAN simpan timestamp_masuk (untuk solusi cepat check-out)
    Firebase.RTDB.setString(&fbdo, userPath + "/status_terakhir", "MASUK");
    Firebase.RTDB.setInt(&fbdo, userPath + "/timestamp_masuk", timestamp);
    Serial.print("  ✓ Timestamp saved: "); Serial.println(timestamp);

    // Buka gate
    bukaGate();
    
  } else {
    // Error koneksi Firebase
    Serial.print("  ✗ Firebase error: "); Serial.println(fbdo.errorReason());
    lcd.clear();
    lcd.print("Error Koneksi!");
    lcd.setCursor(0, 1);
    lcd.print("Coba Lagi");
    digitalWrite(PIN_BUZZER, HIGH); delay(500);
    digitalWrite(PIN_BUZZER, LOW);
    delay(2000);
  }
}

void bukaGate() {
  Serial.println("  ✓ ACCESS GRANTED - Opening gate...");
  
  lcd.clear(); 
  lcd.print("Selamat Datang!");
  lcd.setCursor(0, 1);
  lcd.print("Silakan Masuk");
  
  // LED & Buzzer
  digitalWrite(PIN_LED_MERAH, LOW);
  digitalWrite(PIN_LED_HIJAU, HIGH);
  digitalWrite(PIN_BUZZER, HIGH); delay(200);
  digitalWrite(PIN_BUZZER, LOW);
  
  // Buka servo
  myServo.write(90);
  Serial.println("  ↳ Servo: OPEN (90°)");
  
  delay(3000); // Tahan 3 detik
  
  // Tutup servo
  myServo.write(0);
  Serial.println("  ↳ Servo: CLOSED (0°)");
  
  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, HIGH);
  
  Serial.println("  ✓ Gate closed\n");
}

void aksesDitolak() {
  Serial.println("  ✗ ACCESS DENIED!");
  
  lcd.clear(); 
  lcd.print("Kartu Tidak");
  lcd.setCursor(0, 1); 
  lcd.print("Terdaftar!");
  
  // Buzzer panjang
  digitalWrite(PIN_BUZZER, HIGH); 
  delay(1000); 
  digitalWrite(PIN_BUZZER, LOW);
  
  delay(2000);
  Serial.println();
}
