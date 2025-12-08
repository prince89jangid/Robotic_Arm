#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int joystick0, joystick1, joystick2, joystick3;
int servo0_pose, servo1_pose, servo2_pose, servo3_pose;

int joy_prev, joy_new, joy_init;
int old_pose;
int servo_current_pose;

int RTH_delay = 20;
int speed = 20;  // Enter speed Here (0-100)%


// ====================== Controller ==================


class joystick_controller {

  int pose[2];
public:

  int correct_pos;
  int temp = 0;

  int stp_fluct(int value) {

    joy_new = value;
    temp = joy_new - joy_prev;

    if (abs(temp) < 10) {

      correct_pos = joy_prev;
      return correct_pos;

    } else {

      correct_pos = joy_new;
      joy_prev = joy_new;
      return correct_pos;
    }
  }

  // ============ we are not using now =================

  int* positions(int new_pose) {
    if (new_pose != old_pose) {
      pose[0] = old_pose;
      pose[1] = new_pose;
      old_pose = new_pose;
      return pose;
    } else {
      pose[0] = old_pose;
      pose[1] = old_pose;
      return pose;
    }
  }
};



// ======================= Robotic Arm ==============================

class robotic_Arm {
public:

  void movement(int* joystick, int* servo_pose, int pin_no) {


    if (*joystick > joy_init + 10) {

      moveForward(servo_pose, pin_no);

    } else if (*joystick < joy_init - 10) {
      movebackward(servo_pose, pin_no);
    }
    return;
  }


  void moveForward(int* servo_pose, int pin_no) {


    while (*servo_pose <= 700) {

      *servo_pose = *servo_pose + 2;
      pwm.setPWM(pin_no, 0, *servo_pose);

      //=============center condition to stop ========================

      if (pin_no == 0) {
        if (analogRead(A0) < joy_init + 10 && analogRead(A0) > joy_init - 10) {
          return;
        }

      } else if (pin_no == 1) {
        if (analogRead(A1) < joy_init + 10 && analogRead(A1) > joy_init - 10) {
          return;
        }

      } else if (pin_no == 2) {
        if (analogRead(A2) < joy_init + 10 && analogRead(A2) > joy_init - 10) {
          return;
        }
      } else {
        if (analogRead(A3) < joy_init + 10 && analogRead(A3) > joy_init - 10) {
          return;
        }
      }

      delay(speed);
    }
  }

  void movebackward(int* servo_pose, int pin_no) {

    while (*servo_pose >= 0) {

      *servo_pose = *servo_pose - 2;
      pwm.setPWM(pin_no, 0, *servo_pose);

      //=============center condition to stop ========================

      if (pin_no == 0) {
        if (analogRead(A0) < joy_init + 10 && analogRead(A0) > joy_init - 10) {
          return;
        }

      } else if (pin_no == 1) {
        if (analogRead(A1) < joy_init + 10 && analogRead(A1) > joy_init - 10) {
          return;
        }

      } else if (pin_no == 2) {
        if (analogRead(A2) < joy_init + 10 && analogRead(A2) > joy_init - 10) {
          return;
        }
      } else {
        if (analogRead(A3) < joy_init + 10 && analogRead(A3) > joy_init - 10) {
          return;
        }
      }

      delay(speed);
    }
  }
};


// ============== Return to home =========
void return_to_home(int RTH) {
  if (RTH == 0) {

    // for servo0

    if (servo0_pose > joy_init + 10) {
      while (servo0_pose >= joy_init) {

        servo0_pose = servo0_pose - 2;
        pwm.setPWM(0, 0, servo0_pose);
        delay(RTH_delay);
      }
    } else if (servo0_pose < joy_init - 10) {
      while (servo0_pose <= joy_init) {
        servo0_pose = servo0_pose + 2;
        pwm.setPWM(0, 0, servo0_pose);
        delay(RTH_delay);
      }
    }

    // for servo1

    if (servo1_pose > joy_init + 10) {
      while (servo1_pose >= joy_init) {

        servo1_pose = servo1_pose - 2;
        pwm.setPWM(1, 0, servo1_pose);
        delay(RTH_delay);
      }
    } else if (servo1_pose < joy_init - 10) {
      while (servo1_pose <= joy_init) {
        servo1_pose = servo1_pose + 2;
        pwm.setPWM(1, 0, servo1_pose);
        delay(RTH_delay);
      }
    }


    // for servo2

    if (servo2_pose > joy_init + 10) {
      while (servo1_pose >= joy_init) {

        servo2_pose = servo2_pose - 2;
        pwm.setPWM(2, 0, servo2_pose);
        delay(RTH_delay);
      }
    } else if (servo2_pose < joy_init - 10) {
      while (servo2_pose <= joy_init) {
        servo2_pose = servo2_pose + 2;
        pwm.setPWM(2, 0, servo2_pose);
        delay(RTH_delay);
      }
    }


    //for servo3

    if (servo3_pose > joy_init + 10) {
      while (servo3_pose >= joy_init) {

        servo3_pose = servo3_pose - 2;
        pwm.setPWM(3, 0, servo3_pose);
        delay(RTH_delay);
      }
    } else if (servo3_pose < joy_init - 10) {
      while (servo3_pose <= joy_init) {
        servo3_pose = servo3_pose + 2;
        pwm.setPWM(3, 0, servo3_pose);
        delay(RTH_delay);
      }
    }
  }
}


// ============= objects here===============

joystick_controller controller;
robotic_Arm Servo_movement;
int* data;




// ================= main code ===========================


void setup() {


  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(10);
  joy_prev = analogRead(A0);
  joy_init = joy_prev;
  old_pose = joy_prev;

  pinMode(11, INPUT_PULLUP);

  servo0_pose = joy_init;
  servo1_pose = joy_init;
  servo2_pose = joy_init;
  servo3_pose = joy_init;


  // set value to initial joystick value
  pwm.setPWM(0, 0, servo0_pose);
  pwm.setPWM(1, 0, servo1_pose);
  pwm.setPWM(2, 0, servo2_pose);
  pwm.setPWM(3, 0, servo3_pose);
}


// ========================= Loop ===============================


void loop() {

  // so that value mil sake
  joystick0 = analogRead(A0);
  joystick1 = analogRead(A1);
  joystick2 = analogRead(A2);
  joystick3 = analogRead(A3);

  joystick0 = controller.stp_fluct(joystick0);
  joystick1 = controller.stp_fluct(joystick1);
  joystick2 = controller.stp_fluct(joystick2);
  joystick3 = controller.stp_fluct(joystick3);

  Servo_movement.movement(&joystick0, &servo0_pose, 0);
  Servo_movement.movement(&joystick1, &servo1_pose, 1);
  Servo_movement.movement(&joystick2, &servo2_pose, 2);
  Servo_movement.movement(&joystick3, &servo3_pose, 3);

  Serial.print("servo0_pose: ");
  Serial.print(servo0_pose);
  Serial.print("   servo1_pose: ");
  Serial.print(servo1_pose);
  Serial.print("   servo2_pose: ");
  Serial.print(servo2_pose);
  Serial.print("   servo3_pose: ");
  Serial.print(servo3_pose);

  int RTH = digitalRead(11);
  return_to_home(RTH);

  Serial.println();
}