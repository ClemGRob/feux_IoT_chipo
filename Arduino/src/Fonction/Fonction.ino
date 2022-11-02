/*
   Fonction int Traf_light_color (int select_traf_light, int light_color)
   Le choix du feu tricolor
   Mettre 0 ou traf_light_1 à select_traf_light pour sélectionner le feu 1
   Mettre 1 ou traf_light_2 à select_traf_light pour sélectionner le feu 2
   Le choix de la couleur du feu
   Mettre rouge ou 1 pour sélectionner la couleur rouge
   Mettre orange ou 2 pour sélectionner la couleur orange
   Mettre vert ou 3 pour sélectionner la couleur vert
   Mettre etient ou 4 pour étiendre toutes les leds

   Fonction int Traf_light_panne (int select_traf_light, int light_panne)
   Le choix du feu tricolor
   Mettre 0 ou traf_light_1 à select_traf_light pour sélectionner le feu 1
   Mettre 1 ou traf_light_2 à select_traf_light pour sélectionner le feu 2
   Le feu est en panne, feu orange clignotant

   Fonction int sensor_distance (int select_sensor_distance)
   Le choix du feu tricolor
   Mettre 0 ou traf_light_1 à sensor_distance_1 pour sélectionner le feu 1
   Mettre 1 ou traf_light_2 à sensor_distance_2 pour sélectionner le feu 2
return pour le feu 1 :
  dist_sens_1
return pour le feu 2 :
  dist_sens_2

   Fonction int presence(int echoPin, int trigPin)
   Not used fonction (déja utilisé daans la fonction sensor_distance
   echoPin et trigPin permet l'utilisation de la fonction presence pour les 2 capteurs

  

   Fonction void init_setup()
   initialisation des GPIO Led et capteur de distance


*/

// define
//   pin Traffic light
#define vert_rgb_0 10 // vert_rgb_0
#define rouge_rgb_0 A1 // rouge_RGB_0
#define vert_rgb_1 A3 // vert_rgb_1
#define rouge_rgb_1 A4 // rouge_RGB_1

//   traf light
#define traf_light_1 0
#define traf_light_2 1

//   light color
#define rouge 1
#define orange 2
#define vert 3
#define eteint 4

//   sensor distance
#define sensor_distance_1 0
#define sensor_distance_2 1

//   pin sensor distance
#define sens_1_echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define sens_1_trigPin 5 //attach pin D3 Arduino to pin Trig of HC-SR04
#define sens_2_echoPin 6 // attach pin D4 Arduino to pin Echo of HC-SR04       // a confirmer 
#define sens_2_trigPin 7 //attach pin D5 Arduino to pin Trig of HC-SR04        // a confirmer 


// init
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

int dist_sens_1 = 0;
int dist_sens_2 = 0;

int vert_led_0 = 0;
int rouge_led_0 = 0;
int vert_led_1 = 0;
int rouge_led_1 = 0;


// Fonction traffic_light_colors
int Traf_light_color (int select_traf_light, int light_color) {
  if (select_traf_light == traf_light_1) {
    switch (light_color) {
      case 1: //rouge
        // feu rouge
        analogWrite(vert_led_0, 0);
        analogWrite(rouge_led_0, 255);
        break;

      case 2: //orange
        // feu orange
        analogWrite(vert_led_0, 30);
        analogWrite(rouge_led_0, 255);
        break;

      case 3: //vert
        // feu vert
        analogWrite(vert_led_0, 255);
        analogWrite(rouge_led_0, 0);
        break;

      case 4: //Eteint
        // feu vert
        analogWrite(vert_led_0, 0);
        analogWrite(rouge_led_0, 0);
        break;
      default:
        break;
    }
  }
  else {
    if ( select_traf_light == traf_light_2) {
      switch (light_color) {
        case 1: //rouge
          // feu rouge
          analogWrite(vert_led_1, 0);
          analogWrite(rouge_led_1, 255);
          break;

        case 2: //orange
          // feu orange
          analogWrite(vert_led_1, 215);
          analogWrite(rouge_led_1, 0);
          break;

        case 3: //vert
          // feu vert
          analogWrite(vert_led_1, 255);
          analogWrite(rouge_led_1, 0);
          break;

        case 4: //Eteint
          analogWrite(vert_led_1, 0);
          analogWrite(rouge_led_1, 0);
          break;

        default:
          break;
      }
    }
    else
    {
      Serial.print("t'es un bouffon");
    }
  }
}

// Fonction Traf_light_panne
int Traf_light_panne (int select_traf_light, int light_panne) {
  if (select_traf_light == traf_light_1) {
    Serial.print("Signal traf_light_1 en Panne");
    Traf_light_color(traf_light_1, orange);
    delay(500);
    Traf_light_color(traf_light_1, eteint);
  }
  else {
    if ( select_traf_light == traf_light_2) {
      Serial.print("Signal traf_light_2 en Panne");
      Traf_light_color(traf_light_2, orange);
      delay(500);
      Traf_light_color(traf_light_2, eteint);
    }
  }
}

// Fonction sensor_distance
int sensor_distance (int select_sensor_distance) {
  if (select_sensor_distance == sensor_distance_1) {
    dist_sens_1 = presence(sens_1_echoPin,sens_1_triPin);
  /*
  //Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(dist_sens_1);
  Serial.println(" cm");
     */
    return (dist_sens_1);
  }
  else {
    if (select_sensor_distance == sensor_distance_2) {
      dist_sens_2 = presence(sens_2_echoPin,sens_2_triPin);
   /*
  //Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(dist_sens_2);
  Serial.println(" cm");
  */
      return (dist_sens_2);
    }
  }
}

// Fonction presence
int presence(int echoPin, int trigPin) {
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return (distance);
}

void init_setup() {
pinMode(vert_led_0, OUTPUT);
pinMode(vert_led_1, OUTPUT);

pinMode(sens_1_triPin, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(sens_1_echoPin, INPUT); // Sets the echoPin as an INPUT

pinMode(sens_2_triPin, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(sens_2_echoPin, INPUT); // Sets the echoPin as an INPUT*/
}



void loop()
{
  
}
