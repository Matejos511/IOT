#include <SPI.h>
#include <Ethernet.h>

// MAC naslov in IP naslov za vaš Ethernet ščit
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x19 };
IPAddress ip(10, 8, 128, 41);

EthernetServer server(80);

const int ledPin = 2; // Definicija pina za LED
const int hcPin = 3; // Definicija pina za 74HC čip

// Funkcija za izračun temperature
float temperatura(float x) {
  float result = (((x / 1023.0) * 5000.0) - 400.0) / 19.53;
  return result;
}

void setup() {
  pinMode(ledPin, OUTPUT); // Nastavitev pina za LED kot izhod
  pinMode(hcPin, OUTPUT); // Nastavitev pina za 74HC kot izhod
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  Serial.println("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New client");
    boolean currentLineIsBlank = true;
    String request = ""; // Spremenljivka za shranjevanje zahteve
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c; // Zajemanje zahteve
        if (c == '\n' && currentLineIsBlank) {
          Serial.println(request);
          // Preverjanje zahteve za prižiganje in ugašanje LED
          if (request.indexOf("GET /LED=ON") != -1) {
            digitalWrite(ledPin, HIGH);
          }
          if (request.indexOf("GET /LED=OFF") != -1) {
            digitalWrite(ledPin, LOW);
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><style>body { background-color: blue; }</style></head>"); // Dodano: Modro ozadje
          client.println("<body>");
          client.println("<h1>Nadzor LED</h1>"); // Spremenjeno
          client.println("<div style=\"border:2px solid black; padding:10px; display:inline-block;\">"); // Dodano: Okvirček za gumb
          client.println("<form action=\"/LED=ON\" method=\"get\"><button type=\"submit\">Prižgi LED</button></form>"); // Spremenjeno
          client.println("</div>");
          client.println("<div style=\"border:2px solid black; padding:10px; display:inline-block; margin-left:10px;\">"); // Dodano: Okvirček za gumb
          client.println("<form action=\"/LED=OFF\" method=\"get\"><button type=\"submit\">Ugasni LED</button></form>"); // Spremenjeno
          client.println("</div>");
          // Izračun in prikaz temperature
          float k = temperatura(analogRead(A1));
          client.print("<p style=\"color:green;\">Temperatura je: ");
          client.print(k);
          client.println("</p>");
          client.print("<p style=\"border:2px solid red;\">Svetlost: ");
          client.print(analogRead(A0));
          client.println("</p>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }

  // Izmenično prižiganje in ugašanje LED lučk na 74HC čipu
  digitalWrite(hcPin, HIGH);
  delay(500); // Počakaj 500 ms
  digitalWrite(hcPin, LOW);
  delay(500); // Počakaj 500 ms
}
