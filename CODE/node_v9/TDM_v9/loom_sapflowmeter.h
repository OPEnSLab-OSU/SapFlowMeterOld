// ---------- Main LOOP -----------------
//  #if is_sapflow == 1
//    run_sapflowmeter(&send_bndl);
//  #endif
// --------------------------------------  
// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <Adafruit_MAX31865.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define HEATPIN 12

#if sensor_type == 1  //0: NTC, 1: RTD
// Use software SPI: CS, DI, DO, CLK
    #define PIN_SPI_CS0           A0
    #define PIN_SPI_CS1           A1
    #define PIN_SPI_MISO          6
    #define PIN_SPI_MOSI          9
    #define PIN_SPI_SCK           5


//   Use hardware SPI, just pass in the CS pin
//  #define PIN_SPI_CS0           A3
//  #define PIN_SPI_CS1           A5
//  #define PIN_SPI_MISO         (22u)
//  #define PIN_SPI_MOSI         (23u)
//  #define PIN_SPI_SCK          (24u)

  Adafruit_MAX31865 max0 = Adafruit_MAX31865(PIN_SPI_CS0, PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);
  Adafruit_MAX31865 max1 = Adafruit_MAX31865(PIN_SPI_CS1, PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);

  // The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
  #define RREF      430.0
  // The 'nominal' 0-degrees-C resistance of the sensor
  // 100.0 for PT100, 1000.0 for PT1000
  #define RNOMINAL0  101.45
  #define RNOMINAL1  101.60 //RNOMINAL0 + 0.15(depends on RTD resistance)

#endif// of sensor_type == 1
// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_sapflow_t {
  float temp0, temp1, temp_diff;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

unsigned long lastUpdate;//heat duration
unsigned long currentTime;//heat duration

unsigned long lastPulseUpdate;//heat pulse
unsigned long currentPulseTime;//heat pulse

unsigned long startMillis;//send data
unsigned long currentMillis;//send data

bool sapflow_relay_on = false;
bool pulseflag = false;

struct state_sapflow_t state_sapflow;
uint8_t testcounter;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
#if hub_node_type == 1 // 0: HUB, 1:NODE
void package_sapflow(OSCBundle *bndl, char packet_header_string[]);
void measure_sapflow();
double voltTotemp(double vout);
void heat();
void testsenddata();
void senddata();
#endif
void setup_sapflow();
void measure_sapflow();
void run_sapflowmeter(OSCBundle *bndl);

// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any startup for sapflowmeter that should occur on device startup
// 
// @return  Whether or not sensor initialization was successful
//
void setup_sapflow() 
{
  digitalWrite(8, HIGH);			//For using both LoRa and SD card

//  pinMode(A3, INPUT_PULLUP); 
//  pinMode(A5, INPUT_PULLUP);  
  
  #if hub_node_type == 1 //NODE
    setup_sht31d();					//setup Ambient temperature
//    sd_delete_file(FileName);		//Delete csv file
    pinMode(HEATPIN,OUTPUT);		//setup heater resistor
    lastUpdate = millis();
    startMillis = millis();
    lastPulseUpdate = millis();
    testcounter = 0;
    #if sensor_type == 1
      max0.begin(MAX31865_2WIRE);  // set to 2WIRE or 4WIRE as necessary
      max1.begin(MAX31865_2WIRE);  // set to 2WIRE or 4WIRE as necessary
      max0.clearFault();
      max1.clearFault();
    #endif //of sensor_type == 1
  #endif //of hub_node_type == 1
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

#if hub_node_type == 1 //NODE

// --- SAP FLOW METER
//Convert voltage output to temperature value
double voltTotemp( double vout )
{ 
  double vcc = 3300; //3.3V
  double Ro = 100000.0;
  double To = 298.15;
  double B = 4700.0;

  double Rbias = 100000.0;
  double long R = ((vcc / vout) - 1) * Ro;

  double long T = B / ( log( R * exp( B / To) ) - (log(Ro) ) ) - 273.15 ;
  return T;
}

void senddata()
{
  OSCBundle bndl;
 
  currentMillis = millis();
  if (currentMillis - startMillis >= senddelay) {
    measure_sensors();    //"loom_interface.h"
    package_data(&bndl);  //"loom_interface.h"
    #if LOOM_DEBUG == 1
      print_bundle(&bndl);
    #endif
    #if is_lora == 1
//    send_bundle(&bndl, LORA); //
      lora_send_bundle(&bndl); 
    #endif
    #if is_sd == 1
      sd_save_bundle(FileName, &bndl, 0, 3);
      //  read_all_from_file("newlog");
    #endif
    startMillis = currentMillis;
  }
}

void testsenddata()
{
  OSCBundle bndl;

  if (currentMillis - startMillis >= 6000) {
    if (testcounter <= 2) {
      measure_sensors();
      Serial.print("\n\n< Data log test [");Serial.print(testcounter+1);Serial.print("/3]... >\n");
      package_data(&bndl);
      #if is_lora == 1
      lora_send_bundle(&bndl);
      #endif
      
      #if LOOM_DEBUG == 1
        print_bundle(&bndl);
      #endif
      #if is_sd == 1
        sd_save_bundle(FileName, &bndl, 0, 3);
      #endif
      testcounter++;
    }
  }
}


// --- HEAT ---
//
// Turn on heater every "heatpulse" seconds
//
void heat()
{
  currentTime = millis();
  
  #if probe_type  == 0      // 0:TDM, 1: HRM
      if ( (currentTime - lastUpdate >= senddelay - heatduration) && (currentTime - lastUpdate < senddelay) ){//turn on 10min before sending data
        digitalWrite(HEATPIN, HIGH); //turn on heater
      }
      else if(currentTime - lastUpdate >= senddelay)
      {
        digitalWrite(HEATPIN, HIGH); //turn on heater
        lastUpdate = currentTime;
        pulseflag = true;
      }
      if(pulseflag){
        if(currentTime - lastUpdate >= 10000){
          digitalWrite(HEATPIN, LOW); //turn off heater
          pulseflag = false;
        } 
      }   
  #endif //of probe_type == 0
  
  #if probe_type == 1      // 0:TDM, 1: HRM
    currentPulseTime = millis();
    
    if ( (currentTime - lastUpdate >= senddelay - heatduration) && (currentTime - lastUpdate < senddelay) ){//turn on pulse 1 min before sending data
      if (currentPulseTime - lastPulseUpdate > heatpulse) {//turn on/off every 2.5 sec
        digitalWrite(HEATPIN, sapflow_relay_on ? HIGH : LOW);
        sapflow_relay_on = !sapflow_relay_on;
        lastPulseUpdate = currentPulseTime;  
      }
    }
    else if(currentTime - lastUpdate >= senddelay){ 
      if (currentPulseTime - lastPulseUpdate > heatpulse) {//turn on/off every 2.5 sec
        digitalWrite(HEATPIN, sapflow_relay_on ? HIGH : LOW);
        sapflow_relay_on = !sapflow_relay_on;
        lastPulseUpdate = currentPulseTime;  
      }
      lastUpdate = currentTime;
      pulseflag = true;  
    }
    if(pulseflag){
      if (currentPulseTime - lastPulseUpdate > heatpulse) {//turn on/off every 2.5 sec
        digitalWrite(HEATPIN, sapflow_relay_on ? HIGH : LOW);
        sapflow_relay_on = !sapflow_relay_on;
        lastPulseUpdate = currentPulseTime;  
      }
      if(currentTime - lastUpdate >= 10000){ //Turn off pulse 10 sec after sending data
        digitalWrite(HEATPIN, LOW); //turn off heater
        pulseflag = false;
      } 
    }
  #endif //of probe_type == 1
  

}

// Packaging function
void package_sapflow(OSCBundle *bndl, char packet_header_string[]) 
{
  char addressString[255];
  
  sprintf(addressString, "%s%s", packet_header_string, "/temp0");
  bndl->add(addressString).add(state_sapflow.temp0);
  sprintf(addressString, "%s%s", packet_header_string, "/temp1");
  bndl->add(addressString ).add(state_sapflow.temp1);
  sprintf(addressString, "%s%s", packet_header_string, "/temp_diff");
  bndl->add(addressString).add(state_sapflow.temp_diff);
}


// --- MEASURE Temperature ---
//
// Measure analog data and update analog state to most recent readings. 
//
void measure_sapflow()  
{
  pinMode(VBATPIN, OUTPUT);

  #if sensor_type == 0 // NTC thermistor
    double temp;
  
    temp = read_analog(0);  //analog Read gives values from 0-1023(or 4095 at 12bit) based on 0-3.3V
    temp = map(temp, 0, 4095, 0, 3300);//map these to mV value
    state_sapflow.temp0 = voltTotemp(temp);
  
    temp = read_analog(1);  //analog Read gives values from 0-1023 based on 0-3.3V
    temp = map(temp, 0, 4095, 0, 3300);//map these to mV value
    state_sapflow.temp1 = voltTotemp(temp);
  
    state_sapflow.temp_diff = state_sapflow.temp0 - state_sapflow.temp1;
    if (state_sapflow.temp_diff < 0) {
      state_sapflow.temp_diff *= -1;
    }
  #endif //of sensor_type == 0

  #if sensor_type == 1 // RTD temperature sensor
    uint16_t rtd0; 
    uint16_t rtd1;
    float ratio0;
    float ratio1;
    float approxtemp0 = 0;
    float approxtemp1 = 0;
    
    for (int i=0; i <= 9; i++){ //Measure temperature for 10 times
      rtd0 = max0.readRTD();
      rtd1 = max1.readRTD();
      ratio0 = rtd0;
      ratio1= rtd1;
      ratio0 /= 32767;
      ratio1 /= 32767;
      digitalWrite(8, HIGH);
      approxtemp0 += max0.temperature(RNOMINAL0, RREF);
      approxtemp1 += max1.temperature(RNOMINAL1, RREF);
      delay(5);
    }
    state_sapflow.temp0 = approxtemp0/10;	//Average 10 temperature data
    state_sapflow.temp1 = approxtemp1/10;	//Average 10 temperature data
    state_sapflow.temp_diff = state_sapflow.temp0 - state_sapflow.temp1;
    // Serial.print("RTD temp values: ");
    // Serial.print(state_sapflow.temp0);Serial.print(" , ");
    // Serial.println(state_sapflow.temp1);
    
    // Check and print any faults
    uint8_t fault0 = max0.readFault();
    uint8_t fault1 = max1.readFault();
    if (fault0) {
      Serial.println("[probe0]"); 
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
    if (fault1) {
      Serial.println("[probe1]"); 
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
   #endif
}

#endif // of is_node == 1

void run_sapflowmeter(OSCBundle *bndl)
{
    #if hub_node_type == 0 // HUB
    // Receive bundles, takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
      //receive_bundle(bndl, LORA);
      lora_receive_bundle(bndl);		//receive bundle through LoRa
      
      if (bundle_empty(bndl)) return;

      #if is_pushingbox == 1
        log_bundle(bndl, PUSHINGBOX);
      #endif
      #if is_sd == 1
        sd_save_bundle(FileName, bndl, 0, 3);
      #endif
      print_bundle(bndl);

    #endif // of is_hub

    #if hub_node_type == 1 //NODE
      testsenddata(); 
      if(testcounter>=2){
        senddata();  
        heat();
      }
    #endif // of is_node
}



