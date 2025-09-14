#pragma once

#include <Arduino.h>
#include "IEncoder.h"
#include "Settings.h"

class RotaryEncoder : public IEncoder
{
  private:
    const int bit_0_pin_;
    const int bit_1_pin_;
    const int push_button_pin_;
    byte prev_encoder_read_;
    byte new_encoder_read_;
    // int concensus_read[4] = {0,0,0,0};
    // int concensus_read_loc = 0;
  public:  
  int readDelta()
    {
      prev_encoder_read_ = new_encoder_read_ ;
      new_encoder_read_ = (((digitalRead(bit_1_pin_)) << 1) + (digitalRead(bit_0_pin_))) ;
      byte check_direction  = (prev_encoder_read_ << 2) + new_encoder_read_  ; // x4 = 2 rotate left 

      switch (check_direction) // see https://commons.wikimedia.org/wiki/File:Incremental_directional_encoder.gif
      {
        case 1: case 7: case 8: case 14:
          // Serial.println("-1");
          return -1;
        case 2: case 4: case 11: case 13:
          // Serial.println("1");
          return 1;
        default:
          return 0;
      }
    }

    RotaryEncoder(int bit_0_pin,int bit_1_pin,int push_button_pin)
    :bit_0_pin_(bit_0_pin),bit_1_pin_(bit_1_pin),push_button_pin_(push_button_pin),
    prev_encoder_read_(0), new_encoder_read_(0)
    {
      pinMode(bit_0_pin_, INPUT_PULLUP);  
      pinMode(bit_1_pin_, INPUT_PULLUP);  
      pinMode(push_button_pin_, INPUT_PULLUP);
    }

    bool is_pressed(){
      return !digitalRead(push_button_pin_);
    }

};