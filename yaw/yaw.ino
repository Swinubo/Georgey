#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float yaw = 0.0; // Yaw angle in degrees
unsigned long prevTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully!");
  } else {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  prevTime = millis(); // Initialize time
}

void loop() {
  int16_t gz; // Gyroscope Z-axis value

  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0; // Calculate delta time in seconds
  prevTime = currentTime;

  // Get gyroscope Z-axis data directly
  gz = mpu.getRotationZ();
  float gyroZ = gz / 131.0; // Convert raw value to degrees/second

  // Integrate gyroscope data to estimate yaw
  yaw += gyroZ * dt;

  // Normalize yaw angle to 0-360 degrees
  yaw = fmod(yaw + 360.0, 360.0);

  yaw = yaw - 0.03;

  // Print yaw angle
  Serial.print("Yaw: ");
  Serial.println(yaw);

  delay(10); // Small delay for stability
}


/*#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float yaw = 0.0; // Yaw angle in degrees
unsigned long prevTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully!");
  } else {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  prevTime = millis(); // Initialize time
}

void loop() {
  // Print yaw angle
  Serial.print("Yaw: ");
  Serial.println(yawVal());
  delay(10); // Small delay for stability
}

int yawVal()
{
  int16_t gz; // Gyroscope Z-axis value

  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0; // Calculate delta time in seconds
  prevTime = currentTime;

  // Get gyroscope Z-axis data directly
  gz = mpu.getRotationZ();
  float gyroZ = gz / 131.0; // Convert raw value to degrees/second

  // Integrate gyroscope data to estimate yaw
  yaw += gyroZ * dt;

  // Normalize yaw angle to 0-360 degrees
  yaw = fmod(yaw + 360.0, 360.0);

  return yaw - 0.03;
}
*/
