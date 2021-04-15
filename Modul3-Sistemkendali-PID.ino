    //motor directory
    #define CW  0
    #define CCW 1

    //motor control pin
    #define motorDirPin 7
    #define motorPWMPin 9
    #define enablePin 8

    //encoder pin
    #define encoderPinA 2
    #define encoderPinB 4

    //encoder var
    int encoderPos = 0;

    //P control
    float Kp          = 1;
    int   targetPos	  = 100;
    int   error;
    int   control;
    int   velocity;

    //external interrupt encoder
    void doEncoderA()
    {
      digitalRead(encoderPinB)?encoderPos--:encoderPos++;
    }

    void setup()
    {
      //setup interrupt
        pinMode(encoderPinA, INPUT_PULLUP);
        pinMode(encoderPinB, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA,RISING);

        //setup motor driver
        pinMode(motorDirPin, OUTPUT);
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, HIGH);

        Serial.begin(9600);
    }

    void loop()
    {
        error   = targetPos - encoderPos;
        control = Kp*error;

        velocity = min(max(control, -255), 255);
        if(velocity >= 0)
        {  
            digitalWrite(motorDirPin, CW);
            analogWrite(motorPWMPin, velocity); 
        }
        else
        {
            digitalWrite(motorDirPin, CCW);
            analogWrite(motorPWMPin, 255+velocity);
        }
        Serial.println(encoderPos);            
    }