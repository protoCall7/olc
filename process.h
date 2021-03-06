#ifndef __PROCESS_H__
#define __PROCESS_H__

struct parameters {
	double resistance;
	double current;
	double voltage;
};

void process_resistance(struct kreq *r);
void process_current(struct kreq *r);
void process_voltage(struct kreq *r);

#endif
