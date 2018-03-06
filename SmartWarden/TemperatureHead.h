#include <dht.h>
dht DHT;

#define DHT11_PIN 13
char TTT[100];
char TTTT[100];
char HHH[100];

void Tempu()
{
  int chk = DHT.read1111(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);

  int ff = (DHT.temperature*1.8) + 32;
  *TTTT='\0';
  sprintf(TTTT,"%d",ff);
  *TTT='\0';
  sprintf(TTT,"%d",DHT.temperature);
  *HHH='\0';
  sprintf(HHH,"%d",DHT.humidity);
  strcat(TTTT,"'F");
  strcat(TTT,"'C");
  strcat(HHH,"%");
}
