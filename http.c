#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include <kcgi.h>

void send_http_headers(struct kreq *r, enum khttp code) {
	khttp_head(r, kresps[KRESP_STATUS], "%s", khttps[code]);
	khttp_head(r, kresps[KRESP_CONTENT_TYPE], "%s", kmimetypes[r->mime]);
	khttp_head(r, "X-Frame-Options", "DENY");
	khttp_head(r, "X-XSS-Protection", "1");
}

void start_http(struct kreq *r, enum khttp code) {
	send_http_headers(r, code);
	khttp_body(r);
}
