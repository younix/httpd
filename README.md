# httpd

httpd is a simple web server which only does HTTP.  This is just a research
project for tiny programs and Unix way of live.  The main idea behind this
project is, to write a daemon that just implements the handling one protocol.

*This daemon is NOT for productive use*

 * missing security features!
 * no stable code base!

## Don'ts:
 * no IPv4 and IPv6
 * no SSL and TLS
 * no fastcgi

## Ideas:
 * UNIX Client-Server Program Interface (UCSPI)
 * protocol parsing with regex(3)
 * CGI scripts over file execution permissions
 * authentication over file others permissions and bsd-auth(3)

## expected problems
 * bad performance on large scale
 * bad latency
 * bad performance on many parallel requests

## expected improvements
 * lesser amount of source code
 * simpler source code
 * lesser maintenance

## http features

| Method | Support |
|--------|---------|
| OPTIONS| no      |
| GET    | yes     |
| HEAD   | no      |
| POST   | no      |
| PUT    | no      |
| DELETE | no      |
| TRACE  | no      |
| CONNECT| no      |

## http header features

| Header Field   | Supported |
|----------------|-----------|
| range          | no        |
| authentication | yes       |
| content-length | no        |
| content-type   | no        |

## references
 * http://cr.yp.to/proto/ucspi.txt
 * [RFC: Hypertext Transfer Protocol](http://tools.ietf.org/html/rfc2616)
 * [RFC: HTTP Authentication](http://tools.ietf.org/html/rfc2617)

vim: syntax=markdown spell spelllang=en:
