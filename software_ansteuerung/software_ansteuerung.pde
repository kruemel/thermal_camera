import processing.serial.*;

Serial arduino;
String val;
int zaehler = 0;
String[] data = new String[2400];

void setup() {
  size(60, 40);
  String portName = Serial.list()[0];
  arduino = new Serial(this, portName, 9600);
}

void draw()
{
  
  background(0);
  if ( arduino.available() > 0) {               // Wenn Arduino bereit ist
    val = arduino.readStringUntil('\n');  // Lies seriellen Input bis zur naechsten Zeile
    if ( val != null && val != "0" ) {  
      data[zaehler] = str(int(val));
      print(zaehler);
      print(": ");
      println(data[zaehler]);
      zaehler++;
      
      if ( zaehler == 2399) {
        println(zaehler);
        saveStrings("temp_data.txt", data); //<>//
        zaehler = 0;
      }
    }
  }
}