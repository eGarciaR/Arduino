#include <SPI.h>
#include <Ethernet.h>
 
//Se introducen los valores correspondientes a MAC, IP local, Puerta de Enlace y Máscara de Red
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
IPAddress ip(192,168,1,60);
 
IPAddress gateway(192, 168, 1, 1);
 
IPAddress subnet(255, 255, 255, 0);
 
//Se inicializa la librería Ethernet con el Puerto que se va utilizar en la transmisión
EthernetServer server(80);

int Trig=35;
int Echo=33;

int Dist;

int timesDetected = 0;

int distanciaAlarma[50];
 
void setup() {
 // Se establece comunicación con el monitor serial (para ver si el sistema está funcionando correctamente)
  Serial.begin(9600);
 
  // Comienzo de la conexión
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //Se muestra por el monitor serial que la conexión está establecida
  //Serial.print("server is at ");
  //Serial.println(Ethernet.localIP());

  pinMode (Trig, OUTPUT);
  pinMode (Echo, INPUT);
}

void ultrasonido (int &Distancia){
 
  //Para estabilizar el valor del pin Trig se establece a LOW
  digitalWrite (Trig, LOW);
  delay(2);
   
  //Se lanzan los 8 pulsos
  digitalWrite (Trig, HIGH);
  delay(5);
  digitalWrite (Trig, LOW);
   
  /*
  Se mide el tiempo que tarda la señal en regresar y se calcula la distancia.
   
  Observa que al realizar pulseIn el valor que se obtiene es tiempo, no distancia
   
  Se está reutilizando la variable Distancia.
  */
   
  Distancia= pulseIn (Echo, HIGH);
  Distancia=Distancia/58;

  if (Distancia < 50 && Distancia > 0) {
    distanciaAlarma[timesDetected] = Distancia;
    ++timesDetected;
  }
   
  //delay(100);
  
}

void mostrarValor(EthernetClient client) {
  /*for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    client.print("analog input ");
    client.print(analogChannel);
    client.print(" is ");
    client.print(sensorReading);
    client.println("<br />");       
  }*/
  ultrasonido (Dist);

  if (timesDetected > 0) {
    for (int i = 0; i < timesDetected; ++i) {
      client.print("<h1 style=");
      client.print("color:red;");
      client.println(">");
      client.println("MOVIMIENTO DETECTADO:");
      client.println(distanciaAlarma[i]);
      client.println("</h1>");
    }
  }
  else {
    client.println("<h1>");
    client.println("SENSOR:");
    client.println(Dist);
    client.println("</h1>");
  }
}
 
 
void loop() {
  // Se espera a que alguien acceda a la página web
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    // una petición http termina con una línea en blanco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        //Si se llega al final de la línea se recive un nuevo carácter de
        //línea y si se trata de una línea en blanco la petición http ha terminado
        //así que se envía la respuesta 
        if (c == '\n' && currentLineIsBlank) {
          // cabezera típica http
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // se cierra la conexión una vez se ha respondido a la petición
          client.println("Refresh: 1");  // se refresca la página automáticamente cada 3 segundos
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          mostrarValor(client);
          // se muestran en la web los valores de las entradas analógicas
          /*for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }*/
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // se comienza una nueva línea
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // se ha obtenido un carácter en la línea actual
          currentLineIsBlank = false;
        }
      }
    }
    // se le da tiempo al navegador para recibir los datos
    delay(1);
    // se cierra la conexión
    client.stop();
    //Serial.println("client disonnected");
  }
}
