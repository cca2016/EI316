#include <Wire.h> //IIC库SDA接A4，SCL接A5
#include <math.h> 
#include <dht11.h>
int BH1750address = 0x23;//芯片地址为16位23
byte buff[2];
int num = 0;  
//DTH11

 
dht11 DHT11;
#define DHT11PIN 2
//soil
#define Moisture A0 //定义AO 引脚 为 IO-A0  
#define DO 7        //定义DO 引脚 为 IO-7  
//delay(150); 
void setup()
{
  Wire.begin();
  pinMode(Moisture, INPUT);//定义A0为输入模式  
  pinMode(DO, INPUT);  
  pinMode(12,OUTPUT);//soil moisture
  pinMode(4,OUTPUT);//gy-30 upside-down
  pinMode(8,OUTPUT);//gy-30 upside-down
  Serial.begin(9600);
}
void loop()
{
  delay(100);
//GY-30
 int i;
 uint16_t val=0;
 BH1750_Init(BH1750address);
 //delay(1000);
 if(2==BH1750_Read(BH1750address))
  {
   val=((buff[0]<<8)|buff[1])/1.2;
   if (val<=35) {digitalWrite(8,LOW); digitalWrite(4,HIGH); }
   else {digitalWrite(4,LOW); digitalWrite(8,HIGH); }
   Serial.print(val);     
   Serial.println("[lx]"); 
  }
 
 ///DTH11
 Serial.println("\n");
 
  int chk = DHT11.read(DHT11PIN);
 
  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }
 
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
 
  Serial.print("Temperature (oC): ");
  Serial.println((float)DHT11.temperature, 2);
 
  //Serial.print("Temperature (oF): ");
  //Serial.println(Fahrenheit(DHT11.temperature), 2);
 
  //Serial.print("Temperature (K): ");
  //Serial.println(Kelvin(DHT11.temperature), 2);
 
  //Serial.print("Dew Point (oC): ");
  //Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));
 
  //Serial.print("Dew PointFast (oC): ");
  //Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));
  // delay(2000);
 
  Serial.print("Moisture=");  
  Serial.print(analogRead(Moisture));//读取AO的数值  
  if (analogRead(Moisture)<950) digitalWrite(12,LOW);
  else digitalWrite(12,HIGH); 
  Serial.print("|DO=");  
  Serial.println(digitalRead(DO));//读取DO的数值  
  delay(1000);  
  while(Serial.available())  
   {  
     char c=Serial.read();  
      if(c=='A')  
        {  
          num++;  
          Serial.print("Hello:num=");  
          Serial.println(num);  
        }  
   }  
}
int BH1750_Read(int address) //
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
    buff[i] = Wire.read();  // read one byte
    i++;
  }
  Wire.endTransmission();  
  return i;
}
void BH1750_Init(int address) 
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}
double Fahrenheit(double celsius) 
{
        return 1.8 * celsius + 32;
}    //摄氏温度度转化为华氏温度
 
double Kelvin(double celsius)
{
        return celsius + 273.15;
}     //摄氏温度转化为开氏温度
 
// 露点（点在此温度时，空气饱和并产生露珠）
// 参考: [url=http://wahiduddin.net/calc/density_algorithms.htm]http://wahiduddin.net/calc/density_algorithms.htm[/url] 
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}
 
// 快速计算露点，速度是5倍dewPoint()
// 参考: [url=http://en.wikipedia.org/wiki/Dew_point]http://en.wikipedia.org/wiki/Dew_point[/url]
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}
