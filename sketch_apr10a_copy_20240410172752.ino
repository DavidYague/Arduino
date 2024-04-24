#include <WiFi.h>
#include <WebServer.h>

#define trig 5 // Emisor de pulso o señal
#define echo 18 // Receptor "del eco" del pulso o señal
#define buzzer 4 // Zumbador
#define puerta 2 // Puerta para enviar 5V

const char* ssid = "Daniel"; // Nombre de tu red WiFi
const char* password = "123456789"; // Contraseña de tu red WiFi

WebServer server(80); // Creamos un servidor web en el puerto 80

int contador = 0; // Variable para contar las veces que suena el zumbador

void setup() {
  Serial.begin(115200);

  pinMode(trig, OUTPUT); // Emisor
  pinMode(echo, INPUT); // Receptor
  pinMode(buzzer, OUTPUT); // Zumbador
  pinMode(puerta, OUTPUT); // Puerta

  digitalWrite(puerta, HIGH); // Establecer el pin de la puerta en estado alto (5V)


  WiFi.begin(ssid, password); // Conectamos a la red WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");

  server.on("/", HTTP_GET, handleRoot); // Definimos la ruta de la página web
  server.begin(); // Iniciamos el servidor web
   // Iniciar el servidor
  server.begin();
  Serial.println("Servidor iniciado");

  Serial.print("Dirección IP del ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  long duration, distance;

  digitalWrite(trig, LOW);
  delay(2);
  digitalWrite(trig, HIGH);
  delay(5);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration/2)*0.0343;

  if (distance < 10) {
    
    digitalWrite(buzzer, HIGH);
    delay(5000);
    contador++; // Incrementamos el contador cada vez que suene el zumbador
  } else {
    digitalWrite(buzzer, LOW);
  }

  server.handleClient(); // Manejamos las solicitudes de clientes web
}

void handleRoot() {
  server.send(200, "text/html", "<h1>Contador de veces que ha sonado el zumbador:</h1><p>" + String(contador) + "</p>"); // Enviamos la respuesta HTML con el contador
}