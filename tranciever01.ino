/**
 Author name : Izera ifituburyo pacifique 
 
 University of carnegie mellon Africa 
 department : University of carnegie melon , Electrical and computer engineering
  */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */

#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <DHT22.h>

#define pinDATA 4
DHT22 dht22(pinDATA);

RF24 radio(7, 8);  // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 01;   // Address of our node in Octal format (04, 031, etc)
const uint16_t other_node = 011;  // Address of the other node in Octal format
const uint16_t next_node = 00;
// container to read data from the previous node 
  struct payload_t
   {  // Structure of our payload
              float previous_Temp2;
              float previous_Humid2;
              double previous_node2;
              float previous_Temp3;
              float previous_Humid3;
              double previous_node3;
  };
  payload_t payload;
  //container to add the data ready to be sent 
struct nextpayload_t {
  char indication;
  float previous_Temp2;
  float previous_Humid2;
  double previous_node2;
  float previous_Temp3;
  float previous_Humid3;
  double previous_node3;
  float this_Temp;
  float this_humid;
  double layer_node;
};
nextpayload_t nextpayload;


void setup(void) {
  Serial.begin(115200);
  Serial.print("Humidity");
  Serial.print("\t Temperature");
  Serial.println("\n");
  while (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF24Network/examples/helloworld_rx/"));

  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      // hold in infinite loop
    }
  }
  radio.setChannel(90);
  network.begin(/*node address*/ this_node);
}

void loop(void) {
  float T = dht22.getTemperature();
  float H = dht22.getHumidity();
  delay(500);
  network.update();  // Check the network regularly
  
  while (network.available()) {  // Is there anything ready for us?

    RF24NetworkHeader header;  // If so, grab it and print it out
    
    network.read(header, &payload, sizeof(payload));
    Serial.print(F("Received packet 1 : origin-Node="));
    Serial.print(payload.previous_node3);
    Serial.print(F(", Temp="));
    Serial.print(payload.previous_Temp3);
    Serial.print(F(", Humid="));
    Serial.print(payload.previous_Humid3);
    // Serial.print(F(",   Data:"));
    Serial.print(F("Received packet 2: origin-Node="));
    Serial.print(payload.previous_node2);
    Serial.print(F(", Temp="));
    Serial.print(payload.previous_Temp2);
    Serial.print(F(", Humid="));
    Serial.print(payload.previous_Humid2);
    
    Serial.print("\t");
    nextpayload_t nextpayload;
    nextpayload.previous_node3 = payload.previous_node3;
    nextpayload.previous_Temp3 = payload.previous_Temp3;
    nextpayload.previous_Humid3= payload.previous_Humid3;
    nextpayload.previous_node2 = payload.previous_node2;
    nextpayload.previous_Temp2 = payload.previous_Temp2;
    nextpayload.previous_Humid2 = payload.previous_Humid2;
    nextpayload.layer_node = 11;
    nextpayload.indication= "R";
    nextpayload.this_Temp= T;
    nextpayload.this_humid=H;
    // printing this data of this node. 
    Serial.print(F(": this-Node="));
    Serial.print(nextpayload.layer_node);
    Serial.print(F(", Temp="));
    Serial.print(nextpayload.this_Temp);
    Serial.print(F(", Humid="));
    Serial.print(nextpayload.this_humid);
    Serial.print(F(" Sent to next layer :"));
    Serial.println(next_node);
     RF24NetworkHeader headertosend(next_node);
   bool ok = network.write(headertosend, &nextpayload, sizeof(nextpayload));
          Serial.println(ok ? F("Send ok.") : F("Sent failed."));
  } 
    nextpayload.layer_node = 11;
    nextpayload.indication= "R";
    nextpayload.this_Temp= T;
    nextpayload.this_humid=H;
    Serial.print(F(": this-Node="));
    Serial.print(nextpayload.layer_node);
    Serial.print(F(", Temp="));
    Serial.print(nextpayload.this_Temp);
    Serial.print(F(", Humid="));
    Serial.print(nextpayload.this_humid);
    Serial.print(F(" Sent to next layer :"));
    Serial.println(next_node);
     RF24NetworkHeader headertosend(next_node);
   bool ok = network.write(headertosend, &nextpayload, sizeof(nextpayload));
          Serial.println(ok ? F("Send ok.") : F("Sent failed."));
  // nextpayload_t nextpayload;
  

}

/*

code 
*/