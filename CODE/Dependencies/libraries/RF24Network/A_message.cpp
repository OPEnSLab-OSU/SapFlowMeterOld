/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

// STL headers
// C headers
// Framework headers
// Library headers
#include "RF24Network_config.h"
// Project headers
// This component's header
#include "A_message.h"

char A_message::buffer[36]; // was 12 chars per reading channel, so may need to go 108 long

/****************************************************************************/

char* A_message::toString(void)
{
  snprintf(buffer,sizeof(buffer),"%2u.%02u0 %2u.%02u1 %2u.%02u2 %2u.%02u3 %2u.%02u4 %2u.%02u5 %2u.%02u6 %2u.%02u7 %unum",
      A0_reading >> 8,
      A0_reading & 0xFF,
      A1_reading >> 8,
      A1_reading & 0xFF,
      A2_reading >> 8,
      A2_reading & 0xFF,
      A3_reading >> 8,
      A3_reading & 0xFF,
      A4_reading >> 8,
      A4_reading & 0xFF,
      A5_reading >> 8,
      A5_reading & 0xFF,
      A6_reading >> 8,
      A6_reading & 0xFF,
      A7_reading >> 8,
      A7_reading & 0xFF,
      num_readings
      //lost_packets
      );
  return buffer;
}

/****************************************************************************/
// vim:cin:ai:sts=2 sw=2 ft=cpp
