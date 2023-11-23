#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   // Esta es la direccion Mac que used le asigna a la ethernet Shield
IPAddress ip (192,168,0,236);  // Esta es la direccion IP (Estatica) que usted le asigna a la ethernet Shield ARDUINOOOO
EthernetServer servidor(80); 
int PIN_LED = 8;              // Pin que tiene asignado el led.
String readString=String(30);  // Este el el nombre de la variable tipo String en donde se alamacenar mensajes HTTP del cliente o solictante
String state = String(3);
int ldrValue = 0; // Variable para almacenar el valor del LDR


void setup() {
Ethernet.begin(mac,ip); // inicializa mac e ip
pinMode(PIN_LED,OUTPUT);
digitalWrite(PIN_LED,LOW);
state = "ON";     // Variable State es la variable que se le envia al cliente en el script HTML para indicar el estado actual del led
Serial.begin(9600);  
Serial.print("Server is At: ");
Serial.println(Ethernet.localIP());
pinMode(A0, INPUT); // Configura A0 como entrada para el LDR

}

void loop() {

EthernetClient cliente = servidor.available();
ldrValue = analogRead(A0); // Lee el valor del LDR
if(cliente)         // Verifica si hay un cliente disponible intentando conexion.
Serial.print(ldrValue);
{
  boolean lineaenblanco=true;
  while(cliente.connected()){       // si hay un cliente realiza la conexion 
    if(cliente.available())      
    {
      char c = cliente.read();      // Lee un byte del mensaje del cliente
      Serial.write(c);
      if (c == '\n'){
      Serial.println("NEW LINE");  
      }
      if(readString.length()<30)    // Evalue mensajes continuos del cliente de no mas de 30 caracteres.
      {
        readString.concat(c);       // Concatena el byte actualmente recibido en la cadena "readString"
      }
      if (c== '\n' && lineaenblanco)  // Verifica si el mensaje http enviado por el cliente ha terminado, verificando espacio en blanco
      {  
        int LED = readString.indexOf("LED=");       
        if(readString.substring(LED,LED+5)== "LED=T")
        {
          digitalWrite(PIN_LED,HIGH);
          state = "ON";
        }
        else if (readString.substring(LED,LED+5) == "LED=F")
        {
          digitalWrite(PIN_LED,LOW);
          state = "OFF";
        }  
        cliente.println("HTTP/1.1 200 OK");
        cliente.println("Content-Type: text/html");
        cliente.println();
        cliente.println("<html>");
        cliente.println("<head>");
        
        cliente.println("<title> LAMPARA ON/OFF </title>");
        cliente.println("</head>");
        cliente.println("<meta http-equiv='refresh' content='2'>");  // Refrescar la página cada 5 segundos
        cliente.println("<body width=100% height=100%>");
        cliente.println("<center>");
        cliente.println("<h1>LAMPARA ON/OFF </h1>");
        cliente.print("<br><br>");
        cliente.print("Estado de la lampara Roja:");
        cliente.print(state);
        cliente.print("<br><br><br><br>");
        cliente.println("<input type = submit value = ON style = width:200px;height:75px onClick=location.href='./?LED=T\'>"); // Linea HTML boton de ON 
        cliente.println("<input type = submit value = OFF style = width:200px;height:75px onClick=location.href='./?LED=F\'>"); // Lines HTML boton de OFF
        cliente.println("</body>");
        cliente.print("<br><br>");
        cliente.print("Valor del LDR: ");
        cliente.print(ldrValue);

        cliente.println("</html>");  // finaliza codigo HTML enviado al cliente.
        cliente.stop();
        readString="";
          }
        }
      }
    }
  }
