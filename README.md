DC motor voltage and current controller with a current sensor in a convenient header only class 'Mot_Ctrl.h'. 
<br />
Motor speed is controlled via PWM and change is moderated using slew rates on PWM. 
Motor torque is controlled via a simple current limiting algorithm, keeping it within desired current bounds.
Outputs to PWM control motor driver (e.g. TI DRV8833 in this use case).
<br />
<br />
Usage here was developed for Arduino to control multiple motors psuedo asyncronously.
Depends on TI INA219 current sensor as feedback over I2C ([software](https://github.com/jeffleo/Arduino-INA219) is developed for Arduino hardware).<br />
