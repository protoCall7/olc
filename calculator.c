#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <kcgi.h>
#include <kcgijson.h>

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

const char *const pages[PAGE__MAX] = {
	"resistance",
	"current",
	"voltage",
};

struct parameters {
	double resistance;
	double current;
	double voltage;
};

void sendhttpheaders(struct kreq *r, enum khttp code) {
	khttp_head(r, kresps[KRESP_STATUS], "%s", khttps[code]);
	khttp_head(r, kresps[KRESP_CONTENT_TYPE], "%s", kmimetypes[r->mime]);
}

void starthttp(struct kreq *r, enum khttp code) {
	sendhttpheaders(r, code);
	khttp_body(r);
}

void sendjson(struct kreq *r, struct parameters p) {
	struct kjsonreq jr;

	starthttp(r, KHTTP_200);
	kjson_open(&jr, r);
	kjson_obj_open(&jr);
	kjson_putdoublep(&jr, "voltage", p.voltage);
	kjson_putdoublep(&jr, "current", p.current);
	kjson_putdoublep(&jr, "resistance", p.resistance);
	kjson_obj_close(&jr);
	kjson_close(&jr);
}

void processresistance(struct kreq *r) {
	struct kpair *kpc, *kpv;
	double res, cur, vol;

	kpc = r->fieldmap[KEY_CURRENT];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpc == NULL) || (kpv == NULL)) {
		starthttp(r, KHTTP_400);
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

void processcurrent(struct kreq *r) {
	struct kpair *kpr, *kpv;
	double res, cur, vol;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpr == NULL) || (kpv == NULL)) {
		starthttp(r, KHTTP_400);
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

void processvoltage(struct kreq *r) {
	struct kpair *kpr, *kpc;
	double res, cur, vol;

	kpr = r->fieldmap[KEY_RESISTANCE];
	kpc = r->fieldmap[KEY_CURRENT];

	if ((kpr == NULL) || (kpc == NULL)) {
		starthttp(r, KHTTP_400);
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

int main(void) {
	// defining required variables
	struct kreq r;
	enum kcgi_err er;

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
			processresistance(&r);
			break;
		case (PAGE_CURRENT):
			processcurrent(&r);
			break;
		case (PAGE_VOLTAGE):
			processvoltage(&r);
			break;
		default:
			abort();
	}

	khttp_free(&r);
	return 0;
}
