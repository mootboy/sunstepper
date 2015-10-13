#include "stepper.h"
#define LEFT HIGH
#define RIGHT LOW
// LDR pin connections
#define ldrlt 0 //LDR top left
#define ldrrt 1 //LDR top rigt
#define ldrld 2 //LDR down left
#define ldrrd 3 //LDR down rigt

const int dtime = 25; // when pot, remove const and move to loop
const int tol = 10;
// dir, step, ms1, ms2, spr, speed, timestamp
stepper hstep = {10, 9, 8, 7, 200, 2, 0}; // green, orange, brown, yellow
stepper vstep = {5, 4, 3, 2, 200, 2, 0};

stepper motor(unsigned int dir_pin,
              unsigned int step_pin,
              unsigned int ms1_pin,
              unsigned int ms2_pin,
              unsigned int spr,
              unsigned int speed) {
	unsigned int delay = 1000000 / (spr * speed);
	stepper s = {dir_pin, step_pin, ms1_pin, ms2_pin, spr, delay, 0};
	return s;
}

void init_stepper(stepper *s) {
	pinMode(s->ms1_pin, OUTPUT);
	pinMode(s->ms2_pin, OUTPUT);
	pinMode(s->dir_pin, OUTPUT);
	pinMode(s->step_pin, OUTPUT);
	digitalWrite(s->ms1_pin, LOW);
	digitalWrite(s->ms2_pin, LOW);
	s->last_step = micros();
}

void step_motor(stepper *s, int dir) {
	digitalWrite(s->dir_pin, dir);
	digitalWrite(s->step_pin, HIGH);
	unsigned long current = micros();
	// if current is less than last we have had an overflow and need to step
	// TODO: calc the correct diff.
	if ((current < s->last_step) || ((current - s->last_step) >= s->delay)) {
		digitalWrite(s->step_pin, LOW); // ready for next step
		s->last_step = current;
	}
}

void update_angle(int avg1, int avg2, stepper *stepper) {
	int delta = avg1 - avg2;
	if (-1 * tol > delta || delta > tol) // check if the difference is in the tolerance else change vertical angle
	{
		if (avg1 > avg2) {
			step_motor(stepper, RIGHT);
		} else if (avg1 < avg2) {
			step_motor(stepper, LEFT);
		}
	}
}

void setup()
{
	Serial.begin(9600);
	init_stepper(&vstep);
	init_stepper(&hstep);
}

void loop()
{
	int lt = analogRead(ldrlt); // top left
	int rt = analogRead(ldrrt); // top right
	int ld = analogRead(ldrld); // down left
	int rd = analogRead(ldrrd); // down rigt

	int avt = (lt + rt) / 2; // average value top
	int avd = (ld + rd) / 2; // average value down
	int avl = (lt + ld) / 2; // average value left
	int avr = (rt + rd) / 2; // average value right

	update_angle(avt, avd, &vstep);
	update_angle(avl, avr, &hstep);

	delay(dtime);
}
