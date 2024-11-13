#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <microhttpd.h>

const uint16_t port = 8888;
static mongoc_collection_t *collection; /* Global collection for MongoDB */

/* Helper function to insert parsed JSON data into MongoDB */
void insert_sensor_data(const char *json_str);

/* HTTP POST handler */
static enum MHD_Result request_handler(void *cls, struct MHD_Connection *connection,
                                       const char *url, const char *method,
                                       const char *version, const char *upload_data,
                                       size_t *upload_data_size, void **con_cls);

int main(void) 
{
  /* Initialize MongoDB client */
  mongoc_init();
  mongoc_client_t *client = mongoc_client_new("mongodb://localhost:27017/?appname=sensor-webserver");
  collection              = mongoc_client_get_collection(client, "sensor_db", "sensor_data");

  /* Set up the HTTP server */
  struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port, NULL, NULL, 
                                               &request_handler, NULL, MHD_OPTION_END);
  if (NULL == daemon) {
    fprintf(stderr, "Failed to start HTTP server\n");
    return 1;
  }

  printf("Server running on port %d...\n", port);
  getchar(); /* Run until Enter is pressed */

  /* Cleanup */
  MHD_stop_daemon(daemon);
  mongoc_collection_destroy(collection);
  mongoc_client_destroy(client);
  mongoc_cleanup();

  return 0;
}

void insert_sensor_data(const char *json_str) 
{
  bson_t *document;
  bson_error_t error;

  /* Parse JSON string to BSON document */
  document = bson_new_from_json((const uint8_t *)json_str, -1, &error);
  if (!document) {
    fprintf(stderr, "JSON parse error: %s\n", error.message);
    return;
  }

  /* Insert document into MongoDB */
  if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error)) {
    fprintf(stderr, "Insert failed: %s\n", error.message);
  } else {
    printf("Data inserted successfully\n");
  }

  bson_destroy(document);
}

static enum MHD_Result request_handler(void *cls, struct MHD_Connection *connection,
                                       const char *url, const char *method,
                                       const char *version, const char *upload_data,
                                       size_t *upload_data_size, void **con_cls) 
{
  static int dummy;
  if (&dummy != *con_cls) {
    *con_cls = &dummy;
    return MHD_YES;
  }

  /* Process POST data */
  if (*upload_data_size != 0) {
    insert_sensor_data(upload_data);
    *upload_data_size = 0;
    return MHD_YES;
  }

  /* Send response */
  struct MHD_Response *response = MHD_create_response_from_buffer(strlen("Received"), 
                                                                  (void *)"Received", 
                                                                  MHD_RESPMEM_PERSISTENT);
  int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);
  return ret;
}
