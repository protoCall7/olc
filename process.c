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
	struct parameters p;
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
			exit(EXIT_FAILURE);
		} else if (*ep != '\0') {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (errno == ERANGE) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		}

		/*
		 * convert voltage parameter to a long and check for error
		 * conditions
		 */
		errno = 0;
		vol = strtod(kpv->parsed.s, &ep);
		if (ep == kpv->parsed.s) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (*ep != '\0') {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (errno == ERANGE) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		}

		res = vol / cur;

		p.resistance = res;
		p.current = cur;
		p.voltage = vol;

		send_json(r, p);
	}
}

void process_current(struct kreq *r) {
	struct kpair *kpr, *kpv;
	struct parameters p;
	double res, cur, vol;
	char *ep;

	errno = 0;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpr == NULL) || (kpv == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		errno = 0;
		res = strtod(kpr->parsed.s, &ep);

		if (ep == kpr->parsed.s) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (*ep != '\0') {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (errno == ERANGE) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		}

		errno = 0;
		vol = strtod(kpv->parsed.s, &ep);
		if (ep == kpv->parsed.s) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (*ep != '\0') {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (errno == ERANGE) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		}

		cur = vol / res;

		p.resistance = res;
		p.current = cur;
		p.voltage = vol;

		send_json(r, p);
	}
}

void process_voltage(struct kreq *r) {
	struct kpair *kpr, *kpc;
	struct parameters p;
	double res, cur, vol;
	char *ep;

	errno = 0;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpc = r->fieldmap[KEY_CURRENT];

	if ((kpr == NULL) || (kpc == NULL)) {
		start_http(r, KHTTP_400);
	} else {
		errno = 0;
		res = strtod(kpr->parsed.s, &ep);
		if (ep == kpr->parsed.s) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (*ep != '\0') {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (errno == ERANGE) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		}

		errno = 0;
		cur = strtod(kpc->parsed.s, &ep);
		if (ep == kpc->parsed.s) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (*ep != '\0') {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		} else if (errno == ERANGE) {
			start_http(r, KHTTP_400);
			exit(EXIT_FAILURE);
		}

		vol = cur * res;

		p.resistance = res;
		p.current = cur;
		p.voltage = vol;

		send_json(r, p);
	}
}
