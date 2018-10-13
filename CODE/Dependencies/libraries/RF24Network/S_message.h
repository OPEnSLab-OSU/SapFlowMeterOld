/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __S_MESSAGE_H__
#define __S_MESSAGE_H__

// STL headers
// C headers
// Framework headers
// Library headers
// Project headers

/**
 * Sensor message (type 'S') 
 */

struct S_message
{
  float Yaw_reading;
  float Pitch_reading;
  float Roll_reading;
  float AX_reading;
  float AY_reading;
  float AZ_reading;
  float GX_reading;
  float GY_reading;
  float GZ_reading;
  uint8_t num_readings;
  uint8_t reserved;
  static char buffer[];
  S_message(void): Yaw_reading(0), Pitch_reading(0), Roll_reading(0), AX_reading(0), AY_reading(0), AZ_reading(0), GX_reading(0), GY_reading(0), GZ_reading(0) {}
  char* toString(void);
};

#endif // __S_MESSAGE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
