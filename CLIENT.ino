// ---------------- Water Level Sensor 1 Connection ----------------
#define wl_sens_pwer_1 12
#define wl_sens_sig_1 13

// ---------------- Water Level Sensor 2 Connection ----------------
#define wl_sens_pwer_2 14
#define wl_sens_sig_2 27

// ---------------- Water Level Sensor 3 Connection ----------------
#define wl_sens_pwer_1 12
#define wl_sens_sig_1 13

// ---------------- Water Level Sensor 4 Connection ----------------
#define wl_sens_pwer_1 12
#define wl_sens_sig_1 13

int value = 0;

bool islevel1 = false;
bool islevel2 = false;
bool islevel3 = false;
bool islevel4 = false;

int level1 = 0;
int level2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(wl_sens_pwer_1,OUTPUT);
  digitalWrite(wl_sens_pwer_1,LOW);

  pinMode(wl_sens_pwer_2,OUTPUT);
  digitalWrite(wl_sens_pwer_2,LOW);

  Serial.println("Initializing.....");
  
}

void loop() {

  if(islevel1 == false){
    level1 = waterSensorReading_1();
    Serial.print("WL Sensor 1: ");
    Serial.println(level1);

    if(level1 > 2000){
      islevel1 = true;
//      pinMode(wl_sens_pwer_1,INPUT);
      delay(5000);
    }
  } else if(!islevel2){
    level2 = waterSensorReading_2();
    Serial.print("WL Sensor 2: ");
    Serial.println(level2);

    if(level2 > 2000){
      islevel2 = true;
    }
  }

}

int waterSensorReading_1()
{
  digitalWrite(wl_sens_pwer_1,HIGH);
  delay(10);
  value=analogRead(wl_sens_sig_1);
  delay(10);
  digitalWrite(wl_sens_pwer_1,LOW);
  return value;
}

int waterSensorReading_2()
{
  digitalWrite(wl_sens_pwer_2,HIGH);
  delay(10);
  value=analogRead(wl_sens_sig_2);
  delay(10);
  digitalWrite(wl_sens_pwer_2,LOW);
  return value;
}

//int waterSensorReading_3()
//{
//  digitalWrite(wl_sens_pwer_3,HIGH);
//  delay(10);
//  value=analogRead(wl_sens_sig_3);
//  delay(10);
//  digitalWrite(wl_sens_pwer_3,LOW);
//  return value;
//}
//
//int waterSensorReading_2()
//{
//  digitalWrite(wl_sens_pwer_4,HIGH);
//  delay(10);
//  value=analogRead(wl_sens_sig_4);
//  delay(10);
//  digitalWrite(wl_sens_pwer_4,LOW);
//  return value;
//}
