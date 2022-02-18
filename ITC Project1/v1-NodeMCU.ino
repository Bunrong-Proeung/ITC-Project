#include <SoftwareSerial.h>
#include <Nextion.h>

/*
 *  --- APDU STRUCTURE ---
 */
 
//Define package structure
/*  +--------+---------+----------+-------------+----------+-----------+------------+
    | header | node_id | humidity | temperature | moisture | radiation |            |
    +--------+---------+----------+-------------+----------+-----------+            |
    |   1    |    1    |     4    |      4      |     4    |     4     | = 18 bytes |
    +--------+---------+----------+-------------+----------+-----------+------------+
*/

typedef struct __attribute__((__packed__))
{
  uint8_t header;
  uint8_t node_id;

  float humidity;
  float temperature;
  float moisture;
  float radiation;

} data_struct_t;

typedef union
{
  data_struct_t data_struct;
  uint8_t data_byte[18];
} packet_t;

//Predefine Parameters
float temp1 = 23;
float hum1 = 54;
float mois1 = 45 ;
float temp2 = 22.8;
float hum2 = 53.8;
float mois2 = 46;
//float temp2, hum2, mois2, rad2;

/*
 *  NODEMCU
 */
 
//Define NodeMCU serial pin
HardwareSerial &esp = Serial;

/*
 *  NEXTION DISPLAY
 */
 
//Define Nextion Display serial pin
SoftwareSerial HMISerial (D6, D5);

//Declare NexObject
NexText nexTemp1 = NexText(0, 2, "Tp1");
NexText nexHum1 = NexText(0, 6, "Hm1");
NexText nexMois1 = NexText(0, 3, "Sm1");

NexText nexTemp2 = NexText(0, 5, "Tp2");
NexText nexHum2 = NexText(0, 7, "Hm2");
NexText nexMois2 = NexText(0, 4, "Sm2");

void setup()
{
  nexInit();
  Serial.begin(115200);
  HMISerial.begin(9600);
  esp.begin(115200);
}

void loop() 
{
  //Initialize and start recieved data from XBee
  if (esp.available() > 0)
  {
    packet_t coord;

    for (int i = 0; i < 18; i++)
    {
      coord.data_byte[i] = esp.read();
    }

    //Verify if packet belong to the system
    if (coord.data_struct.header == 0x55)
    {
      //Store data from SENSOE NODE 1
      if (coord.data_struct.node_id == 1)
      {
        temp1 = coord.data_struct.temperature;
        hum1 = coord.data_struct.humidity;
        mois1 = coord.data_struct.moisture;
        //rad1 = coord.data_struct.radiation;
      }

      //Store data from SENSOE NODE 2
      if (coord.data_struct.node_id == 2)
      {
        temp2 = coord.data_struct.temperature;
        hum2 = coord.data_struct.humidity;
        mois2 = coord.data_struct.moisture;
        //rad2 = coord.data_struct.radiation;
      }
    }
  }
  
  //Display on Nextion
  displayOnNex();
  delay(3000);
}

//Fuction to display on Nextion Display
void displayOnNex ()
{
  //Convert parameters to string
  String temp1Str = String(temp1);
  String hum1Str = String(hum1);
  String mois1Str = String(mois1);

  String temp2Str = String(temp2);
  String hum2Str = String(hum2);
  String mois2Str = String(mois2);

  //Sending parameters to Nextion
  nexTemp1.setText(temp1Str.c_str());
  nexHum1.setText(hum1Str.c_str());
  nexMois1.setText(mois1Str.c_str());

  nexTemp2.setText(temp2Str.c_str());
  nexHum2.setText(hum2Str.c_str());
  nexMois2.setText(mois2Str.c_str());
}
