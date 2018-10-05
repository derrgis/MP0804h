////////////////////////////////////////////////////////////

//Conception D.Giscloux avec les contributions d'A.Suzuki
//pour l'installation multimedia de MP0804h https://mp0804hmediaart.wordpress.com/

/////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <DNSServer.h>

//--------------------------------------------------------------------------
//Audio: utilisation d'un JQ6500 avec la bibliothèque https://github.com/sleemanj/JQ6500_Serial/

//Contrairement aux exemples de sleemanj ces lignes sont fausses avec l'utilisation d'un ESP32
//JQ6500_Serial mp3(16,17);
//#include <SoftwareSerial.h>

////Donc dans le cas d'un ESP32 comme microcontroleur il faut le fork de Tux Burner:
// ici https://github.com/tuxBurner/JQ6500_Serial/tree/esp32
//Oter JQ6500_Serial.cpp et JQ6500_Serial.h du dossier et laisser JQ6500_Serial_ESP32.cpp et JQ6500_Serial_ESP32.h
//pour davantage d'explications voir aussi: https://github.com/sleemanj/JQ6500_Serial/pull/7

#include <JQ6500_Serial_ESP32.h>
JQ6500_Serial_ESP32 mp3(2);

//variable pour JQ6500
unsigned int numFiles; // Total number of files on media (autodetected in setup())
byte mediaType;        // Media type (autodetected in setup())                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         


//nécessaire avec la carte Wemos Lolin32
//A placer AVANT fastled
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define NUMLED 78//nombre de leds
#define BRIGHTNESS 64
#define DATA_PIN 4

CRGB leds[NUMLED];
int indexArray[NUMLED];

//L'allumage de chaque nouvelle led se fait avec un bouton ou via wifi
//Section Bouton
#define pouss 12
#define poussON digitalRead(pouss) == 1 //la fonction poussON écoute l'entrée 12 ssi 0

//Section Wifi
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WiFiServer server(80); // Attention, server web HTTP uniquement, ne fonctionne pas et ne redirige pas HTTPS!!!

String responseHTML = ""
                      "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
                      "<h1>Hello World!</h1><p>This is a captive portal. All requests will "
                      "be redirected here.</p></body></html>";

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Milles petits gestes", "noel2909");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  server.begin();

  //remplir le tableau avec nombres de 0 à NUMLED
  for (int i = 0; i < NUMLED; i++) indexArray[i] = i;
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUMLED);

  //----------------Initialisation module audio--------------
  mp3.begin(9600);
  mp3.reset();
  mp3.setVolume(30);
  mp3.setEqualizer(MP3_EQ_NORMAL);

// we select the built in source NOT SD card source
  mp3.setSource(MP3_SRC_BUILTIN);
  numFiles = mp3.countFiles(MP3_SRC_BUILTIN);
  mediaType = MP3_SRC_BUILTIN;
      
}

//-------------tirage aléatoire - credits @Arthur Suzuki-------------
//cette partie permettra d'allumper une led dans le désordre à chaque nvlle intervention (bouton ou via wifi)
void shuffle(int shufArray[], int arrLength, int numPass) {
  /*
    shufArray : c'est le tableau à mélanger
    arrLength : le nombre d'élément dans le tableau
    numPass   : le nombre de passes (combien de fois on coupe)
    //*/
  //Initialisation module random
  randomSeed(analogRead(A0));

  for (int i = 0; i < numPass; i++) {
    int n = random(0, arrLength);  // on prend un nb aleatoire entre 0 et NUMLED
    int temp = shufArray[n];  // on sauve la valeur d'une case prise au hasard (n) dans temp
    shufArray[n] =  shufArray[i % arrLength]; // on remplace la valeur de la case n par la valeur de la case i
    shufArray[i % arrLength] = temp; // on remet la valeur initiale de n dans i
    /*
       j'ai appliqué un modulo du nombre d'élément du tableau sur i,
       de telle sorte qu'on puisse faire plus de passes qu'il d'élément dans le tableau.
       En ce cas le mélange repart de la première case du tableau.
      //*/
  }
}
//--------------------------------------------------


void loop() {

  uint8_t hue;
  boolean nextLED = 0;
  FastLED.clear();
  //on commence par mélanger le tableau / cartes / whatever
  shuffle(indexArray, NUMLED, NUMLED);

  //On établi la valeur globale de setBrightness
  FastLED.setBrightness(BRIGHTNESS);

  //on affiche le tableau (qui contient maintenant les éléments dans le desordre)
  //+1 pour avoir un appui supplémentaire à NUMLED qui va éteindre l'ensemble
  for (int i = 0 ; i <= NUMLED ; i++) {
    hue = random(256); //Pour choisir la teinte, on peux mettre un offset genre 128 + random(64)...
    nextLED = 0;
   

    while (!nextLED && !poussON ) {
      dnsServer.processNextRequest();
      WiFiClient client = server.available();   // listen for incoming clients
      //Ensuite on loop en attendant une connection
      if (client) {
        String currentLine = "";
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            if (c == '\n') {

              // if the current line is blank, you got two newline characters in a row.
              // that's the end of the client HTTP request, so send a response:
              if (currentLine.length() == 0) {
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();

                // the content of the HTTP response follows the header:
                client.print("<a style=\"display:block;width:100%;height:100%;line-height:5;text-align:center;vertical-align:middle;background:black;color:white;\" href=\"/N\">Click here to turn the next LED on.</a><br>");

                // The HTTP response ends with another blank line:
                client.println();
                // break out of the while loop:
                break;
              } else {    // if you got a newline, then clear currentLine:
                currentLine = "";
              }
            } else if (c != '\r') {  // if you got anything else but a carriage return character,
              currentLine += c;      // add it to the end of the currentLine
            }

            // Check to see if the client request was "GET /H" or "GET /L":
            if (currentLine.endsWith("GET /N")) {
              nextLED = 1;
            }
          }
        }
        client.stop();
      }
    }
    //puis ensuite seulement on allume la led dont le numéro est noté dans la case i du tableau

    //for(int j=0 ; j < BRIGHTNESS+200 ; j++) {
    //leds[indexArray[i]] = CHSV(hue++%256,255,BRIGHTNESS);// couleur aléatoire avec fadein
    
    if (i < NUMLED) {
      leds[indexArray[i]] = CHSV(hue, 255, BRIGHTNESS); // couleur aléatoire sans fadein
      delay(10);
      FastLED.show();

//--------------random play JQ6500 audiofile ----------------

       //  ** NOTE: Checking for STOPPED doesn't work with the builtin memory 
  //      because in that case the devie appears to only return PAUSED, not STOPPED  
  byte stat = mp3.getStatus();
  if(stat != MP3_STATUS_PLAYING) 
  {
    // pick a random file, numbered 1 to numFiles (NB: random() returns up to but not including the highest number, hence why we add 1)
    // if the file is the same as the one which was just played, pick a different one
    unsigned int pick;
    do
    { 
      pick = random(1,numFiles+1);
    } while(pick == mp3.currentFileIndexNumber(mediaType));
    
      // and play it
    mp3.playFileByIndexNumber(pick);
    mp3.play();
  }
//----------------------------------------------------------

      //fondu couleur qui finit au blanc + effet flash
        for (int j = BRIGHTNESS, s = 255 ; j < 255, s >= 0 ; j++, s--){
        leds[indexArray[i]] = CHSV(hue--, s, j); //
        delay(10);
        FastLED.show();
      }
    }
    else break;
  }
  
  //on diminue l'intensité globale de toutes les LEDS via la fonction setBrightness de FastLED
  for ( int b = BRIGHTNESS; b >= 0 ; b-- ) {
    FastLED.setBrightness(b);
    FastLED.show();
    delay(20);
  }
}
