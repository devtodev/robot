/*
 * cloud.c
 *
 *  Created on: Aug 18, 2019
 *      Author: Carlos Miguens
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "telemetry.h"
#include "cloud.h"
#include <curl/curl.h>

void init_string(struct string *s)
{
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL)
  {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL)
  {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;
  printf(">> %s ", s->ptr);
  // TODO: here analize the response
  return size*nmemb;
}

int telemetryPost(Sensors sensors, Power power)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl)
  {
	struct string s;
	init_string(&s);

	curl_easy_setopt(curl, CURLOPT_URL, "www.google.com.ar");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	// TODO: here set the http request with the telemetry report
	res = curl_easy_perform(curl);

	printf("%s\n", s.ptr);
	free(s.ptr);

	/* always cleanup */
	curl_easy_cleanup(curl);
  }

  return 0;
}
