#include <ESP8266HTTPClient.h>
#include "local_config.h"

#ifndef ENDPOINT_URL
#define ENDPOINT_URL "http://localhost:3000"
#endif

class Weblog
{
private:
  const String _endpointUrl = ENDPOINT_URL;

public:
  int post(String data);
};