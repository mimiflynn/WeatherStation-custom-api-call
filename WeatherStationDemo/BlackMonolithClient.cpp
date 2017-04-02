#include "BlackMonolithClient.h"


BlackMonolithClient::BlackMonolithClient() {

}

void BlackMonolithClient::getLastChannelItem() {
  JsonStreamingParser parser;
  parser.setListener(this);
  WiFiClient client;

  const char host[] = "<domain of server here>";
  String url = "/api/dht/latest";

  const int httpPort = 2001;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  int retryCounter = 0;
  while(!client.available()) {
    Serial.println(".");
    delay(1000);
    retryCounter++;
    if (retryCounter > 10) {
      return;
    }
  }

  int pos = 0;
  boolean isBody = false;
  char c;

  int size = 0;
  client.setNoDelay(false);
  while(client.connected()) {
    while((size = client.available()) > 0) {
      c = client.read();
      if (c == '{' || c == '[') {
        isBody = true;
      }
      if (isBody) {
        parser.parse(c);
      }
    }
  }
}

void BlackMonolithClient::whitespace(char c) {

}

void BlackMonolithClient::startDocument() {

}

void BlackMonolithClient::key(String key) {
  if (key == "fahrenheit") {
    Serial.println("key " + key);
    isHeader = true;
  } else if (key == "humidity") {
    Serial.println("key " + key);
    isHeader = false;
  }
  currentKey = key;
}

void BlackMonolithClient::value(String value) {
  Serial.println("currentKey " + currentKey);
  Serial.println("value " + value);
  if (currentKey == "fahrenheit") {
    lastFields[0] = value;
  } else if (currentKey == "humidity") {
    lastFields[1] = value;
  }
}

String BlackMonolithClient::getFieldLabel(int index) {
  return fieldLabels[index];
}

String BlackMonolithClient::getFieldValue(int index) {
  return lastFields[index];
}

String BlackMonolithClient::getDate() {
  return date;
}

void BlackMonolithClient::endArray() {

}

void BlackMonolithClient::endObject() {

}

void BlackMonolithClient::endDocument() {

}

void BlackMonolithClient::startArray() {

}

void BlackMonolithClient::startObject() {

}
