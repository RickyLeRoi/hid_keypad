#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Mouse.h> //there are some mouse move functions for encoder_Mode 2 and 3
#include <Adafruit_MCP3008.h>
#include <Keyboard.h>
#include "TFT_eSPI.h" //TFT LCD library

TFT_eSPI tft; //Initializing TFT LCD library
TFT_eSprite spr = TFT_eSprite(&tft); //Initializing the buffer
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

/* MCP */
int dly = 500;
String password;               // string for rando password generator, its a global variable because i might do something fancy with it?

/* KEYPAD */
int KEY = 'X';
#define KEY_pin A0

/* JOYSTICK */
int JOY0_X = 0;
int JOY0_Y = 0;
int JOY0_SW = LOW;
#define JOY0_X_pin A2
#define JOY0_Y_pin A3
#define JOY0_SW_pin 8

/* SLIDER */
int SLIDER0 = 0;
#define SLIDER0_pin A1

/* ENCODER PINS */
int ENCODER0_SW = LOW;
volatile int ENCODER0_LAST = 0;
volatile long ENCODER0_VALUE = 0;
int ENCODER1_SW = LOW;
volatile int ENCODER1_LAST = 0;
volatile long ENCODER1_VALUE = 0;
#define ENCODER0_CLK_pin 2
#define ENCODER0_DT_pin 3
#define ENCODER0_SW_pin 6
#define ENCODER1_CLK_pin 4
#define ENCODER1_DT_pin 5
#define ENCODER1_SW_pin 6


Adafruit_MCP3008 mcp;

void setup() {
  /* DECLARES */
  pinMode(WIO_KEY_A, INPUT);
  pinMode(WIO_KEY_B, INPUT);
  pinMode(WIO_5S_UP, INPUT);
  pinMode(WIO_5S_DOWN, INPUT);
  pinMode(WIO_5S_LEFT, INPUT);
  pinMode(WIO_5S_RIGHT, INPUT);
  pinMode(ENCODER0_CLK_pin, INPUT_PULLUP);
  pinMode(ENCODER0_DT_pin, INPUT_PULLUP);
  pinMode(ENCODER1_CLK_pin, INPUT_PULLUP);
  pinMode(ENCODER1_DT_pin, INPUT_PULLUP);
  pinMode (ENCODER0_SW_pin, INPUT);
  pinMode (ENCODER1_SW_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER0_CLK_pin), updateEncoder0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER0_DT_pin), updateEncoder0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER1_CLK_pin), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER1_DT_pin), updateEncoder1, CHANGE);
  pinMode (JOY0_SW_pin, INPUT);
  
  /* BEGINS */
  Serial.begin(19200);
  Keyboard.begin();
  mcp.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  tft.begin(); //LCD initialization
  tft.setRotation(3); //Setting LCD rotation
  spr.createSprite(TFT_HEIGHT,TFT_WIDTH); //Creating the buffer
  spr.fillSprite(TFT_WHITE); //Setting Background color

  /* DISPLAY */
  display.display();
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Connecting to SSID\n'adafruit':");
  display.print("connected!");
  display.setCursor(0,0);
  display.display(); // actually display all of the above

  spr.fillRect(0,0,320,45,TFT_BLUE); //Drawing rectangle with blue fill
  spr.setTextSize(3); //Setting text size 
  spr.setTextColor(TFT_WHITE); //Setting text color
  spr.drawString("KeyPad1.0",80,10); //Drawing text string

  spr.pushSprite(0,0); //Push to LCD

  delay(500);
}

void loop() {
  JOY0_X = analogRead(JOY0_X_pin);
  JOY0_Y = analogRead(JOY0_Y_pin);
  JOY0_SW = digitalRead(JOY0_SW_pin);
  KEY = analogRead(KEY_pin);
  SLIDER0 = analogRead(SLIDER0_pin);
  // ENCODER0_SW = digitalRead(ENCODER0_SW_pin);
  // ENCODER1_SW = digitalRead(ENCODER1_SW_pin);
  char keyPressed = getKeyPressed3x4(KEY);

  Serial.print("JoyX: ");  Serial.print(JOY0_X);  Serial.print(" - JoyY: ");  Serial.println(JOY0_Y);
  Serial.print("slider: ");  Serial.print(SLIDER0);  Serial.print(" - JoySW: ");  Serial.println(JOY0_SW);
  Serial.print("Enc0: ");  Serial.print(ENCODER0_VALUE);  Serial.print(" - Enc1: ");  Serial.println(ENCODER1_VALUE);
  Serial.print("Key: ");  Serial.print(KEY);  Serial.print(" - Char: ");  Serial.println(keyPressed);
  
  delay(100);

  display.clearDisplay();
  display.display();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("JoyX: ");  display.print(JOY0_X);  display.print("-JoyY: ");  display.println(JOY0_Y);
  display.print("Pot: ");  display.print(SLIDER0);  display.print("-JoySW: ");  display.println(JOY0_SW);
  display.print("Enc0: ");  display.print(ENCODER0_VALUE);  display.print("-Enc1: ");  display.println(ENCODER1_VALUE);
  display.print("Key: ");  display.print(KEY);  display.print("-Char: ");  display.println(keyPressed);
  display.setCursor(0,0);
  display.display(); // actually display all of the above

  if (keyPressed != 'X') {
    switch (keyPressed) {
      case '1':                                             //open notepad to take notes of these great passwords
        Keyboard.press(KEY_LEFT_GUI); 
        Keyboard.press('r'); delay(150); 
        Keyboard.release(KEY_LEFT_GUI); 
        Keyboard.release('r'); 
        delay(150);                                        //give your system time to catch up with these android-speed keyboard presses
        Keyboard.println("notepad");  break;               //don't forget the break statement!
      case '2': RandoPasswordGenerator(); break;           // generate a not-so-sophisticated password 
      case '3': Keyboard.press(KEY_LEFT_CTRL);   
                Keyboard.print('z');                       //undo
                break;
      case '4': Keyboard.press(KEY_LEFT_CTRL);   
                Keyboard.print('a');                        //select all    
                break;
      case '5': Keyboard.press(KEY_LEFT_CTRL); 
                Keyboard.print('c');                         //copy
                break;                
      case '6': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('v');                         // paste
                break;
      case '7': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('x');                         // cut
                break;
      case '8': Keyboard.press(KEY_DELETE); break;
      case '9': Keyboard.println("passw0rd"); break;
      case '0': Keyboard.println("1234"); break;
      case 'A': Keyboard.println("mypassword"); break;
      case 'B': Keyboard.println("1111"); break;
    }
    delay(100); Keyboard.releaseAll(); // this releases the buttons 
  }

  yield();
}

void updateEncoder0() {
  int MSB0 = digitalRead(ENCODER0_CLK_pin); //MSB = most significant bit
  int LSB0 = digitalRead(ENCODER0_DT_pin); //LSB = least significant bit
  int encoded0 = (MSB0 << 1) | LSB0; //converting the 2 pin value to single number
  int sum0  = (ENCODER0_LAST << 2) | encoded0; //adding it to the previous encoded value
  if (sum0 == 0b1000) ENCODER0_VALUE ++;
  if (sum0 == 0b0010) ENCODER0_VALUE --;
  ENCODER0_LAST = encoded0; //store this value for next time
}
void updateEncoder1() {
  int MSB1 = digitalRead(ENCODER1_CLK_pin); //MSB = most significant bit
  int LSB1 = digitalRead(ENCODER1_DT_pin); //LSB = least significant bit
  int encoded1 = (MSB1 << 1) | LSB1; //converting the 2 pin value to single number
  int sum1  = (ENCODER1_LAST << 2) | encoded1; //adding it to the previous encoded value
  if (sum1 == 0b1000) ENCODER1_VALUE ++;
  if (sum1 == 0b0010) ENCODER1_VALUE --;
  ENCODER1_LAST = encoded1; //store this value for next time
}

/*************************************************
 * 1  2  3  A   -   234  272  325  404
 * 4  5  6  B   -   440  460  482  506
 * 7  8  9  C   -   565  598  635  677
 * *  0  #  D   -   787  853  930  1023
 *************************************************/
char getKeyPressed4x4(int val) {
  if (val > 960)  return 'D';
  if (val > 890)  return '#';
  if (val > 820)  return '0';
  if (val > 720)  return '*';
  if (val > 650)  return 'C';
  if (val > 610)  return '9';
  if (val > 580)  return '8';
  if (val > 530)  return '7';
  if (val > 500)  return 'B';
  if (val > 470)  return '6';
  if (val > 450)  return '5';
  if (val > 420)  return '4';
  if (val > 360)  return 'A';
  if (val > 295)  return '3';
  if (val > 250)  return '2';
  if (val > 200)  return '1';
  return 'X';
}

/*************************************************
 * 1  2  3  -   477  502  528
 * 4  5  6  -   560  595  632
 * 7  8  9  -   679  728  784
 * *  0  #  -   852  930  1023
 *************************************************/
 char getKeyPressed3x4(int val) {
  if (val > 960)  return '#';
  if (val > 890)  return '0';
  if (val > 820)  return '*';
  if (val > 750)  return '9';
  if (val > 700)  return '8';
  if (val > 650)  return '7';
  if (val > 610)  return '6';
  if (val > 580)  return '5';
  if (val > 545)  return '4';
  if (val > 510)  return '3';
  if (val > 485)  return '2';
  if (val > 400)  return '1';
  return 'X';
}

void RandoPasswordGenerator(){
  long random_alpha;
  long random_ALPHA;
  long random_numeric;
  long random_special;
  
  char alpha;         // lower case letters
  char ALPHA;         // uppercase letters
  char number;        // numbers!
  char specialChar;   // !@#$%^&* and so on
  char randoPassword; // array to combine these all together
   
  char alphabet[]="qwertyuiopasdfghjklzxcvbnm"; //arrary for lower case letters. alphabet[0]; returns "q"...... alphabet[25]; returns "m"  alphabet[26]; will break something by accessing the null character
  char ALPHABET[]="QWERTYUIOPASDFGHJKLZXCVBNM"; //when declaring an array of type char, one more element than your initialization is required, to hold the required null character
  char numeric[] ="1234567890";
  char special[] ="!@#$%^&*()";
  
  random_alpha = random(sizeof(alphabet)-1);    // random(max) sets the limit of the random number being pulled.
  random_ALPHA = random(sizeof(ALPHABET)-1);    // Sets the random number upper limit to the size of char array.
  random_numeric = random(sizeof(numeric)-1);   // Subtracting 1 so the random index position never pulls the 
  random_special = random(sizeof(special)-1);   // upper limit size (this would be null position).
  
  alpha = alphabet[random_alpha]; //picks a random character in the respective array. 
  ALPHA = ALPHABET[random_ALPHA];
  number = numeric[random_numeric];
  specialChar = special[random_special];
  
  String alpha_String = String(alpha); //convert all the single chars to strings...so we can concatenate them...
  String ALPHA_String = String(ALPHA);
  String number_String = String(number);
  String specialChar_String = String(specialChar);
  
  password = (alpha_String + ALPHA_String + number_String + specialChar_String); //concatenate the random alpha and numerics to format: xX$!
  // This is great and all but it will always generate a set pattern, which isnt secure with machine learning (or any basic pattern recognition)
  // you could offset this by mixing the arrays so they are non homongenous of just letters, numbers...
//  password.toCharArray(randoPassword,sizeof(randoPassword)); //convert the xX# string to char datatype
  Keyboard.print(password);
  }
