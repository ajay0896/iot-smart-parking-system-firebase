/*
 * GATE KELUAR - SISTEM PARKIR IoT (Firebase)
 * Fungsi: Check-out, hitung biaya, potong saldo
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
#define USER_EMAIL "esp32-gate-keluar@sisterparkir.com"  // Email untuk autentikasi
#define USER_PASSWORD "ESP32GateKeluar2024"              // Password untuk autentikasi

// --- PINOUT ---
#define PIN_RFID_SDA  5
#define PIN_RFID_RST  4
#define PIN_SERVO     13
#define PIN_BUZZER    27
#define PIN_LED_HIJAU 25
#define PIN_LED_MERAH 26

// --- TARIF PARKIR ---
#define TARIF_PER_JAM 3000  // Rp 3000/jam

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

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n--- GATE KELUAR STARTING ---");
  
  // Setup Hardware
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_MERAH, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_LED_MERAH, HIGH);
  Serial.println("[INIT] GPIO Configured");

  myServo.setPeriodHertz(50);
  myServo.attach(PIN_SERVO, 500, 2400);
  myServo.write(0);
  Serial.println("[INIT] Servo Attached");

  lcd.init(); lcd.backlight();
  SPI.begin(); rfid.PCD_Init();
  Serial.println("[INIT] LCD & RFID Initialized");

  // Koneksi WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.print("Connect WiFi...");
  Serial.print("[WIFI] Connecting to "); Serial.print(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(300); Serial.print(".");
  }
  Serial.println("\n[WIFI] Connected!");
  Serial.print("[WIFI] IP Address: "); Serial.println(WiFi.localIP());
  
  // Koneksi Firebase dengan Autentikasi
  Serial.println("[FIREBASE] Connecting...");
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  // Set user credentials untuk autentikasi
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  Serial.println("[FIREBASE] ⏳ Waiting for authentication...");
  
  // Tunggu autentikasi berhasil (max 30 detik)
  unsigned long authTimeout = millis();
  while (!Firebase.ready() && (millis() - authTimeout < 30000)) {
    delay(500);
    Serial.print(".");
  }
  
  if (Firebase.ready()) {
    Serial.println("\n[FIREBASE] ✓ Authenticated & Connected!");
    Serial.print("[FIREBASE] ✓ Device ID: ESP32-GATE-KELUAR");
    Serial.print(" | UID: "); Serial.println(auth.token.uid.c_str());
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
  Serial.println("[NTP] Time Client Started");
  
  lcd.clear();
  lcd.print("Gate KELUAR");
  lcd.setCursor(0,1);
  lcd.print("System Ready");
  delay(2000);
  Serial.println("--- SYSTEM READY ---\n");
}

void loop() {
  timeClient.update();
  
  // Tampilan LCD (Update setiap 3 detik)
  static unsigned long lastLcd = 0;
  if (millis() - lastLcd > 3000) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gate KELUAR");
    lcd.setCursor(0,1);
    lcd.print("Tap untuk Exit");
    
    Serial.print("[STATUS] Time: "); Serial.print(timeClient.getFormattedTime());
    Serial.println(" | Gate: KELUAR");
    
    lastLcd = millis();
  }

  // --- SCAN RFID ---
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.println("\n=============================");
  Serial.print("[RFID] Card Detected! UID: "); Serial.println(uidString);
  Serial.println("[LOGIC] Processing CHECK-OUT...");

  checkOutProcess(uidString);

  Serial.println("=============================\n");

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void checkOutProcess(String uid) {
  lcd.clear(); lcd.print("Checking...");
  
  // Cek apakah user terdaftar
  Serial.print("[FIREBASE] Checking /users/"); Serial.println(uid);
  
  if (!Firebase.RTDB.getJSON(&fbdo, "/users/" + uid)) {
    Serial.println("[ACCESS] User NOT FOUND!");
    aksesDitolak("Kartu Tidak", "Terdaftar!");
    return;
  }

  // User ditemukan, ambil data
  FirebaseJson json = fbdo.jsonObject();
  FirebaseJsonData result;
  
  String nama = "";
  String statusTerakhir = "";
  int saldo = 0;
  
  json.get(result, "nama");
  if (result.success) nama = result.stringValue;
  
  json.get(result, "status_terakhir");
  if (result.success) statusTerakhir = result.stringValue;
  
  json.get(result, "saldo");
  if (result.success) saldo = result.intValue;

  Serial.print("[ACCESS] User FOUND: "); Serial.println(nama);
  Serial.print("[ACCESS] Status: "); Serial.println(statusTerakhir);
  Serial.print("[ACCESS] Saldo: Rp "); Serial.println(saldo);

  // Validasi: Apakah user sudah check-in?
  if (statusTerakhir != "MASUK") {
    Serial.println("[ERROR] User belum check-in atau sudah keluar!");
    aksesDitolak("Belum Check-In", "atau Sudah Keluar");
    return;
  }

  // Hitung durasi parkir
  unsigned long timestampMasuk = cariTimestampMasuk(uid);
  unsigned long timestampSekarang = timeClient.getEpochTime();
  
  if (timestampMasuk == 0) {
    Serial.println("[ERROR] Timestamp masuk tidak ditemukan!");
    aksesDitolak("Error Log", "Masuk!");
    return;
  }

  unsigned long durasi = timestampSekarang - timestampMasuk; // dalam detik
  int menitParkir = durasi / 60;
  int biaya = hitungBiaya(menitParkir);

  Serial.print("[CALC] Durasi parkir: "); Serial.print(menitParkir); Serial.println(" menit");
  Serial.print("[CALC] Biaya: Rp "); Serial.println(biaya);

  // Validasi saldo
  if (saldo < biaya) {
    Serial.println("[ERROR] Saldo tidak cukup!");
    lcd.clear(); lcd.print("Saldo Kurang!");
    lcd.setCursor(0,1); lcd.print("Rp "); lcd.print(saldo);
    digitalWrite(PIN_BUZZER, HIGH); delay(1500); digitalWrite(PIN_BUZZER, LOW);
    delay(2000);
    return;
  }

  // Potong saldo
  int saldoBaru = saldo - biaya;
  Serial.print("[FIREBASE] Saldo sebelum: Rp "); Serial.println(saldo);
  Serial.print("[FIREBASE] Saldo sesudah: Rp "); Serial.println(saldoBaru);
  
  Firebase.RTDB.setInt(&fbdo, "/users/" + uid + "/saldo", saldoBaru);
  Firebase.RTDB.setString(&fbdo, "/users/" + uid + "/status_terakhir", "KELUAR");
  Firebase.RTDB.setInt(&fbdo, "/users/" + uid + "/timestamp_masuk", 0);  // Reset timestamp (sudah keluar)
  Serial.println("[FIREBASE] ✓ Timestamp reset to 0");

  // Simpan log
  String pathLog = "/logs/" + String(timestampSekarang);
  FirebaseJson logJson;
  logJson.set("uid", uid);
  logJson.set("nama", nama);
  logJson.set("gate", "KELUAR");
  logJson.set("status", "Check-Out");
  logJson.set("waktu", timeClient.getFormattedTime());
  logJson.set("timestamp", timestampSekarang);
  logJson.set("biaya", biaya);
  logJson.set("durasi", String(menitParkir) + " menit");
  logJson.set("saldo_akhir", saldoBaru);

  Serial.println("[FIREBASE] Sending Log...");
  if (Firebase.RTDB.setJSON(&fbdo, pathLog, &logJson)) {
    Serial.println("[FIREBASE] Log Saved Successfully");
  } else {
    Serial.print("[FIREBASE] Log Error: "); Serial.println(fbdo.errorReason());
  }

  // Buka gate
  bukaGate(biaya, saldoBaru);
}

unsigned long cariTimestampMasuk(String uid) {
  Serial.println("[FIREBASE] Getting timestamp from user profile...");
  
  // SOLUSI CEPAT: Ambil langsung dari /users/{uid}/timestamp_masuk
  // Tidak perlu loop semua logs (2-3 menit) -> Sekarang ~500ms
  if (Firebase.RTDB.getInt(&fbdo, "/users/" + uid + "/timestamp_masuk")) {
    unsigned long ts = fbdo.intData();
    
    if (ts > 0) {
      Serial.print("[FIREBASE] ✓ Found CHECK-IN timestamp: "); 
      Serial.println(ts);
      return ts;
    } else {
      Serial.println("[FIREBASE] ✗ Timestamp is 0 (invalid)");
      return 0;
    }
  } else {
    Serial.print("[FIREBASE] ✗ Error reading timestamp: ");
    Serial.println(fbdo.errorReason());
    return 0;
  }
}

int hitungBiaya(int menit) {
  // Tarif: Rp 3000/jam
  // Pembulatan ke atas per jam
  int jam = (menit + 59) / 60; // Pembulatan ke atas
  if (jam < 1) jam = 1; // Minimal 1 jam
  return jam * TARIF_PER_JAM;
}

void bukaGate(int biaya, int saldaBaru) {
  Serial.println("[GATE] Opening Gate...");
  
  lcd.clear(); 
  lcd.print("Bayar: Rp"); lcd.print(biaya);
  lcd.setCursor(0,1); 
  lcd.print("Sisa: Rp"); lcd.print(saldaBaru);
  
  digitalWrite(PIN_LED_MERAH, LOW);
  digitalWrite(PIN_LED_HIJAU, HIGH);
  bunyiBuzzer(2);
  
  delay(2000);
  
  lcd.clear(); lcd.print("Hati-hati");
  lcd.setCursor(0,1); lcd.print("di Jalan!");
  
  myServo.write(90); // Buka
  delay(4000);       // Tahan lebih lama untuk keluar
  myServo.write(0);  // Tutup
  
  Serial.println("[GATE] Closing Gate...");
  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, HIGH);
}

void aksesDitolak(String baris1, String baris2) {
  Serial.println("[GATE] Access DENIED!");
  lcd.clear(); 
  lcd.print(baris1);
  lcd.setCursor(0,1); 
  lcd.print(baris2);
  digitalWrite(PIN_BUZZER, HIGH); 
  delay(1200); 
  digitalWrite(PIN_BUZZER, LOW);
  delay(1500);
}

void bunyiBuzzer(int kali) {
  for (int i=0; i<kali; i++) {
    digitalWrite(PIN_BUZZER, HIGH); delay(150);
    digitalWrite(PIN_BUZZER, LOW); delay(150);
  }
}
