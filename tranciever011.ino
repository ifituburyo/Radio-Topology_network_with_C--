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
const uint16_t this_node = 02;   // Address of our node in Octal format (04, 031, etc)
const uint16_t other_node = 012;  // Address of the other node in Octal format
const uint16_t next_node = 00;
// for reading the received package;
struct payload_t {  // Structure of our payload
  float Temp;
  float Humid;
  double node_of_origin_slave1;
  
};
struct nextpayload_t {
  float previous_Temp;
  float previous_Humid;
  double previous_node;
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
  network.update();  // Check the network regularly
  
  while (network.available()) {  // Is there anything ready for us?

    RF24NetworkHeader header;  // If so, grab it and print it out
    payload_t payload;
    
    network.read(header, &payload, sizeof(payload));
    Serial.print(F("Received packet: origin-Node="));
    Serial.print(payload.node_of_origin_slave1);
    Serial.print(F(", Temp="));
    Serial.print(payload.Temp);
    Serial.print(F(", Humid="));
    Serial.print(payload.Humid);
    Serial.print(F(", This  Data:"));
    Serial.print("\t");
    
    nextpayload.previous_Humid = payload.Humid;
    nextpayload.previous_Temp = payload.Temp;
    nextpayload.previous_node= payload.node_of_origin_slave1;
    nextpayload.layer_node=211;
    nextpayload.this_humid=H;
    nextpayload.this_Temp= T;
    // this node
    Serial.print(F(": this-Node="));
    Serial.print(nextpayload.layer_node);
    Serial.print(F(", Temp="));
    Serial.print(nextpayload.this_Temp);
    Serial.print(F(", Humid="));
    Serial.print(nextpayload.this_humid);
    Serial.print(F(" Sent to next layer :"));
    Serial.print(next_node);
     RF24NetworkHeader headertosend(next_node);
   bool ok = network.write(headertosend, &nextpayload, sizeof(nextpayload));
          Serial.println(ok ? F("Send ok.") : F("Sent failed."));
  }
    Serial.print(F(": this-Node="));
    Serial.print(nextpayload.layer_node);
    Serial.print(F(", Temp="));
    Serial.print(nextpayload.this_Temp);
    Serial.print(F(", Humid="));
    Serial.print(nextpayload.this_humid);
    Serial.print(F(" Sent to next layer :"));
    Serial.print(next_node);
  // nextpayload_t nextpayload;
   RF24NetworkHeader headertosend(next_node);
   bool ok = network.write(headertosend, &nextpayload, sizeof(nextpayload));
          Serial.println(ok ? F("Send ok.") : F("Sent failed."));
  

}
