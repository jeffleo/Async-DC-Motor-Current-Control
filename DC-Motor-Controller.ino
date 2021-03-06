#include "Mot_Ctrl.h"

//#define DEBUG_PERF

/**
 * 
 * param type, I2C address, pin1, pin2, curent_lim
 */
Motor Mot_A('A', 0x40, 9, 8, 69.0);   // 69ma minimum for front wheel
Motor Mot_B('B', 0x44, 11, 10, 70.0); // 60ma minimum for rear wheel

enum{
  speed_ctrl=false,
  trque_ctrl=true,
};

enum{
  fwd=1,
  rev=-1,
};

int8_t Roll_dir_state=0;                    // drive direction

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      delay(1);                             // will pause until serial console opens
  }

// Enable internal pullups for twi. Necessary for non-MegaX28 brds as I2C/Wire library doesn't support 
// Mega2560 
  #if defined(__AVR_ATmega2560__)
      pinMode(SDA, INPUT);    
      pinMode(SCL, INPUT);
      digitalWrite(SDA, 1);
      digitalWrite(SCL, 1);
//      Serial.println("I'm a mega with I2C pins: ");
//      Serial.println(SDA);                //20  checkme
//      Serial.println(SCL);                //21  checkme
  #endif
  
  Mot_A.setup();
  Mot_B.setup();
  Mot_A.stop();
  Mot_B.stop();

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(DRV_Sleep,HIGH);             // enable DRV
  Roll_dir_state = rev;               
  Roll(rev);   
}

// Communication
byte incomingByte;

// Maintenance and Performance
unsigned long last_time = 0;
bool Watchdog_LED_state = LOW;
unsigned long loop_time;                    // for measuring loop rate

void loop(void) 
{
// Watchdog Blinker (to detect system crash)
  if ( millis() > last_time + 1000){
    last_time = millis();
    digitalWrite(LED_BUILTIN, Watchdog_LED_state);
    Watchdog_LED_state=!Watchdog_LED_state;
  }

  Mot_A.ctrl_loop();
  Mot_B.ctrl_loop();


// measure loop rate for Arduino performance testing
#ifdef DEBUG_PERF
  Serial.println(micros() - loop_time);     // to check actual delay
  loop_time = micros();
#endif 

#ifdef DEBUG
  #ifdef DEBUG_PLOT_B
    if (!debug_called){
      Roll(fwd); 
      Mot_B.debug_enable = true;
      debug_called = true;
    }

  #endif
  #ifdef DEBUG_PLOT_A
    if (!debug_called){
      Roll(rev); 
      Mot_A.debug_enable = true;
      debug_called = true;
    }
  #endif
  
  if (Mot_A.debug_enable == true)
	  Mot_A.debug();                          // for printing drive PWM, I [mA] filtered I
  else if (Mot_B.debug_enable == true)
	  Mot_B.debug();         


  // serial commands for motor control
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print(incomingByte-48, DEC);
	  Serial.print(' ');
    
    switch(incomingByte-48) {
      case 0 :                            // 48 = ascii for '0'
        Mot_A.stop(); 
        Mot_B.stop();   
		    dvr_sleep();
        Roll_dir_state = 0;
        Serial.println("stopping motors");
		break;
      case 1 :			
        digitalWrite(DRV_Sleep,HIGH);     // enable DRV
				Roll_dir_state = fwd;             // todo drive slew
        Roll(fwd);                      
        Serial.println("calling roll fwd");
		break;
      case 2 :					
        digitalWrite(DRV_Sleep,HIGH);     // enable DRV
        Roll_dir_state = rev;		          // todo drive slew
        Roll(rev);                      
        Serial.println("calling roll rev");
		break;
	  case 3:							
		  Mot_B.debug_enable = true;
		  Mot_A.debug_enable = false;
		  Serial.println("enabled debug mot B");
		  break;
	  case 4:
		  Mot_A.debug_enable = true;           
		  Mot_B.debug_enable = false;
		  Serial.println("enabled debug mot A");
		  break;
	  case 5:
		  Mot_B.debug_enable = false;           
		  Mot_A.debug_enable = false;
		  Serial.println("disable debug");
		  break;
    case 6:                                
      Mot_A.ina_.scanBus();
      Serial.println("Scan I2C bus for devices complete");
      break;
    }

  }

#endif
}


void Roll(int8_t dir){
  if (dir == fwd ){
    Mot_A.drive(Default_speed, Mot_Polarity*dir, speed_ctrl);
//    Mot_B.drive(Default_speed, Mot_Polarity*dir, trque_ctrl);
    Mot_B.drive(Default_speed, Mot_Polarity*dir, speed_ctrl);
    
  } else if (dir == rev){
    Mot_B.drive(Default_speed, Mot_Polarity*dir, speed_ctrl);
//    Mot_A.drive(Default_speed, Mot_Polarity*dir, trque_ctrl);
    Mot_A.drive(Default_speed, Mot_Polarity*dir, speed_ctrl);
  }
}


void dvr_sleep(){
  digitalWrite(DRV_Sleep,LOW);
}



