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
  lcd.setCursor(0, 0); //LCD empieza desde bit 0 linea 0
  lcd.print("Corriente (A):");
  lcd.setCursor(0, 1); //LCD emopieza desde bit 0 linea 1
  lcd.print("Potencia (W):");
}

void loop() {
  float sensorPin = analogRead(A0);  // Pin del sensor SCT-013
  float corriente = sensorPin / currentRatio;
  float potenciaActiva = corriente * voltageReference;

  EthernetClient client = server.available();

  if (client) {
    if (client.connected()) {

      lcd.setCursor(13, 0);
      lcd.print("      ");  // Borrar el valor anterior
      lcd.setCursor(13, 0);
      lcd.print(corriente, 4);  // Mostrar la corriente en el LCD

      lcd.setCursor(13, 1);
      lcd.print("      ");  // Borrar el valor anterior
      lcd.setCursor(13, 1);
      lcd.print(potenciaActiva, 2);  // Mostrar la potencia activa en el LCD

      delay(1000);

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();

      client.println("<html><body>");
      client.print("Corriente (A): ");
      client.print(corriente, 4);
      client.println("<br>");
      client.print("Potencia Activa (W): ");
      client.print(poetnciaActiva, 2);
      client.println("</body></html>");

      client.stop();
    }
  }  
}
