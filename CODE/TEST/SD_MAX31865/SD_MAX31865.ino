/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>
#include <Adafruit_MAX31865.h>

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 max0 = Adafruit_MAX31865(A0, 9, 6, 5);
Adafruit_MAX31865 max1 = Adafruit_MAX31865(A1, 9, 6, 5);

// use hardware SPI, just pass in the CS pin
#define PIN_SPI_MISO         (22u)
#define PIN_SPI_MOSI         (23u)
#define PIN_SPI_SCK          (24u)
//Adafruit_MAX31865 max0 = Adafruit_MAX31865(A3, PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);
//Adafruit_MAX31865 max1 = Adafruit_MAX31865(A5, PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

File myFile;

void setup() 
{
    digitalWrite(8, HIGH);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
    Serial.println("Adafruit MAX31865 PT100 Sensor Test!");
  max0.begin(MAX31865_2WIRE);  // set to 2WIRE or 4WIRE as necessary
  max1.begin(MAX31865_2WIRE);  // set to 2WIRE or 4WIRE as necessary
  max0.clearFault();
  max1.clearFault();
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

//  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }

  
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
  uint16_t rtd0 = max0.readRTD();
  uint16_t rtd1 = max1.readRTD();

// Probe 0
  Serial.println("[Probe 0]");
  Serial.print("RTD value: "); Serial.println(rtd0);
  
  float ratio0 = rtd0;
  ratio0 /= 32768;
  
  Serial.print("Ratio = "); Serial.println(ratio0,8);
  Serial.print("Resistance = "); Serial.println(RREF*ratio0,8);
  Serial.print("Temperature = "); Serial.println(max0.temperature(RNOMINAL, RREF));

  // Check and print any faults
  uint8_t fault0 = max0.readFault();
  if (fault0) {
    Serial.print("Fault 0x"); Serial.println(fault0, HEX);
    if (fault0 & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault0 & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault0 & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault0 & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault0 & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault0 & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    max0.clearFault();
  }
  delay(1000);
  //probe 1
  
  Serial.println("[Probe 1]");
  Serial.print("RTD value: "); Serial.println(rtd1);
  float ratio1 = rtd1;
  ratio1 /= 32768;
  Serial.print("Ratio = "); Serial.println(ratio1,8);
  Serial.print("Resistance = "); Serial.println(RREF*ratio1,8);
  Serial.print("Temperature = "); Serial.println(max1.temperature(RNOMINAL, RREF));

  // Check and print any faults
  uint8_t fault1 = max1.readFault();
  if (fault1) {
    Serial.print("Fault 0x"); Serial.println(fault1, HEX);
    if (fault1 & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault1 & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault1 & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault1 & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault1 & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault1 & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    max1.clearFault();
  }
  Serial.println();
  delay(2000);
}


