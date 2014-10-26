

const int buttinIn = A1;
const int led = 22;
int speakerPin = 4;

int prevMillis = 0;
int prevMillis5sec = 0;
int prevInput;
int currentstate = 0;
const int analogLDR = A2;

boolean alarm_enabled = false;

const int persoondetectie = 28;

const int MAX_STATES = 5;
int registered_states = 0;

void (*statemachine_functions[MAX_STATES])();

void AddStateMachine(void (*function)());

int code[] = {256,1023,342,515};
int codeindex = 0;

 /*A    B    C  D
 
 DACB*/

void setup() {
  Serial.begin(9600);
  Serial.print("The setup is done");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  pinMode(speakerPin, OUTPUT);
  pinMode(persoondetectie, INPUT);
  
  AddStateMachine(alarm_STATE);//state 0
}

  //State machine switcher
  void LDR()
  {
    if(analogRead(analogLDR)<640)
    {
      if(currentstate==-1)
      {
      //enable alarm
       currentstate = 0;
       digitalWrite(led, HIGH);
       Serial.println("The shadows grow stronger!");
      }
    }
    else
    {
      if(currentstate==0)
      {
      //disable state machine
       currentstate = -1;
       alarm_enabled = false;
       digitalWrite(led, LOW);
       Serial.println("The day rises!");
      }
    }
  }

//states
  void alarm_STATE()
  {
    
    static boolean state = false;
    
    static boolean new_key = true;
    
    int mili = millis();
    
    if (mili > prevMillis5sec)
    {
      prevMillis5sec = mili + 500;
               
      state = !state;  
    }
    
      if(digitalRead(persoondetectie))
      {
        if(!alarm_enabled)
        {
          alarm_enabled = true;
          Serial.println("Person detected");
        }
      }
    
    if(alarm_enabled)
    {
      alarm(state);
      
      if(prevInput==0)
      {
         new_key = true;
      }
      else
      if(new_key)
      {    
        new_key = false;  
        if(code[codeindex] == prevInput)
        {
           codeindex++;
           if(codeindex<4)
           {
             Serial.println("press next key!");
           }
           else
           {
             Serial.println("disabled alarm");
             digitalWrite(led, HIGH);
             alarm_enabled = false;
             state = false; 
           }
        }
        else
        {
           Serial.println("wrong code!");
           codeindex = 0; 
        }
      }
    }
  }
//state

  void alarm(boolean state)
  {
    
    if(state)
    {
       playNote('g', 10);
    }
    
    digitalWrite(led, state);
  }

void loop() {
  
  int mili = millis(); 
  
  if (mili > prevMillis)
  {
    prevMillis = mili + 5;
    timerTick();
  }
  
  statemachine();  
  LDR();
}

void statemachine()
{
    if(currentstate==-1)return;
    
    currentstate = max(0, min(MAX_STATES-1, currentstate));

    statemachine_functions[currentstate]();
}

void AddStateMachine(void (*function)())
{
    statemachine_functions[registered_states] = function;
  
    registered_states++;
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void timerTick()
{
  int analogInput = analogRead(A1);
  analogInput = checkRange(analogInput, 256, 5);
  analogInput = checkRange(analogInput, 342, 5);
  analogInput = checkRange(analogInput, 515, 5);
  analogInput = checkRange(analogInput, 1023, 20);
  if (analogInput != prevInput)
  {
    prevInput = analogInput;
  }

}

void doSound(int value)
{
  switch (value)
  {
    case 256:
      {
        playNote('C', 10);
        break;
      }
    case 342:
      {
        playNote('b', 10);
        break;
      }
    case 515:
      {
        playNote('a', 10);
        break;
      }
    case 1023:
      {
        playNote('g', 10);
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

