#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include "../include/uECC.h"

#include "../include/ledger.h"

#define PORT 8888
#define MIMETYPE "application/json"

unsigned long balance (struct ledger * ledger, char * account);

int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
    char page[255];
    struct MHD_Response *response;
    int ret;
    int ledger_fd;
    int bytes;
    char * user = (char *) url+1;
    char b[8];

    ledger_fd = open ("ledger.bin", O_RDONLY);
    bytes = read (ledger_fd, &ledger, sizeof (struct ledger));
    if (bytes == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
    }
    close (ledger_fd);

    snprintf(b, 8, "%lu", balance (&ledger, user));
    strcpy (page, "{\"balance\":");
    strcat (page, b);
    strcat (page, "}");

    response = MHD_create_response_from_buffer (
        strlen (page),
        (void*) page, MHD_RESPMEM_MUST_COPY
    );

    MHD_add_response_header(response, "Content-Type", MIMETYPE);

    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);

    return ret;
}


int main (void) {
  struct MHD_Daemon *daemon;

  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon) return 1;
  getchar ();

  MHD_stop_daemon (daemon);
    return 0;
}
