/*
  MonitorDMS
  Reads an analog input using a ADS1231, converts it to strain, and prints the result to the serial monitor.
 
 This example code is in the public domain.
 */

 #include <ADS1231.h>

 
 ADS1231 myDMS;  // create ADS1231 object to monitor a DMS  

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
//  SPI.begin();
//  digitalWrite(SS, LOW);
  //ADS DRDY/DOUT <--------> Arduino Pin 12, ICSP-1 (MISO)
  //ADS SCLK <-------------> Arduino Pin 13, ICSP-3 (SCK)
  //ADS PDWN <-------------> Arduino Pin 10 (SS)
  // attach the DMS on pin SCK, MISO, SS
//  myDMS.attach( 13 , 12 , 10 );
  myDMS.attach( SCK , MISO , 10 );
  Serial.println("ADS1231");
  Serial.println("time [ms] , sensor , dt sensor read [us]");
}

// the loop routine runs over and over again forever:
void loop() {
  // check for data ready
//  digitalWrite(SS,LOW);
  if( myDMS.check() ) {
    // get start time of read
    unsigned long timeStart = micros();
    // read the input on ADS1231:
    long sensorValue = myDMS.readData();
    // get final time of read
    unsigned long timeEnd = micros();
    // print out the time, the value read and time required for read:
    Serial.print(millis());
    Serial.print(" , ");
    Serial.print(sensorValue);
    Serial.print(" , ");
    Serial.println(timeEnd-timeStart);
  }

//  digitalWrite(SS,HIGH);
}
