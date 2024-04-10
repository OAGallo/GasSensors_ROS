#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"  
                                                                  // Library for DHT sensors

#include "ROStopics.hpp"

#define A1 31
#define A2 32
#define A3 34

//************************************Declarations*************************************//
int infoInput_Sensor[3] = {A1,A2,A3};
#define DHTTYPE DHT22
int y = 0;
const int RL_MQ = 1000;                                                              // Module resistor of every MQ sensor
float Slope_MQ[6] = {-0.4077, -0.3537, -0.3542, -0.2663, -0.8363, -0.8785};          // Value of every slop from the calcule in excel
float b_MQ[6] = {0.8042, 1.3617, 1.0530, 1.3230, 3.8416, 2.0889};                    // Value of every intersection point in the slop from the sensitivity graph
float Clean_Air_Theoric_Ratio_MQ[6] = {3.65, 60.86, 4.416, 10, 85.26, 9.723};        // Ratio of the relation RS/R0 which is derivated from the chart
float Clean_Air_Real_Ratio_MQ[6];                               // Real ratio calculated in the moment at the place where the sensors gonna work
float RS_MQ = 0;                                                                     // RS -> The Rs changes as the sensor is in the different consentration of the target gas. 
float R0_MQ = 0;                                                                     // R0 -> The R0 is the sensor resistance in clean air 
float R0_FINAL = 0;
float RS_FINAL = 0;                                                                  
float sensorValue = 0;
float sensorVolts = 0;
DHT dht(infoInput_Sensor[3], DHTTYPE);                                                           // Initialise the DHT library

float Calibration_R0(int);
float PPM_Calculation(int);
float Temperature_Sensor();

void ReadCO2();
void ReadHx();
void ReadHidrogen();
void ReadCO();
void ReadMetano();


// RS_MQ135 =  (RL_MQ135 * (1023-sensorValue)/sensorValue); // 
// R0_MQ135 = RS_MQ135 / Clean_Air_Ratio_MQ135;

void setup(){
   for(int i=0; i <3; i++){
      pinMode(infoInput_Sensor[i], INPUT);
   }
   node.initNode();
   node.advertise(CO2);
   node.advertise(Hx);
   node.advertise(Hidrogen);
   node.advertise(CO);
   node.advertise(Metano);
}

void loop() {
  node.spinOnce();
  ReadCO2();
  ReadHx();
  ReadHidrogen();
  ReadCO();
  ReadMetano();
  delay(500);
}

void ReadCO2(){
  float data = PPM_Calculation(0);
  getCO2.data = 1.222;
  CO2.publish(&getCO2);
}

void ReadHx(){
  float data = PPM_Calculation(1);
  getHexano.data = data;
  Hx.publish(&getHexano);
}

void ReadMetano(){
  float data = PPM_Calculation(2);
  getMetano.data = data;
  Metano.publish(&getMetano);
}

void ReadHidrogen(){
  float data1 = PPM_Calculation(3);
  getHidrogen.data = data1;
  Hidrogen.publish(&getHidrogen);

  float data2 = PPM_Calculation(4);
  getHidrogen.data = data2;
  Hidrogen.publish(&getHidrogen);
}

void ReadCO(){
  float data = PPM_Calculation(5);
  getCO.data = data;
  CO.publish(&getCO);
}






float Calibration_R0(int sensorNumber){
  for(int x = 0 ; x < 100 ; x++){
      sensorValue = sensorValue + analogRead(infoInput_Sensor[y]);    // Reading from the sensor
  }
  sensorValue = sensorValue/100;
  sensorVolts = sensorValue/1024*5;                       // Conversion of the analog information to voltage

  RS_MQ = RL_MQ*((5-sensorVolts)/sensorVolts);            // We use the formula of the voltage divider
  R0_MQ = RS_MQ / Clean_Air_Theoric_Ratio_MQ[sensorNumber];
  return R0_MQ;
}

float PPM_Calculation(int sensorNumber){
  float AnalogRead_Final = analogRead(infoInput_Sensor[y]);
  float sensorValue_Final = AnalogRead_Final/1024*5;
  RS_FINAL = RL_MQ*((5-sensorValue_Final)/sensorValue_Final);
  Clean_Air_Real_Ratio_MQ[sensorNumber] = RS_FINAL/R0_FINAL;           // Calcule of the clean air ratio on the place

  float PPM_MQ = pow(10,((log10(Clean_Air_Real_Ratio_MQ[sensorNumber])-b_MQ[sensorNumber])/Slope_MQ[sensorNumber])); // Calcule the PPM of the gas
  ////Serial.println(PPM_MQ);
  return PPM_MQ;
}

float Temperature_Sensor(){
    float tempValC = 0;
    for(int x = 0; x < 100 ; x++){
    tempValC = dht.readTemperature();
    }
    float tempValC_Final = tempValC/100;
    float valueNeg = 0;
    if(isnan(tempValC_Final)){
      valueNeg = 1;
      return valueNeg;
    }else{
      return tempValC;
    }
}

