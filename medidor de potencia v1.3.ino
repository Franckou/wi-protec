#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

const double voltageReference = 220.0;  // Tensión de referencia del Arduino en voltios
const double currentRatio = 50.0;  // Ratio de transformación de la pinza SCT-013
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // Dirección MAC de tu Ethernet Shield
IPAddress ip(192,168,0,235);  // Dirección IP de tu Arduino
EthernetServer server(80);
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Dirección I2C y dimensiones del LCD (16x2)

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();
  lcd.init();  // Inicializar el LCD
  lcd.backlight();  // Encender el retroiluminación del LCD
}

void loop() {
  float sensorPin = analogRead(A0);  // Pin del sensor SCT-013
  float corriente = sensorPin / currentRatio;
  float potenciaActiva = corriente * voltageReference;
  static double energiaAcumulada = 0;
  energiaAcumulada += (potenciaActiva / 1000.0); // Acumula la energía en kilovatios-hora
  float valorkwh = 22.969; //cambiar el valor de la tarifa cunado sea necesario
  float kwhenpesos = energiaAcumulada * valorkwh;
  
  EthernetClient client = server.available();

  if (client) {
    if (client.connected()) {

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();

      client.println("<html><body>");
      client.print("Bienvenido a tu control de energia");
      client.println("<br>");
      client.print("Corriente (A): ");
      client.print(corriente, 2);
      client.println("<br>");
      client.print("Potencia Activa (W): ");
      client.print(potenciaActiva, 2);
      client.println("<br>");
      client.print("Energía en Kwh: ");
      client.print(energiaAcumulada, 2);
      client.println("<br>");
      client.print("Kilovatios consumidos: ");
      client.print(energiaAcumulada, 2);
      client.println("<br>");
      client.print("Precio final del KwH: ");
      client.print(kwhenpesos, 2);
      client.println("</body></html>");

      client.stop();
    }
  }  
  
  lcd.clear();      
  lcd.setCursor(0, 0); //LCD empieza desde bit 0 linea 0
  lcd.print("Corriente(A):");
  lcd.setCursor(0, 1); //LCD emopieza desde bit 0 linea 1
  lcd.print("Potencia(W):");
  lcd.setCursor(14, 0);
  lcd.print(corriente, 2);  // Mostrar la corriente en el LCD
  lcd.setCursor(13, 1);
  lcd.print(potenciaActiva, 2);  // Mostrar la potencia activa en el LCD
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Kwh acum:");
  lcd.setCursor(0, 1); 
  lcd.print("valor $:");
  lcd.setCursor(9, 0); 
  lcd.print(energiaAcumulada, 2);
  lcd.setCursor(9, 1); 
  lcd.print(kwhenpesos, 2);
  delay(2000);
  }
