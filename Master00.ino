/**
 * Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Update 2014 - TMRh20
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
const uint16_t this_node = 00;   // Address of our node in Octal format (04, 031, etc)
const uint16_t other_node = 01;  // Address of the other node in Octal format
const uint16_t other_node1 = 02;
// container to read data from the previous node on node 01 direction
  struct payload_t
   {  // Structure of our payload
            char indication;
            float previous_Temp2;
            float previous_Humid2;
            double previous_node2;
            float previous_Temp3;
            float previous_Humid3;
            double previous_node3;
            float previous_Temp1;
            float previous_Humid1;
            double previous_node1;
  };
// container to read data from the previous node on node 02 direction
struct nextpayload_t {
  char indication;
  float previous_Temp2;
  float previous_Humid2;
  double previous_node2;
  float previous_Temp1;
  float previous_Humid1;
  double previous_node1;
  // float this_Temp;
  // float this_humid;
  // double layer_node;
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
  
  while (network.available()) 
        {  // Is there anything ready for us?

          RF24NetworkHeader header;  // If so, grab it and print it out
          payload_t payload;
          network.read(header, &payload, sizeof(payload));
          // checking the node that received 

          uint16_t senderNode = header.from_node;
            if (senderNode == other_node) 
                    {
                      Serial.print(F("Right Wing: -------01:::"));
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
                      Serial.print(F("Received packet 3: origin-Node="));
                      Serial.print(payload.previous_node1);
                      Serial.print(F(", Temp="));
                      Serial.print(payload.previous_Temp1);
                      Serial.print(F(", Humid="));
                      Serial.print(payload.previous_Humid1);
                      
                      Serial.print("\t");
                      Serial.println("");
                    } 
          else if (senderNode == other_node1) 
                  {
                    Serial.print(F("Left Wing ----02 :::" ));
                    Serial.print(F("Received packet 1: origin-Node="));
                    Serial.print(nextpayload.previous_node2);
                    Serial.print(F(", Temp="));
                    Serial.print(nextpayload.previous_Temp2);
                    Serial.print(F(", Humid="));
                    Serial.print(nextpayload.previous_Humid2);
                    
                    Serial.print("\t");
                    Serial.print(F("Received packet 2: origin-Node="));
                    Serial.print(nextpayload.previous_node1);
                    Serial.print(F(", Temp="));
                    Serial.print(nextpayload.previous_Temp1);
                    Serial.print(F(", Humid="));
                    Serial.print(nextpayload.previous_Humid1);
                    Serial.print("\t");
                    Serial.println("");
                    

                  } 
          else 
                {
                  Serial.print(F("Received packet no packet received>"));
                }
      }
}
