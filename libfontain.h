// General purpose header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <getopt.h>
#include <sys/stat.h>

// Libevent header files
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>

// Libsrvr configuration settings
#define LIBSRVR_SIGNATURE "fontain v 0.1.0"
#define LIBSRVR_HTDOCS "/Users/sabhinav/libsrvr/www"
#define LIBSRVR_INDEX "/index.html"

// Libsrvr http server and base struct
struct evhttp *libsrvr;
struct event_base *libbase;

// Libsrvr options
struct _options {
    int port;
    const char *address;
    const char *path_to_htdocs;
    int verbose;
} options;
