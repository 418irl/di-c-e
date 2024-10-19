#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// MPU6050 instance
Adafruit_MPU6050 mpu;


// Stopwatch variables
bool stopwatchRunning = false;
unsigned long startTime = 0;
unsigned long stopTime = 0;

// Counter for invalid states
int invalidStateCount = 0;

// Time display variables
unsigned long elapsedTime; // To store elapsed time in milliseconds
unsigned long seconds;
unsigned long minutes;


void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}


void loop() {
    // Get new sensor events with the readings
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Print out the values
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    double ax = a.acceleration.x;

    String (f);

    // Determine face and manage stopwatch
    if (ax > 0.04 && ax < 0.5) {
        Serial.println("face 1");
        f = "face 1";
        if (!stopwatchRunning) {
            stopwatchRunning = true;
            startTime = millis(); // Record start time
            Serial.println("Stopwatch started at: " + String(startTime));
        }
    } 
    else if (ax > -1.0 && ax < -0.05) {  
        Serial.println("face 2");
        f = "face 2";
        if (!stopwatchRunning) {
            stopwatchRunning = true;
            startTime = millis(); // Record start time
            Serial.println("Stopwatch started at: " + String(startTime));
        } 
    } 
    else if (ax < -0.60 && ax > -1.1) {  
        Serial.println("face 3");
        f = "face 3";
        if (!stopwatchRunning) {
            stopwatchRunning = true;
            startTime = millis(); // Record start time
            Serial.println("Stopwatch started at: " + String(startTime));
        } 
    } 
    else if (ax > 0.7 && ax < 1.3) {
        Serial.println("face 4");
        f = "face 4";
        if (!stopwatchRunning) {
            stopwatchRunning = true;
            startTime = millis(); // Record start time
            Serial.println("Stopwatch started at: " + String(startTime));
        } 
    } 

    else if (ax < -8.0 && ax > -10.0) {  // Corrected range
    Serial.println("reset");
    if (stopwatchRunning) {
            stopwatchRunning = false;
            stopTime = millis(); // Record stop time
            Serial.println("Stopwatch stopped at: " + String(stopTime));

            // Calculate elapsed time
            elapsedTime = stopTime - startTime; // Time taken in milliseconds
            seconds = (elapsedTime / 1000) % 60; // Calculate seconds
            minutes = (elapsedTime / 1000) / 60; // Calculate minutes
          Serial.println("Duration="+ String(minutes) + ":" + String(seconds));
    }}
    else if (ax > 2.0 || ax < -2.0) {
        Serial.println("invalid 1");
        f = "invalid 1";
        invalidStateCount++; // Increment the counter for invalid states
    }
    else {
        Serial.println("invalid 3");
        f = "invalid 3";
        invalidStateCount++; // Increment the counter for invalid states
    }

    // Print the number of invalid states
    Serial.print("Invalid states reached: ");
    Serial.println(invalidStateCount);

    String v = String("{\"Mode\":{\"Activity\":\"") + String(f) +
                         "\",\"Time Taken\":\"" + String(minutes) + ":" + String(seconds) +
                         "\",\"Start Time\":" + String(startTime) +
                         ",\"Stop Time\":" + String(stopTime) + 
                         ",\"Invalid States Count\":" + invalidStateCount + 
                         "}}";

    Serial.println(v);

    Serial.println("");  
    delay(500);
}
