#include <UTFT.h>//Загрузка библиотеки экрана
#include <Servo.h>//Загрузка библиотеки сервы
#include <Ultrasonic.h>//Загрузка библиотеки датчика
extern uint8_t BigFont[];   

int rad_max = 100;                 //Максимальный радиус (100 пикселей)
int drawHeight;                    //Переменная для записи инвертированного сигнала


// PINS FOR SERVO 
int trigPin = 6; 
int echoPin = 5;
int servoPin = 9;

// PINS FOR DISPLAY 
// led = 3.3v
// gnd = gnd
// vcc = 5v
int sckPin  = 10;
int sdaPin  = 11;
int rsPin  = 12;
int resetPin  = 13;
int csPin  = 8;

Servo servoMain; // Инициализация сервы
Ultrasonic ultrasonic(trigPin, echoPin);//Инициализация контактов датчика
UTFT myGLCD(TFT18SHLD,   sdaPin,sckPin,csPin,resetPin,rsPin); // Инициализация дисплея 1,8  TFT  (128x160 chip ST7735S)

int i = 0; //Переменная отвечающая за положение сервы
int dist_cm = 0; //Переменная для хранениия данных датчика

void setup(){
    servoMain.attach(servoPin); // Серва присоединена к servoPin выводу
    myGLCD.InitLCD(); //Создаем обьект дисплея
    myGLCD.setFont(BigFont);
    initDisplay();

    Serial.begin(9600);//Инициализируем канал вывода данных. Необязательно
}

void loop() {
    marking();                          //Рисуем разметку на экране

    for (int i=0;i<=180;i=i+2){         //ВРАЩЕНИЕ В ЛЕВУЮ СТОРОНУ
        rangefinder();                     //Выводим значение расстояния
        servoMain.write(i);                    //Повернуть сервопривод на угол i
        dist_cm = ultrasonic.read(CM);
        int drawHeight = map(dist_cm, 0, 70, 0, rad_max);  //Инвертируем сигнал с HC-SR04 и записываем в переменную drawHeight      
        myGLCD.setColor(0,110, 30);                               //Устанавливаем цвет линии и пространства за ней
        myGLCD.drawLine(80,128,80+(rad_max)*cos((360-i)*3.14/180), 128+(rad_max)*sin((360-i)*3.14/180));   //Рисуем линию радара
        myGLCD.setColor(10, 10, 255);                                                                    //Устанавливаем цвет препятствий (красный)
        myGLCD.fillCircle(80+drawHeight*cos((360-i)*3.14/180), 128+drawHeight*sin((360-i)*3.14/180), 3);   //Рисуем препятствие по координатам, полученным с HC-SR04 
    }
    myGLCD.clrScr();       //Очищаем и устанавливаем цвет LCD-дисплея (чёрный)
    marking();                           //Рисуем разметку на экране

    for (int i=180;i>=0;i=i-2){          //ВРАЩЕНИЕ В ПРАВУЮ СТОРОНУ
        rangefinder();                      //Выводим значение расстояния
        servoMain.write(i);                     //Повернуть сервопривод на угол i
        dist_cm = ultrasonic.read(CM);
        int drawHeight = map(dist_cm, 0, 70, 0, rad_max);  //Инвертируем сигнал с HC-SR04 и записываем в переменную drawHeight      
        myGLCD.setColor(0,110, 30);                               //Устанавливаем цвет линии и пространства за ней
        myGLCD.drawLine(80,128,80+(rad_max)*cos((360-i)*3.14/180), 128+(rad_max)*sin((360-i)*3.14/180));   //Рисуем линию радара
        myGLCD.setColor(10, 10, 255);                                                                    //Устанавливаем цвет препятствий (красный)
        myGLCD.fillCircle(80+drawHeight*cos((360-i)*3.14/180), 128+drawHeight*sin((360-i)*3.14/180), 3);   //Рисуем препятствие по координатам, полученным с HC-SR04 
    }  

    myGLCD.clrScr();        //Очищаем и устанавливаем цвет LCD-дисплея (чёрный)
    marking();                            //Рисуем разметку на экране}
}

void initDisplay() {
  myGLCD.clrScr();//Очищаем экран
  marking();
}

void marking(){                              //ФУНКЦИЯ ДЛЯ РИСОВАНИЯ РАЗМЕТКИ НА ЭКРАНЕ
    myGLCD.setColor(VGA_WHITE);//
    myGLCD.drawLine(80,128,0, 80);               //Рисуем линию
    myGLCD.drawLine(80,128,31, 41);              //Рисуем линию
    myGLCD.drawLine(80,128,80, 28);              //Рисуем линию
    myGLCD.drawLine(80,128,129, 42);             //Рисуем линию
    myGLCD.drawLine(80,128,160, 80);             //Рисуем линию
    myGLCD.drawCircle(80,128,rad_max*0.25);      //Рисуем полуокружность
    myGLCD.drawCircle(80,128,rad_max*0.5);       //Рисуем полуокружность
    myGLCD.drawCircle(80,128,rad_max*0.75);      //Рисуем полуокружность
    myGLCD.drawCircle(80,128,rad_max);           //Рисуем полуокружность
}

void rangefinder(){                             //ФУНКЦИЯ ДЛЯ ВЫВЕДЕНИЯ НА ЭКРАН ЗНАЧЕНИЯ РАССТОЯНИЯ
    dist_cm = ultrasonic.read(CM);
    myGLCD.setColor(VGA_LIME);                 //Устанавливаем цвет значения   (жёлтый)
    myGLCD.print("DIST: ",10,5);              //Выводим текст "DISTANCE:" на указанную координату (10,5)
    myGLCD.setColor(VGA_LIME);                 //Устанавливаем цвет значения   (жёлтый)
    myGLCD.printNumI(dist_cm,130,5, 2);          //Выводим текст значения на указанную координату (130,5)
}


