//Arduino PWM Speed Control：
int E1 = 5;  
int M1 = 4; 
int E2 = 6;                      
int M2 = 7;


const int buttonAnalogIn = A1;

const int left = 1;
const int right = 2;

const int none = 0;

const int ONEPHASE = 1;
const int TWOPHASE = 2;
const int HALFSTEP = 3;

const int delayVal = 25;

int STEPPER_STEP = 0;

int prevInput = 0;
 
void setup() 
{ 
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    
    Serial.begin(9200);
    Serial.println("YOLO");
} 
 
void loop() 
{
    static int prevMillis5sec = 0;
    
    int mili = millis();
    
    if (mili > prevMillis5sec)
    {
      prevMillis5sec = mili + delayVal; 
      
      controlmotor();
    }
    
    AnalogButtons();
  
}

void buttonChange(int value)
{
   switch(value)
   {
     case 1023:
     {
        if(STEPPER_STEP!=ONEPHASE)
        {
          STEPPER_STEP = ONEPHASE;
          Serial.println("Changed motor to ONE phase");
        }
        break; 
     }
     case 515:
     {
        if(STEPPER_STEP!=TWOPHASE)
        {
          STEPPER_STEP = TWOPHASE;
          Serial.println("Changed motor to TWO phase");
        }
        break; 
     }
     case 342:
     {
        if(STEPPER_STEP!=HALFSTEP)
        {
          STEPPER_STEP = HALFSTEP;
          Serial.println("Changed motor to HALFSTEP");
        }
        break; 
     }
     case 256:
     {
        if(STEPPER_STEP!=none)
        {
          STEPPER_STEP = none;
          Serial.println("Stopped motor");
        }
        break; 
     }
   } 
}

void AnalogButtons()
{
  int analogInput = analogRead(buttonAnalogIn);
  analogInput = checkRange(analogInput, 256, 10);
  analogInput = checkRange(analogInput, 342, 10);
  analogInput = checkRange(analogInput, 515, 8);
  analogInput = checkRange(analogInput, 1023, 25);
  if (analogInput != prevInput)
  {
    prevInput = analogInput;
    Serial.println(prevInput);
    
    buttonChange(prevInput);
  }  
}

void controlmotor()
{
    static int motorstep = 0;
   
   switch(STEPPER_STEP)
   {
     case ONEPHASE:
     {
       if(motorstep>4)
       {
         motorstep=0;
       }
       motorstep++;
       
       switch(motorstep)
       {
         case 1:
         {
            managecoils(none, right);
            break; 
         }
         case 2:
         {
            managecoils(right, none);
            break; 
         }
         case 3:
         {
           managecoils(none, left);
           break;
         }
         case 4:
         {
           managecoils(left, none);
           break;
         }
       }
       break;
     }
     case TWOPHASE:
     {
       if(motorstep>4)
       {
         motorstep=0;
       }
       motorstep++;
       
       switch(motorstep)
       {
         case 1:
         {
            managecoils(right, right);
            break; 
         }
         case 2:
         {
            managecoils(right, left);
            break; 
         }
         case 3:
         {
           managecoils(left, left);
           break;
         }
         case 4:
         {
           managecoils(left, right);
           break;
         }
       }
       break;
     }
     case HALFSTEP:
     {
       if(motorstep>8)
       {
         motorstep=0;
       }
       motorstep++;
       
       switch(motorstep)
       {
         case 1:
         {
            managecoils(none, right);
            break; 
         }
         case 2:
         {
            managecoils(right, right);
            break; 
         }
         case 3:
         {
           managecoils(right, none);
           break;
         }
         case 4:
         {
           managecoils(right, left);
           break;
         }
         case 5:
         {
           managecoils(none, left);
           break;
         }
         case 6:
         {
           managecoils(left, left);
           break;
         }
         case 7:
         {
           managecoils(left, none);
           break;
         }
         case 8:
         {
           managecoils(left, right);
           break;
         }
       }
       break;
     }
   } 
}

void managecoils(int A, int B)
{
  switch(A)
  {
    case 1:
    {
       digitalWrite(M1, LOW);
       digitalWrite(E1, HIGH);
       break;
    }
    case 2:
    {
       digitalWrite(M1, HIGH);
       digitalWrite(E1, HIGH);
       break;   
    }
    default:
    {
       digitalWrite(M1, LOW);
       digitalWrite(E1, LOW);
       break;    
    }
  } 
  switch(B)
  {
    case 1:
    {
       digitalWrite(M2, LOW);
       digitalWrite(E2, HIGH);
       break;  
    }
    case 2:
    {
       digitalWrite(M2, HIGH);
       digitalWrite(E2, HIGH);
       break;    
    }
    default:
    {
       digitalWrite(M2, LOW);
       digitalWrite(E2, LOW);
       break;    
    }
  } 
}

int checkRange(int input, int target, int tolerance)
{
  if (input + tolerance > target && input - tolerance < target)
  {
    return target;
  }
  return input;
}
