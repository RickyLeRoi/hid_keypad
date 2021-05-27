from rotary0 import Rotary
from machine import ADC, DAC, Pin, LCD
import time, math

# KEYPAD */
KEY = 'X'
KEY_pin = 13

# SLIDER
SLIDER0 = 0
SLIDER0_pin = 15

# JOYSTICK
JOY0_X = 0
JOY0_Y = 0
JOY0_SW = 0
JOY0_X_pin = 16
JOY0_Y_pin = 18
JOY0_SW_pin = 8

# ENCODER PINS
ENCODER0_SW = 0
ENCODER0_LAST = 0
ENCODER0_VALUE = 0
ENCODER1_SW = 0
ENCODER1_LAST = 0
ENCODER1_VALUE = 0
ENCODER0_CLK_pin = 3
ENCODER0_DT_pin = 5
ENCODER0_SW_pin = 7
ENCODER1_CLK_pin = 8
ENCODER1_DT_pin = 10
ENCODER1_SW_pin = 12

lcd = LCD()                            # Initialize LCD and turn the backlight
rotary0 = Rotary(ENCODER0_DT_pin,ENCODER0_CLK_pin,ENCODER0_SW_pin)
rotary1 = Rotary(ENCODER1_DT_pin,ENCODER1_CLK_pin,ENCODER1_SW_pin)

lcd.fillScreen(lcd.color.BLACK)        # Fill the LCD screen with color black
lcd.setRotation(3)
lcd.setTextSize(2)                     # Setting font size to 2
lcd.setTextColor(lcd.color.GREEN)      # Setting test color to Green
lcd.drawString("Hello World!", 0, 0)   # Printing Hello World at (0, 0)

KEY_ADC = ADC(Pin(KEY_pin))
SLIDER_ADC = ADC(Pin(SLIDER0_pin))
JOY0_X_ADC = ADC(Pin(JOY0_X_pin))
JOY0_Y_ADC = ADC(Pin(JOY0_Y_pin))
ENCODER0_CLK_ADC = Pin(ENCODER0_CLK_pin, Pin.IN, Pin.PULL_UP)
ENCODER0_DT_ADC = Pin(ENCODER0_DT_pin, Pin.IN, Pin.PULL_UP)
ENCODER1_CLK_ADC = Pin(ENCODER1_CLK_pin, Pin.IN, Pin.PULL_UP)
ENCODER1_DT_ADC = Pin(ENCODER1_DT_pin, Pin.IN, Pin.PULL_UP)

def stringifyVal(val):
    retVal = ""
    if val < -999:
        retVal = str(val)
    elif val < -99:
        retVal = "-0"+str(abs(val))
    elif val < -9:
        retVal = "-00"+str(abs(val))
    elif val < 0:
        retVal = "-000"+str(abs(val))
    elif val < 10:
        retVal = "000"+str(val)
    elif val < 100:
        retVal = "00"+str(val)
    elif val < 1000:
        retVal = "0"+str(val)
    else:
        retVal = str(val)
    return retVal

'''
    1  2  3  -   489  514  541
    4  5  6  -   570  604  641
    7  8  9  -   683  730  790
    *  0  #  -   852  930  1023
'''
def getKeyPressed3x4(val):
    char = 'X'
    if val > 960:
        char = '#'
    elif val > 890:
        char = '0'
    elif val > 820:
        char = '*'
    elif val > 750:
        char = '9'
    elif val > 700:
        char = '8'
    elif val > 650:
        char = '7'
    elif val > 610:
        char = '6'
    elif val > 585:
        char = '5'
    elif val > 555:
        char = '4'
    elif val > 520:
        char = '3'
    elif val > 500:
        char = '2'
    elif val > 450:
        char = '1'
    return char

def readSlider(lcd, adc):
    val = adc.read()              # reading ADC value, 0 ~ 1023
    lcd.drawString('Slider: ', 0, 20)
    lcd.drawString(stringifyVal(val), 90, 20)

def readKey(lcd, adc):
    val = adc.read()
    lcd.drawString('KEY: ', 0, 40)
    lcd.drawString(stringifyVal(val), 55, 40)
    lcd.drawString('Key: ', 120, 40)
    lcd.drawString(getKeyPressed3x4(val), 175, 40)

def rotary0_changed(change):
    global ENCODER0_VALUE
    if change == Rotary.ROT_CW:
        ENCODER0_VALUE += 1
        # print(val)
    elif change == Rotary.ROT_CCW:
        ENCODER0_VALUE -= 1
        # print(val)
    elif change == Rotary.SW_PRESS:
        print('PRESS')
    elif change == Rotary.SW_RELEASE:
        print('RELEASE')
def rotary1_changed(change):
    global ENCODER1_VALUE
    if change == Rotary.ROT_CW:
        ENCODER1_VALUE += 1
        # print(val)
    elif change == Rotary.ROT_CCW:
        ENCODER1_VALUE -= 1
        # print(val)
    elif change == Rotary.SW_PRESS:
        print('PRESS')
    elif change == Rotary.SW_RELEASE:
        print('RELEASE')

def readJoy(lcd, Xadc, Yadc):
    valX = Xadc.read_u16()
    valY = Yadc.read_u16()
    x = (5 + (valX/65535) * (128-10))
    y = (5 + (valY/65535) * (32-10))
    lcd.drawString('JoyX: ', 0, 40)
    lcd.drawString(stringifyVal(x), 55, 40)
    lcd.drawString('JoyY: ', 120, 40)
    lcd.drawString(getKeyPressed3x4(y), 175, 40)

def main():
    readSlider(lcd, SLIDER_ADC)
    readKey(lcd, KEY_ADC)
    rotary0.add_handler(rotary0_changed)
    rotary1.add_handler(rotary1_changed)
    readJoy(lcd, JOY0_X_ADC, JOY0_Y_ADC)
    # JOY0_X = analogRead(JOY0_X_pin);
    # JOY0_Y = analogRead(JOY0_Y_pin);
    # KEY = analogRead(KEY_pin);
    # SLIDER0 = analogRead(SLIDER0_pin);
    # JOY0_SW = digitalRead(JOY0_SW_pin);
    # // ENCODER0_SW = digitalRead(ENCODER0_SW_pin);
    # // ENCODER1_SW = digitalRead(ENCODER1_SW_pin);
    # char keyPressed = getKeyPressed3x4(KEY);

    # Serial.print("JoyX: ");  Serial.print(JOY0_X);  Serial.print(" - JoyY: ");  Serial.println(JOY0_Y);
    # Serial.print("slider: ");  Serial.print(SLIDER0);  Serial.print(" - JoySW: ");  Serial.println(JOY0_SW);
    # Serial.print("Enc0: ");  Serial.print(ENCODER0_VALUE);  Serial.print(" - Enc1: ");  Serial.println(ENCODER1_VALUE);
    # Serial.print("Key: ");  Serial.print(KEY);  Serial.print(" - Char: ");  Serial.println(keyPressed);

    # delay(100);

    # display.clearDisplay();
    # display.display();

    # display.setTextSize(1);
    # display.setTextColor(SSD1306_WHITE);
    # display.setCursor(0,0);
    # display.print("JoyX: ");  display.print(JOY0_X);  display.print("-JoyY: ");  display.println(JOY0_Y);
    # display.print("Pot: ");  display.print(SLIDER0);  display.print("-JoySW: ");  display.println(JOY0_SW);
    # display.print("Enc0: ");  display.print(ENCODER0_VALUE);  display.print("-Enc1: ");  display.println(ENCODER1_VALUE);
    # display.print("Key: ");  display.print(KEY);  display.print("-Char: ");  display.println(keyPressed);
    # display.setCursor(0,0);
    # display.display(); // actually display all of the above

    # if (keyPressed != 'X') {
    # switch (keyPressed) {
    #     case '1':                                             //open notepad to take notes of these great passwords
    #     Keyboard.press(KEY_LEFT_GUI); 
    #     Keyboard.press('r'); delay(150); 
    #     Keyboard.release(KEY_LEFT_GUI); 
    #     Keyboard.release('r'); 
    #     delay(150);                                        //give your system time to catch up with these android-speed keyboard presses
    #     Keyboard.println("notepad");  break;               //don't forget the break statement!
    #     case '2': RandoPasswordGenerator(); break;           // generate a not-so-sophisticated password 
    #     case '3': Keyboard.press(KEY_LEFT_CTRL);   
    #             Keyboard.print('z');                       //undo
    #             break;
    #     case '4': Keyboard.press(KEY_LEFT_CTRL);   
    #             Keyboard.print('a');                        //select all    
    #             break;
    #     case '5': Keyboard.press(KEY_LEFT_CTRL); 
    #             Keyboard.print('c');                         //copy
    #             break;                
    #     case '6': Keyboard.press(KEY_LEFT_CTRL);  
    #             Keyboard.print('v');                         // paste
    #             break;
    #     case '7': Keyboard.press(KEY_LEFT_CTRL);  
    #             Keyboard.print('x');                         // cut
    #             break;
    #     case '8': Keyboard.press(KEY_DELETE); break;
    #     case '9': Keyboard.println("passw0rd"); break;
    #     case '0': Keyboard.println("1234"); break;
    #     case 'A': Keyboard.println("mypassword"); break;
    #     case 'B': Keyboard.println("1111"); break;
    # }
    # delay(100); Keyboard.releaseAll(); // this releases the buttons 
    # }
    time.sleep(0.1)

if __name__ == "__main__":
    while True:
        main()