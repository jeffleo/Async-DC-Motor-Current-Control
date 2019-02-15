# DC Motor Controller

DC motor voltage and closed-loop current controller in a convenient header only class 'Mot_Ctrl.h'. 
<br />
- Motor speed is open loop controlled via PWM and change is moderated using slew rates on PWM. 
- Motor torque is closed loop controlled via a simple current limiting algorithm with I2C current sensor, keeping it within desired current bounds.
- Outputs to PWM control motor driver (e.g. TI DRV8833 in this use case).

Usage here was developed for Arduino to control multiple motors psuedo asyncronously. <br />
Class dependant on TI INA219 current sensor as feedback over I2C using [firmware](https://github.com/jeffleo/Arduino-INA219) for Arduino/Atmel Atmega hardware.
