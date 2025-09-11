// TODO: put these in IMode maybe.

// #include "Arduino.h"
// #include "RotaryEncoder.h"
// void print_when_press(RotaryEncoder enc_1, RotaryEncoder enc_2)
// {
//     static bool is_pressed = false;
//     if(enc_1.is_pressed() || enc_2.is_pressed())
//     {
//         if(!is_pressed)
//         {
//             Serial.print("Encoder X Count: ");
//             Serial.println(current_x_pos);
//             Serial.print(", Encoder Y Count: ");
//             Serial.println(current_y_pos);
//         }
//         is_pressed = true;
//     }
//     else
//     {
//         is_pressed = false;
//     }
// }



// void home_when_press(Encoder enc)
// {
//         static bool is_pressed = false;
//     if(enc.is_pressed())
//     {
//         if(!is_pressed)
//         {
//             stepper_x.moveTo(0);
//             stepper_y.moveTo(0);
//             while(stepper_x.run() || stepper_y.run())
//             {
//                 stepper_x.run();
//                 stepper_y.run();
//             }
//             encoder_x.reset_counter();
//             encoder_y.reset_counter();
//         }
//         is_pressed = true;
//     }
//     else
//     {
//         is_pressed = false;
//     }
// }