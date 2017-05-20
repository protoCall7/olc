#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include <kcgi.h>

#include "calculator.h"
#include "http.h"
#include "json.h"

void process_resistance(struct kreq *r) {
	struct kpair *kpc, *kpv;
	double res, cur, vol;

	kpc = r->fieldmap[KEY_CURRENT];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpc == NULL) || (kpv == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		cur = strtod(kpc->parsed.s, NULL);
		vol = strtod(kpv->parsed.s, NULL);
		res = vol / cur;

		struct parameters p = {
			res,
			cur,
			vol,
		};

		sendjson(r, p);
	}
}

void process_current(struct kreq *r) {
	struct kpair *kpr, *kpv;
	double res, cur, vol;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpr == NULL) || (kpv == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		res = strtod(kpr->parsed.s, NULL);
		vol = strtod(kpv->parsed.s, NULL);
		cur = vol / res;

		struct parameters p = {
			res,
			cur,
			vol,
		};

		sendjson(r, p);
	}
}

void process_voltage(struct kreq *r) {
	struct kpair *kpr, *kpc;
	double res, cur, vol;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpc = r->fieldmap[KEY_CURRENT];

	if ((kpr == NULL) || (kpc == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		res = strtod(kpr->parsed.s, NULL);
		cur = strtod(kpc->parsed.s, NULL);

		vol = cur * res;

		struct parameters p = {
			res,
			cur,
			vol,
		};

		sendjson(r, p);
	}
}
