#pragma once

/**
 * calculated dimensions of setup:
 * distance between motors
 * מרחק בין מנועים 90 סמ
בין מנוע לגבול בציר x and y - ~14cm
גבול תחתון - ~75 סמ.
1000 צעדים = 8סמ כבל.
מיקום ב-0 בערךךךך
ובמיקום 0 בערך 46 גובה 35 
length of cableat start pos - ~56cm
 */
#define MAX_SPEED 3000
#define ACCELERATION 2000

// MAGNETIC LIMIT SWITCHES
#define LEFT_LIMIT_SW_PIN 10
#define RIGHT_LIMIT_SW_PIN 11

#define STEPPER_STEPSIZE 2

// limits where 0 is top left motor point in mm.
#define POLAR_Y_MIN_LIMIT 160
#define POLAR_X_MIN_LIMIT 230
#define POLAR_Y_MAX_LIMIT 720
#define POLAR_X_MAX_LIMIT 670

// 1000 is 8cm from a simple test done.
#define STEPS_PER_MM (12.5)

#define MOTORS_DISTANCE (900)  // MM distance between the motors

// offset from motors to start of the canvas legal area
#define X_OFFSET (140)
#define Y_OFFSET (140)

// calibration final location in mm from left motor. no xy coords.
#define X_HOME 450
#define Y_HOME 370

#define X_MAZE_START 410
#define Y_MAZE_START 165