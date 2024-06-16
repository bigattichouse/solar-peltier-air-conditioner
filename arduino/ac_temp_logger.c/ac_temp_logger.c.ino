#include <DHT11.h>

// include the SD library:
#include <SPI.h>
#include <SD.h>

// Real time clock libs
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>


// set up variables using the SD utility library functions:
//Sd2Card card;
//SdVolume volume;
//SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
const int chipSelect = 10;
 
/*
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
*/

//tmElements_t tm;

/*
bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
*/

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

 // Serial.print("\n#Initializing SD Card...");
/*
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("#initialization failed.");
    while (1);
  } else {
    Serial.println("#Ready.");
  }
*/
/*
  // print the type of card
  Serial.println();
  Serial.print("#Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("#SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("#SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("#SDHC");
      break;
    default:
      Serial.println("#Unknown");
  }
*/
  //// Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  //if (!volume.init(card)) {
  //  Serial.println("#Could not find FAT16/FAT32 partition.");
  //  while (1);
 // }

/*
  Serial.print("#Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("#Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("#Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();
*/
/*
  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("#Volume:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("#Volume size (Kb):  ");
  Serial.println(volumesize);
  //Serial.print("#Volume size (Mb):  ");
  //volumesize /= 1024;
  //Serial.println(volumesize);
  //Serial.print("#Volume size (Gb):  ");
  //Serial.println((float)volumesize / 1024.0);

  Serial.println("\n#Files: ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
*/

  if (!SD.begin(chipSelect)) {
   // Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  //Serial.println("card initialized.");

/*
  //Real Time Clock DS1307
  bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }

  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
   // Serial.println("Please check your circuitry");
  } /*else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }*/

 /* 
  * To delete the file:
  SD.remove("ac.dat");

 */ 

 // if you want to dump the output file:

  //Serial.println("SD old data...");
  File dataFile = SD.open("ac.dat");
  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }
 // Serial.println("new data...");
/**/

}


String ReadTemperatureAndHumidity(String deviceName, int devicePin){
   String output;
   DHT11 device(devicePin);
   device.setDelay(100);
   delay(250);
   char str_temp[5];
   int temperature = 0;
   float f = 0;
   int humidity = 0;
   int result = device.readTemperatureHumidity(temperature, humidity);
   if (result == 0) {
       f = (float)(1.8 * temperature) + 32;
       dtostrf(f, 4, 1, str_temp);
       output += deviceName;
       output += ",";
       output += str_temp;
       output += ",";
       output += humidity; 
   } 
   return output;
}

String print2digits(int number) {
  String output;
  if (number >= 0 && number < 10) {
    output+= "0";
  }
  output += number;
  return output;
}


int lastMinute = 0;

String readTimeStamp(){
  tmElements_t tm;
   String output;
  if (RTC.read(tm)) {
    output+= tmYearToCalendar(tm.Year); 
    output+=print2digits(tm.Month); 
    output+=print2digits(tm.Day);
    output+="T";
    output+=print2digits(tm.Hour); 
    output+=print2digits(tm.Minute); 
    output+=print2digits(tm.Second);        
  }
  if(lastMinute!=tm.Minute){
     lastMinute = tm.Minute;
     return output;  
  } 
  return "";
}
 

void loop() {
  String data,output;

  int indoor=2;
  int cold=3;
  int outdoor=4;
  int hot=5;

  output = readTimeStamp();
    if(output!=""){
      data = ReadTemperatureAndHumidity( "i", indoor); //2
      output += ","+data;
      data = ReadTemperatureAndHumidity( "c", cold);  //3
      output += ","+data;
      data = ReadTemperatureAndHumidity( "o", outdoor); //4
      output += ","+data;
      data = ReadTemperatureAndHumidity( "h", hot); //5
      output += ","+data;
      //Serial.println(output);

      
  String filename = "ac.dat";
  
  /*
  if (!SD.exists(filename)) {
    File myFile;
    myFile = SD.open(filename, FILE_WRITE);
    myFile.close();
  }
*/
  
      File dataFile = SD.open(filename, FILE_WRITE);
      
        // if the file is available, write to it:
        if (dataFile) {
          dataFile.println(output);
          dataFile.close();
          // print to the serial port too:
          Serial.println(output);
        }
        // if the file isn't open, pop up an error:
        else {
          //Serial.println("SD Write file error.");
        }

      
    }
}
