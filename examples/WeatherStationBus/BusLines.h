#pragma once

#include <JsonListener.h>
#include <JsonStreamingParser.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "BusLine.h"

class BuslinesClient: public JsonListener {
  private:
    BusLine busline_arr[10];
    String tempLineId = "";
    String tempArrivalTime = "";
    String tempTravelTime = "";
    String tempStationName = "";
    String tempDirection = "";
    String currentKey = "";
    boolean isInLine = false;
    boolean isInStates = false;
    boolean isFirstTime = false;
    boolean isFirstTravelTime = false;
    boolean isInRightLine = false;
    String* lineCodes;
    void cleanTemps();
    void setDefaults();
    
  public:
    BuslinesClient(String buslines[],int lSize);
    void getStopDetail(String cityId, String stationId);
    BusLine getLine(int index);
    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();
};
