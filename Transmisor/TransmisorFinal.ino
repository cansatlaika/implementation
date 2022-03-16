/*  Prueba distancia NRF24L01 programa del Transmisor
    NRF24 // Arduino UNO,NANO 
    GND    ->   GND
    Vcc    ->   3.3V Fuente Externa
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12
*/
#include <SPI.h>
#include <Wire.h>
#include <LPS.h>
#include <LSM6.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <nRF24L01.h>
#include <RF24.h>          
//Motor
#include <Servo.h>

//Se inicializan las instancias del sistema de comunicaciones:
const byte identificacion[6]="00001";  //Mismo codigo en el RX y TX, tiene que ser el mismo numero en el receptor
RF24 radio(9, 10);                      // asigna pines para CSN y CE


//Se inicializan las instancias de los sensores:
LPS ps;
LSM6 imu;
TinyGPSPlus gps;
char report[80];
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin,TXPin);

//DECLARACIÓN SERVO
Servo myservo;  // crea el objeto servo
float AltitudRef=930; //Se establece la altitud en la cual se quiere realizar el despliegue del Cansat

void setup()
{ 
  myservo.attach(5);
  myservo.write(50);
  Serial.begin(9600);  
  ss.begin(GPSBaud);
  Wire.begin();
    if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
    
     if (!ps.init()) //PB
  {
    Serial.println("Failed to autodetect pressure sensor!");
    while (1);
  }
  ps.enableDefault();
  radio.begin(); //Comienza la operación del chip
  radio.setAutoAck(false);  //Auto-acknowledgement responds to every recieved payload with an empty ACK packet. If this feature is disabled on a transmitting radio, then the transmitting radio will always report that the payload was recieved (even if it was not).
  //**CONFIGURACIÓN***
  radio.setDataRate(RF24_2MBPS);  // configura 250KBPS, 1BMPS, 2MBPS
  radio.openWritingPipe(identificacion);  // Open a pipe for writing with a specific adress
  radio.setPALevel(RF24_PA_MIN); // Set transmit power
  radio.setChannel(127); // Set the channel to 2 (2527 MHz)
  radio.stopListening(); // Stop listening for incoming messages, and switch to transmit mode.
}

float vector1[10]; //En este vector se guardan los datos provenientes de los sensores

void loop()
{
  vector1[0] = analogRead(A0);  // Variable para almacenar el valor obtenido del sensor de temperatura LM35 (0 a 1023)
  vector1[0] = (5 * vector1[0] * 100)/1024; //conversión a celsius 
  imu.read();
  float pressure = ps.readPressureMillibars(); //Obtenemos el valor de la presión  
  vector1[1] = pressure*100;
  vector1[2] = ps.pressureToAltitudeMeters(pressure); //Obtenemos la altitud
  //Obtenemos todos los datos del IMU
  float imuax=imu.a.x;
  vector1[3]=imuax*0.061*0.001;
  float imuay=imu.a.y;
  vector1[4]=imuay*0.061*0.001;
  float imuaz=imu.a.z;
  vector1[5]=imuaz*0.061*0.001;
  float imugx=imu.g.x;
  vector1[6]=imugx*0.061*0.001;
  float imugy=imu.g.y;
  vector1[7]=imugy*0.061*0.001;
  float imugz=imu.g.z;
  vector1[8]=imugz*0.061*0.001;
  String lati_s;
  String longi_s;
//  GPS NEO 6M, si no hay GPS disponible, se guardan en las variables lati_s y longi_s 0.0 y 0.0
  while (ss.available() > 0)
      gps.encode(ss.read());
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  if (gps.location.isValid())
  { //Esta por probarse si esta porcion del codigo sirve
    vector1[9] = gps.location.lat();
    lati_s = String(vector1[9], 2);
    vector1[10] = gps.location.lng();
    longi_s = String(vector1[10], 2);
  }
   else
  {
     lati_s = "0.0";
     longi_s = "0.0";
  }  

  //Despliegue de paracaídas, si la altitud actual es mayor o igual que la altitud puesta en la referencia, se inicializa el despliegue
  float AltitudActual = vector1[2];
  if(AltitudActual >= AltitudRef){
  Serial.println("entro altura");
   myservo.write(50);
   delay(500);
   myservo.write(180);
  }
  //A partir de este punto lo restante corresponde al protocolo de transmisión del sistema de comunicaciones
  String temp_s, presion_s, altura_s, imuax_s, imuay_s, imuaz_s, imugx_s, imugy_s, imugz_s, paquete1, paquete2, paquete3;
  char trama1[32], trama2[32], trama3[32];
  temp_s = String(vector1[0], 2);
  presion_s = String(vector1[1], 2);
  altura_s = String(vector1[2], 0);
  imuax_s = String(vector1[3], 1);
  imuay_s = String(vector1[4], 1);
  imuaz_s = String(vector1[5], 1);
  imugx_s = String(vector1[6], 1);
  imugy_s = String(vector1[7], 1);
  imugz_s = String(vector1[8], 1);
  paquete1 = temp_s + "&" + presion_s + "&" + altura_s + "&" + lati_s + "&" + longi_s + "*";
  paquete2 = imuax_s + "&" + imuay_s + "&" + imuaz_s + "&" + imugx_s + "&" + imugy_s + "&" + imugz_s + "#";
  paquete1.toCharArray(trama1, 32);
  paquete2.toCharArray(trama2, 32);
  Serial.println(paquete1);
  Serial.println(paquete2);
  radio.write(&trama1, sizeof(trama1));
  radio.write(&trama2, sizeof(trama2));
  delay(1000);
  //El siguiente codigo es para verificar que el modulo transmisor nRF24l01 se encuentre correctamente conectado
  //bool result = radio.isChipConnected();
  //Serial.println (result);
}
