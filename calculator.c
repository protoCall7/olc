#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

void sendhttpheaders(struct kreq *r, enum khttp code) {
	khttp_head(r, kresps[KRESP_STATUS], "%s", khttps[code]);
	khttp_head(r, kresps[KRESP_CONTENT_TYPE], "%s", kmimetypes[r->mime]);
}

void starthttp(struct kreq *r, enum khttp code) {
	sendhttpheaders(r, code);
	khttp_body(r);
}

void processresistance(struct kreq *r) {
	struct kpair *kpc, *kpv;
	struct kjsonreq jr;
	double res, cur, vol;

	kpc = r->fieldmap[KEY_CURRENT];
	kpv = r->fieldmap[KEY_VOLTAGE];

	if ((kpc == NULL) || (kpv == NULL)) {
		starthttp(r, KHTTP_400);
	} else {
		cur = strtod(kpc->parsed.s, NULL);
		vol = strtod(kpv->parsed.s, NULL);

		res = vol / cur;
		fprintf(stderr, "%lf", res);

		starthttp(r, KHTTP_200);
		kjson_open(&jr, r);
		kjson_obj_open(&jr);
		kjson_putdoublep(&jr, "voltage", vol);
		kjson_putdoublep(&jr, "current", cur);
		kjson_putdoublep(&jr, "resistance", res);
		kjson_obj_close(&jr);
		kjson_close(&jr);
	}
}

int main(void) {
	// defining required variables
	struct kreq r;
	struct kpair *p;
	enum kcgi_err er;

	er = khttp_parse(&r, keys, KEY__MAX, pages, PAGE__MAX, 0);

	if (er != KCGI_OK) {
		fprintf(stderr, "HTTP parse error: %d\n", er);
		return(EXIT_FAILURE);
	}
	
	switch(r.page) {
		case (PAGE_RESISTANCE):
			processresistance(&r);
			break;
		case (PAGE_CURRENT):
			break;
		case (PAGE_VOLTAGE):
			break;
		default:
			abort();
	}

	khttp_free(&r);
	return 0;
}
