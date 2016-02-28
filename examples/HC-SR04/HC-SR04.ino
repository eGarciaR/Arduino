int Trig=35;
int Echo=33;
 
//Variable en la que se va a almacenar el valor correspondiente a la distancia
int Dist;
 
void setup() {

Serial.begin(9600);
 
pinMode (Trig, OUTPUT);
pinMode (Echo, INPUT);

pinMode(31,OUTPUT);
digitalWrite(31,LOW);

pinMode(37,OUTPUT);
digitalWrite(37,HIGH);
 
}
 
//Este módulo calcula y devuelve la distancia en cm.
/*
Puedes poner el código del módulo directamente en el loop o utilizar el módulo
para reducir el número de líneas de código del loop o reutilizar el código
*/
void ultrasonido (int &Distancia){
 
//Para estabilizar el valor del pin Trig se establece a LOW
digitalWrite (Trig, LOW);
delay(10);
 
//Se lanzan los 8 pulsos
digitalWrite (Trig, HIGH);
delay(10);
digitalWrite (Trig, LOW);
 
/*
Se mide el tiempo que tarda la señal en regresar y se calcula la distancia.
 
Observa que al realizar pulseIn el valor que se obtiene es tiempo, no distancia
 
Se está reutilizando la variable Distancia.
*/
 
Distancia= pulseIn (Echo, HIGH);
Distancia=Distancia/58;
 
delay(100);
  
}
 
void loop() {
 
ultrasonido (Dist);

Serial.println(Dist);
 
delay (250);
}
