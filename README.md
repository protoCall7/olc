# olc

##TODO:
* Implement remaining security headers (see ZAP report) -- relayd
* _BUG_: system still returns json if sanatizer fails
* Error pages

## Server Setup:
OLC has been tested running under OpenBSD httpd[1] on OpenBSD 6.1[2]. It is intended to run under slowcgi(8)

### Sample /etc/httpd.conf
```
server "default" {
	listen on * port 80
	root "/htdocs"
	location "/cgi-bin/olc/*" {
		fastcgi
		root "/"
	}
}
```

### Sample configuration (assumes httpd.conf is already in place)
```bash
$ doas rcctl enable httpd
$ doas rcctl enable slowcgi
$ doas rcctl start httpd
$ doas rcctl start slowcgi
```

[1]: https://bsd.plumbing/
[2]: https://www.openbsd.org/
