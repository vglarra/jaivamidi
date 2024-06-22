#include <SD.h>


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // Wait for serial port to connect
  }

  // Initialize SD card
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized successfully!");
}

void loop() {
  // Create a file on the SD card
  File dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Hello, Teensy 4.1!"); // Write data to the file
    dataFile.close(); // Close the file
    Serial.println("Data written to data.txt");
  } else {
    Serial.println("Error opening data.txt");
  }

  // Read data from the file
  dataFile = SD.open("data.txt");
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read()); // Print data to serial monitor
    }
    dataFile.close();
  } else {
    Serial.println("Error reading data.txt");
  }

  delay(1000); // Wait for a second
}
