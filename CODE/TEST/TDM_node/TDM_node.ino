// ================================================================
// ===              INCLUDE CONFIGURATION FILE                  ===
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     ===
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================

// Config has to be first has it hold all user specified options
#include "config.h"

// Preamble includes any relevant subroutine files based 
// on options specified in the above config
#include "loom_preamble.h"

#if transmit_check == 1
  unsigned long tc_lastUpdate;
  unsigned long tc_startMillis;
#endif

// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();	

  #if transmit_check == 1
   tc_lastUpdate = millis();
   tc_startMillis = millis();
  #endif

	// Any custom setup code
}

// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle bndl, send_bndl; 
  #if is_sapflow == 1
    run_sapflowmeter(&send_bndl);
  #endif

  #if transmit_check == 1
    unsigned long tc_currentMillis = millis();
    if (tc_currentMillis - tc_startMillis > checkdelay) {
      Serial.println("Data transmission ready...");
      tc_startMillis = tc_currentMillis;  
    }
  #endif
  
//  measure_sensors();
//  package_data(&send_bndl);
//    #if LOOM_DEBUG == 1
//      print_bundle(&send_bndl);
//    #endif
//  send_bundle(&send_bndl, LORA);
  
	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section


