/*
* Wiring Details
* white  - ground
* red    - 3.3+v  - 5 volts seems to work
* green  - data   - Analog 4
* yellow - clock  - Analog 5
*/

#include <Wire.h>
#include <Servo.h>
#include <Nunchuk.h>

// Create Objects
Nunchuk nunchuk = Nunchuk();
Servo SkateESC;

int speed = 90;

#define TopBorder 130
#define BottomBorder 124
#define NormalA 1
#define NormalB 2
#define Minimal 90

void setup() {
  delay(5000);
  nunchuk.init();
  SkateESC.attach(9);
}

void loop() {
  nunchuk.update();
  if(nunchuk.zButton){
    cruiseMode();
  } else {
    speed = 90;
  }
  SkateESC.write(speed);
  delay(70);
}

void cruiseMode(){
  if(nunchuk.analogY < BottomBorder){
    if (speed > 40) {
      speed = speed - NormalB;
    }
  }
  if(nunchuk.analogY > TopBorder) {
    if (speed < 180) {
      speed = speed + NormalA;
    }
  }
}
