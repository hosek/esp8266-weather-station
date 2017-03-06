#include <Arduino.h>
#include "BusLine.h"

BusLine::BusLine(String line,String arrival,String travel,String endsta){
  lineNo = line;
  arrivalTime = arrival;
  travelTime = travel;
  endStation = endsta;
}

BusLine::BusLine(){
}


String BusLine::parseDate(String timeText){

   if(timeText == "-1"){
    return "no time";
   } 
   long epochtime = atol(timeText.c_str())/1000;
  
   int hours = (int) ((epochtime % 86400L) / 3600) % 24;
   int minutes = (int) ((epochtime % 3600) / 60);
   return String(hours) + ":" + String(minutes);
}


String BusLine::getArrivalTime() {
    int traveltimeInt = travelTime.toInt();
    if(traveltimeInt/60 > 60 ){
      return parseDate(arrivalTime);
      }
      else if(traveltimeInt / 60 >= 1){
        return String(traveltimeInt / 60) + " m";
      }
      else if(traveltimeInt > 0){
        return String(traveltimeInt) + " s";
      }else{
        return "Offline";
      }
}

String BusLine::translate(String chineseName) {
    if(chineseName == "重庆北站南广场"){
      return "Chongqing North";
    }else if(chineseName == "汉渝路"){
      return "Han Yu Road";
    }else if(chineseName == "天竺路"){
      return "Tianzhu Road";
    }else if(chineseName == "两路城南"){
      return "Two road south";
    }else{
      return " - ";
    }
}

String BusLine::getEndStationName(){
   return translate(endStation);
}

String BusLine::getLineId(){
  return lineNo;
}

