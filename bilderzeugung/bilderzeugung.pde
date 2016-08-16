void setup() {
  size(600,400); // Breite: 600, Hoehe: 400. Ein Pixel ist 10x10 -> 60x10 Breite, 40x10 Hoehe
  noStroke();    // Keine schwarze Rahmen und die Pixel
  noLoop();      // Keine Endlosschleife, erzeugt nur 1x das Bild
}

void draw() {
  String[] farben = create_colormap();                  // Hol die Farbwerttabelle
  String[] temp_werte = loadStrings("temp_data.txt");       // Lies Datei mit Werten ein
  int zaehler = 0;
  int zeile = 40;
  int spalte = 60;
  
  for(int i = (zeile-1); i>=0; i--) {
    for(int j = (spalte-1); j>=0; j--) {
      // mappe temp_wert mit Farbtabelle und fuelle das Pixel mit dieser Farbe:
      fill(unhex(farben[int(temp_werte[zaehler])]));
      zaehler++;
      rect(j*10,i*10,10,10);        // Pixel mit Groesse 10x10
    }/*
    i--;
    for(int j = 0; j < spalte; j++) {
      fill(unhex(farben[int(temp_werte[zaehler])]));
      zaehler++;
      rect(j*10,i*10,10,10);        // Pixel mit Groesse 10x10
    }*/
  }
}

String[] create_colormap() {

  String[] data = new String[100];
  int r = 0;        // Rot und Gruen starten bei 00
  int g = 0;
  int b = 50;       // Blau startet bei FF
  int zaehler = 0;
  
  // Farbverlauf von blau (0 °C) zu rot (50 °C)
  colorMode(RGB,50,50,50);  // 50er Schritte. Ist einfacher als 255er Schritte umzurechnen
  for(int i = 0; i<50; i++) {
    b--;                                // Blau-Wert erhoehen
    r++;                                // Rot-Wert verringern
    data[zaehler] = hex(color(r,g,b));  // Speichert den Hexwert der Farbe im Array
    zaehler++;
  }
  
  //Farbverlauf von rot (50 °C) zu gelb (75 °C)
  colorMode(RGB,25,25,25);              // 25er Farbschritte
  for(int i = 0; i<25; i++) {
    g++;                                // Gruen-Wert erhoehen
    data[zaehler] = hex(color(r,g,b));
    zaehler++;
  }
  
  //Farbverlauf von gelb (75 °C) zu weiss (99 °C);
  for(int i = 0; i<25; i++) {          
    b++;                                // Blau-Wert erhoehen
    data[zaehler] = hex(color(r,g,b));
    zaehler++;
  }
  return data;
}