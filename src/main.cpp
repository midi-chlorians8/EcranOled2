//04.09
#include <Arduino.h>
//bool RightInt=false; // Удалить похоже
bool OneRazOffGabarit=true; //Габариты. Один раз выкл габариты
bool AftherOn=false; //Габариты. Cлужебный буль. Отключает свечение габаритов на время пока светит поворотник 
int8_t Mode=1; //Влияет на степень "Яркости" Габаритов. Может быть ECO(0) DRIVE(1) SPORT(2) .

#include "DebounceLeftRight.h"   // Обработка горизонтальных кнопок
#include "DebounceUpDown.h"      // Обработка вертикальных кнопок

// Для экрана
#include <U8g2lib.h>    
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* cs=*/ 12, /* dc=*/ 14, /* reset=*/ 23); //Работает в ардуино иде
// Для экрана
#include "EEPROM.h" 

#include "MenuVariables_PrototypeFunc.h" //Переменные для работы меню и Прототипы функий SaveBlink

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

  bool BuzzerOn = false; // 3.1 Буззер включить или выключить вообще
  bool VolumeOnSpeed = false;  // 3.1 Включить или выключить громкость в зависимости от скорости
  int8_t SettingMaxVolumeOnSpeed = 0; // 3.2 На какой скорости вкляются 2 буззера   
  bool VolumeTimePressed = false; // 3.3 Включить или выключить громкость в зависимости от времени нажатия на клавишу 
  int8_t SettingTimePresseMax;    // 3.4 диапазон времени нажатия до максимальной громкости
  int8_t HowLongTimeBeep; // 3.6 Сколько времени длится гудок
  int8_t HowLongTimeBeepMute; // 3.7 Сколько времени длится пауза

  bool DrawPovorotniki = false; // 4.1
  bool TactPovorotnikiToLightOrBeep; // 4.2 //Будет ли стрелочка в такт со светом моргать или с буззером 

  bool ActivateDayLight = false; // 5.1
  int8_t BrightnessDayLight;     // 5.2
  int8_t BrightnessInEcoMode;    // 5.3
  int8_t FadingWhiteWhenTurning; // 5.4
  int8_t BrightnessOnActivatedPassingLights; // 5.5

  int8_t SpeedPovorotnikBlink; // 1.1
  bool IntelligentMode;        // 1.2
  int8_t CountBlinkIntMode;    // 1.3
  bool AutomaticMode;          // 1.4 
  int8_t TimePressToOnAutoMode;// 1.5

  bool EnterOnTheAutoMode;     // 1.6
// Переменные которые мы изменяем из меню. Которые и влияют на работу системы

// Переменные используемые поворотниками
//#define YarkiyYellow // Переключатель на яркий жёлтый
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
  // Переменные для пищалки поворотника
  bool OnSound = false; //Когда 
  bool OnSoundR; //Для ристования стрелки по звуку
  bool OnSoundL; //Для ристования стрелки по звуку

  // Переменные для пищалки поворотника
  #define BUZZER_PIN 25 //Белый буззер пин 
  #define BuzzerON
 bool PovorotnikiOn; // В 101 для демонстрации 2 сразу
// Переменные используемые поворотниками


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

  TunL=                     EEPROM.readBool(0); // Чтение из адреса 0
  sensOnValue=              EEPROM.readByte(1);
  AdaptivBrightness=        EEPROM.readBool(2);
  StartToAccel=             EEPROM.readByte(3);
  StopToAccel=              EEPROM.readByte(4);

  StartPersentBright=       EEPROM.readByte(5);
  StopPersentBright =       EEPROM.readByte(6);
  DelayFallingBright=       EEPROM.readByte(7);
  BrightWhenIdle=           EEPROM.readByte(8);
        
  SpeedFalling =            EEPROM.readByte(9);
  EcoBright =               EEPROM.readByte(10);
  VolumeOnSpeed =           EEPROM.readBool(11);
  SettingMaxVolumeOnSpeed = EEPROM.readByte(12);

  VolumeTimePressed    =    EEPROM.readBool(13);
  SettingTimePresseMax =    EEPROM.readByte(14);

  SpeedPovorotnikBlink =    EEPROM.readByte(15);
  IntelligentMode      =    EEPROM.readBool(16);
  CountBlinkIntMode     =   EEPROM.readByte(17);
  AutomaticMode         =   EEPROM.readByte(18);
  TimePressToOnAutoMode =   EEPROM.readByte(19);

  EnterOnTheAutoMode    =   EEPROM.readBool(20);
  BuzzerOn              =   EEPROM.readBool(21);
  DrawPovorotniki       =   EEPROM.readBool(22);
  HowLongTimeBeep       =   EEPROM.readByte(23);
  HowLongTimeBeepMute   =   EEPROM.readByte(24);

  TactPovorotnikiToLightOrBeep=EEPROM.readByte(25);
  ActivateDayLight            =EEPROM.readBool(26);
  BrightnessDayLight          =EEPROM.readByte(27);
  BrightnessInEcoMode         =EEPROM.readByte(28); // 5.3
  FadingWhiteWhenTurning      =EEPROM.readByte(29); // 5.4
  BrightnessOnActivatedPassingLights=EEPROM.readByte(30);// 5.5

  // Чтение значений из Eeprom и присваивание их значений переменным
}
//void Debounce(const int8_t buttonPin,bool& buttonState,bool& lastButtonState,unsigned long& lastDebounceTime,uint8_t debounceDelay);
//bool MinusUP4 = false;

void loop(void) {
while(1){

 GlobalPrint();// Печать отладки

// Обработка кнопок
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
        if(OneRazMLayerMin1 == false){ FirstZamerMLayerMin1_Val = millis();OneRazMLayerMin1 = true;}
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

  Povorotniki(); // Функция обрабатывает нажание на кнопки право и лево и включает выключает поворотники.
  // Измеряем время нажатия правой или левой кнопки для входа для включения автоматического режима
// /*
   
  // Измеряем время нажатия правой или левой кнопки для входа для включения автоматического режима
}
// Главный экран

// Ползаем по главному меню
if(MenuLayer==0 || MenuLayer==1){
  //RightInt=false; // Cкидываем чтоб после того как выходим из меню в главный экран не включался правый поворотник
  if(ActivateDayLight!=1){ // Если не включён режим day light (чтоб не было мерцания)
    PovorotnikiRightOff(); PovorotnikiLeftOff();           //Защита от загорания при переходе в главное меню из int mode
  }
  beginIntModeBlinkR = false; beginIntModeBlinkL = false;//Защита от загорания при переходе в главное меню из int mode
  
  //PovorotOnRight=false; // Чтоб при выходе из меню в главный экран не загорался поворотник
  if(PositionUpCount == 0){PositionUpCount = 7;} // Ограничить вертикальный ползунок при движении вверх
  if(PositionUpCount > 7){PositionUpCount = 7;} // Ограничить вертикальный ползунок при движении вниз

  if(PositionUpCount==1){   CirclY = 18; //MenuLayer=0;  
                            if( PositionRightCount == 1){ MenuLayer=10;  PositionUpCount=50; } 
  }
  if(PositionUpCount==2){   CirclY = 29; //MenuLayer=0; 
                            if( PositionRightCount == 1){ MenuLayer=20;  PositionUpCount=100; } 
                        }                   
  if(PositionUpCount==3){   CirclY = 39; //MenuLayer=0;
                            if( PositionRightCount == 1){ MenuLayer=30;  PositionUpCount=100; }
                        }
  if(PositionUpCount==4){   CirclY = 48; MenuLayer=0;  
                            if(PositionRightCount == 1) { MenuLayer = 40;PositionUpCount=1; } 
                        }
  if(PositionUpCount==5){   CirclY = 8;  MenuLayer=1;  
                            if(PositionRightCount == 1) {MenuLayer = 50;PositionUpCount=100;} 
                        }
  if(PositionUpCount==6){   CirclY = 18; MenuLayer=1;  
                            if(PositionRightCount == 1) {PositionRightCount=0;} // Заглушка
                        }
  if(PositionUpCount==7){   CirclY = 28; MenuLayer=1;  // exit
                            if( PositionRightCount == 1){ // Если мы жмякаем в правую сторону на exit
                              MenuLayer=-1; OffPovorotniki = true; timingOffPovorotniki=millis(); 
                              PovorotOnRight=true;
                              PositionRightCount = OldPositionRightCountInt; // Чтоб убрать баги с морганием при выходе из меню интеллигент режим
                            } // Выключить поворотники при выходе из меню
                            if( PositionRightCount == -1){ // Если мы жмякаем в левую сторону на exit 
                              MenuLayer=-1; OffPovorotniki = true; timingOffPovorotniki=millis(); 
                              PovorotOnRight=true;
                              PositionRightCount = OldPositionRightCountInt; // Чтоб убрать баги с морганием при выходе из меню интеллигент режим
                            } // Выключить поворотники при выходе из меню
                        }
                         
  
}
// Ползаем по главному меню

// Перебираем вкладку 1
if(MenuLayer == 10){
  //if(PositionUpCount < 50){PositionUpCount = 50;} // Ограничить вертикальный ползунок при движении вверх
  if(PositionUpCount == 49){PositionUpCount = 58;MenuLayer=14;} // Ограничить вертикальный ползунок при движении вверх


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
  if(PositionUpCount==55){  CirclY = 22+13; }

  if(PositionRightCount == 2 && PositionUpCount==54){ MenuLayer=105; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 1.1
  if(PositionRightCount == 2 && PositionUpCount==55){ MenuLayer=106; PositionUpCount=120; }

  if(PositionRightCount == 0){ //Если в подменю 0.2 Нажата кнопка влево то выйти в главное меню
      MenuLayer=0;PositionUpCount=1;
  }
  if(PositionUpCount== 53)  { MenuLayer = 11; }         //При скролле вверх перейти на верхнюю часть страницы
  if(PositionUpCount== 56)  { MenuLayer = 13; }
  //if(PositionUpCount > 55)  { PositionUpCount = 55; }   //Ограничить ползунок вниз

  OneRazPosition1_5 = false;
  saveBlink_sensOnValue1_5 = false;

  saveBlink_sensOnValue1_6=false;
  OneRazGalochka1_6=false;
}
if(MenuLayer == 13){
  if(PositionUpCount==56){  CirclY = 10-1;  }
  if(PositionUpCount==57){  CirclY = 22+13; }

  if(PositionRightCount == 2 && PositionUpCount==56){ MenuLayer=107; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 1.1
  if(PositionRightCount == 2 && PositionUpCount==57){ MenuLayer=108; PositionUpCount=180; }

  if(PositionRightCount == 0){ //Если в подменю 0.2 Нажата кнопка влево то выйти в главное меню
      MenuLayer=0;PositionUpCount=1;
  }
  if(PositionUpCount== 55)  { MenuLayer = 12; }         //При скролле вверх перейти на верхнюю часть страницы
  //if(PositionUpCount== 56)  { MenuLayer = 13; }
  //if(PositionUpCount > 57)  {MenuLayer=10; PositionUpCount = 50; }   //Ограничить ползунок вниз
  if(PositionUpCount == 58 || PositionUpCount == 59)  {MenuLayer=14; PositionUpCount = 58; }   //Ограничить ползунок вниз

    OneRazPosition3_6 = false;
    saveBlink_sensOnValue3_6 = false;

    OneRazPosition3_7 = false;

    saveBlink_sensOnValue3_7 = false;
    digitalWrite(25,LOW); // Выключить буззер
    }
if(MenuLayer == 14){
  if(PositionUpCount==58){  CirclY = 10-1;  }
  //if(PositionUpCount==59){  CirclY = 22+13; }

  if(PositionRightCount == 2 && PositionUpCount==58) { MenuLayer=109; PositionUpCount=120; }//Если курсор первая строка и есть нажатие вправо - перейти в 1.1
  //if(PositionRightCount == 2 && PositionUpCount==57){ MenuLayer=108; PositionUpCount=180; }

  if(PositionRightCount == 0){ //Если в подменю 0.2 Нажата кнопка влево то выйти в главное меню
      MenuLayer=0; PositionUpCount=1;
  }
  saveBlink_sensOnValue1_9=false;
  OneRazGalochka1_9=false;

  if(PositionUpCount== 57)  { MenuLayer = 13; }         //При скролле вверх перейти на верхнюю часть страницы
  if(PositionUpCount == 59 || PositionUpCount == 60 )  {MenuLayer=10; PositionUpCount = 50; }   //Ограничить ползунок вниз

/*
    OneRazPosition3_6 = false;
    saveBlink_sensOnValue3_6 = false;

    OneRazPosition3_7 = false;
    saveBlink_sensOnValue3_7 = false;
    digitalWrite(25,LOW); // Выключить буззер
*/
    }
//TactPovorotnikiToLightOrBeep
if(MenuLayer == 101){ // 1.1 SpeedPovorotnikBlink
     
    static unsigned long timingLeftRightBlink101; // Используется для таймера моргания правого поворотника
    
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
              if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                  PovorotnikiRightOff(); PovorotnikiLeftOff(); // turn off the pixels
              }
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
                  #ifdef YarkiyYellow
                  RgbwColor  color = RgbwColor(200, 255, 0,0); //Создали жёлтый
                  #else
                  RgbwColor  color = RgbwColor(5, 5, 0,0); //Создали лёгкий жёлтый
                  #endif
                  for(int i=0;  i<26;++i) {strip.SetPixelColor(i, color);}   strip.Show();// Зажечь левый правый поворотник
              }                          // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
              else{                       // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ
                    if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                          PovorotnikiRightOff(); // turn off the pixels
                          PovorotnikiLeftOff(); // turn off the pixels
                    }
                  }
              if (millis() - timing101 > 4000){  //Время сколько происходит моргание 
                  //Serial.println ("4 seconds Proslo");
                  if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                    PovorotnikiRightOff(); PovorotnikiLeftOff(); // turn off the pixels
                  }
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
          if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
            PovorotnikiRightOff(); // turn off the pixels
            PovorotnikiLeftOff(); // turn off the pixels
          }
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
if(MenuLayer == 106){ // 1.6
    if(PositionRightCount ==1){ // back
        MenuLayer=12;PositionUpCount=55;
    }
    if(PositionRightCount ==3){ // save

        if(PositionUpCount ==120){
          EnterOnTheAutoMode=true;
        }
        if(PositionUpCount ==121){
          EnterOnTheAutoMode=false;
        }
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(20, EnterOnTheAutoMode); EEPROM.commit();

        saveBlink1_6=true;
        PositionRightCount =2;
    }
    
}
if(MenuLayer == 107){ // 3.6 TimePressToOnAutoMode
    if(OneRazPosition3_6==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_HowLongTimeBeep = HowLongTimeBeep;       
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(HowLongTimeBeep,0,50,-180,-230);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition3_6=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount3_6=PositionUpCount; // Постоянно присваивать в старое значение
      HowLongTimeBeep=abs(    (180 + PositionUpCount)   );   
  
       HowLongTimeBeep=constrain(HowLongTimeBeep,0,50);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-230){PositionUpCount=-230;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeByte(23, HowLongTimeBeep);
          EEPROM.commit();

          saveBlink_sensOnValue3_6=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink3_6=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back

          if(saveBlink_sensOnValue3_6 != true){
              HowLongTimeBeep=old_HowLongTimeBeep;
          }

          MenuLayer=13;PositionUpCount=56;
       } 
          // Проигрывание звука вызванного работой поворотников
      static unsigned long timingOn; // Время Паузы 
      static unsigned long timingPause; // Время Паузы
      static int8_t Perebor;

           if (Perebor == 0){
              //if (millis() - timingOn > 500){ // Вместо 10000 подставьте нужное вам значение паузы
              digitalWrite(25,HIGH); // Включить буззер
              if (millis() - timingOn > (HowLongTimeBeep*100) ){ // Вместо 10000 подставьте нужное вам значение паузы                  
                  timingPause= millis(); 
                  Perebor=1;
              }
           }
           if (Perebor == 1){
              //if (millis() - timingPause > 500){ // Вместо 10000 подставьте нужное вам значение паузы
              digitalWrite(25,LOW); // Выключить буззер
              if (millis() - timingPause > (HowLongTimeBeepMute*100) ){ // Вместо 10000 подставьте нужное вам значение паузы 
                  timingOn = millis();
                  Perebor=0; 
              }
           }

      
      // Проигрывание звука вызванного работой поворотников
}
if(MenuLayer == 108){ // 3.7 
    if(OneRazPosition3_7==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_HowLongTimeBeepMute = HowLongTimeBeepMute;       
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(HowLongTimeBeepMute,0,50,-180,-230);
        // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition3_7=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount3_7=PositionUpCount; // Постоянно присваивать в старое значение
      HowLongTimeBeepMute=abs(    (180 + PositionUpCount)   );   
  
       HowLongTimeBeepMute=constrain(HowLongTimeBeepMute,0,50);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-230){PositionUpCount=-230;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeByte(24, HowLongTimeBeepMute);
          EEPROM.commit();

          saveBlink_sensOnValue3_7=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink3_7=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back

          if(saveBlink_sensOnValue3_7 != true){
              HowLongTimeBeepMute=old_HowLongTimeBeepMute;
          }

      MenuLayer=13;PositionUpCount=57;
      }
                // Проигрывание звука вызванного работой поворотников
      static unsigned long timingOn; // Время Паузы 
      static unsigned long timingPause; // Время Паузы
      static int8_t Perebor;

           if (Perebor == 0){
              //if (millis() - timingOn > 500){ // Вместо 10000 подставьте нужное вам значение паузы
              digitalWrite(25,HIGH); // Включить буззер
              if (millis() - timingOn > (HowLongTimeBeep*100) ){ // Вместо 10000 подставьте нужное вам значение паузы                  
                  timingPause= millis(); 
                  Perebor=1;
              }
           }
           if (Perebor == 1){
              //if (millis() - timingPause > 500){ // Вместо 10000 подставьте нужное вам значение паузы
              digitalWrite(25,LOW); // Выключить буззер
              if (millis() - timingPause > (HowLongTimeBeepMute*100) ){ // Вместо 10000 подставьте нужное вам значение паузы 
                  timingOn = millis();
                  Perebor=0; 
              }
           }

      
      // Проигрывание звука вызванного работой поворотников
}
if(MenuLayer == 109){ // 1.9
      if(PositionRightCount ==1){ // back
          MenuLayer=14;PositionUpCount=58;
      }
      if(PositionRightCount ==3){ // save

          if(PositionUpCount ==120){
            TactPovorotnikiToLightOrBeep=true;
          }
          if(PositionUpCount ==121){
            TactPovorotnikiToLightOrBeep=false;
          }
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
          EEPROM.writeBool(25, TactPovorotnikiToLightOrBeep);
          EEPROM.commit();

          saveBlink1_9=true;
          PositionRightCount =2;
      }
      //PositionUpCount=constrain(PositionUpCount,120,121);
      //Serial.println("201!");
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
       // Исполнить ��дин раз чтоб галочка соответствовала значению
              
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

    if(PositionRightCount == 2 && PositionUpCount==100){ MenuLayer=300; PositionUpCount=120; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.11 //TunL
    if(PositionRightCount == 2 && PositionUpCount==101){ MenuLayer=301; PositionUpCount=120; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    //if(PositionRightCount == 2 && PositionUpCount==102){ MenuLayer=303; PositionUpCount=160; }//Если курсор третяя строка и есть нажатие вправо - перейти в 2.1.3 //AdaptivBrightness   
    
    if(PositionUpCount==102){    MenuLayer=31;  } //При скролле вниз перейти на нижнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=0;  PositionUpCount=3; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
    
    OneRazGalochka3_0 = false;
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
    if(PositionRightCount == 2 && PositionUpCount==102){ MenuLayer=3011; PositionUpCount=140; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    if(PositionRightCount == 2 && PositionUpCount==103){ MenuLayer=302; PositionUpCount=140; }//Если курсор третяя строка и есть нажатие вправо - перейти в 2.1.3 //AdaptivBrightness   
    
    if(PositionUpCount==101){    MenuLayer=30;  } //При скролле вверх перейти на нижнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=0;  PositionUpCount=3; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
   
    OneRazGalochka3_1 = false;
    OneRazGalochka3_2  =false;
    saveBlink_sensOnValue3_21=false;
    OneRazPosition3_21=false;
    //  if(PositionUpCount > 103){PositionUpCount = 103;} // Ограничить вертикальный ползунок при движении вниз
    if(PositionUpCount==104){    MenuLayer=32;  } //При скролле вниз перейти на нижнюю часть страницы
    //saveBlink_sensOnValue3_2=false;
    //OneRazPosition3_2=false;

    //OneRazGalochka3_3 = false;
}
if(MenuLayer == 32){ //Если в подменю 3
    if(PositionUpCount==104){   CirclY = 7;   }  //TunL  //Перебираем ползунок
   // if(PositionUpCount==105){   CirclY = 37;  }          //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount==104){ MenuLayer=3021; PositionUpCount=160; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    if(PositionRightCount == 2 && PositionUpCount==105){ MenuLayer=306; PositionUpCount=180; } //
    
    if(PositionUpCount==103)   { MenuLayer=31;  } //При скролле вниз перейти на нижнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=0;  PositionUpCount=3; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
   
    /*
    OneRazGalochka3_1 = false;
    OneRazGalochka3_2  =false;
    saveBlink_sensOnValue3_21=false;
    OneRazPosition3_21=false;
    */
    OneRazPosition3_6 = false;
    saveBlink_sensOnValue3_6 = false;
    if(PositionUpCount>104){  MenuLayer=30;  PositionUpCount=100;  } //При скролле вниз перейти на нижнюю часть страницы

    //if(PositionUpCount > 105){PositionUpCount = 105;} // Ограничить вертикальный ползунок при движении вниз
    
    //saveBlink_sensOnValue3_2=false;
    //OneRazPosition3_2=false;

    //OneRazGalochka3_3 = false;
}
/*
if(MenuLayer == 33){ //Если в подменю 3
    if(PositionUpCount==106){   CirclY = 7;   }  //TunL  //Перебираем ползунок
    //if(PositionUpCount==105){   CirclY = 37;  }          //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount==106){ MenuLayer=307; PositionUpCount=180; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    //if(PositionRightCount == 2 && PositionUpCount==105){ MenuLayer=306; PositionUpCount=180; } //
    
    if(PositionUpCount==105)   { MenuLayer=32;  } //При скролле вверх перейти на верхнюю часть страницы
    if(PositionUpCount > 106){PositionUpCount = 106;} // Ограничить вертикальный ползунок при движении вниз
    if(PositionRightCount == 0){ MenuLayer=0;  PositionUpCount=3; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
   
    
    OneRazGalochka3_1 = false;
    OneRazGalochka3_2  =false;
    saveBlink_sensOnValue3_21=false;
    OneRazPosition3_21=false;
    
 
    //if(PositionUpCount==106){    MenuLayer=33;  } //При скролле вниз перейти на нижнюю часть страницы

    //if(PositionUpCount > 105){PositionUpCount = 105;} // Ограничить вертикальный ползунок при движении вниз
    
    //saveBlink_sensOnValue3_2=false;
    //OneRazPosition3_2=false;

    //OneRazGalochka3_3 = false;
}
*/
if(MenuLayer == 300){ // 3.0
    if(PositionRightCount ==1){ // back
        MenuLayer=30;PositionUpCount=100;
    }
    if(PositionRightCount ==3){ // save
        if(PositionUpCount ==120){
          BuzzerOn=true;
        }
        if(PositionUpCount ==121){
          BuzzerOn=false;
        }
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(21, BuzzerOn);
        EEPROM.commit();
        //EEPROM.writeBool(0, VolumeOnSpeed);
        //EEPROM.commit();
        //address += sizeof(bool);

        saveBlink3_0=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
if(MenuLayer == 301){ // 3.1
    if(PositionRightCount ==1){ // back
        MenuLayer=30;PositionUpCount=101;
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
          MenuLayer=31;PositionUpCount=102;
    }
}
if(MenuLayer == 302){ // 3.2
    if(PositionRightCount ==1){ // back
        MenuLayer=31;PositionUpCount=103;
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
          MenuLayer=32;PositionUpCount=104;
          if(saveBlink_sensOnValue3_21 != true){
              SettingTimePresseMax=old_SettingTimePresseMax;
          }
      }
}

// Перебираем вкладку 3

// Перебираем вкладку 4
if(MenuLayer == 40){
// Перебираем вкладку 4
  if(PositionUpCount < 1){PositionUpCount = 1;} // Ограничить вертикальный ползунок при движении вверх
  //if(PositionUpCount == 49){PositionUpCount = 55;MenuLayer=12;} // Если вверх то вниз

  if(PositionUpCount==1){   CirclY = 20-1;   }
  if(PositionUpCount==2){   CirclY = 32+13;  }

  if(PositionRightCount == 2 && PositionUpCount==1){ MenuLayer=401; PositionUpCount=120; }//Если курсор первая строка и есть нажатие вправо - перейти в 1.1
  //if(PositionRightCount == 2 && PositionUpCount==51){ MenuLayer=102; PositionUpCount=120; }
  if(PositionRightCount == 0){ //Если в подменю 0.2 Нажата кнопка влево то выйти в главное меню
      MenuLayer=0; PositionUpCount=4;
  }
  //if(PositionUpCount==52){    MenuLayer=11;  } //При скролле вниз перейти на нижнюю часть страницы
  OneRazGalochka4_1 = false;
  /*
  OneRazPosition1_1 = false; // Сброс один раз исполнить
  saveBlink_sensOnValue1_1=false;

  saveBlink_sensOnValue1_2=false;
  OneRazGalochka1_2=false;
  */
  
}
if(MenuLayer == 401){ // 4.1 + Звук от поворотников
    if(PositionRightCount ==1){ // back
        MenuLayer=40;PositionUpCount=1;
    }
    if(PositionRightCount ==3){ // save
        if(PositionUpCount ==120){ DrawPovorotniki=true; }
        if(PositionUpCount ==121){ DrawPovorotniki=false;}
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        EEPROM.writeBool(22, DrawPovorotniki);
        EEPROM.commit();

        saveBlink4_1=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
// Перебираем вкладку 4

// Перебираем вкладку 5
 if(MenuLayer == 50){ //Если в подменю 2.1 Нажата кнопка влево то выйти в главное меню
  if(PositionUpCount < 100){MenuLayer=52; PositionUpCount = 104;} // Ограничить вертикальный ползунок при движении вверх

    if(PositionUpCount==100){   CirclY = 20-1;   }  //TunL  //Перебираем ползунок
    if(PositionUpCount==101){   CirclY = 45-1;   }          //Перебираем ползунок

    if(PositionRightCount == 2 && PositionUpCount==100){ MenuLayer=501; PositionUpCount=120; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.11 //TunL
    if(PositionRightCount == 2 && PositionUpCount==101){ MenuLayer=502; PositionUpCount=180; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    
    if(PositionUpCount==102){    MenuLayer=51;  } //При скролле вниз перейти на нижнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=1;  PositionUpCount=5; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
    
    OneRazGalochka5_1 = false;

    OneRazPosition5_2 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue5_2=false;
    //OneRazGalochka3_3 = false;
}
 if(MenuLayer == 51){ //Если в подменю 2.1 Нажата кнопка влево то выйти в главное меню

    if(PositionUpCount==102){   CirclY = 7;   }  //TunL  //Перебираем ползунок
    if(PositionUpCount==103){   CirclY = 37;   }          //Перебираем ползунок

     if(PositionRightCount == 2 && PositionUpCount==102){ MenuLayer=503; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.11 //TunL
     if(PositionRightCount == 2 && PositionUpCount==103){ MenuLayer=504; PositionUpCount=180; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    
    if(PositionUpCount==101){    MenuLayer=50;  } //При скролле вверх перейти на верхнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=1;  PositionUpCount=5; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
    
    if(PositionUpCount==104){    MenuLayer=52;  } //При скролле вниз перейти на нижнюю часть страницы
    //OneRazGalochka5_1 = false;
    //OneRazGalochka3_3 = false;
    OneRazPosition5_3 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue5_3=false;

    OneRazPosition5_4 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue5_4=false;
}
 if(MenuLayer == 52){ //Если в подменю 2.1 Нажата кнопка влево то выйти в главное меню

    if(PositionUpCount==104){   CirclY = 7;   }  //TunL  //Перебираем ползунок
    //if(PositionUpCount==105){   CirclY = 37;   }          //Перебираем ползунок

     if(PositionRightCount == 2 && PositionUpCount==104){ MenuLayer=505; PositionUpCount=180; }//Если курсор первая строка и есть нажатие вправо - перейти в 2.11 //TunL
     //if(PositionRightCount == 2 && PositionUpCount==103){ MenuLayer=504; PositionUpCount=180; }//Если курсор вторая строка и есть нажатие вправо - перейти в 2.1.2 //sensOn
    
    if(PositionUpCount==103){    MenuLayer=51;  } //При скролле вверх перейти на верхнюю часть страницы
    if(PositionRightCount == 0){ MenuLayer=1;  PositionUpCount=5; }//Если нажать влево то выйти в главное меню и переместить курсор на позицию 3
    
    if(PositionUpCount==105 || PositionUpCount==106 ){ PositionUpCount = 104; MenuLayer=52;  } //При скролле вниз перейти на нижнюю часть страницы

    OneRazPosition5_5 = false; // Сброс один раз исполнить
    saveBlink_sensOnValue5_5=false;
}

 if(MenuLayer == 501){ // 5.1 
    static bool SaveOk501=false; // Для того чтоб убрать глюк ленты при использовании EEPROM
    if(PositionRightCount ==1){ // back
        MenuLayer=50;PositionUpCount=100;
        if(SaveOk501 == true){ EEPROM.writeByte(26, ActivateDayLight); EEPROM.commit(); SaveOk501 = false; }
    }
    
    if(PositionRightCount ==3){ // save
        if(PositionUpCount ==120){ ActivateDayLight=true; }
        if(PositionUpCount ==121){ ActivateDayLight=false;OneRazOffGabarit=1;}
        //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        
        SaveOk501 = true;
        //!EEPROM.writeBool(26, ActivateDayLight);
        //!EEPROM.commit();

        saveBlink5_1=true;
        PositionRightCount =2;
    }
    //PositionUpCount=constrain(PositionUpCount,120,121);
    //Serial.println("201!");
}
 if(MenuLayer == 502){ // 2.6 StartPersentBright
    if(OneRazPosition5_2==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_BrightnessDayLight = BrightnessDayLight;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(BrightnessDayLight,0,100,-180,-280);
       // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition5_2=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount5_2=PositionUpCount; // Постоянно присваивать в старое значение
      BrightnessDayLight=abs(    (180 + PositionUpCount)   );
    
       BrightnessDayLight=constrain(BrightnessDayLight,0,100);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        
        EEPROM.writeByte(27,BrightnessDayLight);//StopPersentBright
        EEPROM.commit();
          saveBlink_sensOnValue5_2=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink5_2=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue5_2 != true){
              BrightnessDayLight=old_BrightnessDayLight;
          }
       MenuLayer=50;PositionUpCount=101;
       }
}
 if(MenuLayer == 503){ // 2.6 StartPersentBright
    if(OneRazPosition5_3==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_BrightnessInEcoMode = BrightnessInEcoMode;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(BrightnessInEcoMode,0,100,-180,-280);
       // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition5_3=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount5_3=PositionUpCount; // Постоянно присваивать в старое значение
      BrightnessInEcoMode=abs(    (180 + PositionUpCount)   );
    
       BrightnessInEcoMode=constrain(BrightnessInEcoMode,0,100);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        
        EEPROM.writeByte(28,BrightnessInEcoMode);EEPROM.commit();
        
          saveBlink_sensOnValue5_3=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink5_3=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue5_3 != true){
              BrightnessInEcoMode=old_BrightnessInEcoMode;
          }
       MenuLayer=51;PositionUpCount=102;
       }
}
 if(MenuLayer == 504){ // 2.6 StartPersentBright
    if(OneRazPosition5_4==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_FadingWhiteWhenTurning = FadingWhiteWhenTurning;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(FadingWhiteWhenTurning,0,100,-180,-280);
       // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition5_4=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount5_4=PositionUpCount; // Постоянно присваивать в старое значение
      FadingWhiteWhenTurning=abs(    (180 + PositionUpCount)   );
    
       FadingWhiteWhenTurning=constrain(FadingWhiteWhenTurning,0,100);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        
        EEPROM.writeByte(29,FadingWhiteWhenTurning);EEPROM.commit();
        
          saveBlink_sensOnValue5_4=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink5_4=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue5_4 != true){
              FadingWhiteWhenTurning=old_FadingWhiteWhenTurning;
          }
       MenuLayer=51;PositionUpCount=103;
       }
}
 if(MenuLayer == 505){ // 2.6 StartPersentBright
    if(OneRazPosition5_5==false){ // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
       old_BrightnessOnActivatedPassingLights = BrightnessOnActivatedPassingLights;
       //PositionUpCount=old_PositionUpCount2_4;
       
       // Исполнить один раз чтоб галочка соответствовала значению      
        PositionUpCount=map(BrightnessOnActivatedPassingLights,0,100,-180,-280);
       // Исполнить один раз чтоб галочка соответствовала значению
              
        OneRazPosition5_5=true;
      }                            // Один раз исполнить. Чтобы появилось в менюшке правильное значение которое в системе
      old_PositionUpCount5_5=PositionUpCount; // Постоянно присваивать в старое значение
      BrightnessOnActivatedPassingLights=abs(    (180 + PositionUpCount)   );
    
       BrightnessOnActivatedPassingLights=constrain(BrightnessOnActivatedPassingLights,0,100);
       if( PositionUpCount>-180){PositionUpCount=-180;}  // Защита от выхода за диапазон)
       if( PositionUpCount<-280){PositionUpCount=-280;}  // Защита от выхода за диапазон)

       if(PositionRightCount ==3){ // save
          //Тут должен быть ввод нового значения переменной и сохранения в EEPROM
        
        EEPROM.writeByte(30,BrightnessOnActivatedPassingLights);EEPROM.commit();
        
          saveBlink_sensOnValue5_5=true; // Нужно чтобы при выходе не сбрасывалось значение sensOnValue 
          saveBlink5_5=true; // Чтобы моргала надпись save
          PositionRightCount =2; // Вернуть ползунок по горизонтали
       }
       if(PositionRightCount ==1){ // back
          if(saveBlink_sensOnValue5_5 != true){
              BrightnessOnActivatedPassingLights=old_BrightnessOnActivatedPassingLights;
          }
       MenuLayer=52;PositionUpCount=104;
       }
}
// Перебираем вкладку 5
// Отрисовка меню

  if (MenuLayer == -1){
      u8g2.clearBuffer();          // clear the internal memory   
      
      static unsigned long timingOn; // Время Писка
      static int8_t Perebor=0; // Cоставная часть пищательного механизма

      if(DrawPovorotniki==false){
        u8g2.setFont(u8g2_font_7x14B_tr);	
        u8g2.drawStr(20, 35-3, "Glavnij Ekran");  //u8g2.drawStr(35+10, 40-3, "Off");
      }
      if(DrawPovorotniki == true){ // Если включена отрисовка поворотников то рисуем стрелочки
          // Стрелочка ресуется в соответствии с могранием поворотника
          if (TactPovorotnikiToLightOrBeep == 1){ // Если включена отрисовка стрелочки в зависимости от света    
              if(  
              (PovorotOnRight==true && beginIntModeBlinkR==true) ||  
              (AutomaticModeActivateR==1 && PovorotOnRight==true) ||  
              (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW && IntelligentMode == 0 && AutomaticModeActivateR==0 && PovorotOnRight==true )  )
              { //Если правый поворотник горит то нарисовать правый поворотник
                  u8g2.drawTriangle(108,62-20, 128,52-20, 108,42-20); // правый
              }
              
              if(  
              (PovorotOnLeft==true && beginIntModeBlinkL==true) ||  
              (AutomaticModeActivateL==1 && PovorotOnLeft==true) ||  
              (digitalRead(LeftButtonPin)==HIGH && digitalRead(RightButtonPin)==LOW && IntelligentMode == 0 && AutomaticModeActivateL==0 && PovorotOnLeft==true )  )
              { //Если левый поворотник горит то нарисовать правый поворотник
                  u8g2.drawTriangle(20,22, 0,32, 20,42);               // левый
              }
          } // Если включена отрисовка стрелочки в зависимости от света
          if (TactPovorotnikiToLightOrBeep == 0){ // Если включена отрисовка стрелочки в зависимости от звука
              if(OnSoundR == true && Perebor == 0){
                  u8g2.drawTriangle(108,62-20, 128,52-20, 108,42-20); // правый
              }
              if(OnSoundL == true && Perebor == 0){
                  u8g2.drawTriangle(20,22, 0,32, 20,42);               // левый
              }
          }
          // Стрелочка ресуется в соответствии с могранием поворотника

      }                           // Если включена отрисовка поворотников
      // Обработка  звука включение и выключение буззера при повороте

      
      //if (TactPovorotnikiToLightOrBeep == 1){ // Если включен звук в зависимости от света
      // Включение режима  звука от правого поворотника
      if(  
          (PovorotOnRight==true && beginIntModeBlinkR==true) || (PovorotOnRight==false && beginIntModeBlinkR==true) || 
          (AutomaticModeActivateR==1) ||  
          (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW && IntelligentMode == 0 && AutomaticModeActivateR==0 ) ||  
          (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW && IntelligentMode == 1 && AutomaticModeActivateR==0 ) )
           {
              OnSound = true; //timingOn=millis(); 
              OnSoundR = true; //Для ристования стрелки по звуку
           }
      // Включение режима звука от правого поворотника  

      // Включение режима звука от левого поворотника 
      else if(  
          (PovorotOnLeft==true && beginIntModeBlinkL==true) || (PovorotOnLeft==false && beginIntModeBlinkL==true) || 
          (AutomaticModeActivateL==1) ||  
          (digitalRead(LeftButtonPin)==HIGH && digitalRead(RightButtonPin)==LOW && IntelligentMode == 0 && AutomaticModeActivateL==0 ) || 
          (digitalRead(LeftButtonPin)==HIGH && digitalRead(RightButtonPin)==LOW && IntelligentMode == 1 && AutomaticModeActivateL==0 ) )
           {
              OnSound = true; //timingOn=millis(); 
              OnSoundL = true; //Для ристования стрелки по звуку
           }
      // Включение режима звука от левого поворотника 
      else{OnSound = false; Perebor=0;timingOn=millis();OnSoundR=false;OnSoundL=false; } 
              
      //}                                     // Если включен звук в зависимости от света     
         // Обработка  звука включение и выключение буззера при повороте

//  /*
      // Проигрывание звука вызванного работой поворотников 
      
      static unsigned long timingPause; // Время Паузы
      
      if(OnSound == true){

           if (Perebor == 0){
              //if (millis() - timingOn > 500){ // Вместо 10000 подставьте нужное вам значение паузы
              #ifdef BuzzerON
              digitalWrite(25,HIGH); // Включить буззер
              #endif
              if (millis() - timingOn > (HowLongTimeBeep*100) ){ // Вместо 10000 подставьте нужное вам значение паузы                  
                  timingPause= millis(); 
                  Perebor=1;
              }
           }
           if (Perebor == 1){
              //if (millis() - timingPause > 500){ // Вместо 10000 подставьте нужное вам значение паузы
              digitalWrite(25,LOW); // Выключить буззер
              if (millis() - timingPause > (HowLongTimeBeepMute*100) ){ // Вместо 10000 подставьте нужное вам значение паузы 
                  timingOn = millis();
                  Perebor=0; 
              }
           }

      }
      else{
        digitalWrite(25,LOW); // Выключить буззер
      }
      // Проигрывание звука вызванного работой поворотников
       //*/
      u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 0 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(45, 7, " MENU"); // write something to the internal memory
    u8g2.drawStr(5, 25-3, "1 Povorotniki"); // write something to the internal memory
    u8g2.drawStr(5, 35-3, "2 Passing lights"); // write something to the internal memory
    u8g2.drawStr(5, 45-3, "3 Buzzer");
    u8g2.drawStr(5, 55-3, "4 Glavnij Ecran"); // write something to the internal memory
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5); 
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 1 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    
    u8g2.drawStr(5, 10,   "5 Dimensions"); // write something to the internal memory
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
    u8g2.drawStr(0, 12, "1.5 Time Press"); // write something to the internal memory TimePressToOnAutoMode
                            u8g2.setCursor(95,12);  
                            u8g2.print(float(TimePressToOnAutoMode)/10 ,1);
    u8g2.drawStr(0, 22, "To On Auto Mode"); // write something to the internal
    
    u8g2.drawLine(0,27, 105, 27);

    u8g2.drawStr(0, 39, "1.6 How start");
    if(EnterOnTheAutoMode == true){      u8g2.drawStr(95, 39, "T");   }
    else                     {      u8g2.drawStr(95, 39, "D");  }
    u8g2.drawStr(0, 32+17, "Automatic mode");
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 13 ) {
    u8g2.clearBuffer();          // clear the internal memory
    
    u8g2.setFont(u8g2_font_6x12_tr);
    //u8g2.drawStr(20, 7, " 1 Povorotniki"); // write something to the internal memory
    u8g2.drawStr(0, 12, "1.7 Set. how"); // write something to the internal memory TimePressToOnAutoMode
                            u8g2.setCursor(95,12);  
                            u8g2.print(float(HowLongTimeBeep)/10 ,1);
    u8g2.drawStr(0, 22, "long time Beep"); // write something to the internal
    
    u8g2.drawLine(0,27, 105, 27);

    u8g2.drawStr(0, 39, "1.8 Set. how");
                            u8g2.setCursor(95,39); 
                            u8g2.print(float(HowLongTimeBeepMute)/10 ,1);
    u8g2.drawStr(0, 32+17, "long time Beep Mute");
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  if (MenuLayer == 14 ) {
    u8g2.clearBuffer();          // clear the internal memory
    
    u8g2.setFont(u8g2_font_6x12_tr);
    //u8g2.drawStr(20, 7, " 1 Povorotniki"); // write something to the internal memory 
    u8g2.drawStr(0, 12, "1.9 Tact Povorotn."); // write something to the internal memory TimePressToOnAutoMode
                            if(TactPovorotnikiToLightOrBeep == true){ u8g2.drawStr(108, 12, "L");   }
                            else                          {   u8g2.drawStr(108, 12, "B"); }
    u8g2.drawStr(0, 22, "To Light Or Beep"); // write something to the internal
    
    u8g2.drawLine(0,27, 105, 27);
/*
    u8g2.drawStr(0, 39, "1.8 Set. how");
                            u8g2.setCursor(95,39); 
                            u8g2.print(float(HowLongTimeBeepMute)/10 ,1);
    u8g2.drawStr(0, 32+17, "long time Beep Mute");
*/   
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  if (MenuLayer == 101 ) { // 1.1
      if(saveBlink1_1==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(15, 7,   "1.1 Speed Blink "); // write something to the internal memory
          
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
  if (MenuLayer == 106 ) { // 1.4
      if(saveBlink1_6==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(0, 7,   "1.6 How Start Automatic Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(5, 25-3, "Pressed Time");  u8g2.drawStr(5, 40-3, "Double click");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_6==false){
              if(EnterOnTheAutoMode==true) { PositionUpCount=120; }
              if(EnterOnTheAutoMode==false){ PositionUpCount=121; }
          OneRazGalochka1_6=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(105,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(105,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
      }
      else{
          SaveBlink1_6();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  }
  if (MenuLayer == 109 ) { // 1.2
      if(saveBlink1_9==false){
          u8g2.clearBuffer();          // clear the internal memory TactPovorotnikiToLightOrBeep
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "1.9 Tact Povorotniki"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "Light");  u8g2.drawStr(35+10, 40-3, "Beep");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_9==false){
              if(TactPovorotnikiToLightOrBeep==true) { PositionUpCount=120; }
              if(TactPovorotnikiToLightOrBeep==false){ PositionUpCount=121; }
          OneRazGalochka1_9=true;
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
        SaveBlink1_9();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
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
    u8g2.drawStr(0, 25-3, "3.0 Activate"); // write something to the internal memory 
     u8g2.drawStr(0, 32, "Buzzer"); // write something to the internal memory
    if(BuzzerOn == true)     {      u8g2.drawStr(97, 25-3, "On");   }
    else                     {      u8g2.drawStr(97, 25-3, "Off");  }
    
   
                            
    u8g2.drawLine(0, 32+5, 105, 32+5);

    u8g2.drawStr(0, 32+17, "3.1 Volume"); 
    u8g2.drawStr(0, 42+17, "on speed");
                           //u8g2.setCursor(97,32+17);  
                           //u8g2.print(SettingMaxVolumeOnSpeed);
    if(VolumeOnSpeed == true){      u8g2.drawStr(97, 32+17, "On");   }
    else                     {      u8g2.drawStr(97, 32+17, "Off");  }

    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  if (MenuLayer == 31 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "3.2 Set. Max "); // write something to the internal memory 3.3 Volume On Time
    u8g2.drawStr(0, 22, "Volume on speed");
                           u8g2.setCursor(97,12);  
                           u8g2.print(SettingMaxVolumeOnSpeed);
    //if(VolumeTimePressed == true){  u8g2.drawStr(97, 12, "On");   }
    //else                         {  u8g2.drawStr(97, 12, "Off");  }
    
    u8g2.drawLine(0, 27, 105, 27);

    u8g2.drawStr(0, 39, "3.3 Volume");
                           //u8g2.setCursor(97,39);  
                           //u8g2.print(SettingTimePresseMax);
    u8g2.drawStr(0, 49, "On Time"); // write something to the internal memory 
    if(VolumeTimePressed == true){  u8g2.drawStr(97, 39, "On");   }
    else                         {  u8g2.drawStr(97, 39, "Off");  }                       
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }  
  if (MenuLayer == 32 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "3.4 Set. time "); // write something to the internal memory 3.3 Volume On Time
    u8g2.drawStr(0, 22, "pressed Max Vol");
                           u8g2.setCursor(97,12);  
                           u8g2.print(SettingTimePresseMax);
    
    
    //if(VolumeTimePressed == true){  u8g2.drawStr(97, 12, "On");   }
    //else                         {  u8g2.drawStr(97, 12, "Off");  }
    
    u8g2.drawLine(0, 27, 105, 27);
    /*
    u8g2.drawStr(0, 39, "3.6 Set. how"); 
                  u8g2.setCursor(97,39);  
                  u8g2.print(float(HowLongTimeBeep)/10 ,1);
    u8g2.drawStr(0, 49, "long time Beep"); // write something to the internal memory 
   */

   //u8g2.setCursor(95,12);  
                          
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  /*
  if (MenuLayer == 33 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "3.7 Set. how"); // write something to the internal memory 3.3 Volume On Time
    u8g2.drawStr(0, 22, "long time Beep Delay");
                  u8g2.setCursor(97,12);  
                  u8g2.print(float(HowLongTimeBeepMute)/10 ,1);
    
    
    //if(VolumeTimePressed == true){  u8g2.drawStr(97, 12, "On");   }
    //else                         {  u8g2.drawStr(97, 12, "Off");  }
    
    u8g2.drawLine(0, 27, 105, 27);
    
    
    u8g2.drawStr(0, 39, "3.8 abc"); 
                  u8g2.setCursor(97,39);  
                  u8g2.print(float(HowLongTimeBeep)/10 ,1);
    u8g2.drawStr(0, 49, "defg"); // write something to the internal memory 
    

   //u8g2.setCursor(95,12);  
                          
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }  
  */
  if (MenuLayer == 300 )  { // 3.1
    
      if(saveBlink3_0==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "3.0 Activate Buzzer"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_0==false){
              if(BuzzerOn==true) { PositionUpCount=120; }
              if(BuzzerOn==false){ PositionUpCount=121; }
          OneRazGalochka3_0=true;
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
        SaveBlink3_0();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
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
  if (MenuLayer == 107 ) { // 1.5
      if(saveBlink3_6==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          u8g2.drawStr(5, 7,   "1.7 HowLongTimeBeep"); // write something to the internal memory
          
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(HowLongTimeBeep)/10 ,1);         

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
           SaveBlink3_6();
      }  
  }
  if (MenuLayer == 108 ) { // 1.5
      if(saveBlink3_7==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          u8g2.drawStr(0, 7,   "1.8 HowLongTimeBeepMute"); // write something to the internal memory
          
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(HowLongTimeBeepMute)/10 ,1);         

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
           SaveBlink3_7();
      }  
  }
  
  if (MenuLayer == 40 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(10, 7, " 4 Glavnij Ecran "); // write something to the internal memory
    u8g2.drawStr(0, 25-3, "4.1 Draw"); // write something to the internal memory 
    u8g2.drawStr(0, 32, "Povorotniki"); // write something to the internal memory
    if(DrawPovorotniki == true)     {      u8g2.drawStr(97, 25-3, "On");   }
    else                            {      u8g2.drawStr(97, 25-3, "Off");  }
    /*                       
    u8g2.drawLine(0, 32+5, 105, 32+5);

    u8g2.drawStr(0, 32+17, "4.2 Punkt"); 
    u8g2.drawStr(0, 42+17, "menu");
                           //u8g2.setCursor(97,32+17);  
                           //u8g2.print(SettingMaxVolumeOnSpeed);
    if(VolumeOnSpeed == true){   u8g2.drawStr(97, 32+17, "On");   }
    else                     {   u8g2.drawStr(97, 32+17, "Off");  }
    */
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  if (MenuLayer == 401 )  { // 4.1
    
      if(saveBlink4_1==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "4.1 Draw Povorotniki"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka4_1==false){
              if(DrawPovorotniki == true) { PositionUpCount=120; }
              if(DrawPovorotniki == false){ PositionUpCount=121; }
          OneRazGalochka4_1 = true;
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
          SaveBlink4_1();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  }

  if (MenuLayer == 50 ) {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(30, 7, " 5 Dimensions "); // write something to the internal memory

    u8g2.drawStr(0, 25-3, "5.1 Activate"); // write something to the internal memory 
    u8g2.drawStr(0, 32, "DayLight"); // write something to the internal memory
    if(ActivateDayLight == true)     {      u8g2.drawStr(97, 25-3, "On");   }
    else                             {      u8g2.drawStr(97, 25-3, "Off");  }

    u8g2.drawLine(0, 32+5, 105, 32+5);

    u8g2.drawStr(0, 32+17, "5.2 Brightness");  
    u8g2.drawStr(0, 42+17, "Day Light");
                           u8g2.setCursor(97,32+17);  
                           u8g2.print(BrightnessDayLight);
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  if (MenuLayer == 51 ) {
      u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0, 12, "5.3 Brightness "); // write something to the internal memory 3.3 Volume On Time
    u8g2.drawStr(0, 22, "In Eco Mode");
                           u8g2.setCursor(97,12);  
                           u8g2.print(BrightnessInEcoMode);
    
    u8g2.drawLine(0, 27, 105, 27);

    u8g2.drawStr(0, 39, "5.4 Fading White");
    u8g2.drawStr(0, 49, "When Turning"); // write something to the internal memory 
                           u8g2.setCursor(97,39);  
                           u8g2.print(FadingWhiteWhenTurning);                     
    
    u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
    u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  if (MenuLayer == 52 ) {
      u8g2.clearBuffer();          // clear the internal memory BrightnessOnActivatedPassingLights
      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(0, 12, "5.5 Brightness"); // write something to the internal memory 3.3 Volume On Time
      u8g2.drawStr(0, 22, "On Act. PassingLights");
                           u8g2.setCursor(97,12);  
                           u8g2.print(BrightnessOnActivatedPassingLights);
      /*
      u8g2.drawLine(0, 27, 105, 27);

      u8g2.drawStr(0, 39, "5.4 Fading White");
      u8g2.drawStr(0, 49, "When Turning"); // write something to the internal memory 
                           u8g2.setCursor(97,39);  
                           u8g2.print(FadingWhiteWhenTurning);                     
      */
      u8g2.drawTriangle(110+20,CirclY-5, 95+20,CirclY, 110+20,CirclY+5);  
      u8g2.sendBuffer();          // transfer internal memory to the display 
  }
  
  if (MenuLayer == 501 )  { // 3.1
    
      if(saveBlink5_1==false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "5.1 ActivateDayLight"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka5_1==false){
              if(ActivateDayLight==true) { PositionUpCount=120; }
              if(ActivateDayLight==false){ PositionUpCount=121; }
          OneRazGalochka5_1=true;
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
        SaveBlink5_1();       
      }
      PositionUpCount=constrain(PositionUpCount,120,121); // Ограничить движение галочки вверх вниз
  } 
  if (MenuLayer == 502 ) { // 2.6
      if(saveBlink5_2 == false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.2 BrightnessDayLight"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessDayLight);         

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
          SaveBlink5_2();
      }  
  }
  if (MenuLayer == 503 ) { // 2.6
      if(saveBlink5_3 == false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.3 BrightnessInEcoMode"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessInEcoMode);         

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
          SaveBlink5_3();
      }  
  }
  if (MenuLayer == 504 ) { // 2.6
      if(saveBlink5_4 == false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.4 FadingWhiteWhenTurning"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(FadingWhiteWhenTurning);         

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
          SaveBlink5_4();
      }  
  }
  if (MenuLayer == 505 ) { // 2.6
      if(saveBlink5_5 == false){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.5 BrightnessOnActivatedPassingLights"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessOnActivatedPassingLights);         

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
          SaveBlink5_5();
      }  
  }
// Отрисовка меню

// Включение и выключение габаритов

// При включённых габаритах замер момента когда включаются поворотники. Если поворотник включён то деактиваруется режим засветки белым
if(ActivateDayLight == true){
 // /*

 if(  // Если сейчас работает правый поворотник (светится)
     (PovorotOnRight==true && beginIntModeBlinkR==true  && MenuLayer == -1 ) ||  
     (AutomaticModeActivateR==1 && PovorotOnRight==true && MenuLayer == -1 ) ||  
     (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW && IntelligentMode == 0 && AutomaticModeActivateR==0 && PovorotOnRight==true && MenuLayer == -1) ||   
      (MenuLayer == 101 && PovorotnikiOn==true)
     )
     { 
        ActivateDayLight = 0; // То отключить габариты. И пока горит жёлтый они не будут мерцать.
        AftherOn=1;
     }
     // Если сейчас работает правый поворотник (светится)
// */

///*
  if(  
      (PovorotOnLeft==true && beginIntModeBlinkL==true  && MenuLayer == -1) ||  
      (AutomaticModeActivateL==1 && PovorotOnLeft==true && MenuLayer == -1) ||  
      (digitalRead(LeftButtonPin)==HIGH && digitalRead(RightButtonPin)==LOW && IntelligentMode == 0 && AutomaticModeActivateL==0 && PovorotOnLeft==true && MenuLayer == -1)   
       //MenuLayer == 101
       )
      { //
        ActivateDayLight = 0; // То отключить габариты. И пока горит жёлтый они не будут мерцать.
        AftherOn=1;
      }
 //*/
}
// При включённых габаритах замер момента когда включаются поворотники. Если поворотник включён то деактиваруется режим засветки белым


  if(ActivateDayLight == true){ // Если включены габариты из меню

      if(
      // Повод затемнить от правого поворотника  
      AutomaticModeActivateR == 1 || beginIntModeBlinkR==true ||
      (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW && MenuLayer == -1 && OffPovorotniki == false)
      // Повод затемнить от правого поворотника
      ||
       // Повод затемнить от левого поворотника  
      AutomaticModeActivateL == 1 || beginIntModeBlinkL==true ||
      (digitalRead(LeftButtonPin)==HIGH && digitalRead(RightButtonPin)==LOW && MenuLayer == -1  && OffPovorotniki == false)
      // Повод затемнить от левого поворотника
      )

      {
      RgbwColor white(FadingWhiteWhenTurning);  //Создали белый. С "яркостью" 5.4
       for(int i=0; i<26;++i){strip.SetPixelColor(i, white);} strip.Show(); // Заливка белым (Габариты)   
      }
      
      else{ //Если НЕ МОРГАЕТ поворотник то светить с яркостью из пункта 5.2 или 5.3 Если сейчас ECO режим
         if(Mode == 1 || Mode == 2){ // Если сейчас режимы DRIVE(1) SPORT(2)
            RgbwColor white(BrightnessDayLight);
            for(int i=0; i<26;++i){strip.SetPixelColor(i, white);} strip.Show(); // Заливка белым (Габариты)   
         }                            // Если сейчас режимы DRIVE(1) SPORT(2)
         if(Mode == 0){ // ECO(0)
            RgbwColor white(BrightnessInEcoMode);
            for(int i=0; i<26;++i){strip.SetPixelColor(i, white);} strip.Show(); // Заливка белым (Габариты)   
         }
      }     //Если НЕ МОРГАЕТ поворотник то светить с яркостью из пункта 5.2 или 5.3 Если сейчас ECO режим
     
  }
  
  else{
    //Один раз выключить
    if(OneRazOffGabarit == 1){
        RgbwColor black(0);

        for(int i=0; i<26;++i){ strip.SetPixelColor(i, black); }
        strip.Show();

        OneRazOffGabarit = 0; //Закрыть замочек
    }
     //Один раз выключить
  }
  
 // Serial.print(" AftherOn:" );Serial.print(AftherOn);Serial.println();
 
if(AftherOn == 1){ // Возвращаем свечение габаритов
  ActivateDayLight=1;
  AftherOn=0; //Закрыть замочек
   
}
//Serial.print(" (S)AftherOn " );Serial.print(AftherOn );
//Serial.println();
// Включение и выключение габаритов


 } // while1
}







































//strip.SetBrightness(BrightnessDayLight);// Установить яркость поворотников  
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
void SaveBlink1_6(){ // Анимация моргания слова save в подпункте 1.4
  static int8_t counterSaveBlink1_6;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink1_6++;
      timing = millis(); 
   }
   if(counterSaveBlink1_6 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(0, 7,   "1.6 How Start Automatic Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(5, 25-3, "Pressed Time");  u8g2.drawStr(5, 40-3, "Double click");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_6==false){
              if(AutomaticMode==true) { PositionUpCount=120; }
              if(AutomaticMode==false){ PositionUpCount=121; }
          OneRazGalochka1_6=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(105,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(105,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "    "); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
          
   if(counterSaveBlink1_6 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(0, 7,   "1.6 How Start Automatic Mode"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(5, 25-3, "Pressed Time");  u8g2.drawStr(5, 40-3, "Double click");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_6==false){
              if(AutomaticMode==true) { PositionUpCount=120; }
              if(AutomaticMode==false){ PositionUpCount=121; }
          OneRazGalochka1_6=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(105,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(105,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "save"); 
          u8g2.drawStr(0, 50, "back");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
          saveBlink1_6=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink1_6=0;
   }
          
}
void SaveBlink1_9(){ // Анимация моргания слова save в подпункте 2.1
  static int8_t counterSaveBlink1_9;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink1_9++;
      timing = millis(); 
   }
   if(counterSaveBlink1_9 == 1){
      u8g2.clearBuffer();          // clear the internal memory TactPovorotnikiToLightOrBeep
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "1.9 Tact Povorotniki"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "Light");  u8g2.drawStr(35+10, 40-3, "Beep");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_9==false){
              if(TactPovorotnikiToLightOrBeep==true) { PositionUpCount=120; }
              if(TactPovorotnikiToLightOrBeep==false){ PositionUpCount=121; }
          OneRazGalochka1_9=true;
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
          
   if(counterSaveBlink1_9 == 2){
      u8g2.clearBuffer();          // clear the internal memory TactPovorotnikiToLightOrBeep
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "1.9 Tact Povorotniki"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "Light");  u8g2.drawStr(35+10, 40-3, "Beep");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka1_9==false){
              if(TactPovorotnikiToLightOrBeep==true) { PositionUpCount=120; }
              if(TactPovorotnikiToLightOrBeep==false){ PositionUpCount=121; }
          OneRazGalochka1_9=true;
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
          saveBlink1_9=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink1_9=0;
   }
          
}
void SaveBlink3_7(){
 static int8_t counterSaveBlink3_7;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink3_7++;
      timing = millis(); 
   }
   if(counterSaveBlink3_7 == 1){
       u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          u8g2.drawStr(0, 7,   "1.8 HowLongTimeBeepMute"); // write something to the internal memory
          
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(HowLongTimeBeepMute)/10 ,1);         

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
          
   if(counterSaveBlink3_7 == 2){
       u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          u8g2.drawStr(0, 7,   "1.8 HowLongTimeBeepMute"); // write something to the internal memory
          
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(HowLongTimeBeepMute)/10 ,1);         

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
          saveBlink3_7=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink3_7=0;
   }
          
 }
void SaveBlink3_6(){
 static int8_t counterSaveBlink3_6;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink3_6++;
      timing = millis(); 
   }
   if(counterSaveBlink3_6 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          //u8g2.drawStr(5, 7,   "3.6 HowLongTimeBeep"); // write something to the internal memory
          u8g2.drawStr(5, 7,   "1.7 HowLongTimeBeep");
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(HowLongTimeBeep)/10 ,1);         

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
          
   if(counterSaveBlink3_6 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	TimePressToOnAutoMode
          //u8g2.drawStr(5, 7,   "3.6 HowLongTimeBeep"); // write something to the internal memory
          u8g2.drawStr(5, 7,   "1.7 HowLongTimeBeep");
          u8g2.drawStr(20, 35,  "Sec:");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(60-10,35);  u8g2.print(float(HowLongTimeBeep)/10 ,1);         

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
          saveBlink3_6=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink3_6=0;
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
void SaveBlink3_0(){
  static int8_t counterSaveBlink3_0;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink3_0++;
      timing = millis(); 
   }
   if(counterSaveBlink3_0 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "3.0 Activate Buzzer"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_0==false){
              if(BuzzerOn==true) { PositionUpCount=120; }
              if(BuzzerOn==false){ PositionUpCount=121; }
          OneRazGalochka3_0=true;
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
          
   if(counterSaveBlink3_0 == 2){
        u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "3.0 Activate Buzzer"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka3_0==false){
              if(BuzzerOn==true) { PositionUpCount=120; }
              if(BuzzerOn==false){ PositionUpCount=121; }
          OneRazGalochka3_0=true;
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
          saveBlink3_0=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink3_0=0;
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

void SaveBlink4_1(){
  static int8_t counterSaveBlink4_1;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink4_1++;
      timing = millis(); 
   }
   if(counterSaveBlink4_1 == 1){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "4.1 Draw Povorotniki"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka4_1==false){
              if(DrawPovorotniki == true) { PositionUpCount=120; }
              if(DrawPovorotniki == false){ PositionUpCount=121; }
          OneRazGalochka4_1 = true;
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
          
   if(counterSaveBlink4_1 == 2){
      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "4.1 Draw Povorotniki"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka4_1==false){
              if(DrawPovorotniki == true) { PositionUpCount=120; }
              if(DrawPovorotniki == false){ PositionUpCount=121; }
          OneRazGalochka4_1 = true;
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

          saveBlink4_1=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink4_1=0;
   }
}

void SaveBlink5_1(){
  static int8_t counterSaveBlink5_1;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink5_1++;
      timing = millis(); 
   }
   if(counterSaveBlink5_1 == 1){


      u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "5.1 ActivateDayLight"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka5_1==false){
              if(ActivateDayLight==true) { PositionUpCount=120; }
              if(ActivateDayLight==false){ PositionUpCount=121; }
          OneRazGalochka5_1=true;
          }
          // Исполнить один раз чтоб галочка соответствовала значению
          u8g2.setFont(u8g2_font_7x14_tr);	
          if(PositionUpCount==120){      u8g2.drawStr(95-10,21,    "V");      }
          if(PositionUpCount==121){      u8g2.drawStr(95-10,21+15, "V");      }
          u8g2.drawTriangle(108,62, 128,57, 108,52); 
          u8g2.drawTriangle(20,62, 0,57, 20,52);

          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(105, 50, "   "); 
          u8g2.drawStr(0, 50, "back");

          //u8g2.setFont(u8g2_font_7x14_tf);
          //u8g2.drawStr(52, 60, "0-1");
      
          u8g2.sendBuffer();          // transfer internal memory to the display
   }
   
   if(counterSaveBlink5_1 == 2){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr);
          u8g2.drawStr(5, 7,   "5.1 ActivateDayLight"); // write something to the internal memory
	 
          u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(35+10, 25-3, "On");  u8g2.drawStr(35+10, 40-3, "Off");
      
          // Исполнить один раз чтоб галочка соответствовала значению
          if(OneRazGalochka5_1==false){
              if(ActivateDayLight==true) { PositionUpCount=120; }
              if(ActivateDayLight==false){ PositionUpCount=121; }
          OneRazGalochka5_1=true;
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

          saveBlink5_1=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink5_1=0;
   }
}
void SaveBlink5_2(){
  static int8_t counterSaveBlink5_2;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink5_2++;
      timing = millis(); 
   }
   if(counterSaveBlink5_2 == 1){


     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.2 BrightnessDayLight"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessDayLight);         

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
   
   if(counterSaveBlink5_2 == 2){
           u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.2 BrightnessDayLight"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessDayLight);         

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

          saveBlink5_2=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink5_2=0;
   }
}
void SaveBlink5_3(){
  static int8_t counterSaveBlink5_3;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink5_3++;
      timing = millis(); 
   }
   if(counterSaveBlink5_3 == 1){


     u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.3 BrightnessInEcoMode"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessInEcoMode);         

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
   
   if(counterSaveBlink5_3 == 2){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.3 BrightnessInEcoMode"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessInEcoMode);         

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

          saveBlink5_3=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink5_3=0;
   }
}
void SaveBlink5_4(){
  static int8_t counterSaveBlink5_4;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink5_4++;
      timing = millis(); 
   }
   if(counterSaveBlink5_4 == 1){

    u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.4 FadingWhiteWhenTurning"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(FadingWhiteWhenTurning);         

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
   
   if(counterSaveBlink5_4 == 2){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.4 FadingWhiteWhenTurning"); // write something to the internal memory
          
          //u8g2.drawStr(0, 35,   "Val KM/h: ");
            u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(FadingWhiteWhenTurning);         

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

          saveBlink5_4=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink5_4=0;
   }
}
void SaveBlink5_5(){
  static int8_t counterSaveBlink5_5;
  static unsigned long timing;
   if (millis() - timing > 200){ // Вместо 10000 подставьте нужное вам значение паузы 
      counterSaveBlink5_5++;
      timing = millis(); 
   }
   if(counterSaveBlink5_5 == 1){

    u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.5 BrightnessOnActivatedPassingLights"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessOnActivatedPassingLights);         

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
   
   if(counterSaveBlink5_5 == 2){
          u8g2.clearBuffer();          // clear the internal memory
  
          u8g2.setFont(u8g2_font_6x12_tr); //u8g2.setFont(u8g2_font_7x14B_tr);	
          u8g2.drawStr(0, 7,   "5.5 BrightnessOnActivatedPassingLights"); // write something to the internal memory
          
          u8g2.drawStr(0, 35,   "Persent: ");
          u8g2.setFont(u8g2_font_10x20_tr);	      
    
          u8g2.setCursor(55,35);  u8g2.print(BrightnessOnActivatedPassingLights);         

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

          saveBlink5_5=false; // Этот буль отключает исполнение функции SaveBlink2_1(); 
          counterSaveBlink5_5=0;
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
        if (CirclY == 2) { // Если круг напротив пункта ближ свет  // Тут enum мо��но

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