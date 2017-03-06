#include <Arduino.h>
#include "BusLines.h"

BuslinesClient::BuslinesClient(String buslines[],int lSize) {
    lineCodes = buslines;
}

void BuslinesClient::getStopDetail(String cityId, String stationId) {
  JsonStreamingParser parser;
  parser.setListener(this);
  
  WiFiClient client;
  // http://api.chelaile.net.cn:7000/bus/stop!stationDetail.action?sign=&v=5.2.0&s=IOS&sv=9.1&vc=10070&cityId=003&stationId=023-2165
  
  const char host[] = "api.chelaile.net.cn";
  String url = "/bus/stop!stationDetail.action?sign=&v=5.2.0&s=IOS&sv=9.1&vc=10070&cityId="+cityId+"&stationId=" + stationId;
  const int httpPort = 7000;
  
//  const char host[] = "romanhosek.cz";
//  String url = "/buslines2.json";
//  const int httpPort = 80;

 
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

void BuslinesClient::whitespace(char c) {

}

void BuslinesClient::startDocument() {

}

void BuslinesClient::cleanTemps(){
  tempLineId = "";
  tempArrivalTime = "";
  tempTravelTime = "";
  tempStationName = "";
  tempDirection = "";
}

void BuslinesClient::setDefaults(){
  for(int x = 0 ; x < sizeof(lineCodes); x++){
          Serial.println("setDefault " + lineCodes[x] + " " + x);
          BusLine defline(lineCodes[x],"","","");
          busline_arr[x] = defline;
  }
}

void BuslinesClient::key(String key) {
  currentKey = key;
  //Serial.println("Current key " + key);
  if(key == "fav"){
    isInLine = false;
    if(tempLineId != "" && tempArrivalTime != "" && tempTravelTime != "" && tempStationName != "" && tempDirection != "" && tempDirection == "0"){
       for(int x = 0 ; x < sizeof(lineCodes); x++){
          if(tempLineId == lineCodes[x]){
            BusLine newline( tempLineId,tempArrivalTime,tempTravelTime,tempStationName );
            Serial.println("Saving " + tempLineId + " "  + tempArrivalTime + " " + tempTravelTime + " " + tempStationName + " " + x);
            busline_arr[x] = newline;
          }
       }
    }
    cleanTemps();
  }

  if(key == "jsonr"){
      setDefaults();
  }
}

void BuslinesClient::value(String value) {

    if (currentKey == "lineNo"){
      for(int i=0;i< sizeof(lineCodes);i++){
        if(value == lineCodes[i]){
         tempLineId = value;
         isInRightLine = true;
         Serial.println("templineNo: " + value);
        }
      }
    }

    if (currentKey == "arrivalTime" && isInLine && isFirstTime){
      tempArrivalTime = value;
      isFirstTime = false;
      isFirstTravelTime = true;
      //Serial.println("tempArrivalTime: " + value);
    }

    
    if (currentKey == "endSn" && isInLine){
      tempStationName = value;
      //Serial.println("tempStationName: " + BusLine::translate(value));
    }

    if(currentKey == "travelTime" && isInLine && isFirstTravelTime){
      tempTravelTime= value;
      isFirstTravelTime = false;
      //Serial.println("tempTravelTime: " + value);
    }

    if(currentKey == "direction" && isInRightLine){
      tempDirection= value;
      Serial.println("tempDirection: " + value);
      isInRightLine = false;
    }
}

BusLine BuslinesClient::getLine(int index) {
   return busline_arr[index];
}

void BuslinesClient::endArray() {
 if(isInStates){
    isInStates = true;
     //Serial.println("end states");
  }
}

void BuslinesClient::endObject() {

}

void BuslinesClient::endDocument() {
    
}

void BuslinesClient::startArray() {
  if(currentKey == "stnStates" && isInLine){
    isInStates = true;
    isFirstTime = true;
      //Serial.println("start states");
    }
}

void BuslinesClient::startObject() {
    if(currentKey == "line"){
    isInLine = true;
    //Serial.println("Line start");
    }
}



