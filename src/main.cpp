
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// ArmMotor             motor         11              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
using namespace vex;

//Define constants to use in the program
int THRESHOLD = 10;
int DURATION = 50;
int MIN_POSITION = 0;
int MAX_POSITION = 200;
int TARGET_POSITION_STEP = 15;
int MAX_VEL = 10;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Take the current position of the Arm as zero
  ArmMotor.setPosition(0, degrees);

  int targetPosition = 0;
  int axisMoving = 0;
  while(1){
    //Get the y-coordinate of the right joystick
    int controllerY = Controller1.Axis2.position(percent);

    if(controllerY > THRESHOLD){
        // If the axis 2 is tapped up...
        if(axisMoving == 0){
        // increase the target position of the arm.
          targetPosition += TARGET_POSITION_STEP;
          // The axisMoving variable makes it so that the position of the arm is changed by the step
          // only once for each tap of the axis.
          axisMoving = 1;
        }
      
    }
    else if(controllerY < -THRESHOLD){
      // If the axis 2 is tapped down...
      if(axisMoving == 0){
      // decrease the target position of the arm.
          targetPosition -= TARGET_POSITION_STEP;
          axisMoving = 1;
      }
    }
    else{
      // reset the axisMoving variable when Axis 2 is in the middle
      axisMoving = 0;
    }

    //These three lines set the target position of the arm based on the button pressed. 
    if(Controller1.ButtonX.pressing()){
      targetPosition = MAX_POSITION;
    }
    else if(Controller1.ButtonY.pressing()){
      targetPosition = MAX_POSITION/2;
    }
    else if(Controller1.ButtonB.pressing()){
      targetPosition = MIN_POSITION;
    }

    //This block makes sure the target position is always between the minimum and maximum position.
    if(targetPosition < MIN_POSITION){
            targetPosition = MIN_POSITION;
          }
    else if(targetPosition > MAX_POSITION){
            targetPosition = MAX_POSITION;
    }

    // This is a proportional control loop. It calculates the error as the difference between
    // the current position of the arm and the target.
    int error =  targetPosition - ArmMotor.position(degrees);
    // The velocity of the motor is set based on the size of this error.
    float velocity = error*0.5;

    // This code block keeps the velocity of the motor to be +/- the maximum velocity variable set above.
    if(velocity>MAX_VEL){
      velocity = MAX_VEL;
    }
    if(velocity < - MAX_VEL){
      velocity = -MAX_VEL;
    }
   
    // Set the velocity of the motor and spin the motor at that velocity.
    ArmMotor.setVelocity(velocity,percent);
    ArmMotor.spin(forward);
    
    //Show the target position of the arm on the screen.
    Brain.Screen.clearLine();
    Brain.Screen.print(targetPosition);
    //Show the target position of the arm on the controller screen.
    Controller1.Screen.clearLine();
    Controller1.Screen.print(targetPosition);
  }
}
