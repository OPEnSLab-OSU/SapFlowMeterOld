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
#include "S_message.h"

char S_message::buffer[36];

/****************************************************************************/
/*
char* S_message::toString(void)
{
  snprintf(buffer,sizeof(buffer),"%2u.%02uYaw %2u.%02uPitch %2u.%02uRoll %2u.%02uAX %2u.%02uAY %2u.%02uAZ %2u.%02uGX %2u.%02uGY %2u.%02uGZ",
      Yaw_reading >> 8,
      Yaw_reading & 0xFF,
      Pitch_reading >> 8,
      Pitch_reading & 0xFF,
      Roll_reading >> 8,
      Roll_reading & 0xFF,
      AX_reading >> 8,
      AX_reading & 0xFF,
      AY_reading >> 8,
      AY_reading & 0xFF,
      AZ_reading >> 8,
      AZ_reading & 0xFF,
      GX_reading >> 8,
      GX_reading & 0xFF,
      GY_reading >> 8,
      GY_reading & 0xFF,
      GZ_reading >> 8,
      GZ_reading & 0xFF
      //lost_packets
      );
  return buffer;
}
*/
/****************************************************************************/
// vim:cin:ai:sts=2 sw=2 ft=cpp
