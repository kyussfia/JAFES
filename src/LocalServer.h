#ifndef LOCALSERVER
#define LOCALSERVER
#include "Wifi.h"
 
class LocalServer {
  public:
    LocalServer(uint16_t port) {
      this->srv = new WiFiServer(port);
      this->srv->begin();
    }

    WiFiClient listen() {
      return this->srv->available();
    }

    void redirect(WiFiClient client) {
      this->httpHeader(client);
      this->html(client);
      this->head(client, false);
      this->redirection(client);
    }

    void respond(WiFiClient client, bool status, bool sleepingMode) {
      this->httpHeader(client);
      this->html(client);
      this->head(client, true);
      this->body(client, status, sleepingMode);
    }

    // read untill the second \n to get url only
    String receive(WiFiClient client) {
      String request = "";
      if (client.available()) {
        char c;
        bool firstLine = false;
        while (-1 != (c = client.read()) && !firstLine) {
          request += c;
          if (c == '\n') {
            firstLine = true;
          }
        }
      }
      return request;
    }

    void close(WiFiClient client) {
      client.stop();
    }

  private:
    WiFiServer* srv;
    
    void httpHeader(WiFiClient client) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      client.println();
    }
    
    void html(WiFiClient client) {
      client.println("<!DOCTYPE html><html>");
    }
    
    void head(WiFiClient client, bool includeStyle) {
      client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
      if (includeStyle) {
        client.println("<style>html { font-family: Helvetica; display: inline-block; margin:  0% 30% 0% 30%; text-align: center;}");
        client.println(".button { background-color: #50d119; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
        client.println(".buttonOff {background-color: #ff371c; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
        client.println(".column-left{ float: left; width: 33.333%; }");
        client.println(".column-right{ float: right; width: 33.333%; }");
        client.println(".column-center{ display: inline-block; width: 33.333%; }");
        client.println("</style>");
      }
      client.println("</head>");
    }

    void redirection(WiFiClient client) {
      client.println("<body><h1>JAFES Web Server</h1>");
      client.println("<script>document.location.href=\"/\";</script>");
      client.println("</body></html>");
      client.println();
    }

    void body(WiFiClient client, bool stat, bool sleepingMode) {
      String statText = stat ? "on" : "off";
      String oppositeStat = stat ? "off" : "on";
      String sleepText = sleepingMode ? "on" : "off";
      String oppositeSleep = sleepingMode ? "off" : "on";
      client.println("<body><h1>JAFES Web Server</h1>");
      client.println("<div class=\"container\">");
      client.println("<div class=\"column-center\">");
      client.println("<p>Status " +statText+ "</p>");
      client.println("<p><a href=\"/status/"+oppositeStat+"\"><button class=\""+ (stat ? "button" : "buttonOff") +"\">"+(stat ? "ON" : "OFF")+"</button></a></p>");
      client.println("<p>Sleeping mode " + sleepText + "</p>");
      client.println("<p><a href=\"/sleep/"+oppositeSleep+"\"><button class=\""+ (sleepingMode ? "button" : "buttonOff") +"\">"+(sleepingMode ? "ON" : "OFF")+"</button></a></p>");
      client.println("</div>");
      client.println("<div class=\"column-left\"><img id=\"cat1\" src=\"https://i.imgur.com/WYEyMau.jpg\"></div>");
      client.println("<div class=\"column-right\"><img id=\"cat2\" src=\"https://i.imgur.com/ShfYJwN.jpg\"></div>");
      client.println("</div>");
      client.println("</body></html>");
      client.println();
    }
};
#endif
