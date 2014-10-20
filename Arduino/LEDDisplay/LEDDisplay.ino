#define COL 14 // x dimension
#define ROW 12 // y dimension

char inputX;

int row[ROW] = {2,3,4,5,6,7,8,9,10,11,12,13};               // Row's Pins on Arduino +
int col[COL] = {31,32,33,34,35,36,37,38,39,40,41,42,43,44};            // Col's Pins on Arduino - 14=A0

void setup() {
    Serial.begin(115200);
  // sets the pins as output
  for (int i = 0; i < COL; i++) {
    pinMode(col[i], OUTPUT);
  }
  for (int i = 0; i < ROW; i++) {
    pinMode(row[i], OUTPUT);
  }
  allOff();
}

void loop() {

  //receive light data from controller
  if ( Serial.available () > 0 ) 
  {
    static char input[3]; //numLights * 3
    static uint8_t i = 1;
    char c = Serial.read ();
    input[0] = 'a';
    
    if (i < 3) {
      input[i++] = c;
    } else {
      
      static char *a;
      static char *ptr;
      a = strtok_r(input, "", &ptr);
     
    
      digitalWrite(row[a[1] - 97], HIGH);
      digitalWrite(col[a[2] - 97], LOW);
      delay(66);
      allOff();
      i=1;
    
    }
  }

}

void ledPositionOn(int r,int c){
  digitalWrite(col[0], HIGH);
}


void allOn(){
  for (int i=0;i<ROW;i++){
    digitalWrite(row[i], HIGH);
    delay(500);
    digitalWrite(row[i], LOW);
  }
}


void allOff()
{
  for (int i=0;i<ROW;i++)
  {
    digitalWrite(row[i], LOW);
  }
  for (int i=0;i<COL;i++)
  {
    digitalWrite(col[i], HIGH);
  }
      
}
