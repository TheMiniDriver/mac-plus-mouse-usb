/* ================================================================================
  Apple Macintosh Plus M0100 Mouse USB converter using Arduino UNO
    by Bradley Van Aardt (https://github.com/TheMiniDriver/mac-plus-mouse-usb)
    23 June 2020
  
   Adpated from  https://geekhack.org/index.php?topic=74340.0  by GuilleAcoustic and Johan Berglund. 
   Using code from https://anhnguyen.me/2014/01/turn-arduino-uno-r3-into-a-mouse/ by Darran Hunt, Anh Nguyen

   I adapted the GuilleAcoustic and Johan Berglund code to run on an Arduino UNO, instead of the Pro Micro. This was purely because I only had an UNO. 
   This meant that another method of USB communication needed to be found, as the UNO does not support the libary calls used in the orignal code. 
   I found a solution by Darren Hunt in a good tutorial by Anh Nguyen, and updated the code with that. 
   Note that it requires the Uno firmware to be changed while running the application. 

   The other significant change was to move to a polling method for the rotary encoders from the mouse, as the UNO only has 2 interrupt pins, where 4 are required for both axis.
   I thought this would severely affect the behaviour of the mouse, but it works quite well.   
   
  
   Connection to M0100 DB9: (note mappings can be changed in the #define section below)
   
   DB9     Arduino UNO
    1       GND
    2       VCC
    3       GND
    4       PIN4     
    5       PIN5      
    6       -       (not connected)
    7       PIN7     
    8       PIN8      
    9       PIN9      
   
   ================================================================================ */
    
#define x1Pin  4  // DB9 pin 4
#define x2Pin  5  // DB9 pin 5
#define y1Pin  9  // DB9 pin 9
#define y2Pin  8  // DB9 pin8
#define buttonPin  7  // DB9 pin 7

// This determines how often to write to the USB port. 
#define writeIntervalLoops 500 

// This determines how much to Amplify each increment, at the expense of accuracy. 
// Without it, it takes a whole desk to move across a modern hi-res display. 
#define movementMultiplier  2 


// =================================================================================
// Type definitions
// =================================================================================
typedef struct
{
  int8_t  coordinate = 0;
  uint8_t index      = 0;
} ENCODER_;

typedef struct
{
  uint8_t current = 0; 
  uint8_t old     = 0; 
} PINRECORD_; 

struct {
  uint8_t buttons;
  int8_t x;
  int8_t y;
  int8_t wheel;   /* Not yet implemented */
} mouseReport;

// =================================================================================
// Constant definition
// =================================================================================
const int8_t lookupTable[] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1,  1,  0};

// =================================================================================
// Variables
// =================================================================================
ENCODER_ xAxis;
ENCODER_ yAxis;

PINRECORD_ x1; 
PINRECORD_ x2;

PINRECORD_ y1; 
PINRECORD_ y2;

int writeDelayCounter = 0; 

// =================================================================================
// Main program
// =================================================================================

void setup();
void loop();

void setup()
{
  // initialize the pushbutton pin as an input with a pullup (mouse button down shorts to ground):
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(x1Pin, INPUT); 
  pinMode(x2Pin, INPUT);
  pinMode(y1Pin, INPUT); 
  pinMode(y2Pin, INPUT); 

  Serial.begin(9600);
  delay(200);
}


void loop()
{
    ReadXYInputs(); 
    if (CheckForInputChange(x1, x2)){
        ISR_HANDLER_X();
    }
    if (CheckForInputChange(y1, y2)){
      ISR_HANDLER_Y();
    }
    UpdateOldXYValues(); 

    // Want to avoid writing to USB too much as it takes a while and we lose too many encoder increments.
    if (writeDelayCounter > writeIntervalLoops){
      
      mouseReport.x = xAxis.coordinate * movementMultiplier;
      mouseReport.y = yAxis.coordinate * movementMultiplier;
      mouseReport.buttons = ReadButtonInput(); 
      
      Serial.write((uint8_t *)&mouseReport, 4);
      //WriteMouseDataToDebug(); 
     
      writeDelayCounter = 0; 
      xAxis.coordinate = 0;
      yAxis.coordinate = 0;
    }
    writeDelayCounter ++; 
}


// =================================================================================
// Mouse input reading
// =================================================================================

uint8_t ReadButtonInput(){
    // The mouse button is active low (ie. when pressed it is GND and will read as 0. So we need to invert it, as the USB driver is High for press. 
    uint8_t buttonState = digitalRead(buttonPin);
    return !buttonState;  
}

void ReadXYInputs()
{
   x1.current =  digitalRead(x1Pin); 
   x2.current =  digitalRead(x2Pin); 
   y1.current =  digitalRead(y1Pin); 
   y2.current =  digitalRead(y2Pin); 
}

void UpdateOldXYValues(){
  x1.old = x1.current; 
  x2.old = x2.current;
  y1.old = y1.current;
  y2.old = y2.current;
}

bool CheckForInputChange(PINRECORD_ &rec1, PINRECORD_ &rec2){
  return ((rec1.old != rec1.current) || (rec2.old != rec2.current)); 
}

// =================================================================================
// Encoder Lookups 
// This was originally on interrupt handlers, but on the UNO there are not enough
// interrupts (only 2, and we need 4). 
// =================================================================================
void ISR_HANDLER_X()
{
  // Build the LUT index from previous and new data
  uint8_t xInput = x2.current << 1; 
  xInput += x1.current;  
  
  xAxis.index       = (xAxis.index << 2) | (xInput);
  xAxis.coordinate += lookupTable[xAxis.index & 0b1111];
}

void ISR_HANDLER_Y()
{
  // Build the LUT index from previous and new data
  uint8_t yInput = y2.current << 1; 
  yInput += y1.current;
  
  yAxis.index       = (yAxis.index << 2) | (yInput);
  yAxis.coordinate += lookupTable[yAxis.index & 0b1111];
}

// =================================================================================
// Debug helpers
// =================================================================================

void WriteMouseDataToDebug(){
    Serial.print("X: "); 
    Serial.print(mouseReport.x); 
    Serial.print(" Y: "); 
    Serial.print(mouseReport.y);
    Serial.print(" Button: "); 
    Serial.println(mouseReport.buttons); 
}
