#if ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include <SD.h>
#include "ESP8266FtpServer.h"
#include <SPI.h>

const char* ssid = "SDcard Reader";
const char* password = "ASDF@123";

// config static IP
IPAddress apIP(192, 168, 12,7);  
IPAddress gateway(192, 168,12, 1);
IPAddress subnet(255, 255, 255, 0); 

#if ESP32
#define SPI_CS_P 13
#elif ESP8266
#define SPI_CS_P 2
#endif
FtpServer ftpSrv;//set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  //Setting the Ip address for the WiFi connectivity
  WiFi.config(apIP, gateway, subnet);
  Serial.println();
  Serial.print("Configuring access point...");
  //Setting the esp to softaccess point mode
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  delay(100);
  //changing softAP config and starting the Start AP
  WiFi.softAPConfig(apIP, gateway, subnet);
  WiFi.softAP(ssid, password);
  #if ESP32
  SPI.begin(14,2,15,SPI_CS_P);
  
  //2 indicates the Chip select pin
  if (SD.begin(SPI_CS_P,SPI)) {
  #elif ESP8266
  if (SD.begin(SPI_CS_P)) {
  #endif
      Serial.println("SD opened!");
      ftpSrv.begin("esp8266","esp8266");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
  }   
}
void loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   
}
