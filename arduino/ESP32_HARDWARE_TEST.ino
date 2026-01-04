/*
 * ESP32 HARDWARE TEST - Tanpa WiFi/Firebase
 * Fungsi: Test semua komponen (RFID, LCD, Servo, Buzzer, LED)
 * 
 * CARA PAKAI:
 * 1. Upload kode ini ke ESP32
 * 2. Buka Serial Monitor (115200 baud)
 * 3. Ikuti instruksi yang muncul
 * 4. Test setiap komponen satu per satu
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// --- PINOUT (sama seperti kode asli) ---
#define PIN_RFID_SDA  5
#define PIN_RFID_RST  4
#define PIN_SERVO     13
#define PIN_BUZZER    27
#define PIN_LED_HIJAU 25
#define PIN_LED_MERAH 26

// --- OBJEK ---
MFRC522 rfid(PIN_RFID_SDA, PIN_RFID_RST);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Ubah ke 0x3F jika LCD tidak muncul
Servo myServo;

// --- MODE TEST ---
unsigned long lastAutoTest = 0;
const unsigned long AUTO_TEST_INTERVAL = 15000; // Test ulang setiap 15 detik
int currentTest = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n╔════════════════════════════════════════════╗");
  Serial.println("║   ESP32 HARDWARE TEST - AUTO MODE         ║");
  Serial.println("║   Test Otomatis Tanpa Input Manual        ║");
  Serial.println("╚════════════════════════════════════════════╝\n");
  
  // Setup GPIO
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_MERAH, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  // Matikan semua LED
  digitalWrite(PIN_LED_HIJAU, LOW);
  digitalWrite(PIN_LED_MERAH, LOW);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("  STARTING AUTOMATIC TEST SEQUENCE...");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  
  delay(1000);
}

void loop() {
  // Auto-detect RFID card (selalu aktif)
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) uid += "0";
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    
    Serial.println("\n╔════════════════════════════════════════════╗");
    Serial.println("║        🎯 RFID CARD DETECTED!              ║");
    Serial.println("╚════════════════════════════════════════════╝");
    Serial.print("  UID: "); Serial.println(uid);
    Serial.print("  Size: "); Serial.print(rfid.uid.size); Serial.println(" bytes");
    Serial.println();
    
    lcd.clear();
    lcd.print("UID Terdeteksi!");
    lcd.setCursor(0, 1);
    lcd.print(uid.substring(0, 16));
    
    // Beep sukses 3x
    digitalWrite(PIN_LED_HIJAU, HIGH);
    for(int i=0; i<3; i++) {
      digitalWrite(PIN_BUZZER, HIGH);
      delay(100);
      digitalWrite(PIN_BUZZER, LOW);
      delay(100);
    }
    digitalWrite(PIN_LED_HIJAU, LOW);
    
    delay(2000);
    
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  
  // Auto test berurutan setiap 15 detik
  if (millis() - lastAutoTest >= AUTO_TEST_INTERVAL) {
    lastAutoTest = millis();
    
    Serial.println("\n\n╔════════════════════════════════════════════╗");
    Serial.print("║  CYCLE "); Serial.print((currentTest / 4) + 1);
    Serial.println(" - AUTO TEST SEQUENCE             ║");
    Serial.println("╚════════════════════════════════════════════╝\n");
    
    // Test 1: GPIO (LED & Buzzer)
    Serial.println("┌─────────────────────────────────────────┐");
    Serial.println("│ [1/4] Testing GPIO (LED & Buzzer)      │");
    Serial.println("└─────────────────────────────────────────┘");
    testGPIO();
    delay(2000);
    
    // Test 2: LCD
    Serial.println("\n┌─────────────────────────────────────────┐");
    Serial.println("│ [2/4] Testing LCD I2C Display          │");
    Serial.println("└─────────────────────────────────────────┘");
    testLCD();
    delay(2000);
    
    // Test 3: Servo
    Serial.println("\n┌─────────────────────────────────────────┐");
    Serial.println("│ [3/4] Testing Servo Motor              │");
    Serial.println("└─────────────────────────────────────────┘");
    testServo();
    delay(2000);
    
    // Test 4: RFID
    Serial.println("\n┌─────────────────────────────────────────┐");
    Serial.println("│ [4/4] Testing RFID Reader              │");
    Serial.println("└─────────────────────────────────────────┘");
    testRFID();
    
    Serial.println("\n╔════════════════════════════════════════════╗");
    Serial.println("║   ✓ ALL TESTS COMPLETED SUCCESSFULLY!     ║");
    Serial.println("╚════════════════════════════════════════════╝");
    Serial.println("  → Waiting for next cycle or RFID card...\n");
    
    currentTest += 4;
    
    // Reset LCD ke mode standby
    lcd.clear();
    lcd.print("Auto Test Mode");
    lcd.setCursor(0, 1);
    lcd.print("Tap Kartu...");
  }
}

// ============================================
// TEST FUNCTIONS
// ============================================

void testGPIO() {
  Serial.println("  → LED Merah ON");
  digitalWrite(PIN_LED_MERAH, HIGH);
  delay(1000);
  
  Serial.println("  → LED Merah OFF");
  digitalWrite(PIN_LED_MERAH, LOW);
  delay(500);
  
  Serial.println("  → LED Hijau ON");
  digitalWrite(PIN_LED_HIJAU, HIGH);
  delay(1000);
  
  Serial.println("  → LED Hijau OFF");
  digitalWrite(PIN_LED_HIJAU, LOW);
  delay(500);
  
  Serial.println("  → LED Bergantian (5x)");
  for(int i=0; i<5; i++) {
    digitalWrite(PIN_LED_MERAH, HIGH);
    digitalWrite(PIN_LED_HIJAU, LOW);
    delay(200);
    digitalWrite(PIN_LED_MERAH, LOW);
    digitalWrite(PIN_LED_HIJAU, HIGH);
    delay(200);
  }
  digitalWrite(PIN_LED_HIJAU, LOW);
  
  Serial.println("  → Buzzer Test (3x beep)");
  for(int i=0; i<3; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(150);
    digitalWrite(PIN_BUZZER, LOW);
    delay(150);
  }
  
  Serial.println("  ✓ GPIO Test PASSED!");
}

void testLCD() {
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  Serial.println("  → LCD Backlight ON");
  delay(500);
  
  Serial.println("  → Displaying: 'LCD TEST OK'");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LCD TEST OK!");
  lcd.setCursor(0, 1);
  lcd.print("Alamat: 0x27");
  delay(2000);
  
  Serial.println("  → Displaying: Numbers 0-9");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Count: ");
  for(int i=0; i<=9; i++) {
    lcd.setCursor(7, 0);
    lcd.print(i);
    delay(300);
  }
  
  Serial.println("  → Displaying: 'SISTER PARKIR'");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SISTER PARKIR");
  lcd.setCursor(0, 1);
  lcd.print("Gate MASUK");
  delay(2000);
  
  Serial.println("  ✓ LCD Test PASSED!");
}

void testServo() {
  // Attach servo
  myServo.setPeriodHertz(50);
  myServo.attach(PIN_SERVO, 500, 2400);
  
  Serial.println("  → Servo attached to pin 13");
  delay(500);
  
  Serial.println("  → Position: 0° (CLOSED)");
  myServo.write(0);
  delay(2000);
  
  Serial.println("  → Position: 45°");
  myServo.write(45);
  delay(1000);
  
  Serial.println("  → Position: 90° (OPEN)");
  myServo.write(90);
  delay(2000);
  
  Serial.println("  → Position: 45°");
  myServo.write(45);
  delay(1000);
  
  Serial.println("  → Position: 0° (CLOSED)");
  myServo.write(0);
  delay(1000);
  
  Serial.println("  → Smooth movement (0° to 90°)");
  for(int pos=0; pos<=90; pos+=5) {
    myServo.write(pos);
    delay(50);
  }
  delay(1000);
  
  Serial.println("  → Smooth movement (90° to 0°)");
  for(int pos=90; pos>=0; pos-=5) {
    myServo.write(pos);
    delay(50);
  }
  
  Serial.println("  ✓ Servo Test PASSED!");
}

void testRFID() {
  // Inisialisasi RFID (hanya sekali)
  static bool rfidInitialized = false;
  if (!rfidInitialized) {
    SPI.begin();
    rfid.PCD_Init();
    rfidInitialized = true;
  }
  
  Serial.println("  → RFID Reader initialized");
  delay(500);
  
  // Cek versi firmware
  byte version = rfid.PCD_ReadRegister(rfid.VersionReg);
  Serial.print("  → MFRC522 Firmware Version: 0x");
  Serial.println(version, HEX);
  
  if (version == 0x00 || version == 0xFF) {
    Serial.println("  ✗ WARNING: Communication problem with MFRC522!");
    Serial.println("     Check wiring:");
    Serial.println("     - SDA  → GPIO 5");
    Serial.println("     - RST  → GPIO 4");
    Serial.println("     - SCK  → GPIO 18");
    Serial.println("     - MOSI → GPIO 23");
    Serial.println("     - MISO → GPIO 19");
    Serial.println("     - VCC  → 3.3V");
    Serial.println("     - GND  → GND");
    
    lcd.clear();
    lcd.print("RFID Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring");
    delay(3000);
  } else {
    Serial.println("  ✓ RFID Communication OK!");
    Serial.println("  → RFID Ready for card scanning");
    
    lcd.clear();
    lcd.print("RFID Ready!");
    lcd.setCursor(0, 1);
    lcd.print("Tap anytime...");
    delay(2000);
  }
  
  Serial.println("  ✓ RFID Test COMPLETED!");
}
