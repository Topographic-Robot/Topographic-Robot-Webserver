#include <stdio.h>
#include <bson/bson.h>
#include <mongoc/mongoc.h>

void print_all_documents(mongoc_collection_t *collection);

int main(void) 
{
  mongoc_init();

  mongoc_client_t     *client     = mongoc_client_new("mongodb://localhost:27017");
  mongoc_collection_t *collection = mongoc_client_get_collection(client, "sensor_db", "sensor_data");

  print_all_documents(collection);

  mongoc_collection_destroy(collection);
  mongoc_client_destroy(client);
  mongoc_cleanup();

  return 0;
}

void print_all_documents(mongoc_collection_t *collection) 
{
  mongoc_cursor_t *cursor;
  const bson_t    *doc;
  char            *str;

  cursor = mongoc_collection_find_with_opts(collection, bson_new(), NULL, NULL);

  while (mongoc_cursor_next(cursor, &doc)) {
    str = bson_as_canonical_extended_json(doc, NULL);
    printf("%s\n", str);
    bson_free(str);
  }

  mongoc_cursor_destroy(cursor);
}
