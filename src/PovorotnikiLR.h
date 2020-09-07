#include <Arduino.h>

unsigned long timingExitAutoToRead;

bool PovorotOnRight = true; // Буль меняет состояние в  int mode
bool PovorotOnLeft = true; 
extern bool AutomaticMode;
extern int8_t TimePressToOnAutoMode;
extern bool AutomaticModeActivateR;
extern bool AutomaticModeActivateL;
extern bool OneRazSavePRK_GE;
//OneRazSavePRK_GE2L
extern bool OneRazSavePRK_GE2L;
extern bool IntelligentMode; 

bool beginIntModeBlinkR;
bool beginIntModeBlinkL;
// SK6812
#include <NeoPixelBus.h>
#include <NeoPixelBrightnessBus.h> // instead of NeoPixelBus.h
// SK6812
const uint16_t PixelCount = 144; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t  PixelPin = 27; 
NeoPixelBrightnessBus<NeoRgbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#define colorSaturation 128

// Начнем с директив препроцессора. ADD_H – это произвольное уникальное имя (обычно используется имя заголовочного файла)
#ifndef PovorotnikiLR_H
#define PovorotnikiLR_H

RgbwColor black(0);
/*
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
//HslColor hslBlack(black);
*/

extern int8_t SpeedPovorotnikBlink;
extern int8_t CountBlinkIntMode;
extern bool OffPovorotniki; // Буль отключающий поворотники при выходе из главного меню
unsigned long timingOffPovorotniki; // Время которое не горят поворотники после выхода из главного экрана
//extern bool RightInt;

bool KIF_AvailableR; //Для выключения автоматического режима (резко) работающего после int
bool KIF_AvailableL; //Для выключения автоматического режима (резко) работающего после int

//bool OneRazAutoSetPolzunokR; // В режиме авто один раз присвоить положение правого ползунка в старое значение. Для того чтоб при авто моргании право нажатие влево просто отключило режим
bool OKlightFromAutoTomanualL =false; //Когда работает правый поворотник
unsigned long timingMozjnoBlinkL; // Для таймера в режиме обычного нажатия клавишь. Связано с режимом авто и кнопкой отмены в обратную сторону

bool OKlightFromAutoTomanualR =false; //Когда работает правый поворотник
unsigned long timingMozjnoBlinkR; // Для таймера в режиме обычного нажатия клавишь. Связано с режимом авто и кнопкой отмены в обратную сторону

int8_t OldPositionRightCount; // Для автомат режима
bool DoubleL=false; //Говорит что есть дабл левый клик
bool DoubleR=false; //Говорит что есть дабл правый клик

bool Stop=false; //Флаг прекращения моргания при заверншении авомат режима
int8_t OldPositionRightCountInt=1; // Для интеллект режима прошлая позиция ползунка

extern bool EnterOnTheAutoMode;
 int8_t CountBlinkOnIntModeR;//Cчётчик морганий фактических правого поворотника
  int8_t CountBlinkOnIntModeL;//Cчётчик морганий фактических правого поворотника
void PovorotnikiRightOff(); // Прототип функции выключения поворотника правого
void PovorotnikiLeftOff();  // Прототип функции выключения поворотника левого
void Povorotniki(){

  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
    static unsigned long timingRightBlink; // Используется для таймера моргания правого поворотника
    static unsigned timingLeftBlink;  // Используется для таймера моргания левого поворотника

    static unsigned long timingPressButtonR;
    static unsigned long timingPressButtonL;

    if(OffPovorotniki == true){                                                  // [При выходе из главного меню] Если поворотникам нельзя моргать
        if(millis() - timingOffPovorotniki >500){ // Если прошло пол секунды
         if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)  
            PovorotnikiRightOff();                // Отключить поворотники
            PovorotnikiLeftOff();
         }
            timingPressButtonR=millis(); // Чтобы сразу не включался автоматический режим после выхода из меню при зажатой кнопке
            timingPressButtonL=millis(); // Чтобы сразу не включался автоматический режим после выхода из меню при зажатой кнопке. До пары
            OffPovorotniki = false; // Разрешить моргать поворотникам
        }
    }
  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
  // OffPovorotniki используются для того чтоб при выходе из главного меню небыло сразу морганий

    if (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==HIGH ){ // [При входе в главное меню]
        if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
        PovorotnikiRightOff(); 
        PovorotnikiLeftOff();
        }
        beginIntModeBlinkR = false; beginIntModeBlinkL = false;
        PositionRightCount =0;     // Cравнять и обнулить. Нужно когда моргает Int mode и в это время попытка есть входа в меню
        OldPositionRightCountInt=0;// Cравнять и обнулить. Нужно когда моргает Int mode и в это время попытка есть входа в меню

        AutomaticModeActivateR=0; //Чтоб при работе автомат режима и входе в меню не было мерцания
        AutomaticModeActivateL=0; //Чтоб при работе автомат режима и входе в меню не было мерцания
     } //Чтоб при нажатии 2х кнопок не светило  // Один раз отключить правый поворотник

        // ============================================          Часть Простое нажатие клавиш        ============================================ 
// /*

    if (millis() - timingExitAutoToRead > 500){                                  // [При выходе из автоматического режима]
    if(Stop == true){ //Если из автоматического режима пришёл сигнал о прекращении работы автоматического режима
      if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
           PovorotnikiRightOff();
           PovorotnikiLeftOff();
      }
           Stop = false;
           PovorotOnRight=1;
           PovorotOnLeft=1;
    }
 }


    if(Stop == false){ // Подписать
        if(IntelligentMode == 0){ // Если intelligentmode выключен
            if(OffPovorotniki == false){ // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
 

            // Таймер считает если сейчас OKlightFromAutoTomanualR == false то через 500мсек сделать тру и разрешить моргать
            
            if (millis() - timingMozjnoBlinkL > 500){ // Вместо 1000 подставьте нужное вам значение паузы  
                OKlightFromAutoTomanualL=true;
            }
            if (millis() - timingMozjnoBlinkR > 500){ // Вместо 1000 подставьте нужное вам значение паузы  
                OKlightFromAutoTomanualR=true;
            }
     // <- Поворотник вправо
        if(  (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW)        ){ // Если кнопка право нажата
            #ifdef YarkiyYellow
            RgbwColor  color = RgbwColor(200, 255, 0,0); //Создали жёлтый
            #else
            RgbwColor  color = RgbwColor(5, 5, 0,0); //Создали лёгкий жёлтый
            #endif
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal;if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingRightBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                PovorotOnRight = !PovorotOnRight;
                timingRightBlink = millis(); 
            }
            // Блинкер
            if(PovorotOnRight == true){ // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                for(int i=0; i<13;++i){strip.SetPixelColor(i, color); } //Вправо
                strip.Show();    
                      
            }
            else{ 
                if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)  
                    PovorotnikiRightOff(); // Если не светят белым - залить чёрным
                }
            }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
            // Блинкер
                  
        }                                  // Если кнопка право нажата
        else{                          // Если кнопка право отпущена   
            if(AutomaticModeActivateR != true){ //Если включен автомат режим то убираем быстрое моргание при отпускании
                timingRightBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
                if(OKlightFromAutoTomanualR == 1){
                    PovorotOnRight=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света  
                }
                if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)      
                    PovorotnikiRightOff();     //  // Один раз отключить правый поворотник
                }
            }
        }                    
      // <- Поворотник вправо          
      // <- Поворотник влево
        if(digitalRead(RightButtonPin)==LOW && digitalRead(LeftButtonPin)==HIGH   ){ // Если кнопка лево нажата
            #ifdef YarkiyYellow
            RgbwColor  color = RgbwColor(200, 255, 0,0); //Создали жёлтый
            #else
            RgbwColor  color = RgbwColor(5, 5, 0,0); //Создали лёгкий жёлтый
            #endif
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal; if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingLeftBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                PovorotOnLeft = !PovorotOnLeft;
                timingLeftBlink = millis(); 
            }
            // Блинкер
            if(PovorotOnLeft == true){ for(int i=13; i<26;++i){strip.SetPixelColor(i, color);}strip.Show();  }// Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
            else{ 
                if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)  
                    PovorotnikiLeftOff();
                }
            } // turn off the pixels // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ
            // Блинкер
            
        }                                                                           // Если кнопка лево нажата
        else{                          // Если кнопка лево отпущена 
            if(AutomaticModeActivateL != true){ //Если включен автомат режим то убираем быстрое моргание при отпускании
                timingLeftBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
                if(OKlightFromAutoTomanualL == 1){
                    PovorotOnLeft=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света  
                }
                if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)   
                    PovorotnikiLeftOff();     // Если кнопка лево отпущена // Один раз отключить правый поворотник
                }
            }   
        }
       //}     
      // <-
  
    }     // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
    }   
}
        // ============================================          Часть Простое нажатие клавиш        ============================================ 

    // ============================================          Часть Интеллигент режим         ============================================ 
    //static 
    //static bool OneRazSavePRKOld_IntMode;
    static bool EndedL=false; // Для интеллигент режима. Если мы прервали пока он не доморгал то чтоб с этого момента моргал заданное кол-во раз
    static bool EndedR=false;
 
    static bool LeftInt=false;
    
    //static int8_t CountBlinkOnIntModeL; //Cчётчик морганий фактических левого поворотника
    static bool HelloFromAutoMode=false; //Нужен чтобы после завершения автомат режима небыло остаточных морганий со счётчика int
    if(IntelligentMode == 1 ){
        if(OffPovorotniki == false){ // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)  
            if(HelloFromAutoMode==true){ OldPositionRightCountInt=PositionRightCount; HelloFromAutoMode=false; }// Буль включается после завершения работы автоматического режима AutomaticModeActivateR. Нужен чтоб не моргало после завершения
                  
            if(PositionRightCount > OldPositionRightCountInt ){
                 //if(AutomaticModeActivateR ==false){ //Авто мод лич
                    beginIntModeBlinkR=true;
                 //}                                  //Авто мод лич
            }    // Право ON Серелина Off Off Лево On
            
            if(PositionRightCount == OldPositionRightCountInt){ // Тут если просто режим int и моргает и мы нажимаем в обратную сторону то гаснет
                beginIntModeBlinkL=false; beginIntModeBlinkR=false;   // Отключение режима моганий в простом int  
                CountBlinkOnIntModeL=0;   CountBlinkOnIntModeR=0; // Cброс количества морганий которое нужно сделать. 
                
                /*
                if(AutomaticModeActivateL != true || AutomaticModeActivateR != true){ // Если мы работаем в автоматическом режиме после int то не мерцать!
                    PovorotnikiRightOff();
                    PovorotnikiLeftOff();
                }
                */
                if(AutomaticModeActivateR != true){   // Если мы работаем в автоматическом режиме после int то не мерцать!
                    if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)  
                        PovorotnikiRightOff();
                    }
                } 

                if(AutomaticModeActivateL != true){ 
                    if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)    
                        PovorotnikiLeftOff(); 
                    }
                } // Если мы работаем в автоматическом режиме после int то не мерцать!

                }    
            if(PositionRightCount < OldPositionRightCountInt ){ beginIntModeBlinkL=true;}
                
            if(PositionRightCount-OldPositionRightCountInt >0  ){
               if(PositionRightCount-OldPositionRightCountInt>=2){
                   Serial.println("R");
                   OldPositionRightCountInt++;//EndedR=false;
                   CountBlinkOnIntModeR=0; //Cбросить счётчик если уже моргает

                   timingRightBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света и нужно кол-во раз
                   PovorotOnRight=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света  
               } 
            }
            if(PositionRightCount-OldPositionRightCountInt <0  ){
                if(PositionRightCount-OldPositionRightCountInt<=-2){
                    Serial.println("L");
                    OldPositionRightCountInt--;
                    CountBlinkOnIntModeL=0; //Cбросить счётчик если уже моргает

                    timingLeftBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света и нужно кол-во раз
                    PovorotOnLeft=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света  
                }
            }
    
            //Для правого поворота intellingent                          
            if(beginIntModeBlinkR == true ) { //Если исполняется интеллигент режим правого поворота
            
                EndedR=false; // Ставим буль в положение Незавершено ( Нет нужного кол-ва морганий)          
                #ifdef YarkiyYellow
                RgbwColor  color = RgbwColor(200, 255, 0,0); //Создали жёлтый
                #else
                RgbwColor  color = RgbwColor(5, 5, 0,0); //Создали лёгкий жёлтый
                #endif
                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                uint16_t TempInvertVal;if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                if(AutomaticModeActivateR==false){ //Авто лич

                if (millis() - timingRightBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                    PovorotOnRight = !PovorotOnRight;
                    CountBlinkOnIntModeR++;//Serial.println(CountBlinkOnIntModeR);  
                    timingRightBlink = millis(); }
               
                // Сам блинкер
                    if(PovorotOnRight == true){ for(int i=0; i<13;++i){strip.SetPixelColor(i, color); }  strip.Show();} // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                    else{  
                        if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                        PovorotnikiRightOff(); 
                        }
                    }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels не тут
                }                                  //Авто лич
                // Сам блинкер

                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое
                    if ( (CountBlinkOnIntModeR) >= (CountBlinkIntMode*2)+1 ){ 
                            beginIntModeBlinkR = false; EndedR=true;//Serial.println("EndedR=true"); //PositionRightCount = OldPositionRightCountInt; 
                            //PovorotOnRight=true;               
                      if (digitalRead(RightButtonPin) ==LOW){
                          if(AutomaticModeActivateR != true){ //Авто лич
                                PositionRightCount = OldPositionRightCountInt;
                          }                                   //Авто лич  
                          PovorotOnRight=true;}  //Только когда мы отпускаем кнопку  
                    } 
                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое
                
            }        
            //Для правого поворота intellingent 

            //Для левого поворота intellingent          
            if(beginIntModeBlinkL == true ) { //Если исполняется интеллигент режим правого поворота           
                //beginIntModeBlinkR = false; PovorotnikiRightOff();
                EndedL=false;   
                #ifdef YarkiyYellow
                RgbwColor  color = RgbwColor(200, 255, 0,0); //Создали жёлтый
                #else
                RgbwColor  color = RgbwColor(5, 5, 0,0); //Создали лёгкий жёлтый
                #endif
                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                uint16_t TempInvertVal;if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                if(AutomaticModeActivateL==false){ //Авто лич L

                    if (millis() - timingLeftBlink > TempInvertVal ){PovorotOnLeft = !PovorotOnLeft;
                        CountBlinkOnIntModeL++;Serial.println(CountBlinkOnIntModeL);
                        timingLeftBlink = millis();} // Таймер отсчёта включения и выключения правого поворотника
                            
                    // Сам блинкер
                    if(PovorotOnLeft == true){for(int i=13; i<26;++i){strip.SetPixelColor(i, color); }  strip.Show();  }// Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ   
                    else{  
                        if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                        PovorotnikiLeftOff(); 
                        }
                          }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
                }                                  //Авто лич L
                // Сам блинкер

                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое
                      if ( (CountBlinkOnIntModeL) >= (CountBlinkIntMode*2)+1  ){ 
                        beginIntModeBlinkL = false; EndedL=true;
                        //Serial.println("EndedL=true");PositionRightCount = OldPositionRightCountInt;PovorotOnLeft=true;
                        if (digitalRead(LeftButtonPin) ==LOW){
                            if(AutomaticModeActivateL != true){ //Авто лич L
                                PositionRightCount = OldPositionRightCountInt; 
                            }                                   //Авто лич L
                            PovorotOnLeft=true;}  //Только когда мы отпускаем кнопку 
                      }          
                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое      
            }
            //Для левого поворота intellingent
 
        }  
    }    
    // ============================================          Часть Интеллигент режим         ============================================

    // ============================================          Часть автоматический режим         ============================================ 
  
    if(AutomaticMode == 1 ){ // Если из настроек мы получили разрешение активировать автоматический режим
     //if(IntelligentMode != 1){ // ==0 //Если сейчас не интеллигент режим
      if(OffPovorotniki == false){
      
      // Захват автоматического режима по времени удержания кнопки   
        if(EnterOnTheAutoMode==1){ // Если в меню выставлен пункт вкл авто режим по долгому нажатию
            
            if( (digitalRead(RightButtonPin)==HIGH) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата правая кнопка и не зажата левая
                if (millis() - timingPressButtonR > (TimePressToOnAutoMode*100) ){ // Вместо 500 подставьте нужное вам значение паузы 
                    Serial.println ("AutomaticModeActivateR == true"); //OldPositionRightCount = PositionRightCount; //!!! 
                    AutomaticModeActivateR=true; //Включение автоматического режима правого поворотника(Если в булях выставлено On)
                }
            }
            if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==HIGH) ){  //Если зажата левая кнопка и не зажата правая
                if (millis() - timingPressButtonL > (TimePressToOnAutoMode*100) ){ // Вместо 500 подставьте нужное вам значение паузы 
                    Serial.println ("AutomaticModeActivateL == true");  
                    AutomaticModeActivateL=true; //Включение автоматического режима левого поворотника(Если в булях выставлено On)
                }
            }
            if( digitalRead(RightButtonPin)==LOW && digitalRead(LeftButtonPin)==LOW ){  timingPressButtonR = millis();timingPressButtonL = millis(); }
            // Захват автоматического режима по времени удержания кнопки
        
        }                        // Если в меню выставлен пункт вкл авто режим по долгому нажатию
      // Захвата автоматического режима по двойному нажатию кнопки
        if(EnterOnTheAutoMode==0){ // Если в меню выставлен пункт вкл авто режим по двойному щелчку

//Пачка переменных для Захват левой кнопки double click 
    static unsigned long TimeDoublePressedL;
    static unsigned long TimeDoublePressedL2;
    static int8_t DoublePressedStepL=1;

    static bool OneShowDoubleL1;// Один раз зафиксить двойное нажатие левое
    static bool OneShowDoubleL2;
//Пачка переменных для Захват левой кнопки double click


//Пачка переменных для Захват правой кнопки double click 
    static unsigned long TimeDoublePressedR;
    static unsigned long TimeDoublePressedR2;
    static int8_t DoublePressedStepR=1;

    static bool OneShowDoubleR1;// Один раз зафиксить двойное нажатие левое
    static bool OneShowDoubleR2;
//Пачка переменных для Захват правой кнопки double click

//Захват левой кнопки double click
if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==HIGH) ){  //Если зажата левая кнопка и не зажата правая
    if(DoublePressedStepL==1){ OneShowDoubleL1=false;
        TimeDoublePressedL=millis();
        DoublePressedStepL=2;
    }
    if(DoublePressedStepL==3){
        TimeDoublePressedL2=millis();
        DoublePressedStepL=4;
    }
    
}
if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата левая кнопка и не зажата правая
    if(DoublePressedStepL==2){
        DoublePressedStepL=3;OneShowDoubleL2=false;
    }
    if(DoublePressedStepL==4){DoublePressedStepL=1;}

    if(DoubleL==true){
        DoubleL=false;
    }
}
//Захват левой кнопки double click
// /*
//Захват правой кнопки double click
if( (digitalRead(RightButtonPin)==HIGH) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата левая кнопка и не зажата правая
    if(DoublePressedStepR==1){ OneShowDoubleR1=false;
        TimeDoublePressedR=millis();
        DoublePressedStepR=2;
    }
    if(DoublePressedStepR==3){
        TimeDoublePressedR2=millis();
        DoublePressedStepR=4;
    }
    
}
if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата левая кнопка и не зажата правая
    if(DoublePressedStepR==2){
        DoublePressedStepR=3;OneShowDoubleR2=false;
    }
    if(DoublePressedStepR==4){DoublePressedStepR=1;}

    if(DoubleR==true){
        DoubleR=false;
    }
}
//Захват правой кнопки double click
// */ 
//Печать и обработка левой кнопки double click to auto mode
    //Serial.print(" DoublePressedStepL:" );Serial.print(DoublePressedStepL); Serial.print(" TimeDoublePressedL:" );Serial.print(TimeDoublePressedL); Serial.print(" TimeDoublePressedL2:" );Serial.print(TimeDoublePressedL2);
    if(TimeDoublePressedL2>TimeDoublePressedL){
        //Serial.print(" TimeDoublePressedL2-TimeDoublePressedL " );Serial.print( TimeDoublePressedL2-TimeDoublePressedL );
        if(OneShowDoubleL1==false){ //OneShowDoubleL == true
            if((TimeDoublePressedL2-TimeDoublePressedL) < 380) { 
                AutomaticModeActivateL=true;  OldPositionRightCount = PositionRightCount;DoubleL=true;Serial.println("Double L");
            }
            OneShowDoubleL1=true;
        }                          //OneShowDoubleL == true
    }
    if(TimeDoublePressedL2<TimeDoublePressedL){
        //Serial.print(" TimeDoublePressedL-TimeDoublePressedL2 " );Serial.print( TimeDoublePressedL-TimeDoublePressedL2 );
        if(OneShowDoubleL2==false){
            if((TimeDoublePressedL-TimeDoublePressedL2) < 380) { 
                Serial.println("Double L");AutomaticModeActivateL=true;DoubleL=true;//!
                OldPositionRightCount = PositionRightCount; 
                }
        OneShowDoubleL2=true;
        }
    }
    //Serial.print(" AutomaticModeActivateL " );Serial.print( AutomaticModeActivateL );
    //Serial.print(" PositionRightCount " );Serial.print( PositionRightCount );
    //Serial.print(" OldPositionRightCount " );Serial.print( OldPositionRightCount );
 //Serial.print(" EnterOnTheAutoMode " );Serial.print( EnterOnTheAutoMode );
//Печать и обработка левой кнопки double click to auto mode

// /*
//Печать и обработка правой кнопки double click to auto mode
    //Serial.print(" DoublePressedStepR:" );Serial.print(DoublePressedStepR); Serial.print(" TimeDoublePressedR:" );Serial.print(TimeDoublePressedR); Serial.print(" TimeDoublePressedR2:" );Serial.print(TimeDoublePressedR2);
    if(TimeDoublePressedR2>TimeDoublePressedR){
        //Serial.print(" TimeDoublePressedR2-TimeDoublePressedR " );Serial.print( TimeDoublePressedR2-TimeDoublePressedR );
        if(OneShowDoubleR1==false){ //OneShowDoubleL == true
            if((TimeDoublePressedR2-TimeDoublePressedR) < 380) { 
                Serial.println("Double R"); AutomaticModeActivateR=true;DoubleR=true;//!
                OldPositionRightCount = PositionRightCount; 
                }
            OneShowDoubleR1=true;
        }                          //OneShowDoubleL == true
    }
    if(TimeDoublePressedR2<TimeDoublePressedR){
       // Serial.print(" TimeDoublePressedR-TimeDoublePressedR2 " );Serial.print( TimeDoublePressedR-TimeDoublePressedR2 );
        if(OneShowDoubleR2==false){
            if((TimeDoublePressedR-TimeDoublePressedR2) < 380) {
                Serial.println("Double R"); AutomaticModeActivateR=true;DoubleR=true;//!
                OldPositionRightCount = PositionRightCount; 
                }
        OneShowDoubleR2=true;
        }
    }
//Печать и обработка правой кнопки double click to auto mode
// */
   // Serial.println();
    // */
    // Захвата автоматического режима по двойному нажатию кнопки



} // Если в меню выставлен пункт вкл авто режим по двойному щелчку
        // Один раз присвоить значение текущего ползунка право
          
      if(DoubleR==false){ //DOUBLE CLICK Если дабл клик сработал кнопка отпустилась то так правельно приравняет и неуспеет испортить OldPositionRightCount = PositionRightCount
        if(AutomaticModeActivateR == true ) {//Если исполняется автоматический режим правого поворота
            // Один раз сохранить значение правого ползунка в переменную прошлого состояния
            if(OneRazSavePRK_GE == false){
                OldPositionRightCount = PositionRightCount;
                OneRazSavePRK_GE = true;     
            }    
            // Один раз сохранить значение правого ползунка в переменную прошлого состояния
            
              if(PositionRightCount > OldPositionRightCount){
                  AutomaticModeActivateR = false; // Отключить автоматический режим правого поворотника
                  OneRazSavePRK_GE = false;
                  beginIntModeBlinkR=0; //Чтобы после инта и выкл автомата сразу гасло ^^
              }
              if(PositionRightCount < OldPositionRightCount){
                  AutomaticModeActivateR = false; // Отключить автоматический режим правого поворотника
                  //AutomaticModeActivateL = true; // Включить автоматический режим левого поворотника
                  if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                    PovorotnikiRightOff();
                    PovorotnikiLeftOff();
                  }
                  timingMozjnoBlinkL=millis();OKlightFromAutoTomanualL = false;
                  Stop = true;timingExitAutoToRead = millis(); 
                  //OffPovorotniki=1;timingOffPovorotniki=millis();
                  //PovorotOnLeft=false;  timingLeftBlink = millis(); 
                  OneRazSavePRK_GE = false;
              }
        PovorotOnLeft = false;
      }
      }//DOUBLE CLICK Если дабл клик сработал кнопка отпустилась то так правельно приравняет и неуспеет испортить OldPositionRightCount = PositionRightCount

      if(DoubleL==false){ //DOUBLE CLICK Если дабл клик сработал кнопка отпустилась то так правельно приравняет и неуспеет испортить OldPositionRightCount = PositionRightCount
        if(AutomaticModeActivateL == true ) {//Если исполняется автоматический режим правого поворота
          // Один раз сохранить значение правого ползунка в переменную прошлого состояния
          if(OneRazSavePRK_GE2L == false){
              OldPositionRightCount = PositionRightCount;
              OneRazSavePRK_GE2L = true;     
          }    
          // Один раз сохранить значение правого ползунка в переменную прошлого состояния
          
      //Обработка нажатий в автоматическом режиме левого поворотника
                   
              if(PositionRightCount < OldPositionRightCount){               
                  AutomaticModeActivateL = false; // Отключить автоматический режим правого поворотника
                  OneRazSavePRK_GE2L = false;
              }
              if(PositionRightCount > OldPositionRightCount){
                  AutomaticModeActivateL = false; // Отключить автоматический режим правого поворотника
                  //AutomaticModeActivateR = true; // Включить автоматический режим левого поворотника
                  if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                    PovorotnikiRightOff();
                    PovorotnikiLeftOff();
                  }
                  timingMozjnoBlinkR=millis();OKlightFromAutoTomanualR = false;
                  Stop = true;timingExitAutoToRead = millis(); 
                  OneRazSavePRK_GE2L = false;
              }
        PovorotOnRight = false;
      //Обработка нажатий в автоматическом режиме левого поворотника
      } 
      }//DOUBLE CLICK Если дабл клик сработал кнопка отпустилась то так правельно приравняет и неуспеет испортить OldPositionRightCount = PositionRightCount

   } 
  //}                 // Если из настроек мы получили разрешение активировать автоматический режим
   
        // Правый        
        if(AutomaticModeActivateR==true){
            #ifdef YarkiyYellow
            RgbwColor  color = RgbwColor(200, 255, 0,0); //Создали жёлтый
            #else
            RgbwColor  color = RgbwColor(5, 5, 0,0); //Создали лёгкий жёлтый
            #endif
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal;if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250);}if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162);  }                    
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingRightBlink > TempInvertVal ){PovorotOnRight = !PovorotOnRight;timingRightBlink = millis(); } // Таймер отсчёта включения и выключения правого поворотника
                
            // Блинкер
            if(PovorotOnRight == true){for(int i=0; i<13;++i){strip.SetPixelColor(i, color);} strip.Show();   }        // Если включен по таймеру буль светится правому поворотнику то зажечься правому ЖЁЛТОМУ                          
            else{  
                if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)
                PovorotnikiRightOff();
                }
            }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
            // Блинкер
            KIF_AvailableR = true;
            HelloFromAutoMode=true;//Чтоб не моргало в инте после авто режима
        }
        else{    
            //PovorotnikiRightOff();
        }
        // Правый 

        // Левый
        if(AutomaticModeActivateL==true){
            #ifdef YarkiyYellow
            RgbwColor  color = RgbwColor(200, 255, 0,0); //Создали жёлтый
            #else
            RgbwColor  color = RgbwColor(5, 5, 0,0); //Создали лёгкий жёлтый
            #endif
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal;
            if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }
            if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingLeftBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                PovorotOnLeft = !PovorotOnLeft;
                timingLeftBlink = millis(); 
            }
        //Serial.print("TempInvertVal: ");Serial.print(TempInvertVal);
        //Serial.print("PovorotOnLeft: ");Serial.print(PovorotOnLeft);
        //Serial.print("AutomaticModeActivateL: ");Serial.println(AutomaticModeActivateL);

            // Блинкер
            if(PovorotOnLeft == true){for(int i=13; i<26;++i){strip.SetPixelColor(i, color); }strip.Show();}    // Если включен по таймеру буль светится  //Влево поворотнику то зажечься ЖЁЛТОМУ    
            else{
                if(ActivateDayLight!=1){ //Лич! Если не включён режим day light (чтоб не было мерцания)  
                PovorotnikiLeftOff();
                }
                   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
            // Блинкер
            KIF_AvailableL = true;
            HelloFromAutoMode=true;//Чтоб не моргало в инте после авто режима
        }
        else{
            //PovorotnikiRightOff();
        }
        // Левый

   // */
 }//Если сейчас не интеллигент режим

  // Serial.print(" TempInvertVal:");Serial.print(TempInvertVal);
    // ============================================          Часть автоматический режим         ============================================ 
    /*
    Serial.print("AutomaticModeActivateR:");Serial.print(AutomaticModeActivateR);
    Serial.print(" AutomaticModeActivateL:");Serial.print(AutomaticModeActivateL);  
    Serial.print(" OKlightFromAutoTomanualL:");Serial.print(OKlightFromAutoTomanualL);
    Serial.print(" OKlightFromAutoTomanualR:");Serial.print(OKlightFromAutoTomanualR);  
    Serial.print(" PositionRightCount:");Serial.print(PositionRightCount);
    Serial.print(" OldPositionRightCount:");Serial.print(OldPositionRightCount);        
    Serial.print(" PovorotOnLeft:");Serial.print(PovorotOnLeft);
    Serial.print(" PovorotOnRight:");Serial.print(PovorotOnRight);
    */
    //Serial.println();
    /*
     Serial.print(" PositionRightCount:" );      Serial.print(PositionRightCount );
            Serial.print(" OldPositionRightCountInt:" );Serial.print(OldPositionRightCountInt );

            Serial.print(" begIntModeBlinkL:" );      Serial.print(beginIntModeBlinkL );
            Serial.print(" begIntModeBlinkR:" );      Serial.print(beginIntModeBlinkR );

            Serial.print(" CountBlinkOnIntModeR:" );      Serial.print(CountBlinkOnIntModeR );
            Serial.print(" CountBlinkOnIntModeL:" );      Serial.print(CountBlinkOnIntModeL );
            
            Serial.print(" EndedR:" );      Serial.print(EndedR ); 
            Serial.print(" EndedL:" );      Serial.print(EndedL ); 
            Serial.print(" PovorotOnRight:" );      Serial.print(PovorotOnRight );
            Serial.print(" PovorotOnLeft:" );      Serial.print(PovorotOnLeft ); 
            
            //Serial.print(" OffPovorotniki:" );      Serial.print(OffPovorotniki );
            */
}
 

void PovorotnikiRightOff(){
    for(int i=0; i<13;++i){
        strip.SetPixelColor(i, black);
    }
    strip.Show();  
}
void PovorotnikiLeftOff(){
    for(int i=13; i<26;++i){
          strip.SetPixelColor(i, black);
    }
      strip.Show();
}
#endif

  /*
              Serial.print("AutomaticModeActivateL: ");Serial.print(AutomaticModeActivateL);
              Serial.print(" AutomaticModeActivateR: ");Serial.print(AutomaticModeActivateR);
              Serial.print(" PositionRightCount: ");Serial.print(PositionRightCount);
              Serial.print(" OldPositionRightCount: ");Serial.println(OldPositionRightCount);
              */

             /*
              Serial.print("AutomaticModeActivateL: ");Serial.print(AutomaticModeActivateL);
              Serial.print(" AutomaticModeActivateR: ");Serial.print(AutomaticModeActivateR);
              Serial.print(" PositionRightCount: ");Serial.print(PositionRightCount);
              Serial.print(" OldPositionRightCount: ");Serial.println(OldPositionRightCount);
              */

             /*
                Serial.print(" CountBlinkOnIntMode:" ); Serial.print(CountBlinkOnIntMode);
                Serial.print(" SpeedPovorotnikBlink:" );Serial.print(SpeedPovorotnikBlink );
                Serial.print(" CountBlinkIntMode:" );   Serial.print(CountBlinkIntMode );
                Serial.println();
*/