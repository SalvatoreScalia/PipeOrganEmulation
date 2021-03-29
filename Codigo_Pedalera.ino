
/* http://www.arduino.cc/en/Tutorial/Midi */

//VARIABLES DE ESTADO PASADO/PRESENTE
int nota_pasado[32];
int nota_presente[32];
int volumen_presente;
int volumen_pasado;
int foot_presente;
int foot_pasado;

//VARIABLES
int t;
int analog0;
int controlChange = 176;            //Command 176: 0xB0 (MIDI Channel 1)
int FootController = 4;       //ContinouousController,  Command   4: FootController  ->  Often used with aftertouch messages. It can send a continuous stream of values based on how the pedal is used.  http://nickfever.com/music/midi-cc-list                    
int VolumeController = 7;     //ContinouousController,  Command   7: Volume          ->  Control the volume of the channel.                                                                              http://nickfever.com/music/midi-cc-list
int channel = 0;                //command  nn: channel 1

//Pines para teclado   
int x=8,y=4;      //  x: OUTPUT     y: INPUT_PULLUP
//          x  y
int teclado[8][4]={{  0, 8,16,24 }, 
                   {  1, 9,17,25 },
                   {  2,10,18,26 },
                   {  3,11,19,27 },
                   {  4,12,20,28 },
                   {  5,13,21,29 },
                   {  6,14,22,30 },
                   {  7,15,23,31 }};

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(31250);

 
  //volumen de la nota
  pinMode(A0,INPUT);

  //INICIAR PINES DE ENTRADA (PULL UP)
  for(int i = 2+x; i < 2+x+y; i++){
    pinMode(i, INPUT_PULLUP);
  }

  //INICIAR PINES DE SALIDA
  for(int i = 2; i < x+2; i++){
    pinMode(i, OUTPUT);  
  }
}

void loop() {
  
  //LECTURA DE PINES DE SALIDA
  for(int j = x+2; j < x+2+y; j++){
  
    //ACTIVACION DE PINES DE ENTRADA
    for(int i = 2; i < x+2; i++){
      //____________________________________________________________________________________________________________________________________________for

      //__RESETEAR LOS PINES__________________________
      //__for( t = 2; t < 2+x; t++){digitalWrite(t, HIGH);}

        //ACTIVAR PIN
        digitalWrite(i, LOW);

      //foot_controller_________read
      foot_presente = 127;                                //-> actualization___127 -> vol_2();
      //foot_controller_________comparator   
      if(foot_presente != foot_pasado){

        Serial.write(177);   //command 177: 10110001 : 0xB0 + channel: 2 (ControlChange)
        Serial.write(FootController);     //foot controller
        Serial.write(foot_presente); //foot_presente
      }

        //DEFINIR NOTA  aux es igual a una nota desde 0 a 32 
        int aux = teclado[i-2][j-(x+2)];

//____________________________________________________________________

      //DETECTAR TECLA PRESIONADA
      if (digitalRead(i) == LOW && digitalRead(j) == LOW){
        nota_presente[aux] = HIGH;
      }
      else{
        nota_presente[aux] = LOW;
      }

      //ENVIAR PACKET ON/OFF
      if(nota_presente[aux] != nota_pasado[aux]){
        if(nota_presente[aux] == HIGH){          
        
        //ENVIAR NOTE ON
        Serial.write(144+channel);//(int)(2 + aux)/2);       //command 144:  1001cccc  :  0x90 +  channel
        Serial.write(36+aux);                     //command  36:  00100100  :  0x24  key C    (C central = 0x3c,  4° do piano)
        Serial.write(127);                        //Control volumen : 127 0x7F
        }
        
        else{
        //ENVIAR NOTE OFF
        Serial.write(128+channel);//(int)(2 + aux)/2); ->  Channel swith
        Serial.write(36+aux);
        Serial.write(0);
        
        }
      }         

      //COPIAR LA VARIABLE DEL PRESENTE AL PASADO
      nota_pasado[aux] = nota_presente[aux];
     
     //RESET PIN__________________________ 
     digitalWrite(i, HIGH);
     
     
     foot_pasado = foot_presente;
     //____________________________________________________________________________________________________________________________________________for
     }

      /*volumen_presente = vol();
      //Volumen Channel_____________________________________________     
      if(volumen_presente != volumen_pasado){
        //for(controlChange = 177; controlChange < 191; controlChange++){     //Command 177&191: 0xB1 0xBF    Chennel 2 to 16
          Serial.write(controlChange);
          Serial.write(VolumeController);
          Serial.write(vol_1());
        //}
      }
      volumen_pasado = volumen_presente;
      */
          
  }  
}

//función volumen
int vol_1(){
          int analog0 = analogRead(A0);
          int volumen_presente = map(analog0,0,1023,0,127);
          return volumen_presente;

          
  }

//foot_function
int vol_2(){
          int analog0 = analogRead(A0);
          int foot_presente = map(analog0,0,1023,0,127);
          return foot_presente;
          
  }
