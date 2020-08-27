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
// SK6812
const uint16_t PixelCount = 66; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t  PixelPin = 12; 
NeoPixelBus<NeoRgbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#define colorSaturation 128

// Начнем с директив препроцессора. ADD_H – это произвольное уникальное имя (обычно используется имя заголовочного файла)
#ifndef PovorotnikiLR_H
#define PovorotnikiLR_H

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

extern int8_t SpeedPovorotnikBlink;
extern int8_t CountBlinkIntMode;
extern bool OffPovorotniki; // Буль отключающий поворотники при выходе из главного меню
unsigned long timingOffPovorotniki; // Время которое не горят поворотники после выхода из главного экрана
extern bool RightInt;

//bool KillIntFromAuto; // Буль призван при прерывании авто режима сразу выключить поворотник
bool KIF_AvailableR; //Для выключения автоматического режима (резко) работающего после int
bool KIF_AvailableL; //Для выключения автоматического режима (резко) работающего после int

//bool OneRazAutoSetPolzunokR; // В режиме авто один раз присвоить положение правого ползунка в старое значение. Для того чтоб при авто моргании право нажатие влево просто отключило режим.
bool OKlightFromAutoTomanualL =false; //Когда работает правый поворотник
unsigned long timingMozjnoBlinkL; // Для таймера в режиме обычного нажатия клавишь. Связано с режимом авто и кнопкой отмены в обратную сторону

bool OKlightFromAutoTomanualR =false; //Когда работает правый поворотник
unsigned long timingMozjnoBlinkR; // Для таймера в режиме обычного нажатия клавишь. Связано с режимом авто и кнопкой отмены в обратную сторону

int8_t OldPositionRightCount; // Для автомат режима

bool Stop=false;
int8_t OldPositionRightCountInt=1; // Для интеллект режима прошлая позиция ползунка
void PovorotnikiRightOff(); // Прототип функции выключения поворотника правого
void PovorotnikiLeftOff();  // Прототип функции выключения поворотника левого
void Povorotniki(){
   
  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
    static unsigned long timingRightBlink; // Используется для таймера моргания правого поворотника
    static unsigned timingLeftBlink;  // Используется для таймера моргания левого поворотника

    static unsigned long timingPressButtonR;
    static unsigned long timingPressButtonL;

    if(OffPovorotniki == true){ // Если поворотникам нельзя моргать
        if(millis() - timingOffPovorotniki >500){ // Если прошло пол секунды
            PovorotnikiRightOff();                // Отключить поворотники
            PovorotnikiLeftOff();
            timingPressButtonR=millis(); // Чтобы сразу не включался автоматический режим после выхода из меню при зажатой кнопке
            timingPressButtonL=millis(); // Чтобы сразу не включался автоматический режим после выхода из меню при зажатой кнопке. До пары
            OffPovorotniki = false; // Разрешить моргать поворотникам
        }
    }
  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
  // OffPovorotniki используются для того чтоб при выходе из главного меню небыло сразу морганий

    if (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==HIGH ){ 
        PovorotnikiRightOff(); PovorotnikiLeftOff();
        beginIntModeBlinkR = false; beginIntModeBlinkL = false;
        PositionRightCount =0;     // Cравнять и обнулить. Нужно когда моргает Int mode и в это время попытка есть входа в меню
        OldPositionRightCountInt=0;// Cравнять и обнулить. Нужно когда моргает Int mode и в это время попытка есть входа в меню

        AutomaticModeActivateR=0; //Чтоб при работе автомат режима и входе в меню не было мерцания
        AutomaticModeActivateL=0; //Чтоб при работе автомат режима и входе в меню не было мерцания
     } //Чтоб при нажатии 2х кнопок не светило  // Один раз отключить правый поворотник

        // ============================================          Часть Простое нажатие клавиш        ============================================ 
// /*

 if (millis() - timingExitAutoToRead > 500){ // Вместо 10000 подставьте нужное вам значение паузы 
    if(Stop == true){ //Если из автоматического режима пришёл сигнал о прекращении работы автоматического режима
           PovorotnikiRightOff();
           PovorotnikiLeftOff();
           Stop = false;
           PovorotOnRight=1;
           PovorotOnLeft=1;
    }
 }


 
    
   
if(Stop == false){
    if(IntelligentMode == 0){ // Если intelligentmode выключен
        if(OffPovorotniki == false){ // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
      // <- Поворотник вправо

      // Таймер считает если сейчас OKlightFromAutoTomanualR == false то через 500мсек сделать тру и разрешить моргать
      
      if (millis() - timingMozjnoBlinkL > 500){ // Вместо 1000 подставьте нужное вам значение паузы  
        OKlightFromAutoTomanualL=true;
      }
      if (millis() - timingMozjnoBlinkR > 500){ // Вместо 1000 подставьте нужное вам значение паузы  
        OKlightFromAutoTomanualR=true;
      }

        if(     (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW)        ){ // Если кнопка право нажата
            RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal;
            if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }
            if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingRightBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                PovorotOnRight = !PovorotOnRight;
                timingRightBlink = millis(); 
            }
            // Блинкер
            if(PovorotOnRight == true){ // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                for(int i=0; i<13;++i){strip.SetPixelColor(i, color); } //Впрво
                 strip.Show();              
            }
            else{  PovorotnikiRightOff();   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
            // Блинкер
                  
        }                                  // Если кнопка право нажата
        else{                          // Если кнопка право отпущена   
            if(AutomaticModeActivateR != true){ //Если включен автомат режим то убираем быстрое моргание при отпускании
                timingRightBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
                if(OKlightFromAutoTomanualR == 1){
                    PovorotOnRight=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света  
                }
                      
                PovorotnikiRightOff();     //  // Один раз отключить правый поворотник
            }
        }                    
      // <- Поворотник вправо          
      // <- Поворотник влево
       //if(OKlightFromAutoTomanualL == true){ //Если можно гореть левому поворотнику
        if(digitalRead(RightButtonPin)==LOW && digitalRead(LeftButtonPin)==HIGH   ){ // Если кнопка лево нажата
            RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal;
            if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }
            if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingLeftBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                PovorotOnLeft = !PovorotOnLeft;
                timingLeftBlink = millis(); 
            }
            //if(OKlightFromAutoTomanualL==true){ //! Не помогло
               if(PovorotOnLeft == true){ for(int i=13; i<26;++i){strip.SetPixelColor(i, color);strip.Show(); } }// Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
            //}    
            
            else{ PovorotnikiLeftOff();} // turn off the pixels // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ
            
        }                                  // Если кнопка право нажата
        else{                          // Если кнопка лево отпущена 
            if(AutomaticModeActivateL != true){ //Если включен автомат режим то убираем быстрое моргание при отпускании
                timingLeftBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
                if(OKlightFromAutoTomanualL == 1){
                    PovorotOnLeft=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света  
                }
                PovorotnikiLeftOff();     // Если кнопка лево отпущена // Один раз отключить правый поворотник
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
    static int8_t CountBlinkOnIntModeR;//Cчётчик морганий фактических правого поворотника
    static int8_t CountBlinkOnIntModeL; //Cчётчик морганий фактических левого поворотника

    if(IntelligentMode == 1 ){
        if(OffPovorotniki == false){ // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)  

            if(PositionRightCount > OldPositionRightCountInt ){ beginIntModeBlinkR=true;}    // Право ON Серелина Off Off Лево On
            if(PositionRightCount == OldPositionRightCountInt){ 
                beginIntModeBlinkR=false;beginIntModeBlinkL=false;PovorotnikiRightOff();CountBlinkOnIntModeR=0;
                beginIntModeBlinkL=false;PovorotnikiLeftOff();CountBlinkOnIntModeL=0;
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
    
            
             // /*
            //Для правого поворота intellingent 
                          
           
          //  Serial.print(" AutomaticModeActivateR:" );      Serial.print(AutomaticModeActivateR );
            
            //Serial.println();
            if(beginIntModeBlinkR == true ) { //Если исполняется интеллигент режим правого поворота
                //beginIntModeBlinkL = false; PovorotnikiLeftOff(); // Если моргает право - молчит лево)
                EndedR=false; // Ставим буль в положение Незавершено ( Нет нужного кол-ва морганий)          
                RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                uint16_t TempInvertVal;if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
               
                if (millis() - timingRightBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                    PovorotOnRight = !PovorotOnRight;
                    CountBlinkOnIntModeR++;//Serial.println(CountBlinkOnIntModeR);  
                    timingRightBlink = millis(); 
                }
               
                // Сам блинкер
                if(PovorotOnRight == true){ for(int i=0; i<13;++i){strip.SetPixelColor(i, color); }  strip.Show();} // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                else{  PovorotnikiRightOff();   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
                // Сам блинкер

                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое
                    if ( (CountBlinkOnIntModeR) >= (CountBlinkIntMode*2)+1 ){ 
                            beginIntModeBlinkR = false; EndedR=true;//Serial.println("EndedR=true"); //PositionRightCount = OldPositionRightCountInt; 
                            //PovorotOnRight=true;               
                      if (digitalRead(RightButtonPin) ==LOW){PositionRightCount = OldPositionRightCountInt;PovorotOnRight=true;}  //Только когда мы отпускаем кнопку  
                    } 
                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое
                
            }
         
            //Для правого поворота intellingent 
 

            //Для левого поворота intellingent
            
            if(beginIntModeBlinkL == true ) { //Если исполняется интеллигент режим правого поворота           
               //beginIntModeBlinkR = false; PovorotnikiRightOff();
                EndedL=false;   
                RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                uint16_t TempInvertVal;if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250); }if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162); }
                // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
                
                if (millis() - timingLeftBlink > TempInvertVal ){PovorotOnLeft = !PovorotOnLeft;CountBlinkOnIntModeL++;Serial.println(CountBlinkOnIntModeL);timingLeftBlink = millis();} // Таймер отсчёта включения и выключения правого поворотника
                          
                // Сам блинкер
                if(PovorotOnLeft == true){for(int i=13; i<26;++i){strip.SetPixelColor(i, color); }  strip.Show();  }// Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ   
                else{  PovorotnikiLeftOff();   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
                // Сам блинкер

                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое
                      if ( (CountBlinkOnIntModeL) >= (CountBlinkIntMode*2)+1  ){ 
                        beginIntModeBlinkL = false; EndedL=true;
                        //Serial.println("EndedL=true");PositionRightCount = OldPositionRightCountInt;PovorotOnLeft=true;
                        if (digitalRead(LeftButtonPin) ==LOW){PositionRightCount = OldPositionRightCountInt;PovorotOnLeft=true;}  //Только когда мы отпускаем кнопку 
                      }
                         
                // Если моргнули заданное из меню кол-во раз то деактивировать интеллигент моргание правое
                
            }
            else{                          // Если кнопка право отпущена
/*
                if(AutomaticModeActivateL != true){ 
                    if(EndedL==false){ beginIntModeBlinkL=1; }  //Если прервано раньше времени то перезапуск режима моргания                
                    
                    timingLeftBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
                
                    CountBlinkOnIntModeL=0;
                    PovorotOnLeft=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света                 
                }
*/
            } 
            //*/
          
        }  
    }    
    // ============================================          Часть Интеллигент режим         ============================================

    // ============================================          Часть автоматический режим         ============================================ 
   // /*
  
   //Serial.println();
    if(AutomaticMode == 1 ){ // Если из настроек мы получили разрешение активировать автоматический режим
     if(IntelligentMode != 1){ // ==0 //Если сейчас не интеллигент режим
      if(OffPovorotniki == false){
      /*    
      // Захвата автоматического режима по времени удержания кнопки
      if( (digitalRead(RightButtonPin)==HIGH) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата правая кнопка и не зажата левая
          if (millis() - timingPressButtonR > (TimePressToOnAutoMode*100) ){ // Вместо 500 подставьте нужное вам значение паузы 
              Serial.println ("AutomaticModeActivateR == true");  
              AutomaticModeActivateR=true; //Включение автоматического режима правого поворотника(Если в булях выставлено On)
          }
      }
      if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==HIGH) ){  //Если зажата левая кнопка и не зажата правая
          if (millis() - timingPressButtonL > (TimePressToOnAutoMode*100) ){ // Вместо 500 подставьте нужное вам значение паузы 
              Serial.println ("AutomaticModeActivateL == true");  
              AutomaticModeActivateL=true; //Включение автоматического режима левого поворотника(Если в булях выставлено On)
          }
      }
     // Захвата автоматического режима по времени удержания кнопки
    */
    // Захвата автоматического режима по двойному нажатию кнопки
    static unsigned long TimeDoublePressed;
    static unsigned long TimeDoublePressed2;
    static int8_t DoublePressedStep=1;

if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==HIGH) ){  //Если зажата левая кнопка и не зажата правая
    if(DoublePressedStep==1){
        TimeDoublePressed=millis();
        DoublePressedStep=2;
    }
    if(DoublePressedStep==3){
        TimeDoublePressed2=millis();
        DoublePressedStep=4;
    }
    
}
if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата левая кнопка и не зажата правая
    if(DoublePressedStep==2){
        DoublePressedStep=3;
    }
    if(DoublePressedStep==4){
        // <
        if(TimeDoublePressed2>TimeDoublePressed){
            if((TimeDoublePressed2-TimeDoublePressed)<600){        
                Serial.println("                                                                                                                                    Double click");
                //TimeDoublePressed=0;
                //TimeDoublePressed2=0;
            }
        }
        // /*
        if(TimeDoublePressed2<TimeDoublePressed){
            if((TimeDoublePressed-TimeDoublePressed2)<600){
                Serial.println("                                                                                                                                    Double click");
                //TimeDoublePressed=0;
                //TimeDoublePressed2=0;
            }
        }   
        // */ 
        // <
        DoublePressedStep=1;
    }
}

    /*
    if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==HIGH) ){  //Если зажата левая кнопка и не зажата правая
        if(DoublePressedStep==1){
            TimeDoublePressed=millis();
            DoublePressedStep = 2;
        }    
        if(DoublePressedStep==3){
            if ((millis()-TimeDoublePressed) < 600){
                Serial.println("                                                                                            Double click");
                DoublePressedStep = 1;
            }
            else{
                DoublePressedStep = 1;Serial.println("                                                                                      %");
                TimeDoublePressed=millis();
            }
        }
    }

    if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата левая кнопка и не зажата правая
        if(DoublePressedStep==2){
            DoublePressedStep=3;
        }
    }    
    */
    // /*
    Serial.print(" TimeDoublePressedStep:" );Serial.print(DoublePressedStep);   
    Serial.print(" TimeDoublePressed:" );Serial.print(TimeDoublePressed);
    Serial.print(" TimeDoublePressed2:" );Serial.print(TimeDoublePressed2);
    if(TimeDoublePressed2>TimeDoublePressed){
        Serial.print(" TimeDoublePressed2-TimeDoublePressed " );Serial.print( TimeDoublePressed2-TimeDoublePressed );
    }
    if(TimeDoublePressed2<TimeDoublePressed){
        Serial.print(" TimeDoublePressed-TimeDoublePressed2 " );Serial.print( TimeDoublePressed-TimeDoublePressed2 );
    }
    Serial.println();
    // */
    // Захвата автоматического режима по двойному нажатию кнопки

    if( digitalRead(RightButtonPin)==LOW && digitalRead(LeftButtonPin)==LOW ){  timingPressButtonR = millis();timingPressButtonL = millis(); }
        /*
        static int8_t OldPositionRightCountAuto;
        // Один раз присвоить значение текущего ползунка право
        if(OneRazAutoSetPolzunokR == false){
            OldPositionRightCountAuto = PositionRightCount;
        OneRazAutoSetPolzunokR = true;
        }
        */
        // Один раз присвоить значение текущего ползунка право
        

      

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
              }
              if(PositionRightCount < OldPositionRightCount){
                  AutomaticModeActivateR = false; // Отключить автоматический режим правого поворотника
                  //AutomaticModeActivateL = true; // Включить автоматический режим левого поворотника
                 
                  PovorotnikiRightOff();
                  PovorotnikiLeftOff();
                 
                  timingMozjnoBlinkL=millis();OKlightFromAutoTomanualL = false;
                  Stop = true;timingExitAutoToRead = millis(); 
                  //OffPovorotniki=1;timingOffPovorotniki=millis();
                  //PovorotOnLeft=false;  timingLeftBlink = millis(); 
                  OneRazSavePRK_GE = false;
              }
        PovorotOnLeft = false;
      }
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
                  
                PovorotnikiRightOff();
                PovorotnikiLeftOff();

                  timingMozjnoBlinkR=millis();OKlightFromAutoTomanualR = false;
                  Stop = true;timingExitAutoToRead = millis(); 
                  OneRazSavePRK_GE2L = false;
              }
        PovorotOnRight = false;
      //Обработка нажатий в автоматическом режиме левого поворотника
      }
      
   } // if offpovorotniki == false
  }                 // Если из настроек мы получили разрешение активировать автоматический режим
   
    
        // Правый        
        if(AutomaticModeActivateR==true){
        RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal;
            if(SpeedPovorotnikBlink <= 20){ TempInvertVal= map( SpeedPovorotnikBlink,10,20,500,250);}
            if(SpeedPovorotnikBlink >20 && SpeedPovorotnikBlink <=30 ){ TempInvertVal= map( SpeedPovorotnikBlink,21,30,225,162);  }       
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingRightBlink > TempInvertVal ){PovorotOnRight = !PovorotOnRight;timingRightBlink = millis(); } // Таймер отсчёта включения и выключения правого поворотника
                
            // Блинкер
            if(PovorotOnRight == true){for(int i=0; i<13;++i){strip.SetPixelColor(i, color);} strip.Show();   }        // Если включен по таймеру буль светится правому поворотнику то зажечься правому ЖЁЛТОМУ                          
            else{  PovorotnikiRightOff();   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
            // Блинкер
            KIF_AvailableR = true;
        }
        else{
            
            //PovorotnikiRightOff();
        }
        // Правый 

        // Левый
        if(AutomaticModeActivateL==true){
            RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

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
            else{  PovorotnikiLeftOff();   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
            // Блинкер
            KIF_AvailableL = true;
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