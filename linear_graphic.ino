#include <UTFT.h>//Загрузка библиотеки экрана
#include <Servo.h>//Загрузка библиотеки сервы

int maxDistancePX = 100;
int aligned = 0;
int digitWidth = 10;
int startOffset = aligned + digitWidth * 3;

int trigPin = 6; 
int echoPin = 5;
int servoPin = 9;
extern uint8_t BigFont[];   
// led = 3.3v
// gnd = gnd
// vcc = 5v
int sckPin  = 10;
int sdaPin  = 11;
int rsPin  = 12;
int resetPin  = 13;
int csPin  = 8;

#include <Ultrasonic.h>//Загрузка библиотеки датчика
Servo servoMain; // Инициализация сервы
Ultrasonic ultrasonic(trigPin, echoPin);//Инициализация контактов датчика
UTFT myGLCD(TFT18SHLD,   sdaPin,sckPin,csPin,resetPin,rsPin); // тип дисплея 1,8  TFT  (128x160 chip ST7735S)

int i = 0; //Переменная отвечающая за положение сервы
int dist_cm = 0; //Переменная для хранениия данных датчика
boolean trig = false; //Направление вращения

void setup()
{
  servoMain.attach(servoPin); // Серва присоединена к servoPin выводу
  myGLCD.InitLCD(); //Создаем обьект дисплея
  myGLCD.setFont(BigFont);
  initDisplay();

  Serial.begin(9600);//Инициализируем канал вывода данных. Необязательно
}
 
void loop()
{
  if(!trig){i++;}//Если trig false увеличиваем i на еденицу каждую итерацию
  if(trig){i--;}//Если trig true уменьшаем i на еденицу каждую итерацию

  dist_cm = ultrasonic.read(CM);  //Записываем в переменную данные датчика
  
  
  myGLCD.setColor(VGA_FUCHSIA); //Устанавливаем цвет рисования обьекта на экране
  int y = (maxDistancePX - min(dist_cm, maxDistancePX));
  myGLCD.fillCircle(i + (startOffset), y, 2);//Рисуем круг радиусом 2 пикселя, в координатах x = i, y = данные датчика
  
  if(i==159 - (startOffset)) { //Если i достигла края экрана{
    trig = true;//Меняем значение trig на true
    initDisplay();
  } else if(i==0) {
    trig = false;//Меняем значение trig на false
    initDisplay();
  }
  delay(5);//Задержка в 5мс. Необходима для исключения влияния помех сервы на датчик
  servoMain.write(i);	//Поворачиваем серву на значение i/2
  delay(5);//Задержка в 5мс. Необходима для исключения влияния помех сервы на датчик
}


void initDisplay() {
  myGLCD.clrScr();//Очищаем экран
  // myGLCD.setBackColor(VGA_WHITE);//

  myGLCD.setColor(VGA_RED); //Устанавливаем цвет рисования обьекта на экране
  myGLCD.fillRect(aligned + 5, maxDistancePX , 150, maxDistancePX + 2 );            // рисуем линию (через точки с координатами 10x10 - 150x10)
  myGLCD.printNumI(0,aligned,maxDistancePX - 5);
  myGLCD.printNumI(50,aligned,maxDistancePX - 50 - 5);
  myGLCD.printNumI(100,aligned,maxDistancePX - 100 - 5);
  // myGLCD.printNumI(0,aligned,maxDistancePX - 5);
}









