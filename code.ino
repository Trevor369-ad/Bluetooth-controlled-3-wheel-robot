// Include the Arduino Bluetooth Low Energy library
#include <ArduinoBLE.h>

// Define BLE service and characteristic UUIDs
BLEService controlService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEByteCharacteristic directionCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// Define pins for motor control
#define LEFT_MOTOR_SPEED  D2
#define RIGHT_MOTOR_SPEED  D3
#define FORWARD_RIGHT  D5
#define REVERSE_RIGHT  D4
#define FORWARD_LEFT  D7
#define REVERSE_LEFT  D6

// Function prototypes
void bleRover();
void forwardReverse();
void right(int dir);
void left(int dir);
void stop_();
void forward();
void reverse();
void turnLeft();
void turnRight();

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Set motor control pins as outputs
  pinMode(RIGHT_MOTOR_SPEED, OUTPUT);
  pinMode(LEFT_MOTOR_SPEED, OUTPUT);
  pinMode(FORWARD_RIGHT, OUTPUT);
  pinMode(REVERSE_RIGHT, OUTPUT);

  // Set initial motor speeds
  analogWrite(RIGHT_MOTOR_SPEED, 100);
  analogWrite(LEFT_MOTOR_SPEED, 100);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Bluetooth® Low Energy module failed to start");
    while (1);
  }

  // Set device name and advertise BLE service
  BLE.setLocalName("ROVER");
  BLE.setAdvertisedService(controlService);
  controlService.addCharacteristic(directionCharacteristic);
  BLE.addService(controlService);

  // Set initial direction value
  directionCharacteristic.writeValue(0);
  BLE.advertise();
}

void loop() {
  bleRover(); 
}

// Function to handle BLE communication
void bleRover() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      digitalWrite(LED_BUILTIN, HIGH);
      if (directionCharacteristic.written()) {
        switch(directionCharacteristic.value()){
          case 'F':
            forward();
            Serial.println("Moving forward");
            break;
          case 'R':
            reverse();
            Serial.println("Reversing");
            break;
          case 'L':
            turnLeft();
            forward();
            Serial.println("Turning Left");
            break;
          case 'T':
            turnRight();
            forward();
            Serial.println("Turning Right");
            break;
          case 'S':
            stop_();
            Serial.println("Stop");
            break;
        }
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

// Function to control right motor direction
void right(int dir) {
  if (dir == 1){
    digitalWrite(FORWARD_RIGHT, HIGH);
    digitalWrite(REVERSE_RIGHT, LOW);
  }

  if (dir == 2){
    digitalWrite(FORWARD_RIGHT, LOW);
    digitalWrite(REVERSE_RIGHT, HIGH);
  }

  if (dir == 3){
    digitalWrite(FORWARD_RIGHT, LOW);
    digitalWrite(REVERSE_RIGHT, LOW);
  }
}

// Function to control left motor direction
void left(int dir) {
  if (dir == 1){
    digitalWrite(FORWARD_LEFT, HIGH);
    digitalWrite(REVERSE_LEFT, LOW);
  }

  if (dir == 2){
    digitalWrite(FORWARD_LEFT, LOW);
    digitalWrite(REVERSE_LEFT, HIGH);
  }

  if (dir == 3){
    digitalWrite(FORWARD_LEFT, LOW);
    digitalWrite(REVERSE_LEFT, LOW);
  }
}

// Function to stop both motors
void stop_() {
  right(3);
  left(3);
}

// Function to move forward
void forward() {
  right(1);
  left(1);
  delay(400);
}

// Function to move in reverse
void reverse() {
  right(2);
  left(2);
}

// Function to turn left
void turnLeft() {
  right(1);
  left(2);
  delay(500);
}

// Function to turn right
void turnRight() {
  left(1);
  right(2);
  delay(500);
}
