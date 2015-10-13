struct stepper {
  const unsigned int dir_pin;
  const unsigned int step_pin;
  const unsigned int ms1_pin;
  const unsigned int ms2_pin;
  const unsigned int spr;   // steps per revolution
  const unsigned int delay; // in microseconds, controls the speed of the motor
  unsigned long last_step;  // timestamp of last step
};


/* n steps per revolution
 * m revolutions per s
 */
