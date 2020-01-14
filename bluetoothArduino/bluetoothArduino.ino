/*
    Bluetooh Basic: LED ON OFF - Avishkar
    Coder - Mayoogh Girish
    Website - http://bit.do/Avishkar
    Download the App :
    This program lets you to control a LED on pin 13 of arduino using a bluetooth module
*/
char Incoming_value = 0;                //Variable for storing Incoming_value
const int trigPin = 2;
const int echoPin = 3;
// defines variables
long duration;
int distance;
int mod;
int index, avg [5];
void setup()
{
  // Serial1.begin(9600);
  Serial.begin(9600);//Sets the data rate in bits per second (baud) for serial data transmission
  pinMode(13, OUTPUT);        //Sets digital pin 13 as output pin
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  mod=1;
}
void loop()
{
 avg[index++] = read_distance();
 index%=5;
 distance = 0;
 for(int i=0;i<5;i++)
 distance += avg[i];
 distance/=5; 
 
  // Prints the distance on the Serial Monitor

//  Serial.println(distance);
  if (Serial.available() > 0)
  {
    Incoming_value = Serial.read();      //Read the incoming data and store it into variable Incoming_value
   // Serial.print(Incoming_value);        //Print Value of Incoming_value in Serial monitor
    if (Incoming_value == 'f')
    {
      mod = 0;

    }
    if (Incoming_value == 'e')
    {
      mod = 1;
    }
    if (mod == 1)
    {
      Serial.write(Incoming_value);
    }


  }  
  if (mod == 0)
    {
      if (distance < 30)
      {
        Serial.write('c');
      }
      else
      {
        if (distance < 80)
        {
          Serial.write(0);
        }
        else
        {
          Serial.write('a');
        }
      }
    }
    delay(10);
}

double read_distance(){
// Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  return duration * 0.034 / 2;
}
