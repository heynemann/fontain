#include </home/heynemann/dev/fontain/libfontain.h>

void router(struct evhttp_request *r, void *arg) {
    const char *uri = evhttp_request_get_uri(r);

    char *static_file = (char *) malloc(strlen(options.path_to_htdocs) + strlen(uri) + strlen(LIBSRVR_INDEX) + 1);
    stpcpy(stpcpy(static_file, options.path_to_htdocs), uri);

    bool file_exists = true;
    struct stat st;
    if(stat(static_file, &st) == -1) {
        file_exists = false;
        evhttp_send_error(r, HTTP_NOTFOUND, "NOTFOUND");
    }
    else {
        if(S_ISDIR(st.st_mode)) {
            strcat(static_file, LIBSRVR_INDEX);

            if(stat(static_file, &st) == -1) {
                file_exists = false;
                evhttp_send_error(r, HTTP_NOTFOUND, "NOTFOUND");
            }
        }
    }

    if(file_exists) {
        int file_size = st.st_size;

        char *html;
        html = (char *) alloca(file_size);

        if(file_size != 0) {
            FILE *fp = fopen(static_file, "r");
            fread(html, 1, file_size, fp);
            fclose(fp);
        }

        struct evbuffer *buffer;
        buffer = evbuffer_new();

        struct evkeyvalq *headers = evhttp_request_get_output_headers(r);
        evhttp_add_header(headers, "Content-Type", "text/html; charset=UTF-8");
        evhttp_add_header(headers, "Server", LIBSRVR_SIGNATURE);

        evbuffer_add_printf(buffer, "%s", html);
        evhttp_send_reply(r, HTTP_OK, "OK", buffer);
        evbuffer_free(buffer);

        if(options.verbose) fprintf(stderr, "%s\t%d\n", static_file, file_size);
    }
    else {
        if(options.verbose) fprintf(stderr, "%s\t%s\n", static_file, "404 Not Found");
    }

    free(static_file);
}

int main(int argc, char **argv) {
    int opt;

    options.port = 4080;
    options.address = "0.0.0.0";
    options.verbose = 0;
    options.path_to_htdocs = "/tmp";

    while((opt = getopt(argc,argv,"p:d:vh")) != -1) {
        switch(opt) {
            case 'p':
                options.port = atoi(optarg);
                break;
            case 'v':
                options.verbose = 1;
                break;
            case 'd':
                options.path_to_htdocs = optarg;
                break;
            case 'h':
                printf("Usage: ./libfontain -p port -v[erbose] -d /path/to/static/files -h[elp]\n");
                exit(1);
        }
    }

    printf("Running server at %s:%d\n", options.address, options.port);

    libbase = event_base_new();
    libsrvr = evhttp_new(libbase);
    evhttp_bind_socket(libsrvr, options.address, options.port);
    evhttp_set_gencb(libsrvr, router, NULL);
    event_base_dispatch(libbase);

    return 0;
}
