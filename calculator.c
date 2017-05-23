#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

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

	static const char *const pages[PAGE__MAX] = {
		"resistance",
		"current",
		"voltage",
	};

	er = khttp_parse(&r, keys, KEY__MAX, pages, PAGE__MAX, 0);

	if (er != KCGI_OK) {
		fprintf(stderr, "HTTP parse error: %d\n", er);
		return(EXIT_FAILURE);
	}

#ifdef __OpenBSD__
	errno = 0;
	if (pledge("stdio", NULL) == -1) {
		perror("Failed to pledge");
		return(EXIT_FAILURE);
	}
#endif
	
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
			start_http(&r, KHTTP_404);
			break;
	}

	khttp_free(&r);
	return 0;
}
