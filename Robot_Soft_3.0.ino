
#include <WiFi.h>
#include <ESP32Servo.h>

const char* ssid = "SFR_89F0";
const char* password = "tnr46rmdb9dvh23tqdz5";
WiFiServer server(80);
// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//Servo setup
Servo R1;
int servoPin1 = 5;
Servo R2;
int servoPin2 = 16;
Servo L1;
int servoPin3 = 2;
Servo L2;
int servoPin4 = 15;
Servo Rb1;
int servoPin5 = 18;
Servo Rb2;
int servoPin6 = 19;
Servo Lb1;
int servoPin7 = 22;
Servo Lb2;
int servoPin8 = 23;

// Decode HTTP GET value
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

void setup() {
  R1.attach(servoPin1);
  R2.attach(servoPin2);
  L1.attach(servoPin3);
  L2.attach(servoPin4);
  Rb1.attach(servoPin5);
  Rb2.attach(servoPin6);
  Lb1.attach(servoPin7);
  Lb2.attach(servoPin8);
  
  R1.write(50);
  R2.write(100);
  L1.write(180 - 50);
  L2.write(180 - 100);
  Rb1.write(50);
  Rb2.write(100);
  Lb1.write(180 - 50);
  Lb2.write(180 - 100);

  
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

int walkspeed = 200;

void rest() {
  R1.write(50);
  R2.write(100);
  L1.write(180 - 50);
  L2.write(180 - 100);
  Rb1.write(50);
  Rb2.write(100);
  Lb1.write(180 - 50);
  Lb2.write(180 - 100);
}


void walk() {

  //int itera = 10;  
  //int walkspeed = 250;

  int one1 = 50; int two1 = 90;
  int one2 = 40; int two2 = 60;

  //int i = 0;
  //while(i < itera) {

    R1.write(one1); R2.write(two1);
    Lb1.write(180 - one1); Lb2.write(180 - two1);

    L1.write(180 - one2); L2.write(180 - two2);
    Rb1.write(one2); Rb2.write(two2);

    delay(walkspeed);

    R1.write(one2); R2.write(two2);
    Lb1.write(180 - one2); Lb2.write(180 - two2);

    L1.write(180 - one1); L2.write(180 - two1);
    Rb1.write(one1); Rb2.write(two1);

    delay(walkspeed);

  //  i++;
  //}

  //rest();

}



void trot() {

  //int itera = 1;  
  //int walkspeed = 200;

  int one1 = 30; int two1 = 100;
  int one2 = 50; int two2 = 70;

  //int i = 0;
  //while(i < itera) {

    R1.write(one1); R2.write(two1);
    Lb1.write(180 - one1); Lb2.write(180 - two1);

    L1.write(180 - one2); L2.write(180 - two2);
    Rb1.write(one2); Rb2.write(two2);

    delay(walkspeed);

    R1.write(one2); R2.write(two2);
    Lb1.write(180 - one2); Lb2.write(180 - two2);

    L1.write(180 - one1); L2.write(180 - two1);
    Rb1.write(one1); Rb2.write(two1);

    delay(walkspeed);

  //  i++;
  //}

  //rest();

}

void runLoop() {
  if(header.indexOf("GET /?value=")>=0) {
    pos1 = header.indexOf('=');
    pos2 = header.indexOf('&');
    valueString = header.substring(pos1+1, pos2);
    walkspeed = valueString.toInt() * 10;
    trot();        
    runLoop();
  }else{
    trot();        
    runLoop();
  }
} 

void loop(){

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            


            
            /*
            if (header.indexOf("GET /forward") >= 0) {
              walk();                
            } else if (header.indexOf("GET /foleft") >= 0) {
              walkLeft();
            } else if (header.indexOf("GET /stand") >= 0) {
              trot();
            }
            */
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Web Page
            client.println("</head><body><h1>ESP32 with Servo</h1>");
            client.println("<p>Position: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            
            client.println("<script>");
            /*
            client.println("var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}");
            */
            client.println("const slider = document.getElementById(\"servoSlider\"); const servoP = document.getElementById(\"servoPos\"); let timer; servoP.innerHTML = slider.value; slider.addEventListener(\"input\", function() { slider.value = this.value; servoP.innerHTML = this.value; }); function sendAjaxRequest(pos) { $.ajax({ url: \"/\", data: { value: pos }, type: \"GET\", timeout: 1000, headers: { Connection: \"close\" }, success: ");
            client.println("function() { console.log(`Sent servo position ${pos} to server`); }, error: function(xhr, status, error) { console.log(`Error sending servo position: ${error}`); } }); } slider.addEventListener(\"change\", function() { clearInterval(timer); timer = setInterval(function() { sendAjaxRequest(slider.value); }, 100); });");
            client.println("</script>");
           
            client.println("</body></html>");   

            //GET /?value=180& HTTP/1.1
            
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              //R1.write(valueString.toInt());
              walkspeed = valueString.toInt() * 10;
              if(walkspeed > 1000){
                rest();
              }else if(walkspeed > 150){
                walk();        
              }else{
                trot();        
              }
              
            }

            //runLoop();
                      
            // The HTTP response ends with another blank line
            client.println();


            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}