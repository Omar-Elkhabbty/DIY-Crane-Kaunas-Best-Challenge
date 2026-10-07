#include <Wire.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// إعدادات درايفر Joy-IT (شريحة PCA9634)
#define DRIVER_ADDR 0x60 
#define SDA_PIN 14 // 21 or 22
#define SCL_PIN 27  // 21 or 22

// --- تعريف السرعات المطلوبة ---
const int moveSpeed = 150;    // سرعة الحركة (متوسطة)
const int liftSpeed = 220;    // سرعة الرفع (عالية)
const int holdPower = 60;     // قوة الفرملة (Braking) لمنع السقوط
const int gripPower = 255;    // قوة الإمساك القصوى

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Tower_Of_Babel_Crane"); // اسم البلوتوث
  
  Wire.begin(SDA_PIN, SCL_PIN);
  initDriver();
  Serial.println("System Ready - APP Connected");
}

void initDriver() {
  writeRegister(0x00, 0x00); // إيقاظ الشريحة
  writeRegister(0x08, 0xAA); // تفعيل وضع PWM للمخارج 0-3
  writeRegister(0x09, 0xAA); // تفعيل وضع PWM للمخارج 4-7
  stopAll();
}

void writeRegister(byte reg, byte value) {
  Wire.beginTransmission(DRIVER_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// دالة التحكم في الموتور: m=رقم الموتور(0-3)، s=السرعة(0-255)، d=الاتجاه(1، -1، 0)
void setMotor(int m, int s, int d) {
  byte regBase = 0x02 + (m * 2);
  if (d == 1) { 
    writeRegister(regBase, s); 
    writeRegister(regBase + 1, 0); 
  } else if (d == -1) { 
    writeRegister(regBase, 0); 
    writeRegister(regBase + 1, s); 
  } else { 
    writeRegister(regBase, 0); 
    writeRegister(regBase + 1, 0); 
  }
}

void stopAll() {
  for(int i=0; i<4; i++) setMotor(i, 0, 0);
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    
    switch (cmd) {
      // --- حركة العربة (موتور 0 و 1) ---
      case 'F': setMotor(0, moveSpeed, 1);  setMotor(1, moveSpeed, 1); break; // للأمام
      case 'B': setMotor(0, moveSpeed, -1); setMotor(1, moveSpeed, -1); break; // للخلف
      case 'L': setMotor(0, moveSpeed, -1); setMotor(1, moveSpeed, 1);  break; // يسار
      case 'R': setMotor(0, moveSpeed, 1);  setMotor(1, moveSpeed, -1); break; // يمين
      case 'S': setMotor(0, 0, 0);          setMotor(1, 0, 0);          break; // توقف الحركة

      // --- محرك الرفع (موتور 2) ---
      case 'U': setMotor(2, liftSpeed, 1); break;   // ارفع طالما ضاغط
      case 'D': setMotor(2, liftSpeed, -1); break;  // انزل طالما ضاغط
      case 'H': setMotor(2, holdPower, 1); break;   // عند الترك: وضع الفرملة (Braking)

      // --- محرك الالتقاط (موتور 3) ---
      case 'G': setMotor(3, gripPower, 1); break;   // إمساك بأقصى قوة ويظل ممسكاً
      case 'O': setMotor(3, gripPower, -1); break;  // فتح الملقط لترك الجسم
      case 'X': setMotor(3, 0, 0); break;           // إرخاء الموتور تماماً
    }
  }
}