
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <SBC_MotoDriver3.h>
#include <Wire.h>


#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <SBC_MotoDriver3.h>
#include <Wire.h>

void driveMotor(int left, int right);
SBCMotoDriver3 MotorDriver(0x15, 4);

const int LeftMotor_f = 0;
const int LeftMotor_r = 1;

const int RightMotor_f = 4;
const int RightMotor_r = 5;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int xaxisData; //4095
  int yaxisData;
  int Button_1;
  int Button_2;
  int Button_3;
} struct_message;

// Create a struct_message called myData
struct_message myData;
unsigned long timer ;
int xaxisData;
int yaxisData;

const int DEADZONE = 40; // Ignore joystick movement within +/- 30 of center
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("X axis: ");
  Serial.println(myData.xaxisData);
  Serial.print("Y axis: ");
  Serial.println(myData.yaxisData);
  Serial.print("Button_1: ");
  Serial.println(myData.Button_1);
  Serial.print("Button_2: ");
  Serial.println(myData.Button_2);
  Serial.print("Button_2: ");
  Serial.println(myData.Button_3);
  xaxisData = myData.xaxisData;
  yaxisData = myData.yaxisData;

}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println(WiFi.macAddress());
  // Motor driver setup
  MotorDriver.begin();
  MotorDriver.allOff();
  // Define the RPM and the maximum number of steps for the stepper motor
  MotorDriver.StepperSpeed(300, 2048);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {


  // Map values to range -255 to 255
  // Note: Adjust mapping if your joystick center isn't exactly 512
  int forward = map(yaxisData, 0, 4095, -255, 255);
  int turn = map(xaxisData, 0, 4095, -255, 255);
  
  if (abs(forward) < DEADZONE) forward = 0;
  if (abs(turn) < DEADZONE) turn = 0; 
  Serial.print("forward speed: ");
  Serial.println(forward);
  Serial.print("turn speed: ");
  Serial.println(turn);
  // Differential Drive Logic
  int leftSpeed = forward + turn;
  int rightSpeed = forward - turn;

  // Constrain speeds to PWM limits
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  Serial.print("left speed: ");
  Serial.println(leftSpeed);
  Serial.print("right speed: ");
  Serial.println(rightSpeed);
  if (millis()-timer>=100){
  driveMotor(leftSpeed, rightSpeed);
  timer = millis();
}

}
void driveMotor(int left, int right) {
  // Left Motor Control
  // Left Motor Control
  if (left > 0) {
    MotorDriver.pwm(0,left);
    MotorDriver.pwm(1,0);
  } else if (left < 0) {
    MotorDriver.pwm(0,0);
    MotorDriver.pwm(1,abs(left));
  } else {
    // Full stop if speed is 0
    MotorDriver.pwm(0,0);
    MotorDriver.pwm(1,0);
  }

  // Right Motor Control
  if (right > 0) {
    MotorDriver.pwm(6,right);
    MotorDriver.pwm(7,0);
  } else if (right < 0) {
    MotorDriver.pwm(6,0);
    MotorDriver.pwm(7,abs(right));
  } else {
    // Full stop if speed is 0
    MotorDriver.pwm(6,0);
    MotorDriver.pwm(7,0);
  }
} 
