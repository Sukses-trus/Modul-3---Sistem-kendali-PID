//deklarasi sensor photodioda
int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int sensor5 = A4;
int sensor6 = A5;
int baca_sensor[6];
int datasensor[6];
//deklarasi pin output
int pinEnable = 4;
int pinEnable2 = 2;
//deklarasi motor kiri
int motor_in1 = 5;
int motor_in2 = 6;
//deklarasi motor kanan
int motor_on1 = 3;
int motor_on2 = 11;
//deklarasi dua button
int but_1 = 9;
int but_2 = 10;
//deklarasi untuk PID
int last_error = 0;
int error = 0;
int kp = 5;
int kd = 0;
int ki = 0;
int rate_d = 0;
int rate_i = 0;
int robotspeed = 150;
int movecontrol = 0;
int robotspeedL = 0;
int robotspeedR = 0;
int maxpwm = 255;
char sensorBit;


// setup pin
void setup()
{
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(sensor6, INPUT);
  pinMode(but_1, INPUT_PULLUP);
  pinMode(but_2, INPUT_PULLUP);  

  pinMode(pinEnable, OUTPUT);
  pinMode(pinEnable2, OUTPUT);
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(motor_on1, OUTPUT);
  pinMode(motor_on2, OUTPUT);
  Serial.begin(9600);
}


void readsensor(){
baca_sensor[0] = analogRead(sensor1);
baca_sensor[1] = analogRead(sensor2);
baca_sensor[2] = analogRead(sensor3);
baca_sensor[3] = analogRead(sensor4);
baca_sensor[4] = analogRead(sensor5);
baca_sensor[5] = analogRead(sensor6);

 
  for(int i = 0; i<=5; i++){
    if(baca_sensor[i]<=33){
      datasensor[i] = 1;
    }
    else{
      datasensor[i] = 0;
    }
  }
  for(int i = 0; i<=5; i++){
    sensorBit += datasensor[i] * (1 << i);
}
}

void pv(){
  switch (sensorBit){
    case 0b100000: error = -3; break;
    case 0b110000: error = -2; break;
    case 0b010000: error = -1; break;
    //  ||
    case 0b001100: error = 0; break;
    case 0b001000: error = 0; break;
    case 0b000100: error = 0; break;
    //  ||
    case 0b000010: error = 1; break;
    case 0b000011: error = 2; break;
    case 0b000001: error = 3; break;
    
    default: error; break;
  }
}


void loop(){
  digitalWrite(pinEnable, HIGH);
  digitalWrite(pinEnable2, HIGH);
  follow_line();
  butkp();
}

void butkp(){
  //ketika button 1 ditekan maka akan menambah kp
  if(digitalRead(but_1)==HIGH){
    kp++;
  }
  //ketika button 2 ditekan maka akan mengurangi kp
  if(digitalRead(but_2)==HIGH){
    kp--;
  }
}

void follow_line(){
  readsensor();
  pv();
  rate_d = error - last_error;
  rate_i = error + last_error;
  last_error = error;

  movecontrol = (kp * error) + (ki * rate_i) + (kd * rate_d);

  robotspeedR = robotspeed - movecontrol;
  robotspeedL = robotspeed + movecontrol;
  setMotor(robotspeedR, robotspeedL);
  Serial.println(robotspeedR);
}

void setMotor(int pwmR, int pwmL){
    
  if(pwmL > maxpwm){
    pwmL = maxpwm;
  }
  else if(pwmL < -maxpwm){
    pwmL = -maxpwm;
  }
  if (pwmL < 0){
    pwmL *= -1;
    analogWrite(motor_in1, pwmL);
    analogWrite(motor_in2, 0);
  }
  else{
    analogWrite(motor_in1, 0);
    analogWrite(motor_in2, pwmL);
  }
  if(pwmR > maxpwm){
    pwmR = maxpwm;
  }
  else if(pwmR < -maxpwm){
    pwmR = -maxpwm;
  }
  if (pwmR < 0){
    pwmR *= -1;
    analogWrite(motor_on1, pwmR);
    analogWrite(motor_on2, 0);
  }
  else{
    analogWrite(motor_on1, 0);
    analogWrite(motor_on2, pwmR);
  }
}
