// Code to bit bang the protocol for BriteStar tri-color LED christmas lights
// Based off work by James Brown (https://github.com/jbrown123/WalgreenLights)
// Modifed by Nathan Lapierre (https://github.com/nlap/lightdisplay)

class LightDisplay
{
  private:
    int  pin;
    int  numLights;

    volatile uint8_t *pinPort;
    int pinMask;
    
    //sends a single packet for tri-color LED value
    void SendPacket(unsigned char r, unsigned char g, unsigned char b)
    {
      register volatile uint8_t *port = pinPort;
      register int mask = pinMask;
      
      //////////////////////
      // INTERRUPTS DISABLED
      //////////////////////
      uint8_t oldSREG = SREG;  // get interrupt register state
      cli();    // disable interrupts

      // 3 pulses of H 6.25us + L 6.67us (start bits)
      for (int i = 0; i < 3; i++)
      {
        *port |= mask;  // HIGH
        delayMicroseconds(6);
        *port &= ~mask;  // LOW
        delayMicroseconds(6);
      }
      
      //todo: refactor to nested loops
      //green intensity
      for (int i = 0; i < 4; i++)
      {
        *port |= mask;  // HIGH
        delayMicroseconds((g & 1) ? 3 : 1);
        *port &= ~mask;  // LOW
        delayMicroseconds(3);
        g >>= 1;
      }
      
      //blue intensity
      for (int i = 0; i < 4; i++)
      {
        *port |= mask;  // HIGH
        delayMicroseconds((b & 1) ? 3 : 1);
        *port &= ~mask;  // LOW
        delayMicroseconds(3);
        b >>= 1;
      }
      
      //red intensity
      for (int i = 0; i < 4; i++)
      {
        *port |= mask;  // HIGH
        delayMicroseconds((r & 1) ? 3 : 1);
        *port &= ~mask;  // LOW
        delayMicroseconds(3);
        r >>= 1;
      }

      SREG = oldSREG;
      /////////////////////
      // INTERRUPTS ENABLED
      /////////////////////

      //interpacket gap
      delayMicroseconds(100);
    }


    void InitString(void)
    {
      //start-up, become the master 'light'
      digitalWrite(pin, HIGH);
      delay(150);
      digitalWrite(pin, LOW);
      delay(225);
      
      for (int i = 0; i < numLights; i++)
      {
        SendPacket(0, 0, 0);
      }
    }

  public:
    LightDisplay(int _pin, int _numLights)
    {
      pin = _pin;
      numLights = _numLights;

      pinPort = (pin < 8) ? &PORTD : &PORTB;
      pinMask = (pin < 8) ? 1 << pin : 1 << (pin - 8);
      
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
      
      InitString();
    }
    
    ~LightDisplay(void)
    {
      digitalWrite(pin, LOW);
      pinMode(pin, INPUT);
    }
    
    //loop through array of RGB values for each bulb in string
    void SendValues(unsigned char *rPtr, unsigned char *gPtr, unsigned char *bPtr)
    {
      for (int i = 0; i < numLights; i++)
      {
        SendPacket(*rPtr++, *gPtr++, *bPtr++);
      }
    }
    
    //set all lights to same color
    void AllTo(unsigned char r, unsigned char g, unsigned char b)
    {
      for (int i = 0; i < numLights; i++)
      {
        SendPacket(r, g , b);
      }
    }
    
    //turn all lights off
    void AllOff(void)
    {
      AllTo(0, 0, 0);
    }
};

class LightDisplay *lights;

void setup()
{
  //output packets for 15 lights to pin 13
  lights = new LightDisplay(13, 15);
}

void loop()
{

  //red, green, blue sequence
  lights->AllTo(15, 0, 0);delay(1500);
  lights->AllTo(0, 15, 0);delay(1500);
  lights->AllTo(0, 0, 15);delay(1500);
  
}


