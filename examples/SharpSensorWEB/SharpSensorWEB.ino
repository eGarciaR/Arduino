#include <SPI.h>
#include <Ethernet.h>
 
//Se introducen los valores correspondientes a MAC, IP local, Puerta de Enlace y Máscara de Red
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
IPAddress ip(192,168,1,60);
 
IPAddress gateway(192, 168, 1, 1);
 
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(80);
EthernetClient client;

int cont = 0;
int movementsDetected = 0;
bool detected = false;
 
void setup() {
  Serial.begin(9600);

  pinMode(12,OUTPUT);
  digitalWrite(12,HIGH);
 
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  client = server.available();

}

void detectMotion(int sensorData) {
  if (sensorData > 100) {
    ++cont;
    if (cont > 10) {
      detected = true;
    }
  }
}

void mostrarValor(EthernetClient client) {
  cont = 0;
  if (detected) {
    ++movementsDetected;
    detected = false;
  }
  if (movementsDetected > 0) {
    client.print("<h1 style=");
    client.print("color:red;");
    client.println(">");
    client.println("MOVIMIENTO DETECTADO:");
    client.println(movementsDetected);
    client.println("</h1>");
  }
  else {
    client.print("<h1 style=");
    client.print("color:black;");
    client.println(">");
    client.println("NO SE HA DETECTADO MOVIMIENTO");
    client.println("</h1>");
  }
}

void loop() {
  int sensorData = analogRead(0);
  detectMotion(sensorData);
  Serial.println(sensorData);
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && currentLineIsBlank) {
          // cabezera típica http
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          client.println("Refresh: 5"); 
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          mostrarValor(client);
          client.println("</html>");
          delayMicroseconds(10);
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delayMicroseconds(1);
    client.stop();
  }
}
