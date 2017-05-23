#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include <kcgi.h>

static enum khttp sanitize(struct kreq *r) {
	if (KMIME_APP_JSON != r->mime) {
		return(KHTTP_404);
	} else if (KMETHOD_GET != r->method) {
		return(KHTTP_405);
	}

	return(KHTTP_200);
}

static void send_http_headers(struct kreq *r, enum khttp code) {
	khttp_head(r, kresps[KRESP_STATUS], "%s", khttps[code]);
	khttp_head(r, kresps[KRESP_CONTENT_TYPE], "%s", kmimetypes[r->mime]);
	khttp_head(r, "X-Frame-Options", "DENY");
	khttp_head(r, "X-XSS-Protection", "1");
	khttp_head(r, "X-Content-Type-Options", "nosniff");
}

void start_http(struct kreq *r, enum khttp code) {
	enum khttp er;
	er = sanitize(r);

	if (er == KHTTP_200) {
		send_http_headers(r, code);
	} else {
		send_http_headers(r, er);
	}

	(void) khttp_body(r);
}
