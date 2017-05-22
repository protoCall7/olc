#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include <kcgi.h>

#include "calculator.h"
#include "http.h"
#include "process.h"
#include "json.h"

void process_resistance(struct kreq *r) {
	struct kpair *kpc, *kpv;
	double res, cur, vol;
	char *ep; // strtod end pointer

	/*
	 * assign current and voltage query parameter values
	 * to kpc and kpv pairs
	 */
	kpc = r->fieldmap[KEY_CURRENT];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpc == NULL) || (kpv == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		/*
		 * convert current parameter to a long and check for error
		 * conditions
		 */
		errno = 0;

		cur = strtod(kpc->parsed.s, &ep);
		if (ep == kpc->parsed.s) {
			start_http(r, KHTTP_400);
			return(EXIT_FAILURE);
		} else if (*ep != '\0') {
			start_http(r, KHTTP_400);
			return(EXIT_FAILURE);
		}

		errno = 0;

		vol = strtod(kpv->parsed.s, &ep);
		res = vol / cur;

		struct parameters p = {
			res,
			cur,
			vol,
		};

		send_json(r, p);
	}
}

void process_current(struct kreq *r) {
	struct kpair *kpr, *kpv;
	double res, cur, vol;
	char *ep;

	errno = 0;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpr == NULL) || (kpv == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		res = strtod(kpr->parsed.s, &ep);
		vol = strtod(kpv->parsed.s, &ep);
		cur = vol / res;

		struct parameters p = {
			res,
			cur,
			vol,
		};

		send_json(r, p);
	}
}

void process_voltage(struct kreq *r) {
	struct kpair *kpr, *kpc;
	double res, cur, vol;
	char *ep;

	errno = 0;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpc = r->fieldmap[KEY_CURRENT];

	if ((kpr == NULL) || (kpc == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		res = strtod(kpr->parsed.s, &ep);
		cur = strtod(kpc->parsed.s, &ep);

		vol = cur * res;

		struct parameters p = {
			res,
			cur,
			vol,
		};

		send_json(r, p);
	}
}
