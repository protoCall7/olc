#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include <kcgi.h>
#include <kcgijson.h>

#include "http.h"
#include "process.h"

void send_json(struct kreq *r, struct parameters p) {
	struct kjsonreq jr;

	start_http(r, KHTTP_200);
	kjson_open(&jr, r);
	if(kjson_obj_open(&jr) == 0)
		abort();
	if(kjson_putdoublep(&jr, "voltage", p.voltage) == 0)
		abort();
	if(kjson_putdoublep(&jr, "current", p.current) == 0)
		abort();
	if(kjson_putdoublep(&jr, "resistance", p.resistance) == 0)
		abort();
	if(kjson_obj_close(&jr) == 0)
		abort();
	if(kjson_close(&jr) == 0)
		abort();
}
