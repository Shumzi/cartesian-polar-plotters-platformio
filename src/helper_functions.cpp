// void home_when_press()
//     {
//         static bool is_pressed = false;
//         if(enc.is_pressed())
//         {
//             if(!is_pressed)
//             {
//                 stepper_x->moveTo(0);
//                 stepper_y->moveTo(0);
//                 while(stepper_x->run() || stepper_y->run())
//                 {
//                     stepper_x->run();
//                     stepper_y->run();
//                 }
//                 encoder_x.reset_counter();
//                 encoder_y.reset_counter();
//             }
//             is_pressed = true;
//         }
//         else
//         {
//             is_pressed = false;
//         }
//     }
    
//     void print_when_press(IEncoder* e)
//     {
//         static bool was_pressed = false;
//         if(e->is_pressed())
//         {
//             if(!was_pressed)
//             {
//                 Serial.print("Encoder X Count: ");
//                 Serial.println(current_x_pos);
//                 Serial.print(", Encoder Y Count: ");
//                 Serial.println(current_y_pos);
//             }
//             is_pressed = true;
//         }
//         else
//         {
//             is_pressed = false;
//         }
//     }