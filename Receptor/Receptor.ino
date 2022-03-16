#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);                  // Establece los pines de CE, CSN

const byte identificacion[6] = "00001"; // Se le asigna una dirección para la transmisión
char caract_especial_actual = ' ';
char caract_especial_anterior = ' ';

void setup() {
  Serial.begin(9600);               // Iniciamos la comunicacion serie hacia el Monitor Serie en la PC

  radio.begin();

  radio.openReadingPipe(0, identificacion);
  radio.setChannel(127);            //Establece el canal de comunicación
  radio.setAutoAck(false);          //Desactiva el AutoAck (Confirmación de llegada de paquetes)
  radio.setPALevel(RF24_PA_MIN);    //Establece el nivel de potencia
  radio.setDataRate(RF24_2MBPS);  //Establece el bitrate para la transmisión
  radio.startListening();           // Setea en modo receptor
  
}

void loop() {
  if (radio.available()) {
    const char reception[32];
    radio.read(&reception, sizeof(reception));

    char paquete1[32],paquete2[32], paquete3[32];

  //****ALGORITMO DE RECONOCIMIENTO Y CLASIFICACIÓN DE PAQUETES****
  
  for(int i =0; i<=sizeof(reception); i++){  // Recorre cada elemento de una trama para encontrar el Caracter Especial distintivo de cada paquete

    // ---PRIMER PAQUETE---
   if(reception[i] == '*'){     
      for(int j1=0; j1<32;j1++){
        paquete1[j1] = reception[j1];   //Guarda el paquete ya reconocido para imprimir
      }
      caract_especial_actual = '*';
      }

    // ---SEGUNDO PAQUETE---
   if(reception[i] == '#'){     
      for(int j2=0; j2<32;j2++){
        paquete2[j2] = reception[j2];   //Guarda el paquete ya reconocido para imprimir
      }
      caract_especial_actual = '#';
      }
      
    // ---TERCER PAQUETE---
    if(reception[i] == '~'){     
      for(int j3=0; j3<32;j3++){
        paquete3[j3] = reception[j3];   //Guarda el paquete ya reconocido para imprimir
      }
      caract_especial_actual = '~';
      }   

   }

//-------------------------------------------------------------

   
  //****ALGORITMO DE IMPRENTA****
  
  if(caract_especial_actual == '*'){
        if( (caract_especial_anterior == '#') || (caract_especial_anterior == caract_especial_actual) ){
          Serial.println(' ');    //Imprime saltos de línea si se llega a perder el último paquete
        }
       Serial.print('*');
       Serial.print(paquete1);      //Imprime el paquete 1
       
   }
   else if(caract_especial_actual == '#'){
        if(caract_especial_anterior == caract_especial_actual){
          Serial.println(' ');    //Imprime saltos de línea si se llega a perder el último paquete
        }
       Serial.print('#');
       Serial.print(paquete2);      //Imprime el paquete 2
   }
   else if(caract_especial_actual == '~'){
       Serial.print('~');
       Serial.println(paquete3);    //Imprime el paquete 3
   }
   
   caract_especial_anterior = caract_especial_actual;  //Guarda el caracter especial del paquete anterior para la lógica siguiente

  //------------------------------------------------------
     
  }

 else {
 }
 
}
