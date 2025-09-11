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
  
    unsigned long time_stamp_;
  
    byte prev_encoder_read_;
    byte new_encoder_read_;
    public:
    int readDelta()
    {
      prev_encoder_read_ = new_encoder_read_ ;
      new_encoder_read_ = (((digitalRead(bit_1_pin_)) << 1) + (digitalRead(bit_0_pin_))) ;
      byte check_direction  = (prev_encoder_read_ << 2) + new_encoder_read_  ; // x4 = 2 rotate left 
      switch (check_direction) // see https://commons.wikimedia.org/wiki/File:Incremental_directional_encoder.gif
      {
        case 1: case 7: case 8: case 14:
          return -1;
        case 2: case 4: case 11: case 13:
          return 1;
        default:
          return 0;
      }
    }

  RotaryEncoder(int bit_0_pin,int bit_1_pin,int push_button_pin)
  :bit_0_pin_(bit_0_pin),bit_1_pin_(bit_1_pin),push_button_pin_(push_button_pin),time_stamp_(0),
  prev_encoder_read_(0), new_encoder_read_(0)
  {
    pinMode(bit_0_pin_, INPUT_PULLUP);  
    pinMode(bit_1_pin_, INPUT_PULLUP);  
    pinMode(push_button_pin_, INPUT_PULLUP);
    time_stamp_ = millis();
  }



  bool is_pressed(){
  return !digitalRead(push_button_pin_);
  }
  
//  void read_encoder_toggle_press(Encoder &encoder_a, Encoder &encoder_b, int *UV_state)
//  {
//     if(!Encoder::is_pressed_ && (encoder_a.is_pressed() || encoder_b.is_pressed()))
//     {
//       Encoder::is_pressed_ = true;
//     }
//  }

};