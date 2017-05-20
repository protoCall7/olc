#ifndef __HTTP_H__
#define __HTTP_H__

void send_http_headers(struct kreq *r, enum khttp code);
void start_http(struct kreq *r, enum khttp code);

#endif
