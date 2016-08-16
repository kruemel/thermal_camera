#include <i2cmaster.h>
#include <Servo.h>
#define OFFSET_HOR 0
#define OFFSET_VER 50

// Festlegen der Servo Pins
int servo_horizontal = 9;
int servo_vertikal = 8;

// Weitere Variablen für PWM und Drehwinkel (Position)
int pwm;
int pos_horizontal;
int pos_vertikal;


/* Funktion um das Servo zu bewegen. Hier wird die Position
berechnet und der Wert der neuen Position an das Servo
übergeben. */

void servoMove(int servo, int pos){
  //Winkel in Mikrosekunden umrechnen
  pwm = (pos * 5) + 800;
  // Servo Pin auf HIGH zum aktivieren des Servos
  digitalWrite(servo, HIGH);
  // Kurze Zeit warten
  delayMicroseconds(pwm);
  // Servo Pin auf LOW zum deaktivieren des servos
  digitalWrite(servo, LOW);
  // 20 ms warten
  delay(20);
}

float readSensor() {
    int dev = 0x5A<<1;
    int data_low = 0;
    int data_high = 0;
    int pec = 0;
    
    i2c_start_wait(dev+I2C_WRITE);
    i2c_write(0x07);
    
    // read
    i2c_rep_start(dev+I2C_READ);
    data_low = i2c_readAck(); //Read 1 byte and then send ack
    data_high = i2c_readAck(); //Read 1 byte and then send ack
    pec = i2c_readNak();
    i2c_stop();
    
    //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
    double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
    double tempData = 0x0000; // zero out the data
    int frac; // data past the decimal point
    
    // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
    tempData = (double)(((data_high & 0x007F) << 8) + data_low);
    tempData = (tempData * tempFactor)-0.01;
    
    float celcius = tempData - 273.15;

    return celcius;
}

void setup(){
  pinMode(servo_horizontal, OUTPUT); //setze horizontalen Servo
  pinMode(servo_vertikal, OUTPUT);   //setze vertikalen Servo
	Serial.begin(9600);
	Serial.println("Setup...");
	
	i2c_init(); //Initialise the i2c bus
	PORTC = (1 << PORTC4) | (1 << PORTC5);//enable pullups

  servoMove(servo_vertikal, OFFSET_VER);
  servoMove(servo_horizontal, OFFSET_HOR);
  delay(5000);
}

void loop(){
  float temp;
  int zaehler = 0; //Zaehler; nur fuers Debugging
  int pos_vertikal = 0;
  Serial.println("start");

  while(pos_vertikal<40) {
      for(pos_horizontal=0; pos_horizontal<60; pos_horizontal++) {     
          servoMove(servo_horizontal, pos_horizontal+OFFSET_HOR); 
          temp = readSensor(); //Temperatur aus Sensor lesen
          Serial.println(temp);
          zaehler++;
      }
      pos_vertikal++;
      servoMove(servo_vertikal, pos_vertikal+OFFSET_VER);
      //Serial.println("Naechste Zeile!");
      zaehler = 0; // Zaehler zuruecksetzen
        
      for (pos_horizontal=59; pos_horizontal>=0; pos_horizontal--) {
          servoMove(servo_horizontal, pos_horizontal+OFFSET_HOR); 
          temp = readSensor();
          /*Serial.println("Wert Nr: ");
          Serial.println(zaehler);
          Serial.println(" Temperatur: "); */
          Serial.println(temp);
          zaehler++;
      }
      pos_vertikal++;
      servoMove(servo_vertikal, pos_vertikal+OFFSET_VER);
  }
  Serial.println("stop");
}
