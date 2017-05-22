#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <kcgi.h>

#include "calculator.h"
#include "process.h"
#include "json.h"
#include "http.h"

int main(void) {
	struct kreq r;
	enum kcgi_err er;
	
	static const struct kvalid keys[KEY__MAX] = {
		{ kvalid_string, "resistance" },
		{ kvalid_string, "current" },
		{ kvalid_string, "voltage" },
	};

	const char *const pages[PAGE__MAX] = {
		"resistance",
		"current",
		"voltage",
	};

	er = khttp_parse(&r, keys, KEY__MAX, pages, PAGE__MAX, 0);

	if (er != KCGI_OK) {
		fprintf(stderr, "HTTP parse error: %d\n", er);
		return(EXIT_FAILURE);
	}

	if (pledge("stdio", NULL) == -1) {
		return(EXIT_FAILURE);
	}
	
	switch(r.page) {
		case (PAGE_RESISTANCE):
			process_resistance(&r);
			break;
		case (PAGE_CURRENT):
			process_current(&r);
			break;
		case (PAGE_VOLTAGE):
			process_voltage(&r);
			break;
		default:
			abort();
	}

	khttp_free(&r);
	return 0;
}
