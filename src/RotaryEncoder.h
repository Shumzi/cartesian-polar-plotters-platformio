#pragma once

#include "Settings.h"
#include "IEncoder.h"
#include <Arduino.h>


class RotaryEncoder : public IEncoder{
  private:
  const int bit_0_pin_;
  const int bit_1_pin_;
  const int push_button_pin_;
  
  unsigned long time_stamp_;
  
  byte prev_encoder_read_;
  byte new_encoder_read_;
  // negative value for clockwise spin  
  
  
  public:
  static bool is_pressed_;
  int last_direction_;

  int counter_;
  int prev_counter_;
  int direction_;
  int steps_to_move_;
  int readDelta();
  RotaryEncoder(int bit_0_pin,int bit_1_pin,int push_button_pin);
  
/**
  * read the encoders' current value. 
  * @retun the encoder current value (int)
  */
  int read_encoder();

  /**
  * Set the head current movement direction, based on the encoder.
  */
  void set_direction();

  /**
  * intialize the encoders' default values.
  */
  void initialize_encoder();
  
  /**
  * print the encoders' current counter value.
  */
  void print_current_counter();

  /**
  * returns the encoders' button state.
  * @retun true one of the encoders is currently pressed.
  */
  bool is_pressed();
}