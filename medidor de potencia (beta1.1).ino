const int sensorPin = A0; // Pin analógico al que está conectado el sensor
const float referenciaVoltaje = 5.0; // Voltaje de referencia del Arduino (en V)
const float resistorValor = 10.0; // Valor de la resistencia utilizada en el circuito (en ohmios)

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin); // Leer el valor analógico
  float voltage = (sensorValue / 1023.0) * referenciaVoltaje; // Convertir el valor a voltaje
  float corriente = voltage / resistorValor; // Calcular la corriente
  float potencia = voltage * corriente; // Calcular la potencia

  Serial.print("Voltaje: ");
  Serial.print(voltage, 2); // Mostrar el voltaje con 2 decimales
  Serial.print(" V\t");

  Serial.print("Corriente: ");
  Serial.print(corriente, 2); // Mostrar la corriente con 2 decimales
  Serial.print(" A\t");

  Serial.print("Potencia: ");
  Serial.print(potencia, 2); // Mostrar la potencia con 2 decimales
  Serial.println(" W");

  delay(1000); // Esperar 1 segundo antes de realizar otra lectura
}
