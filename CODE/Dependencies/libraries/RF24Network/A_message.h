/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __A_MESSAGE_H__
#define __A_MESSAGE_H__

// STL headers
// C headers
// Framework headers
// Library headers
// Project headers

/**
 * Sensor message (type 'A') 
 */

struct A_message
{
  uint16_t A0_reading;
  uint16_t A1_reading;
  uint16_t A2_reading;
  uint16_t A3_reading;
  uint16_t A4_reading;
  uint16_t A5_reading;
  uint16_t A6_reading;
  uint16_t A7_reading;
 // uint8_t lost_packets;
  uint8_t num_readings;
  uint8_t reserved;
  static char buffer[];
  A_message(void): A0_reading(0), A1_reading(0), A2_reading(0), A3_reading(0), A4_reading(0), A5_reading(0), A6_reading(0), A7_reading(0) {}
  char* toString(void);
};

#endif // __A_MESSAGE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
