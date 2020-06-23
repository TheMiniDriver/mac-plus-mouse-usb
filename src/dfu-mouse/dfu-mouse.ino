/* Arduino USB Mouse HID demo */

/* Author: Darran Hunt
   Release into the public domain.
*/

struct {
  uint8_t buttons;
  int8_t x;
  int8_t y;
  int8_t wheel;   /* Not yet implemented */
} mouseReport;

uint8_t nullReport[4] = { 0, 0, 0, 0 };
const int buttonPin = 6;     // the number of the mouse button pin
const int y1Pin = 2; 
const int y2Pin = 3; 
const int x1Pin = 4; 
const int x2Pin = 5; 


// =================================================================================
// Type definition
// =================================================================================
typedef struct
{
  int8_t  coordinate = 0;
  uint8_t index      = 0;
} ENCODER_;

// =================================================================================
// Constant definition
// =================================================================================
const int8_t lookupTable[] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1,  1,  0};

// =================================================================================
// Volatile variables
// =================================================================================
volatile ENCODER_ xAxis;
volatile ENCODER_ yAxis;

uint8_t x1Old;
uint8_t x2Old; 
uint8_t y1Old; 
uint8_t y2Old; 

uint8_t x1Current;
uint8_t x2Current; 
uint8_t y1Current; 
uint8_t y2Current;

int writeDelayCounter = 0; 

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
    // read the state of the pushbutton value:
    Read_XYInputs(); 
    
    if (Check_for_Input_Change_X()){
        ISR_HANDLER_X();
    }

    if (Check_for_Input_Change_Y()){
      ISR_HANDLER_Y();
    }

    UpdateOldXYValues(); 
    
    if (writeDelayCounter > 1000){
      // Serial.print("X: "); 
      // Serial.print(mouseReport.x); 
      // Serial.print(" Y: "); 
      // Serial.println(mouseReport.y);
      mouseReport.x = xAxis.coordinate;
      mouseReport.y = yAxis.coordinate;
      mouseReport.buttons = ReadButtonInput(); 
      Serial.write((uint8_t *)&mouseReport, 4);
      writeDelayCounter = 0; 
      xAxis.coordinate = 0;
      yAxis.coordinate = 0;
    }
    writeDelayCounter ++; 
}


void ReadButtonInput(){
    // The mouse button is active low (ie. when pressed it is GND and will read as 0. So we need to invert it, as the USB driver is High for press. 
    uint8_t buttonState = digitalRead(buttonPin);
    return !buttonState;  
}

void Read_XYInputs()
{
   x1Current =  digitalRead(x1Pin); 
   x2Current =  digitalRead(x2Pin); 
   y1Current =  digitalRead(y1Pin); 
   y2Current =  digitalRead(y2Pin); 
}

void UpdateOldXYValues(){
  x1Old = x1Current; 
  x2Old = x2Current;
  y1Old = y1Current;
  y2Old = y2Current;
}

bool Check_for_Input_Change_X(){
  return ((x1Old != x1Current) || (x2Old != x2Current)); 
}

bool Check_for_Input_Change_Y(){
  return ((y1Old != y1Current) || (y2Old != y2Current)); 
}

// =================================================================================
// Interrupt handlers
// =================================================================================
void ISR_HANDLER_X()
{
  // Build the LUT index from previous and new data
  uint8_t xInput = x2Current << 1; 
  xInput += x1Current;  
  
  xAxis.index       = (xAxis.index << 2) | (xInput);
  xAxis.coordinate += lookupTable[xAxis.index & 0b1111];
}

void ISR_HANDLER_Y()
{
  // Build the LUT index from previous and new data
  uint8_t yInput = y2Current << 1; 
  yInput += y1Current;
  
  yAxis.index       = (yAxis.index << 2) | (yInput);
  yAxis.coordinate += lookupTable[yAxis.index & 0b1111];
}
