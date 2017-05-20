#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

enum key {
	KEY_RESISTANCE,
	KEY_CURRENT,
	KEY_VOLTAGE,
	KEY__MAX
};

static const struct kvalid keys[KEY__MAX] = {
	{ kvalid_string, "resistance" },
	{ kvalid_string, "current" },
	{ kvalid_string, "voltage" },
};

enum page {
	PAGE_RESISTANCE,
	PAGE_CURRENT,
	PAGE_VOLTAGE,
	PAGE__MAX
};

struct parameters {
	double resistance;
	double current;
	double voltage;
};

#endif
