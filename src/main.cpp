//27.08

#include <Arduino.h>
bool RightInt=false;

#include "DebounceLeftRight.h"   // Обработка горизонтальных кнопок
#include "DebounceV2.h" // Обработка вертикальных кнопок
int8_t CountStepTiming=0;// Перебор пунктиков меню 101 для демонстрации скорости моргания поворотников


bool OffPovorotniki = false; //Переменная выключае поворотники при выходе из главного меню

// Переменные для того чтоб в 101 при переборе значения поворотники моргали
bool change101 = false; // Флаг что кнопка нажата и значение изменилось
unsigned long timing101;  //для задержки в 500 мсек в 1.1 (101)
// Переменные для того чтоб в 101 при переборе значения поворотники моргали
bool AutomaticModeActivateR; // Активирует автоматический режим. (Если On cчитано из памяти) . Для правого поворотника   >0
bool AutomaticModeActivateL;
#include "PovorotnikiLR.h" // Включение и выключение поворотников

bool OneRazSavePRK_GE;// Для корректного сбороса автоматического режима правого поворотника
bool OneRazSavePRK_GE2L;// Для корректного сбороса автоматического режима левого поворотника

#include <U8g2lib.h>    // Для экрана
#include "EEPROM.h" 

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

void SaveBlink1_1();
void SaveBlink1_2();
void SaveBlink1_3(); 
void SaveBlink1_4(); 
void SaveBlink1_5(); 
void SaveBlink2_1();
void SaveBlink2_2();
void SaveBlink2_3();
void SaveBlink2_4();
void SaveBlink2_5();
void SaveBlink2_6();
void SaveBlink2_7();
void SaveBlink2_8();
void SaveBlink2_9();
void SaveBlink2_10();
void SaveBlink2_11();
void SaveBlink3_1();
void SaveBlink3_11();
void SaveBlink3_2();
void SaveBlink3_21();
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// End of constructor list

// Переменные для работы меню
int16_t MenuLayer = -1; //Начальный 0 слой где видны все пункты. 2 - вход в меню CirclY
int16_t PositionUpCount=1; //Ползунок по вертикали
int8_t PositionRightCount=1;
bool EnableSdvig=false; //Чтоб ползунок один раз сдвигался 
int CirclY = 45; // Позиция курсора
unsigned long FirstZamerMLayerMin1_Val; // Замеряет время нажатия кнопок правого и левого поворотников. Для входа в главное меню
bool OneRazMLayerMin1=false; // При зажатии 2х кнопок чтоб стартовый замер был один раз
// Переменные для работы меню

// Переменные для вкладки 1
bool saveBlink1_1=false;
bool OneRazPosition1_1=false;     // Один раз завести правильное значение из меню
int16_t old_SpeedPovorotnikBlink; // Старое значения скорости моргания поворотника
int16_t old_PositionUpCount1_1;
bool saveBlink_sensOnValue1_1;

bool OneRazPosition1_2=false; 
bool OneRazGalochka1_2=false; // Используется для того чтоб галочка ставилась напротив текущего значения
bool saveBlink1_2=false;
bool saveBlink_sensOnValue1_2=false;

bool saveBlink1_3=false;
bool OneRazPosition1_3=false;     // Один раз завести правильное значение из меню
int16_t old_CountBlinkIntMode; // Старое значения скорости моргания поворотника
int16_t old_PositionUpCount1_3;
bool saveBlink_sensOnValue1_3;

bool OneRazPosition1_4=false; 
bool OneRazGalochka1_4=false; // Используется для того чтоб галочка ставилась напротив текущего значения
bool saveBlink1_4=false;
bool saveBlink_sensOnValue1_4=false;

bool saveBlink1_5=false;    
int16_t old_TimePressToOnAutoMode; // Старое значение
int16_t old_PositionUpCount1_5;
bool OneRazPosition1_5 = false; // Один раз завести правильное значение из меню
bool saveBlink_sensOnValue1_5 = false;
// Переменные для вкладки 1

// Переменные для вкладки 2
bool OneRazGalochka2_1=false; // Используется для того чтоб галочка ставилась напротив текущего значения
bool saveBlink2_1=false;


int16_t old_sensOnValue=0;
bool OneRazPosition2_2=false;
bool saveBlink2_2=false;// Буль для моргания надписи save Подпункт 2.2
bool saveBlink_sensOnValue2_2=false;

bool saveBlink2_3=false;
bool OneRazGalochka2_3=false;

bool saveBlink2_4=false;
bool OneRazPosition2_4=false; // Один раз завести правильное значение из меню
int16_t old_StartToAccelValue;
int16_t old_PositionUpCount2_4;
bool saveBlink_sensOnValue2_4;

bool saveBlink2_5=false;
bool OneRazPosition2_5=false; // Один раз завести правильное значение из меню
int16_t old_StopToAccelValue;
int16_t old_PositionUpCount2_5;
bool saveBlink_sensOnValue2_5;

bool saveBlink2_6=false;
bool OneRazPosition2_6=false; // Один раз завести правильное значение из меню
int16_t old_StartPersentBright;
int16_t old_PositionUpCount2_6;
bool saveBlink_sensOnValue2_6;

bool saveBlink2_7=false;
bool OneRazPosition2_7=false; // Один раз завести правильное значение из меню
int16_t old_StopPersentBright;
int16_t old_PositionUpCount2_7;
bool saveBlink_sensOnValue2_7;

bool saveBlink2_8=false;
bool OneRazPosition2_8=false; // Один раз завести правильное значение из меню
int16_t old_DelayFallingBright;
bool saveBlink_sensOnValue2_8;

bool saveBlink2_9=false;
bool OneRazPosition2_9=false; // Один раз завести правильное значение из меню
int16_t old_BrightWhenIdle;
int16_t old_PositionUpCount2_9;
bool saveBlink_sensOnValue2_9;

bool saveBlink2_10=false;
bool OneRazPosition2_10=false; // Один раз завести правильное значение из меню
int16_t old_SpeedFalling;
int16_t old_PositionUpCount2_10;
bool saveBlink_SpeedFalling2_10;

bool saveBlink2_11=false;
bool OneRazPosition2_11=false; // Один раз завести правильное значение из меню
int16_t old_EcoBright;
int16_t old_PositionUpCount2_11;
bool saveBlink_EcoBright2_11;
// Переменные для вкладки 2

// Переменные для вкладки 3
bool saveBlink3_1=false;
bool OneRazGalochka3_1=false; // Один раз завести правильное значение из меню
int16_t old_PositionUpCount3_1;
bool saveBlink_EcoBright3_1;

bool saveBlink3_11=false;
bool OneRazPosition3_11=false; // Один раз завести правильное значение из меню
int16_t old_SettingMaxVolumeOnSpeed;
int16_t old_PositionUpCount3_11;
bool saveBlink_sensOnValue3_11;

bool saveBlink3_2=false;
bool OneRazGalochka3_2=false;
//bool OneRazPosition3_2=false; // Один раз завести правильное значение из меню
int16_t old_PositionUpCount3_2;
bool saveBlink_sensOnValue3_2;

bool saveBlink3_21=false;
bool OneRazPosition3_21;
int16_t old_SettingTimePresseMax;
bool saveBlink_sensOnValue3_21;
// Переменные для вкладки 3

// Переменные которые мы изменяем из меню. Которые и влияют на работу системы
bool TunL=false;              //2.1  //Включает и выключает свет при вьезде в туннель
int sensOnValue=0;            //2.2  //Параметр. Изминение чуствительности.
bool AdaptivBrightness=false; //2.3
int8_t StartToAccel=0;        //2.4

int8_t StopToAccel=0;        //2.5
int8_t StartPersentBright=0; //2.6
int8_t StopPersentBright=0;  //2.7
int8_t DelayFallingBright=0; //2.8

int8_t BrightWhenIdle=0;     //2.9
int8_t SpeedFalling=0;       //2.10
int8_t EcoBright=0;          //2.11

bool VolumeOnSpeed = false;  // 3.1
int8_t SettingMaxVolumeOnSpeed = 0; // 3.2 На какой скорости вкляются 2 буззера   
bool VolumeTimePressed = false;
int8_t SettingTimePresseMax;

int8_t SpeedPovorotnikBlink; // 1.1
bool IntelligentMode;        // 1.2
int8_t CountBlinkIntMode;    // 1.3
bool AutomaticMode;          // 1.4 
int8_t TimePressToOnAutoMode;// 1.5

// Переменные которые мы изменяем из меню. Которые и влияют на работу системы
#include "GlobalPrint.h"
void setup(void) {
  Serial.begin(115200);
  u8g2.begin();
  //Eeprom
  if (!EEPROM.begin(512)) {
      Serial.println("Failed to initialise EEPROM");
      Serial.println("Restarting...");
      delay(1000);
      ESP.restart();
  }
  //Eeprom
  // SK6812
    strip.Begin();
    strip.Show();
  // SK6812
  PinMode(); // Описание режима пинов
  // Чтение значений из Eeprom и присваивание их значений переменным
  // delay(100);

  TunL=              EEPROM.readBool(0); // Чтение из адреса 0
  sensOnValue=       EEPROM.readByte(1);
  AdaptivBrightness= EEPROM.readBool(2);
  StartToAccel=      EEPROM.readByte(3);
  StopToAccel=       EEPROM.readByte(4);

  StartPersentBright=EEPROM.readByte(5);
  StopPersentBright =EEPROM.readByte(6);
  DelayFallingBright=EEPROM.readByte(7);
  BrightWhenIdle=    EEPROM.readByte(8);

  SpeedFalling =     EEPROM.readByte(9);
  EcoBright =        EEPROM.readByte(10);
  VolumeOnSpeed =    EEPROM.readBool(11);
  SettingMaxVolumeOnSpeed = EEPROM.readByte(12);

  VolumeTimePressed    = EEPROM.readBool(13);
  SettingTimePresseMax = EEPROM.readByte(14);

  SpeedPovorotnikBlink = EEPROM.readByte(15);
  IntelligentMode      = EEPROM.readBool(16);
  CountBlinkIntMode     =EEPROM.readByte(17);
  AutomaticMode         =EEPROM.readByte(18);
  TimePressToOnAutoMode =EEPROM.readByte(19);

  // Чтение значений из Eeprom и присваивание их значений переменным
}
//void Debounce(const int8_t buttonPin,bool& buttonState,bool& lastButtonState,unsigned long& lastDebounceTime,uint8_t debounceDelay);
//bool MinusUP4 = false;

void loop(void) {
while(1){

// Печать отладки
// /*
 GlobalPrint();
// */
// Печать отладки  

// Обработка кнопок
  //Debounce(UpButtonPin,buttonState4,lastButtonState4,lastDebounceTime4,debounceDelay4);//Обработка нажатия вверх
  DebounceV2(UpButtonPin,buttonState4,lastButtonState4,lastDebounceTime4,debounceDelay4,MinusUP4,OneRazFirstTimePressed4,TimePressed4,FirstTimePressed4);//Обработка нажатия вверх +
  DebounceV2(DownButtonPin,buttonState2,lastButtonState2,lastDebounceTime2,debounceDelay2,MinusUP2,OneRazFirstTimePressed2,TimePressed2,FirstTimePressed2);

  Debounce(RightButtonPin,buttonState26,lastbuttonState26,lastDebounceTime26,debounceDelay26);//Обработка нажатия вверх
  Debounce(LeftButtonPin,buttonState32,lastbuttonState32,lastDebounceTime32,debounceDelay32);//Обработка нажатия вверх
// Обработка кнопок 

// Главный экран
if(MenuLayer==-1){
  // Обработка входа в главное меню
    if( (digitalRead(RightButtonPin)==HIGH) && (digitalRead(LeftButtonPin)==HIGH) ){ //Если нажаты левая и правая кнопки одновременно

        // Один раз замерять текущее время
        if(OneRazMLayerMin1 == false){
            FirstZamerMLayerMin1_Val = millis();
            OneRazMLayerMin1 = true;
        }
        // Один раз замерять текущее время

        // Если текущее время - замерянное больше 2х секунд. То войти в меню 
        if(millis() - FirstZamerMLayerMin1_Val >1200){
            MenuLayer=0;PositionUpCount=1;PositionRightCount=0; // Войти в меню
            // OffPovorotniki = true; //Выставить флаг погасить поворотник при входе в меню
        }
        // Если текущее время - замерянное больше 2х секунд. То войти в меню
    }
    else{OneRazMLayerMin1=false;} 
  // Обработка входа в главное меню
/*
    if(IntelligentMode == false){ //!! Возможно не соответствует ТЗ! Если не включен режим интеллигент то моргать пока нажаты кнопки вправо или влево
        
    }                             //Если не включен режим интеллигент то моргать пока нажаты кнопки вправо или влево
    */
Povorotniki(); // Функция обрабатывает нажание на кнопки право и лево и включает выключает поворотники.
  // Измеряем время нажатия правой или левой кнопки для входа для включения автоматического режима
// /*
   
  // Измеряем время нажатия правой или левой кнопки для входа для включения автоматического режима
}
// Главный экран

// Ползаем по главному меню
if(MenuLayer==0 || MenuLayer==1){
  RightInt=false; // Cкидываем чтоб после того как выходим из меню в главный экран не включался правый поворотник
  PovorotnikiRightOff(); PovorotnikiLeftOff();           //Защита от загорания при переходе в главное меню из int mode
  beginIntModeBlinkR = false; beginIntModeBlinkL = false;//Защита от загорания при переходе в главное меню из int mode
  
  //PovorotOnRight=false; // Чтоб при выходе из меню в главный экран не загорался поворотник
  if(PositionUpCount == 0){PositionUpCount = 7;} // Ограничить вертикальный ползунок при движении вверх
  if(PositionUpCount > 7){PositionUpCount = 7;} // Ограничить вертикальный ползунок при движении вниз

  if(PositionUpCount==1){   CirclY = 18; //MenuLayer=0;  
                            if( PositionRightCount == 1){ MenuLayer=10; PositionUpCount=50; } 
  }
  if(PositionUpCount==2){   CirclY = 29; //MenuLayer=0; 
                            if( PositionRightCount == 1){ MenuLayer=20; PositionUpCount=100; } 
                        }                   
  if(PositionUpCount==3){   CirclY = 39; //MenuLayer=0;
                            if( PositionRightCount == 1){ MenuLayer=30; PositionUpCount=100; }
                        }
  if(PositionUpCount==4){   CirclY = 48; MenuLayer=0;  
                            if(PositionRightCount == 1) {PositionRightCount=0;} // Заглушка
                        }
  if(PositionUpCount==5){   CirclY = 8;  MenuLayer=1;  
                            if(PositionRightCount == 1) {PositionRightCount=0;} // Заглушка
                        }
  if(PositionUpCount==6){   CirclY = 18; MenuLayer=1;  
                            if(PositionRightCount == 1) {PositionRightCount=0;} // Заглушка
                        }
  if(PositionUpCount==7){   CirclY = 28; MenuLayer=1;  // exit
                            if( PositionRightCount == 1){ 
                              MenuLayer=-1; OffPovorotniki = true; timingOffPovorotniki=millis(); 
                              PovorotOnRight=true;
                              PositionRightCount = OldPositionRightCountInt; // Чтоб убрать баги с морганием при выходе из меню интеллигент режим
                            } // Выключить поворотники при выходе из меню
                        }
                         
  
}
// Ползаем по главному меню

// Перебираем вкладку 1
if(MenuLayer == 10){
  if(PositionUpCount < 50){PositionUpCount = 50;} // Ограничить вертикальный ползунок при движении вверх

  if(PositionUpCount==50){   CirclY = 20-1;   }
  if(PositionUpCount==51){   CirclY = 32+13;  }

  if(PositionRightCount == 2 && PositionUpCount==50){ MenuLayer=101; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 1.1
  if(PositionRightCount == 2 && PositionUpCount==51){ MenuLayer=102; PositionUpCount=120; }
  if(PositionRightCount == 0){ //Если в подменю 0.2 Нажата кнопка влево то выйти в главное меню
      MenuLayer=0;PositionUpCount=1;
  }
  if(PositionUpCount==52){    MenuLayer=11;  } //При скролле вниз перейти на нижнюю часть страницы
  OneRazPosition1_1 = false; // Сброс один раз исполнить
  saveBlink_sensOnValue1_1=false;

  saveBlink_sensOnValue1_2=false;
  OneRazGalochka1_2=false;

  
}
if(MenuLayer == 11){
  
  if(PositionUpCount==52){   CirclY = 10-1;   }
  if(PositionUpCount==53){   CirclY = 22+13;  }
  
  if(PositionRightCount == 2 && PositionUpCount==52){ MenuLayer=103; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 1.1
  if(PositionRightCount == 2 && PositionUpCount==53){ MenuLayer=104; PositionUpCount=120; }
  if(PositionRightCount == 0){ //Если в подменю 0.2 Нажата кнопка влево то выйти в главное меню
      MenuLayer=0;PositionUpCount=1;
  }
  if(PositionUpCount==51)   { MenuLayer=10;  }   //При скролле вверх перейти на верхнюю часть страницы
  if(PositionUpCount==54){    MenuLayer=12;  } //При скролле вниз перейти на нижнюю часть страницы

  OneRazPosition1_3 = false; // Сброс один раз исполнить
  saveBlink_sensOnValue1_3=false;

  //OneRazPosition1_2 = false; // Сброс один раз исполнить
  saveBlink_sensOnValue1_4=false;
  OneRazGalochka1_4=false;
}
if(MenuLayer == 12){
  if(PositionUpCount==54){  CirclY = 10-1;  }
  
  if(PositionRightCount == 2 && PositionUpCount==54){ MenuLayer=105; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 1.1
  
  if(PositionRightCount == 0){ //Если в подменю 0.2 Нажата кнопка влево то выйти в главное меню
      MenuLayer=0;PositionUpCount=1;
  }
  if(PositionUpCount==53)   { MenuLayer=11;  }   //При скролле вверх перейти на верхнюю часть страницы
  if(PositionUpCount > 54)  { PositionUpCount=54;  }   //Ограничить ползунок вниз

  OneRazPosition1_5 = false;
  saveBlink_sensOnValue1_5=false;
}


if(MenuLayer == 101){ // 1.1 SpeedPovorotnikBlink
     
    static unsigned long timingLeftRightBlink101; // Используется для таймера моргания правого поворотника
    static bool PovorotnikiOn;
     /*
    Serial.print("PositionRightCount:" );Serial.print(PositionRightCount );
    Serial.print(" timingLeftRightBlink101:" );Serial.print(timingLeftRightBlink101 );
    Serial.print(" PovorotnikiOn:" );Serial.print(PovorotnikiOn);
    Serial.print(" CountStepTiming:" );Serial.print(CountStepTiming);
    Serial.print(" change101:" );Serial.print(change101);
    Serial.print("  SpeedPovorotnikBlink:" );Serial.print( SpeedPovorotnikBlink);
    Serial.println();
     */
    if(OneRazPosition1_1==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_SpeedPovorotnikBlink = SpeedPovorotnikBlink;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(SpeedPovorotnikBlink,0,30,-180,-210);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition1_1=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount1_1=PositionUpCount; // Постоянно присваивать в старое значение
      SpeedPovorotnikBlink=abs(    (180 + PositionUpCount)   );   
  
       SpeedPovorotnikBlink=constrain(SpeedPovorotnikBlink,10,30);
       if( PositionUpCount>-190){PositionUpCount=-190;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-210){PositionUpCount=-210;}  // Защита от выхода за диапазон)
       
       // <- Лич
       //Если значение изменилось то 500миллисек пауза, затем 2 секунды моргают с заданной скоростью левый и правый поворотник
   
       if(change101 == true){ // Если в менюшке 101 нажата кнопка вверх или вниз. Или кнопка вправо (с моментами)
          if(digitalRead(4)==true || digitalRead(2)==true ){ CountStepTiming = 0; } //Если в любой момент нажата кнопка вверх или вниз то гасимся и ждём секунду
          
          //static unsigned long timing101;  //для задержки в 500 мсек в 1.1 (101)
          if(CountStepTiming == 0){ //Пауза в 0.5 сек перед тем как начать моргать
              PovorotnikiRightOff(); PovorotnikiLeftOff(); // turn off the pixels
             
              if (millis() - timing101 > 1000){ // Если прошла секунда то увеличить счётчик и начать моргать
                  Serial.println ("0.5 seconds Prostoi. Next");
                  timing101=millis();
                  CountStepTiming=1;
              }
          }
          if(CountStepTiming == 1){ // Исполнить 2 сек (Само моргание - Демонстрация скорости)
              
              // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                  uint16_t TempInvertVal;// = SpeedPovorotnikBlink*100;
                  if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }
                  if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
                  //Serial.print("MAP TempInvertVal:");Serial.println(TempInvertVal);
              // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

              if (millis() - timingLeftRightBlink101 >= TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                  PovorotnikiOn = !PovorotnikiOn;
                  timingLeftRightBlink101 = millis(); 
              }
              if(PovorotnikiOn == true){ // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                  RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый
                  for(int i=0;  i<26;++i) {strip.SetPixelColor(i, color);}   strip.Show();// Зажечь левый правый поворотник
              }                          // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
              else{                       // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ
                    PovorotnikiRightOff(); // turn off the pixels
                    PovorotnikiLeftOff(); // turn off the pixels
                  }
              if (millis() - timing101 > 4000){  //Время сколько происходит моргание 
                  //Serial.println ("4 seconds Proslo");
                  
                  PovorotnikiRightOff(); PovorotnikiLeftOff(); // turn off the pixels
                  CountStepTiming = 0;
                  change101 = false; //Перезаряд
                  PovorotnikiOn=0;
                  //timing101=millis();
              } // Исполнить 2 секунды и прервать
          }
                 
         // /*
                 
       } // Если в менюшке 101 нажата кнопка вверх или вниз
       static bool SaveOk101=false; // Для того чтоб убрать глюк ленты при использовании EEPROM
       // <- Лич
       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          //EEPROM.writeByte(15, SpeedPovorotnikBlink); EEPROM.commit();
          saveBlink_sensOnValue1_1=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink1_1=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали

          SaveOk101 = true;
          if(change101 == false){ // Чтоб не перезапускало моргание если уже моргает
            CountStepTiming=0;
            change101=true;timing101 = millis();
          }
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue1_1 != true){SpeedPovorotnikBlink=old_SpeedPovorotnikBlink; }
          if(SaveOk101 == true){ EEPROM.writeByte(15, SpeedPovorotnikBlink); EEPROM.commit(); SaveOk101 = false; }
          // Чтоб если мы выходим на моменте когда лента в демонстрации горит
          PovorotnikiRightOff(); // turn off the pixels
          PovorotnikiLeftOff(); // turn off the pixels
          CountStepTiming=0;
          change101=false;timing101 = millis();
          MenuLayer=10;PositionUpCount=50;
       }
}
if(MenuLayer == 102){ // 1.2
    if(PositionRightCount ==1){ // back
        MenuLayer=10;PositionUpCount=51;
    }
    if(PositionRightCount ==3){ // save

        if(PositionUpCount ==120){
          IntelligentMode=true;
        }
        if(PositionUpCount ==121){
          IntelligentMode=false;
        }
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(16, IntelligentMode);
        EEPROM.commit();
        //address += sizeof(bool);

        saveBlink1_2=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
if(MenuLayer == 103){ // 1.3 CountBlinkIntMode
    if(OneRazPosition1_3==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_CountBlinkIntMode = CountBlinkIntMode;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(CountBlinkIntMode,0,30,-180,-210);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition1_3=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount1_3=PositionUpCount; // Постоянно присваивать в старое значение
      CountBlinkIntMode=abs(    (180 + PositionUpCount)   );   
  
       CountBlinkIntMode=constrain(CountBlinkIntMode,0,30);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-210){PositionUpCount=-210;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeByte(17, CountBlinkIntMode);
          EEPROM.commit();

          saveBlink_sensOnValue1_3=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink1_3=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back

       if(saveBlink_sensOnValue1_3 != true){
          CountBlinkIntMode=CountBlinkIntMode;
       }

      MenuLayer=11;PositionUpCount=52;
    }
}
if(MenuLayer == 104){ // 1.4
    if(PositionRightCount ==1){ // back
        MenuLayer=11;PositionUpCount=53;
    }
    if(PositionRightCount ==3){ // save

        if(PositionUpCount ==120){
          AutomaticMode=true;
        }
        if(PositionUpCount ==121){
          AutomaticMode=false;
        }
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(18, AutomaticMode);
        EEPROM.commit();
        //address += sizeof(bool);

        saveBlink1_4=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
if(MenuLayer == 105){ // 1.5 TimePressToOnAutoMode
    if(OneRazPosition1_5==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_TimePressToOnAutoMode = TimePressToOnAutoMode;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(TimePressToOnAutoMode,0,50,-180,-230);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition1_5=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount1_5=PositionUpCount; // Постоянно присваивать в старое значение
      TimePressToOnAutoMode=abs(    (180 + PositionUpCount)   );   
  
       TimePressToOnAutoMode=constrain(TimePressToOnAutoMode,0,50);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-230){PositionUpCount=-230;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeByte(19, TimePressToOnAutoMode);
          EEPROM.commit();

          saveBlink_sensOnValue1_5=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink1_5=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back

       if(saveBlink_sensOnValue1_5 != true){
          TimePressToOnAutoMode=old_TimePressToOnAutoMode;
       }

      MenuLayer=12;PositionUpCount=54;
    }
}
// Перебираем вкладку 1

// Перебираем вкладку 2
if(MenuLayer == 20){ //Если в подменю 2.1 Нажата кнопка влево то выйти в главное меню
    if(PositionUpCount==100){   CirclY = 20-1;   }  //TunL  //Перебираем ползунок
    if(PositionUpCount==101){   CirclY = 30-1;   }          //Перебираем ползунок
    if(PositionUpCount==102){   CirclY = 40-1;   }          //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount==100){ MenuLayer=201; PositionUpCount=120; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.11 //TunL
    if(PositionRightCount == 2 && PositionUpCount==101){ MenuLayer=202; PositionUpCount=140; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    if(PositionRightCount == 2 && PositionUpCount==102){ MenuLayer=203; PositionUpCount=160; }//Если курсор третяя строка и есть нажатие вправо - перейти в 2.1.3 //AdaptivBrightness   
    
    if(PositionUpCount==103){    MenuLayer=21;  } //При скролле вниз перейти на нижнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=0;  PositionUpCount=2; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 2
   
    OneRazGalochka2_1 = false;

    saveBlink_sensOnValue2_2=false;
    OneRazPosition2_2=false;

    OneRazGalochka2_3 = false;
}
if(MenuLayer == 21){
    if(PositionUpCount==103){   CirclY = 7;    }  //Перебираем ползунок
    if(PositionUpCount==104){   CirclY = 27;   }  //Перебираем ползунок
    if(PositionUpCount==105){   CirclY = 47;   }  //Перебираем ползунок
    if(PositionUpCount==106){   CirclY = 57;   }  //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount==103){ MenuLayer=204; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.4 
    if(PositionRightCount == 2 && PositionUpCount==104){ MenuLayer=205; PositionUpCount=180; }
    if(PositionRightCount == 2 && PositionUpCount==105){ MenuLayer=206; PositionUpCount=180; }

    if(PositionRightCount == 0){ MenuLayer=0;   PositionUpCount=2;   } //Если нажать влево то выйти в главное меню и переместить курсор на позицию 2
    if(PositionUpCount==102)   { MenuLayer=20;  }   //При скролле вверх перейти на верхнюю часть страницы
    if(PositionUpCount==106)   { MenuLayer=22;  } //При скролле вниз перейти на нижнюю часть страницы
   
    OneRazPosition2_4 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue2_4=false;
    OneRazPosition2_5 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue2_5=false;
    OneRazPosition2_6 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue2_6=false;
}
if(MenuLayer == 22){
    if(PositionUpCount==106){   CirclY = 7;     }  //Перебираем ползунок
    if(PositionUpCount==107){   CirclY = 35;    }  //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount == 106){ MenuLayer=207; PositionUpCount=180; }
    if(PositionRightCount == 2 && PositionUpCount == 107){ MenuLayer=208; PositionUpCount=140; }

    if(PositionRightCount == 0){ MenuLayer=0;   PositionUpCount=2;   }  //Если нажать влево то выйти в главное меню и переместить курсор на позицию 2
    if(PositionUpCount==105){    MenuLayer=21;  }  //При скролле вверх перейти на верхнюю часть страницы
    if(PositionUpCount==108){    MenuLayer=23;  } //При скролле вниз 

    OneRazPosition2_7 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue2_7=false;
    OneRazPosition2_8 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue2_8=false;
    OneRazPosition2_9 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue2_9=false;
}
if(MenuLayer == 23){
    if(PositionUpCount==108){    CirclY = 7;     }  //Перебираем ползунок
    if(PositionUpCount==109){    CirclY = 37;    }  //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount == 108){ MenuLayer=209; PositionUpCount=140; }
    if(PositionRightCount == 2 && PositionUpCount == 109){ MenuLayer=210; PositionUpCount=180; }
    if(PositionUpCount==107){    MenuLayer=22;  }   //При скролле вверх перейти на верхнюю часть страницы
    if(PositionUpCount==110){    MenuLayer=24;  }   //При скролле ввниз перейти на нижнюю  часть страницы
    if(PositionRightCount == 0){ MenuLayer=0;   PositionUpCount=2;   }  //Если нажать влево то выйти в главное меню и переместить курсор на позицию 2
    OneRazPosition2_10 = false; // Сброс один раз исполнить
    saveBlink_SpeedFalling2_10=false;
    OneRazPosition2_11 = false; // Сброс один раз исполнить
    saveBlink_EcoBright2_11=false;
    //if(PositionUpCount > 111){PositionUpCount = 111;} // Ограничить вертикальный ползунок при движении вниз
}
if(MenuLayer == 24){
    if(PositionUpCount==110){    CirclY = 7;    }   //Перебираем ползунок
      if(PositionRightCount == 2 && PositionUpCount == 110){ MenuLayer=211; PositionUpCount=140; }

    if(PositionRightCount == 0){ MenuLayer=0;   PositionUpCount=2;   }  //Если нажать влево то выйти в главное меню и переместить курсор на позицию 2
    if(PositionUpCount == 109){    MenuLayer=23;  }   //При скролле вверх перейти на верхнюю часть страницы
    if(PositionUpCount > 110){PositionUpCount = 110;} // Ограничить вертикальный ползунок при движении вниз
} 

if(MenuLayer == 201){ // 2.1
    if(PositionRightCount ==1){ // back
        MenuLayer=20;PositionUpCount=100;
    }
    if(PositionRightCount ==3){ // save

        if(PositionUpCount ==120){
          TunL=true;
        }
        if(PositionUpCount ==121){
          TunL=false;
        }
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(0, TunL);
        EEPROM.commit();
        //address += sizeof(bool);

        saveBlink2_1=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
if(MenuLayer == 202){ // 2.2
    
     if(OneRazPosition2_2==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_sensOnValue = sensOnValue;
       //PositionUpCount=old_PositionUpCount2_2;
              // Исполнить один раз чтоб галочка соответствовала значению          
              if(sensOnValue==0){   PositionUpCount =140;    }
              if(sensOnValue==1){   PositionUpCount =139;    }
              if(sensOnValue==2){   PositionUpCount =138;    }
              if(sensOnValue==3){   PositionUpCount =137;    }
              if(sensOnValue==4){   PositionUpCount =136;    }
              if(sensOnValue==5){   PositionUpCount =135;    }
              // Исполнить один раз чтоб галочка соответствовала значению
              
              OneRazPosition2_2=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      //old_PositionUpCount2_2=PositionUpCount; // Постоянно присваивать в старое значение
      //                             137             
      sensOnValue=abs(    (140 -PositionUpCount   ));
      sensOnValue=constrain(sensOnValue,0,5);

      if(PositionUpCount<135){PositionUpCount=135;}  // Защита от выхода за диапазон
      if(PositionUpCount>140){PositionUpCount=140;}  // Защита от выхода за диапазон
      
      
      if(PositionRightCount ==3){ // save
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        //EEPROM.writeByte(2, abc);  
        EEPROM.writeByte(1,sensOnValue);                  // -2^7
        EEPROM.commit();
        //address += sizeof(byte);

        saveBlink_sensOnValue2_2=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
        saveBlink2_2=true; // Чтобы моргала надпись save
        PositionRightCount =2; // Вернуть ползунок по горизонтали
      }
      if(PositionRightCount ==1){ // back
          MenuLayer=20;PositionUpCount=101;
          if(saveBlink_sensOnValue2_2 != true){
              sensOnValue=old_sensOnValue;
          }
      }
     
}
if(MenuLayer == 203){ // 2.3
     if(PositionRightCount ==1){ // back
        MenuLayer=20;PositionUpCount=102;
     }
     if(PositionRightCount ==3){ // save
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        if(PositionUpCount ==160){
          AdaptivBrightness=true;
        }
        if(PositionUpCount ==161){
          AdaptivBrightness=false;
        }
        EEPROM.writeBool(2, AdaptivBrightness);
        EEPROM.commit();

        saveBlink2_3=true;
        PositionRightCount =2;
     }
}
if(MenuLayer == 204){ // 2.4
    if(OneRazPosition2_4==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_StartToAccelValue = StartToAccel;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(StartToAccel,0,99,-180,-279);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition2_4=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount2_4=PositionUpCount; // Постоянно присваивать в старое значение
      StartToAccel=abs(    (180 + PositionUpCount)   );   
  
      // Km/h  PositionUpCount
      // 0        -180
      // 99       -81
       StartToAccel=constrain(StartToAccel,0,99);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-279){PositionUpCount=-279;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeByte(3, StartToAccel);
          EEPROM.commit();

          saveBlink_sensOnValue2_4=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink2_4=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back

       if(saveBlink_sensOnValue2_4 != true){
          StartToAccel=old_StartToAccelValue;
       }

          MenuLayer=21;PositionUpCount=103;
    }
}
if(MenuLayer == 205){ // 2.5
    if(OneRazPosition2_5==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_StopToAccelValue = StopToAccel;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(StopToAccel,0,99,-180,-279);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition2_5=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount2_5=PositionUpCount; // Постоянно присваивать в старое значение
      StopToAccel=abs(    (180 + PositionUpCount)   );
    
       StopToAccel=constrain(StopToAccel,0,99);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-279){PositionUpCount=-279;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeByte(4, StopToAccel);
          EEPROM.commit();

          saveBlink_sensOnValue2_5=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink2_5=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue2_5 != true){
              StopToAccel=old_StopToAccelValue;
          }
          MenuLayer=21;PositionUpCount=104;
       }
}
if(MenuLayer == 206){ // 2.6 StartPersentBright
    if(OneRazPosition2_6==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_StartPersentBright = StartPersentBright;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(StartPersentBright,0,100,-180,-280);
       // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition2_6=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount2_6=PositionUpCount; // Постоянно присваивать в старое значение
      StartPersentBright=abs(    (180 + PositionUpCount)   );
    
       StartPersentBright=constrain(StartPersentBright,0,100);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        
        EEPROM.writeByte(5,StartPersentBright);//StopPersentBright
        EEPROM.commit();
          saveBlink_sensOnValue2_6=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink2_6=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue2_6 != true){
              StartPersentBright=old_StartPersentBright;
          }
       MenuLayer=21;PositionUpCount=105;
       }
}
if(MenuLayer == 207){ // 2.7 Stop % Bright.
    if(OneRazPosition2_7==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_StopPersentBright = StopPersentBright;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(StopPersentBright,0,100,-180,-280);
       // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition2_7=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount2_7=PositionUpCount; // Постоянно присваивать в старое значение
      StopPersentBright=abs(    (180 + PositionUpCount)   );
    
       StopPersentBright=constrain(StopPersentBright,0,100);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
         EEPROM.writeByte(6,StopPersentBright);//
         EEPROM.commit();
          saveBlink_sensOnValue2_7=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink2_7=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue2_7 != true){
              StopPersentBright=old_StopPersentBright;
          }
       MenuLayer=22;PositionUpCount=106;
       }
}
if(MenuLayer == 208){ // 2.8 Delay fall bright
    
     if(OneRazPosition2_8==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_DelayFallingBright = DelayFallingBright;
       //PositionUpCount=old_PositionUpCount2_8;
              // Исполнить один раз чтоб галочка соответствовала значению          
              if(DelayFallingBright==0){   PositionUpCount =140;    }
              if(DelayFallingBright==1){   PositionUpCount =139;    }
              if(DelayFallingBright==2){   PositionUpCount =138;    }
              if(DelayFallingBright==3){   PositionUpCount =137;    }
              if(DelayFallingBright==4){   PositionUpCount =136;    }
              if(DelayFallingBright==5){   PositionUpCount =135;    }
              if(DelayFallingBright==6){   PositionUpCount =134;    }
              if(DelayFallingBright==7){   PositionUpCount =133;    }
              if(DelayFallingBright==8){   PositionUpCount =132;    }
              if(DelayFallingBright==9){   PositionUpCount =131;    }
              if(DelayFallingBright==10){   PositionUpCount =130;    }
              // Исполнить один раз чтоб галочка соответствовала значению
              
              OneRazPosition2_8=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      //old_PositionUpCount2_8=PositionUpCount; // Постоянно присваивать в старое значение
      //                             137             
      DelayFallingBright=abs(    (140 -PositionUpCount   ));
      DelayFallingBright=constrain(DelayFallingBright,0,10);

      if(PositionUpCount<130){PositionUpCount=130;}  // Защита от выхода за диапазон
      if(PositionUpCount>140){PositionUpCount=140;}  // Защита от выхода за диапазон
      
      
      if(PositionRightCount ==3){ // save
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
         EEPROM.writeByte(7,DelayFallingBright);// 
         EEPROM.commit();

        saveBlink_sensOnValue2_8=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
        saveBlink2_8=true; // Чтобы моргала надпись save
        PositionRightCount =2; // Вернуть ползунок по горизонтали
      }
      if(PositionRightCount ==1){ // back
          MenuLayer=22;PositionUpCount=107;
          if(saveBlink_sensOnValue2_8 != true){
              DelayFallingBright=old_DelayFallingBright;
          }
      }
     
}
if(MenuLayer == 209){ // 2.9 BrightWhenIdle
    if(OneRazPosition2_9==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
        old_BrightWhenIdle = BrightWhenIdle;
       
        // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(BrightWhenIdle,0,100,-180,-280);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition2_9=true;
    }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
    old_PositionUpCount2_9=PositionUpCount; // Постоянно присваивать в старое значение
    BrightWhenIdle=abs(    (180 + PositionUpCount)   );
    
    StopPersentBright=constrain(StopPersentBright,0,100);
    if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
    if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

    if(PositionRightCount ==3){ // save
    //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
    EEPROM.writeByte(8,BrightWhenIdle);// BrightWhenIdle
    EEPROM.commit();

    saveBlink_sensOnValue2_9=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
    saveBlink2_9=true; // Чтобы моргала надпись save
    PositionRightCount =2; // Вернуть ползунок по горизонтали
    }
    if(PositionRightCount ==1){ // back
        if(saveBlink_sensOnValue2_9 != true){
            BrightWhenIdle=old_BrightWhenIdle;
        }
    MenuLayer=23;PositionUpCount=108;
    }
}
if(MenuLayer == 210){ // 2.10 Delay fallin bright
    
     if(OneRazPosition2_10==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_SpeedFalling = SpeedFalling;
       PositionUpCount=old_PositionUpCount2_10;
              // Исполнить один раз чтоб галочка соответствовала значению          
              if(SpeedFalling==0){   PositionUpCount =140;    }
              if(SpeedFalling==1){   PositionUpCount =139;    }
              if(SpeedFalling==2){   PositionUpCount =138;    }
              if(SpeedFalling==3){   PositionUpCount =137;    }
              //if(SpeedFalling==4){   PositionUpCount =136;    }
              //if(SpeedFalling==5){   PositionUpCount =135;    }
              // Исполнить один раз чтоб галочка соответствовала значению
              
              OneRazPosition2_10=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount2_10=PositionUpCount; // Постоянно присваивать в старое значение
      //                             137             
      SpeedFalling=abs(    (140 -PositionUpCount   ));
      SpeedFalling=constrain(SpeedFalling,0,3);

      if(PositionUpCount<137){PositionUpCount=137;}  // Защита от выхода за диапазон
      if(PositionUpCount>140){PositionUpCount=140;}  // Защита от выхода за диапазон
      
      if(PositionRightCount ==3){ // save
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
       EEPROM.writeByte(9,SpeedFalling);// BrightWhenIdle
       EEPROM.commit();
        saveBlink_SpeedFalling2_10=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
        saveBlink2_10=true; // Чтобы моргала надпись save
        PositionRightCount =2; // Вернуть ползунок по горизонтали
      }
      if(PositionRightCount ==1){ // back
          MenuLayer=23;PositionUpCount=109;
          if(saveBlink_SpeedFalling2_10 != true){
              SpeedFalling=old_SpeedFalling;
          }
      }
     
}
if(MenuLayer == 211){ // 2.11 Eco bright
    if(OneRazPosition2_11==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
        old_EcoBright = EcoBright;
       
        // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(EcoBright,0,100,-180,-280);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition2_11=true;
    }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
    old_PositionUpCount2_11=PositionUpCount; // Постоянно присваивать в старое значение
    EcoBright=abs(    (180 + PositionUpCount)   );
    EcoBright=constrain(EcoBright,0,100);

    if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
    if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

    if(PositionRightCount ==3){ // save
    //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
      EEPROM.writeByte(10,EcoBright);// BrightWhenIdle
      EEPROM.commit();

    saveBlink_EcoBright2_11=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
    saveBlink2_11=true; // Чтобы моргала надпись save
    PositionRightCount =2; // Вернуть ползунок по горизонтали
    }
    if(PositionRightCount ==1){ // back
        if(saveBlink_EcoBright2_11 != true){
            EcoBright=old_EcoBright;
        }
    MenuLayer=24;PositionUpCount=110;
    }
}
// Перебираем вкладку 2

// Перебираем вкладку 3
if(MenuLayer == 30){ //Если в подменю 2.1 Нажата кнопка влево то выйти в главное меню
if(PositionUpCount < 100){PositionUpCount = 100;} // Ограничить вертикальный ползунок при движении вверх

    if(PositionUpCount==100){   CirclY = 20-1;   }  //TunL  //Перебираем ползунок
    if(PositionUpCount==101){   CirclY = 45-1;   }          //Перебираем ползунок
    //if(PositionUpCount==102){   CirclY = 40-1;   }        //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount==100){ MenuLayer=301; PositionUpCount=120; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.11 //TunL
    if(PositionRightCount == 2 && PositionUpCount==101){ MenuLayer=3011; PositionUpCount=140; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    //if(PositionRightCount == 2 && PositionUpCount==102){ MenuLayer=303; PositionUpCount=160; }//Если курсор третяя строка и есть нажатие вправо - перейти в 2.1.3 //AdaptivBrightness   
    
    if(PositionUpCount==102){    MenuLayer=31;  } //При скролле вниз перейти на нижнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=0;  PositionUpCount=3; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
   
    OneRazGalochka3_1 = false;
    
    saveBlink_sensOnValue3_11=false;
    OneRazPosition3_11=false;

    //OneRazGalochka3_3 = false;
}
if(MenuLayer == 31){ //Если в подменю 3
    if(PositionUpCount==102){   CirclY = 7;   }  //TunL  //Перебираем ползунок
    if(PositionUpCount==103){   CirclY = 37;   }          //Перебираем ползунок
    //if(PositionUpCount==105){   CirclY = 40-1;   }          //Перебираем ползунок

    //if(PositionRightCount == 2 && PositionUpCount==100){ MenuLayer=304; PositionUpCount=120; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.11 //TunL
    if(PositionRightCount == 2 && PositionUpCount==102){ MenuLayer=302; PositionUpCount=140; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    if(PositionRightCount == 2 && PositionUpCount==103){ MenuLayer=3021; PositionUpCount=160; }//Если курсор третяя строка и есть нажатие вправо - перейти в 2.1.3 //AdaptivBrightness   
    
    if(PositionUpCount==101){    MenuLayer=30;  } //При скролле вниз перейти на нижнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=0;  PositionUpCount=3; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
   
    OneRazGalochka3_1 = false;
    OneRazGalochka3_2  =false;
    saveBlink_sensOnValue3_21=false;
    OneRazPosition3_21=false;
    if(PositionUpCount > 103){PositionUpCount = 103;} // Ограничить вертикальный ползунок при движении вниз
    //saveBlink_sensOnValue3_2=false;
    //OneRazPosition3_2=false;

    //OneRazGalochka3_3 = false;
}
if(MenuLayer == 301){ // 3.1
    if(PositionRightCount ==1){ // back
        MenuLayer=30;PositionUpCount=100;
    }
    if(PositionRightCount ==3){ // save
        if(PositionUpCount ==120){
          VolumeOnSpeed=true;
        }
        if(PositionUpCount ==121){
          VolumeOnSpeed=false;
        }
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(11, VolumeOnSpeed);
        EEPROM.commit();
        //EEPROM.writeBool(0, VolumeOnSpeed);
        //EEPROM.commit();
        //address += sizeof(bool);

        saveBlink3_1=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
if(MenuLayer == 3011){ // 3.11
    if(OneRazPosition3_11==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_SettingMaxVolumeOnSpeed = SettingMaxVolumeOnSpeed;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(SettingMaxVolumeOnSpeed,0,99,-180,-279);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition3_11=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      //old_SettingMaxVolumeOnSpeed=SettingMaxVolumeOnSpeed; // Постоянно присваивать в старое значение
      SettingMaxVolumeOnSpeed=abs(    (180 + PositionUpCount)   );   
  
      // Km/h  PositionUpCount
      // 0        -180
      // 99       -81
       SettingMaxVolumeOnSpeed=constrain(SettingMaxVolumeOnSpeed,0,99);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-279){PositionUpCount=-279;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeByte(12, SettingMaxVolumeOnSpeed);
          EEPROM.commit();

          saveBlink_sensOnValue3_11=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink3_11=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back

       if(saveBlink_sensOnValue3_11 != true){
          SettingMaxVolumeOnSpeed=old_SettingMaxVolumeOnSpeed;
       }
          MenuLayer=30;PositionUpCount=101;
    }
}
if(MenuLayer == 302){ // 3.2
    if(PositionRightCount ==1){ // back
        MenuLayer=31;PositionUpCount=102;
    }
    if(PositionRightCount ==3){ // save
        if(PositionUpCount ==120){
          VolumeTimePressed=true;
        }
        if(PositionUpCount ==121){
          VolumeTimePressed=false;
        }
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(13, VolumeTimePressed);
        EEPROM.commit();
        //EEPROM.writeBool(0, VolumeOnSpeed);
        //EEPROM.commit();
        //address += sizeof(bool);

        saveBlink3_2=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
if(MenuLayer == 3021){ // 3.21
     if(OneRazPosition3_21==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_SettingTimePresseMax = SettingTimePresseMax;
       //PositionUpCount=old_PositionUpCount3_21;
              // Исполнить один раз чтоб галочка соответствовала значению          
              if(SettingTimePresseMax==0){   PositionUpCount =140;    }
              if(SettingTimePresseMax==1){   PositionUpCount =139;    }
              if(SettingTimePresseMax==2){   PositionUpCount =138;    }
              if(SettingTimePresseMax==3){   PositionUpCount =137;    }
              if(SettingTimePresseMax==4){   PositionUpCount =136;    }
              if(SettingTimePresseMax==5){   PositionUpCount =135;    }
              if(SettingTimePresseMax==6){   PositionUpCount =134;    }
              if(SettingTimePresseMax==7){   PositionUpCount =133;    }
              if(SettingTimePresseMax==8){   PositionUpCount =132;    }
              if(SettingTimePresseMax==9){   PositionUpCount =131;    }
              if(SettingTimePresseMax==10){   PositionUpCount =130;    }
              // Исполнить один раз чтоб галочка соответствовала значению
              
              OneRazPosition3_21=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      //old_PositionUpCount3_21=PositionUpCount; // Постоянно присваивать в старое значение
      //                             137             
      SettingTimePresseMax=abs(    (140 -PositionUpCount   ));
      SettingTimePresseMax=constrain(SettingTimePresseMax,0,10);

      if(PositionUpCount<130){PositionUpCount=130;}  // Защита от выхода за диапазон
      if(PositionUpCount>140){PositionUpCount=140;}  // Защита от выхода за диапазон
      
      
      if(PositionRightCount ==3){ // save
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
         EEPROM.writeByte(14,SettingTimePresseMax);// 
         EEPROM.commit();

        saveBlink_sensOnValue3_21=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
        saveBlink3_21=true; // Чтобы моргала надпись save
        PositionRightCount =2; // Вернуть ползунок по горизонтали
      }
      if(PositionRightCount ==1){ // back
          MenuLayer=31;PositionUpCount=103;
          if(saveBlink_sensOnValue3_21 != true){
              SettingTimePresseMax=old_SettingTimePresseMax;
          }
      }
}
// Перебираем вкладку 3
// Отрисовка меню
  if (MenuLayer == -1){
      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_7x14B_tr);	
      u8g2.drawStr(20, 35-3, "Glavnij Ekran");  //u8g2.drawStr(35+10, 40-3, "Off");
      u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 0 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(45, 7, " MENU"); // write something to the internal memory
    u8g2.drawStr(5, 25-3, "1 Povorotniki"); // write something to the internal memory
    u8g2.drawStr(5, 35-3, "2 Passing lights"); // write something to the internal memory
    u8g2.drawStr(5, 45-3, "3 Buzzer");
    u8g2.drawStr(5, 55-3, "4 Punkt menu"); // write something to the internal memory
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5); 
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 1 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    
    u8g2.drawStr(5, 10,   "5 Punkt menu"); // write something to the internal memory
    u8g2.drawStr(5, 25-3, "6 Punkt menu");// write something to the internal memory
    u8g2.drawStr(5, 35-3, "Exit ");
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 10 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(20, 7, " 1 Povorotniki"); // write something to the internal memory
    u8g2.drawStr(0, 22, "1.1 Speed  "); // write something to the internal memory
                            u8g2.setCursor(95,22);  
                            u8g2.print(float(SpeedPovorotnikBlink)/10 ,1);
    u8g2.drawStr(0, 32, "Blink"); // write something to the internal 
    
    u8g2.drawLine(0, 32+5, 105, 32+5);

    //u8g2.drawStr(0, 32+17, "1.2 PodPunkt menu");
    u8g2.drawStr(0, 32+17, "1.2 Intelllig");
    if(IntelligentMode == true){      u8g2.drawStr(95, 32+17, "On");   }
    else                       {      u8g2.drawStr(95, 32+17, "Off");  }
    u8g2.drawStr(0, 32+17+10, "mode"); // write something to the internal memory u8g2.print(float(SpeedPovorotnikBlink)/10 ,1); 
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 11 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    //u8g2.drawStr(20, 7, " 1 Povorotniki"); // write something to the internal memory
    u8g2.drawStr(0, 12, "1.3 Count Blink"); // write something to the internal memory
                            u8g2.setCursor(95,12);  
                            u8g2.print(CountBlinkIntMode);
    u8g2.drawStr(0, 22, "Intelllig Mode"); // write something to the internal

    u8g2.drawLine(0,27, 105, 27);

    //u8g2.drawStr(0, 32+17, "1.2 PodPunkt menu");
    u8g2.drawStr(0, 39, "1.4 Automatic");
    if(AutomaticMode == true){      u8g2.drawStr(95, 39, "On");   }
    else                     {      u8g2.drawStr(95, 39, "Off");  }
    u8g2.drawStr(0, 49, "mode"); // write something to the internal memory u8g2.print(float(SpeedPovorotnikBlink)/10 ,1); 
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
  }  
  if (MenuLayer == 12 ) {
    u8g2.clearBuffer();          // clear the internal memory
    
    u8g2.setFont(u8g2_font_6x12_tr);
    //u8g2.drawStr(20, 7, " 1 Povorotniki"); // write something to the internal memory
    u8g2.drawStr(0, 12, "1.5 Time Press"); // write something to the internal memory TimePressToOnAutoMode
                            u8g2.setCursor(95,12);  
                            u8g2.print(float(TimePressToOnAutoMode)/10 ,1);
    u8g2.drawStr(0, 22, "To On Auto Mode"); // write something to the internal
    /*
    u8g2.drawLine(0,27, 105, 27);

    //u8g2.drawStr(0, 32+17, "1.2 PodPunkt menu");
    u8g2.drawStr(0, 39, "1.4 Automatic");
    if(AutomaticMode == true){      u8g2.drawStr(95, 39, "On");   }
    else                     {      u8g2.drawStr(95, 39, "Off");  }
    u8g2.drawStr(0, 49, "mode"); // write something to the internal memory u8g2.print(float(SpeedPovorotnikBlink)/10 ,1); 
    */
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 101 ) { // 1.1
      if(saveBlink1_1==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(15, 7,   "1.1 Speed Blink "); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.drawStr(0, 35,   "Times/sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(float(SpeedPovorotnikBlink)/10 ,1);         

          u8g2.drawTriangle(85+8,28, 88+8,16, 91+8,28);
          u8g2.drawTriangle(85+8,32, 88+8,44, 91+8,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "1 - 3");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink1_1();
      }  
  }
  if (MenuLayer == 102 ) { // 1.2
      if(saveBlink1_2==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "1.2 Intelligent Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_2==false){
              if(IntelligentMode==true) { PositionUpCount=120; }
              if(IntelligentMode==false){ PositionUpCount=121; }
          OneRazGalochka1_2=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
        SaveBlink1_2();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  }
  if (MenuLayer == 103 ) { // 1.3
      if(saveBlink1_3==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(15, 7,   "1.3 Count Blink "); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.drawStr(0, 35,   "Count blink:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60+12,35);  u8g2.print(CountBlinkIntMode);         

          u8g2.drawTriangle(85-2+12,28, 88-2+12,16, 91-2+12,28);
          u8g2.drawTriangle(85-2+12,32, 88-2+12,44, 91-2+12,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "1 - 30");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink1_3();
      }  
  }
  if (MenuLayer == 104 ) { // 1.4
      if(saveBlink1_4==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "1.4 Automatic Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_4==false){
              if(AutomaticMode==true) { PositionUpCount=120; }
              if(AutomaticMode==false){ PositionUpCount=121; }
          OneRazGalochka1_4=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
        SaveBlink1_4();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  }
  if (MenuLayer == 105 ) { // 1.5
      if(saveBlink1_5==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          u8g2.drawStr(20, 7,   "1.5 Time Press"); // write something to the internal memory
          
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(TimePressToOnAutoMode)/10 ,1);         

          u8g2.drawTriangle(85,28, 88,16, 91,28);
          u8g2.drawTriangle(85,32, 88,44, 91,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(36, 60, "0 - 5 sec");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink1_5();
      }  
  }
  if (MenuLayer == 20 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(15, 7, " 2 Passing lights "); // write something to the internal memory
    u8g2.drawStr(0, 22, "2.1 TunL"); // write something to the internal memory 
    if(TunL == true){      u8g2.drawStr(95, 25-3, "On");   }
    else            {      u8g2.drawStr(95, 25-3, "Off");  }
    
    u8g2.drawStr(0, 32, "2.2 senOn"); // write something to the internal memory
                           u8g2.setCursor(95,35-3);  
                           u8g2.print(sensOnValue);
    
    u8g2.drawStr(0, 42, "2.3 Adaptiv ");u8g2.drawStr(0, 55-3, "brightness");
    if(AdaptivBrightness == true){      u8g2.drawStr(95, 45-3, "On");   }
    else                         {      u8g2.drawStr(95, 45-3, "Off");  }
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 21 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "2.4 Start accel "); // write something to the internal memory
    //u8g2.drawLine(0, 12+5, 105, 27);
    //u8g2.drawStr(5, 22, " accel");
                           u8g2.setCursor(95,12);  
                           u8g2.print(StartToAccel);
    u8g2.drawLine(0, 12+6, 105,12+6);
    u8g2.drawStr(0, 32, "2.5 Stop accel"); // write something to the internal memory 
    //u8g2.drawStr(5, 42, "to accel"); // write something to the internal memory
                           u8g2.setCursor(95,32);  
                           u8g2.print(StopToAccel);
    u8g2.drawLine(0, 32+6, 105,32+6);
    u8g2.drawStr(0, 52, "2.6 Start %");//u8g2.drawStr(5, 55-3, "brightness");
    u8g2.drawStr(0, 62, "brightness");
                           u8g2.setCursor(95,55-3);  
                           u8g2.print(StartPersentBright);
    // u8g2.drawStr(5, 65-3, "2.7 Stop  % Bright.");
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
    
  }
  if (MenuLayer == 22 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "2.7 Stop %"); // write something to the internal memory
    u8g2.drawStr(0, 22, "brightness");
                           u8g2.setCursor(95,12);  
                           u8g2.print(StopPersentBright);
    u8g2.drawLine(0,27, 105, 27);
    u8g2.drawStr(0, 39, "2.8 Delay "); // write something to the internal memory 
    u8g2.drawStr(0, 49, "fall bright.");
                           u8g2.setCursor(95,39);  
                           u8g2.print(DelayFallingBright);
                           /*
    u8g2.drawStr(0, 52, "2.9 bright."); // write something to the internal memory 
    u8g2.drawStr(0, 62, "when idle"); // write something to the internal memory
                           u8g2.setCursor(95,55-3);  
                           u8g2.print(BrightWhenIdle);
    */
   
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
    
  }
  if (MenuLayer == 23 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "2.9 bright."); // write something to the internal memory 
    u8g2.drawStr(0, 22, "when idle"); // write something to the internal memory
                           u8g2.setCursor(95,12);  
                           u8g2.print(BrightWhenIdle);
    u8g2.drawLine(0,27, 105, 27);
    u8g2.drawStr(0, 39, "2.10 Speed");//u8g2.drawStr(5, 55-3, "brightness");
    u8g2.drawStr(0, 49, "falling");
                          u8g2.setCursor(95,39);  
                          u8g2.print(SpeedFalling);
                          /*
    u8g2.drawStr(0, 35-3, "2.11 ECO"); // write something to the internal memory 
                          u8g2.setCursor(95,35-3);  
                          u8g2.print(EcoBright);
    u8g2.drawStr(0, 45-3, "brightness");          
    */             
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
    
  }
  if (MenuLayer == 24 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 15-3, "2.11 ECO");//u8g2.drawStr(5, 55-3, "brightness");
    u8g2.drawStr(0, 25-3, "brightness");
                          u8g2.setCursor(95,15-3);  
                          u8g2.print(EcoBright);
    /*
    u8g2.drawStr(0, 35-3, "2.11 ECO"); // write something to the internal memory 
                          u8g2.setCursor(95,35-3);  
                          u8g2.print(EcoBright);
    u8g2.drawStr(0, 45-3, "brightness");
    */                       
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
    
  } 
   
  if (MenuLayer == 201 ) { // 2.1
    
      if(saveBlink2_1==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(45, 7,   "2.1 TunL "); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka2_1==false){
              if(TunL==true) { PositionUpCount=120; }
              if(TunL==false){ PositionUpCount=121; }
          OneRazGalochka2_1=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
        SaveBlink2_1();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  }
  if (MenuLayer == 202 ) { // 2.2
      if(saveBlink2_2==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(40, 7,   "2.2 sensOn "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "sensOn val: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(70,35);  u8g2.print(sensOnValue);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(48, 60, "0 - 5");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      
      else{
          SaveBlink2_2();
        
      }
      
  }
  if (MenuLayer == 203 ) { // 2.3
      if(saveBlink2_3==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(10, 7,   "2.3 Adaptiv Bright."); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka2_3==false){
              if(AdaptivBrightness==true) { PositionUpCount=160; }
              if(AdaptivBrightness==false){ PositionUpCount=161; }
          OneRazGalochka2_3=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==160){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==161){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink2_3();       
      }
      PositionUpCount=constrain(PositionUpCount,160,161); // Ограничить движение галочки вверх вниз
  }
  if (MenuLayer == 204 ) { // 2.4
      if(saveBlink2_4==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(20, 7,   "2.4 Start accel "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(StartToAccel);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink2_4();
      }  
  }
  if (MenuLayer == 205 ) { // 2.5
      if(saveBlink2_5==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(20, 7,   "2.5 Stop accel "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(StopToAccel);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink2_5();
      }  
  }
  if (MenuLayer == 206 ) { // 2.6
      if(saveBlink2_6==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(10, 7,   "2.6 Start % Bright. "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(StartPersentBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink2_6();
      }  
  }
  if (MenuLayer == 207 ) { // 2.7
      if(saveBlink2_7==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(10, 7,   "2.7 Stop % Bright. "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(StopPersentBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink2_7();
      }  
  }
  if (MenuLayer == 208 ) { //// 2.8 Delay fall bright
      if(saveBlink2_8==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.8 Delay fall bright."); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Step val: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(DelayFallingBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 10");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      
      else{
          SaveBlink2_8();
        
      }
      
  }
  if (MenuLayer == 209 ) { // 2.9 Bright When Idle
      if(saveBlink2_9==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.9 Bright When Idle "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(BrightWhenIdle);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink2_9();
      }  
  }
  if (MenuLayer == 210 ) { // 2.10 Speed Falling.
      if(saveBlink2_10==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(5, 7,   "2.10 Speed Falling"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value step: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(67,35);  u8g2.print(SpeedFalling);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(48, 60, "0 - 3");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      
      else{
          SaveBlink2_10();
        
      }
      
  }
  if (MenuLayer == 211 ) { // 2.11 Eco bright
      if(saveBlink2_11==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.11 ECO bright"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(EcoBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink2_11();
      }  
  }  

  if (MenuLayer == 30 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(30, 7, " 3 Buzzer "); // write something to the internal memory
    u8g2.drawStr(0, 25-3, "3.1 Volume"); // write something to the internal memory 
    if(VolumeOnSpeed == true){      u8g2.drawStr(97, 25-3, "On");   }
    else                     {      u8g2.drawStr(97, 25-3, "Off");  }
    
    u8g2.drawStr(0, 32, "on speed"); // write something to the internal memory
                            
    u8g2.drawLine(0, 32+5, 105, 32+5);

    u8g2.drawStr(0, 32+17, "3.2 Set. Max"); 
    u8g2.drawStr(0, 42+17, "Volume on speed");
                           u8g2.setCursor(97,32+17);  
                           u8g2.print(SettingMaxVolumeOnSpeed);
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  if (MenuLayer == 31 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "3.3 Volume "); // write something to the internal memory 3.3 Volume On Time
    u8g2.drawStr(0, 22, "On Time");
    if(VolumeTimePressed == true){  u8g2.drawStr(97, 12, "On");   }
    else                         {  u8g2.drawStr(97, 12, "Off");  }
    
    u8g2.drawLine(0, 27, 105, 27);

    u8g2.drawStr(0, 39, "3.4 Set. time");
                           u8g2.setCursor(97,39);  
                           u8g2.print(SettingTimePresseMax);
    u8g2.drawStr(0, 49, "pressed Max"); // write something to the internal memory 
                           
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  
  if (MenuLayer == 301 )  { // 3.1
    
      if(saveBlink3_1==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "3.1 Volume on speed"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_1==false){
              if(VolumeOnSpeed==true) { PositionUpCount=120; }
              if(VolumeOnSpeed==false){ PositionUpCount=121; }
          OneRazGalochka3_1=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
        SaveBlink3_1();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  }
  if (MenuLayer == 3011 ) { // 3.11
      if(saveBlink3_11==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "3.2 Set. Max Volume"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(SettingMaxVolumeOnSpeed);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink3_11();
      }  
  }
  if (MenuLayer == 302 )  { // 3.2
      if(saveBlink3_2==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(0, 7, "3.3 Volume On Time"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_2==false){
              if(VolumeTimePressed==true) { PositionUpCount=120; }
              if(VolumeTimePressed==false){ PositionUpCount=121; }
          OneRazGalochka3_2=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
        SaveBlink3_2();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  }
  if (MenuLayer == 3021 ) { // 3.21
      if(saveBlink3_21==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "3.4 Set. time pressed"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Step val: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(SettingTimePresseMax);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(29, 60, "0 - 10 sec");
          
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      
      else{
          SaveBlink3_21();  
      }
  }
// Отрисовка меню

 }
}
void SaveBlink1_1(){ // Анимация моргания слова save в подпункте 2.3
  static int8_t counterSaveBlink1_1;
  static unsigned long timing1_1;  if (millis() - timing1_1 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink1_1++;
      timing1_1 = millis(); 
   }
   if(counterSaveBlink1_1 == 1){
        u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(15, 7,   "1.1 Speed Blink "); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.drawStr(0, 35,   "Times/sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(float(SpeedPovorotnikBlink)/10 ,1);        

          u8g2.drawTriangle(85+8,28, 88+8,16, 91+8,28);
          u8g2.drawTriangle(85+8,32, 88+8,44, 91+8,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "1 - 3");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink1_1 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(15, 7,   "1.1 Speed Blink "); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.drawStr(0, 35,   "Times/sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(float(SpeedPovorotnikBlink)/10 ,1);         

          u8g2.drawTriangle(85+8,28, 88+8,16, 91+8,28);
          u8g2.drawTriangle(85+8,32, 88+8,44, 91+8,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "1 - 3");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink1_1=false; // Этот буль отключает исполнение функции SaveBlink1_1(); 
          counterSaveBlink1_1=0;
   }
}
void SaveBlink1_2(){ // Анимация моргания слова save в подпункте 2.1
  static int8_t counterSaveBlink1_2;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink1_2++;
      timing = millis(); 
   }
   if(counterSaveBlink1_2 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "1.2 Intelligent Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_2==false){
              if(IntelligentMode==true) { PositionUpCount=120; }
              if(IntelligentMode==false){ PositionUpCount=121; }
          OneRazGalochka1_2=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink1_2 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "1.2 Intelligent Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_2==false){
              if(IntelligentMode==true) { PositionUpCount=120; }
              if(IntelligentMode==false){ PositionUpCount=121; }
          OneRazGalochka1_2=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink1_2=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink1_2=0;
   }
          
}
void SaveBlink1_3(){ // Анимация моргания слова save в подпункте 2.1
  static int8_t counterSaveBlink1_3;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink1_3++;
      timing = millis(); 
   }
   if(counterSaveBlink1_3 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(15, 7,   "1.3 Count Blink "); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.drawStr(0, 35,   "Count blink:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60+12,35);  u8g2.print(CountBlinkIntMode);         

          u8g2.drawTriangle(85-2+12,28, 88-2+12,16, 91-2+12,28);
          u8g2.drawTriangle(85-2+12,32, 88-2+12,44, 91-2+12,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "1 - 30");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink1_3 == 2){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(15, 7,   "1.3 Count Blink "); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.drawStr(0, 35,   "Count blink:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60+12,35);  u8g2.print(CountBlinkIntMode);         

          u8g2.drawTriangle(85-2+12,28, 88-2+12,16, 91-2+12,28);
          u8g2.drawTriangle(85-2+12,32, 88-2+12,44, 91-2+12,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "1 - 30");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink1_3=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink1_3=0;
   }
          
}
void SaveBlink1_4(){ // Анимация моргания слова save в подпункте 1.4
  static int8_t counterSaveBlink1_4;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink1_4++;
      timing = millis(); 
   }
   if(counterSaveBlink1_4 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7, "1.4 Automatic Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_4==false){
              if(AutomaticMode==true) { PositionUpCount=120; }
              if(AutomaticMode==false){ PositionUpCount=121; }
          OneRazGalochka1_4=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink1_4 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7, "1.4 Automatic Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
           if(OneRazGalochka1_4==false){
              if(AutomaticMode==true) { PositionUpCount=120; }
              if(AutomaticMode==false){ PositionUpCount=121; }
          OneRazGalochka1_4=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink1_4=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink1_4=0;
   }
          
}
void SaveBlink1_5(){ // Анимация моргания слова save в подпункте 1.5
  static int8_t counterSaveBlink1_5;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink1_5++;
      timing = millis(); 
   }
   if(counterSaveBlink1_5 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
      u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
      u8g2.drawStr(20, 7,   "1.5 Time Press"); // write something to the internal memory
          
      u8g2.drawStr(20, 35,  "Sec:");
      u8g2.setFont(u8g2_font_10x20_tr);	      
    
      u8g2.setCursor(60-10,35);  u8g2.print(float(TimePressToOnAutoMode)/10 ,1);         

      u8g2.drawTriangle(85,28, 88,16, 91,28);
      u8g2.drawTriangle(85,32, 88,44, 91,32);
       
      u8g2.setFont(u8g2_font_7x14_tr);	

      u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
      u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

      u8g2.setFont(u8g2_font_7x14_tf);
      u8g2.drawStr(36, 60, "0 - 5 sec");

      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(105, 50, "    "); 
      u8g2.drawStr(0,   50, "back"); 

      u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink1_5 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          u8g2.drawStr(20, 7,   "1.5 Time Press"); // write something to the internal memory
          
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(TimePressToOnAutoMode)/10 ,1);         

          u8g2.drawTriangle(85,28, 88,16, 91,28);
          u8g2.drawTriangle(85,32, 88,44, 91,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(36, 60, "0 - 5 sec");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink1_5=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink1_5=0;
   }
          
}

void SaveBlink3_21(){
  static int8_t counterSaveBlink3_21;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink3_21++;
      timing = millis(); 
   }
   if(counterSaveBlink3_21 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "3.4 Set. time pressed"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Step val: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(SettingTimePresseMax);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(29, 60, "0 - 10 sec");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display  
   }
          
   if(counterSaveBlink3_21 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
      u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
      u8g2.drawStr(0, 7,   "3.4 Set. time pressed"); // write something to the internal memory
          
      u8g2.drawStr(0, 35,   "Step val: ");
      u8g2.setFont(u8g2_font_10x20_tr);	      
    
      u8g2.setCursor(60,35);  u8g2.print(SettingTimePresseMax);         

      u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
      u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
      u8g2.setFont(u8g2_font_7x14_tr);	

      u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
      u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
      u8g2.setFont(u8g2_font_7x14_tf);
      u8g2.drawStr(29, 60, "0 - 10 sec");

      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(105, 50, "save"); 
      u8g2.drawStr(0, 50, "back"); 
      
      u8g2.sendBuffer();          // transfer internal memory to the display

      saveBlink3_21=false; // Этот буль отключает исполнение функции SaveBlink3_11(); 
      counterSaveBlink3_21=0;
   }
}

void SaveBlink3_2(){
  static int8_t counterSaveBlink3_2;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink3_2++;
      timing = millis(); 
   }
   if(counterSaveBlink3_2 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(0, 7,   "3.3 Volume On Time"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_2==false){
              if(VolumeTimePressed==true) { PositionUpCount=120; }
              if(VolumeTimePressed==false){ PositionUpCount=121; }
          OneRazGalochka3_2=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display   
   }
          
   if(counterSaveBlink3_2 == 2){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(0, 7,   "3.3 Volume On Time"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_2==false){
              if(VolumeTimePressed==true) { PositionUpCount=120; }
              if(VolumeTimePressed==false){ PositionUpCount=121; }
          OneRazGalochka3_2=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display   
          saveBlink3_2=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink3_2=0;
   }
}

void SaveBlink3_11(){
  static int8_t counterSaveBlink3_11;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink3_11++;
      timing = millis(); 
   }
   if(counterSaveBlink3_11 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
      u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	u8g2.drawStr(0, 7,   "3.2 Set. Max Volume");
      u8g2.drawStr(0, 7,   "3.2 Set. Max Volume"); // write something to the internal memory
          
      u8g2.drawStr(0, 35,   "Val KM/h: ");
      u8g2.setFont(u8g2_font_10x20_tr);	      
    
      u8g2.setCursor(60,35);  u8g2.print(SettingMaxVolumeOnSpeed);         

      u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
      u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
      u8g2.setFont(u8g2_font_7x14_tr);	

      u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
      u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

      u8g2.setFont(u8g2_font_7x14_tf);
      u8g2.drawStr(46, 60, "0 - 99");

      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(105, 50, "    "); 
      u8g2.drawStr(0,   50, "back"); 

      u8g2.sendBuffer();   
   }
          
   if(counterSaveBlink3_11 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "3.2 Set. Max Volume"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(SettingMaxVolumeOnSpeed);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();   
          saveBlink3_11=false; // Этот буль отключает исполнение функции SaveBlink3_11(); 
          counterSaveBlink3_11=0;
   }
}

void SaveBlink3_1(){
  static int8_t counterSaveBlink3_1;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink3_1++;
      timing = millis(); 
   }
   if(counterSaveBlink3_1 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "3.1 Volume on speed"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_1==false){
              if(VolumeOnSpeed==true) { PositionUpCount=120; }
              if(VolumeOnSpeed==false){ PositionUpCount=121; }
          OneRazGalochka3_1=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back");

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");
      
          u8g2.sendBuffer();         // transfer internal memory to the display
   }
          
   if(counterSaveBlink3_1 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "3.1 Volume on speed"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_1==false){
              if(VolumeOnSpeed==true) { PositionUpCount=120; }
              if(VolumeOnSpeed==false){ PositionUpCount=121; }
          OneRazGalochka3_1=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");
      
          u8g2.sendBuffer();
          saveBlink3_1=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink3_1=0;
   }
}

void SaveBlink2_11(){ // Анимация моргания слова save в подпункте 2.11
  static int8_t counterSaveBlink2_11;
  static unsigned long timing2_11;  if (millis() - timing2_11 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_11++;
      timing2_11 = millis(); 
   }
   if(counterSaveBlink2_11 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.11 ECO bright"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(EcoBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink2_11 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.11 ECO bright"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(EcoBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_11=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_11=0;
   }
}
void SaveBlink2_10(){ // Анимация моргания слова save в подпункте 2.10
  static int8_t counterSaveBlink2_10;
  static unsigned long timing2_10;  if (millis() - timing2_10 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_10++;
      timing2_10 = millis(); 
   }
   if(counterSaveBlink2_10 == 1){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(5, 7,   "2.10 Speed Falling");// write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value step: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(67,35);  u8g2.print(SpeedFalling);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(48, 60, "0 - 3");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink2_10 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(5, 7,   "2.10 Speed Falling"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value step: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(67,35);  u8g2.print(SpeedFalling);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(48, 60, "0 - 3");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_10=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_10=0;
   }
}

void SaveBlink2_9(){ // Анимация моргания слова save в подпункте 2.9
  static int8_t counterSaveBlink2_9;
  static unsigned long timing2_9;  if (millis() - timing2_9 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_9++;
      timing2_9 = millis(); 
   }
   if(counterSaveBlink2_9 == 1){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.9 Bright When Idle ");// write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(BrightWhenIdle);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink2_9 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.9 Bright When Idle "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Value: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(BrightWhenIdle);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_9=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_9=0;
   }
}

void SaveBlink2_8(){ // Анимация моргания слова save в подпункте 2.8
  static int8_t counterSaveBlink2_8;
  static unsigned long timing2_8;
   if (millis() - timing2_8 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_8++;
      timing2_8 = millis(); 
   }
   if(counterSaveBlink2_8 == 1){
        u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.8 Delay fall bright.");  // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Step val: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(DelayFallingBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	
          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 10");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink2_8 == 2){
        u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "2.8 Delay fall bright.");  // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Step val:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(DelayFallingBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	
          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 10");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_8=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_8=0;
   }
          
}

void SaveBlink2_7(){ // Анимация моргания слова save в подпункте 2.7
  static int8_t counterSaveBlink2_7;
  static unsigned long timing2_7;  if (millis() - timing2_7 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_7++;
      timing2_7 = millis(); 
   }
   if(counterSaveBlink2_7 == 1){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(10, 7,   "2.7 Stop % Bright. "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(StopPersentBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink2_7 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(10, 7,   "2.7 Stop % Bright.  "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(53,35);  u8g2.print(StopPersentBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");
          
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 

          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_7=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_7=0;
   }
}

void SaveBlink2_6(){ // Анимация моргания слова save в подпункте 2.6
  static int8_t counterSaveBlink2_6;
  static unsigned long timing2_6;  if (millis() - timing2_6 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_6++;
      timing2_6 = millis(); 
   }
   if(counterSaveBlink2_6 == 1){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(10, 7,   "2.6 Start % Bright. "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(StartPersentBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink2_6 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(10, 7,   "2.6 Start % Bright.  "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(StartPersentBright);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(40, 60, "0 - 100");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_6=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_6=0;
   }
}

void SaveBlink2_5(){ // Анимация моргания слова save в подпункте 2.5
  static int8_t counterSaveBlink2_5;
  static unsigned long timing2_4;  if (millis() - timing2_4 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_5++;
      timing2_4 = millis(); 
   }
   if(counterSaveBlink2_5 == 1){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(20, 7,   "2.5 Stop accel "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(StopToAccel);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink2_5 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(20, 7,   "2.5 Stop accel "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(StopToAccel);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_5=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_5=0;
   }
}

void SaveBlink2_4(){ // Анимация моргания слова save в подпункте 2.3
  static int8_t counterSaveBlink2_4;
  static unsigned long timing2_4;  if (millis() - timing2_4 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_4++;
      timing2_4 = millis(); 
   }
   if(counterSaveBlink2_4 == 1){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(20, 7,   "2.4 Start accel "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(StartToAccel);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
           u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   if(counterSaveBlink2_4 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(20, 7,   "2.4 Start accel "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Val KM/h: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60,35);  u8g2.print(StartToAccel);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	

          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(46, 60, "0 - 99");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0,   50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_4=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_4=0;
   }
}

void SaveBlink2_3(){ // Анимация моргания слова save в подпункте 2.3
  static int8_t counterSaveBlink2_3;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_3++;
      timing = millis(); 
   }
   if(counterSaveBlink2_3 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(10, 7,   "2.3 Adaptiv Bright."); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka2_3==false){
              if(AdaptivBrightness==true) { PositionUpCount=160; }
              if(AdaptivBrightness==false){ PositionUpCount=161; }
          OneRazGalochka2_3=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==160){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==161){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink2_3 == 2){
     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(10, 7,   "2.3 Adaptiv Bright."); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka2_3==false){
              if(AdaptivBrightness==true) { PositionUpCount=160; }
              if(AdaptivBrightness==false){ PositionUpCount=161; }
          OneRazGalochka2_3=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==160){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==161){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_3=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_3=0;
   }
}

void SaveBlink2_2(){ // Анимация моргания слова save в подпункте 2.2
  static int8_t counterSaveBlink2_2;
  static unsigned long timing2_2;
   if (millis() - timing2_2 > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_2++;
      timing2_2 = millis(); 
   }
   if(counterSaveBlink2_2 == 1){
        u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(40, 7,   "2.2 sensOn "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "sensOn val: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(70,35);  u8g2.print(sensOnValue);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	
          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back

          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(48, 60, "0 - 5");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink2_2 == 2){
        u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(40, 7,   "2.2 sensOn "); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "sensOn val: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(70,35);  u8g2.print(sensOnValue);         

          u8g2.drawTriangle(85+1,28, 88+1,16, 91+1,28);
          u8g2.drawTriangle(85+1,32, 88+1,44, 91+1,32);
       
          u8g2.setFont(u8g2_font_7x14_tr);	
          u8g2.drawTriangle(108,62, 128,57, 108,52); // стрелка под save
          u8g2.drawTriangle(20,62, 0,57, 20,52);     // стрелка по back
          
          u8g2.setFont(u8g2_font_7x14_tf);
          u8g2.drawStr(48, 60, "0 - 5");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_2=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_2=0;
   }
          
}

void SaveBlink2_1(){ // Анимация моргания слова save в подпункте 2.1
  static int8_t counterSaveBlink2_10;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink2_10++;
      timing = millis(); 
   }
   if(counterSaveBlink2_10 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(45, 7,   "2.1 TunL "); // write something to the internal memory
	 
      u8g2.setFont(u8g2_font_7x14B_tr);	
      u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
      // Исполнить один раз чтоб галочка соответствовала значению
      if(OneRazGalochka2_1==false){
          if(TunL==true) { PositionUpCount=120; }
          if(TunL==false){ PositionUpCount=121; }
          OneRazGalochka2_1=true;
      }
      // Исполнить один раз чтоб галочка соответствовала значению
      u8g2.setFont(u8g2_font_7x14_tr);	
      if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
      if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
      u8g2.drawTriangle(108,62, 128,57, 108,52); 
      u8g2.drawTriangle(20,62, 0,57, 20,52);

      //u8g2.setFont(u8g2_font_7x14_tf);
      //u8g2.drawStr(52, 60, "0-1");

      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(105, 50, "    "); 
      u8g2.drawStr(0, 50, "back"); 
      
      u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink2_10 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(45, 7,   "2.1 TunL "); // write something to the internal memory
	 
      u8g2.setFont(u8g2_font_7x14B_tr);	
      u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
      // Исполнить один раз чтоб галочка соответствовала значению
      if(OneRazGalochka2_1==false){
          if(TunL==true) { PositionUpCount=120; }
          if(TunL==false){ PositionUpCount=121; }
          OneRazGalochka2_1=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back"); 
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink2_1=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink2_10=0;
   }
          
}


























































/*
void Debounce(const int8_t buttonPin,bool& buttonState,bool& lastButtonState,
unsigned long& lastDebounceTime,uint8_t debounceDelay){
  bool reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;


      if (buttonState == HIGH) {
//        ledState = !ledState;
        if(buttonPin == 2){
         PositionUpCount++; //Ползунок по вертикали
        }
         if(buttonPin == 4){
         PositionUpCount--; //Ползунок по горизонатали
        }
        if(buttonPin == RightButtonPin){
         PositionRightCount--; //Ползунок по горизонатали
        }
      }
    }
  }

  // set the LED:
  //digitalWrite(ledPin, ledState);
  

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
*/

/*
// keyboard input
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
// keyboard input


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
*/

/*
void SerialInput( ) {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    Serial.println(inputString);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      Serial.println("stringComplete");
      Serial.print("stringComplete:");
      Serial.println(inputString);
      if (inputString == "s\n") { //низ
        CirclY += 20;
        MenuCount += 1;
        //CirclY= constrain(CirclY,5,45);
      }
      if (inputString == "w\n") { //верх
        CirclY -= 20;
        MenuCount -= 1;
        //CirclY= constrain(CirclY,5,45);
      }
      if (inputString == "e\n") { //вход
        if (CirclY == 2) { // Если круг напротив пункта ближ свет  // Тут enum можно

          MenuLayer = 2; // Переключение
          CirclY = 10;
        }
      }
      if (inputString == "q\n") { //верх
        MenuLayer = 0;
        //CirclY= constrain(CirclY,5,45);
      }
      inputString = "";
    }
  }
*/
  /*
  if (MenuLayer == 0 && CirclY <= 50) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(20, 15, "Punkt menu 1"); // write something to the internal memory
    u8g2.drawStr(20, 35, "Passing lights"); // write something to the internal memory
    u8g2.drawStr(20, 55, "Punkt menu 3");
    u8g2.drawCircle(110, CirclY, 6);
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 0 && CirclY > 50) {
    //if(UpCirkl==false){    CirclY=10;      UpCirkl=true;   }
    Serial.println("!!!");
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(20, 15, "Punkt menu 4"); // write something to the internal memory
    u8g2.drawStr(20, 35, "Punkt menu 5"); // write something to the internal memory
    u8g2.drawStr(20, 55, "Punkt menu 6");
    u8g2.drawCircle(110, CirclY, 6);
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 2) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 15, "TunL"); // write something to the internal memory
    u8g2.drawStr(0, 35, "senOn"); // write something to the internal memory
    u8g2.drawStr(0, 55, "Adaptiv  brightness");
    u8g2.drawCircle(110, CirclY, 6);
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  delay(50);
  SerialInput( );

  Serial.println(CirclY);
  

}
*/