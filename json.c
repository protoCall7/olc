#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include <kcgi.h>
#include <kcgijson.h>

#include "calculator.h"
#include "http.h"

void sendjson(struct kreq *r, struct parameters p) {
	struct kjsonreq jr;

	start_http(r, KHTTP_200);
	kjson_open(&jr, r);
	kjson_obj_open(&jr);
	kjson_putdoublep(&jr, "voltage", p.voltage);
	kjson_putdoublep(&jr, "current", p.current);
	kjson_putdoublep(&jr, "resistance", p.resistance);
	kjson_obj_close(&jr);
	kjson_close(&jr);
}
