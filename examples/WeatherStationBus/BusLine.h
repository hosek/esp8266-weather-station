#ifndef BusLine_h
#define BusLine_h

class BusLine
{
private:
      String lineNo;
      String arrivalTime;
      String travelTime;
      String endStation;
      String parseDate(String timeText);
      
public:
      BusLine(String line,String arrival,String travel,String endsta);
      BusLine();
      String getArrivalTime();
      String getEndStationName();
      String getLineId();
      static String translate(String chineseName);
};

#endif //BusLine_h
