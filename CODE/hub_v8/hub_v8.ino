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


// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{
  
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();	

	// Any custom setup code

}


// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle bndl, send_bndl; 
// uint8_t marco[] = "MARCO";
//  uint8_t marco_buf[RH_RF95_MAX_MESSAGE_LEN];
	// OSCBundle send_bndl;

//	receive_bundle(&bndl, LORA);
//	print_bundle(&bndl);

//	log_bundle(&bndl, PUSHINGBOX);

#if is_sapflow == 1
  run_sapflowmeter(&send_bndl);
#endif
//    char testmsg[LORA_MESSAGE_SIZE] = "marco";
//    LOOM_DEBUG_Println("Sending to node...");
//    if (manager.sendtoWait((uint8_t*)testmsg, strlen(testmsg)+1, LORA_NODE_ADDRESS)) {}
//    else {LOOM_DEBUG_Println("Failed to send bundle!");}
//    
//    if(manager.available()) {
//      uint8_t marco_buf[RH_RF95_MAX_MESSAGE_LEN];
//      uint8_t marco_len = sizeof(marco_buf);
//      uint8_t marco_from;
//      
//      if (manager.recvfromAck(marco_buf, &marco_len, &marco_from)){
//        Serial.print("Received"); Serial.print(marco_from, HEX); Serial.println((char*)marco_buf);
//        }//of if(manager.recvfromAck)
//      } //of if(manager.available)

	additional_loop_checks();

}
