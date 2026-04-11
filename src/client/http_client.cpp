// #include <client/http_client.h>
#include "client/http_client.h"


String extractAccessToken(String json) {
  DynamicJsonDocument doc(2048);

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.println("Error parseando JSON");
    return "";
  }

  String token = doc["access_token"];
  return token;
}

String recoveryAccessToken() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  String url = "https://wso2is-service-7.abexa.pe/oauth2/token";

  if (!https.begin(client, url)) {
    Serial.println("Error iniciando HTTPS");
    return "";
  }

  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  https.addHeader("Authorization", "Basic X0ppOWtpaW5GM2thSEY1NlBSVEtaRDhudkxRYTo2WmxKVF9wemc3VFVNVldrV1RlV29xT2RfNzhZaTJDRl9vZUFfRXdQQWVNYQ==");

  String body = "grant_type=password";
  body += "&username=xtu-mobile-private-app";
  body += "&password=xtu-mobile-private-app";
  body += "&scope=xtu-mobile.read xtu-mobile.write monitor-logging.read monitor-logging.write profile openid generic-notifier.read generic-notifier.write";

  int httpCode = https.POST(body);

  if (httpCode > 0) {
    String response = https.getString();

    String accessToken = extractAccessToken(response);

    Serial.println("Access token: " + accessToken);

    https.end();
    return accessToken;
  }

  https.end();
  return "";
}