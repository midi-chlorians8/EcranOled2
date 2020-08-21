#include <Arduino.h>
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

extern bool OffPovorotniki; // Буль отключающий поворотники при выходе из главного меню
unsigned long timingOffPovorotniki; // Время которое не горят поворотники после выхода из главного экрана

void PovorotnikiRightOff(); // Прототип функции выключения поворотника правого
void PovorotnikiLeftOff();  // Прототип функции выключения поворотника левого
void Povorotniki(){
 
    static bool PovorotOnRight = true; // Буль меняет состояние в зависимости от тика таймера выше
    static bool PovorotOnLeft = true;
  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
    static unsigned long timingRightBlink; // Используется для таймера моргания правого поворотника
    static unsigned timingLeftBlink;  // Используется для таймера моргания левого поворотника

    static unsigned long timingPressButtonR;
    static unsigned long timingPressButtonL;

    if(OffPovorotniki == true){ // Если поворотникам нельзя моргать
        if(millis() - timingOffPovorotniki >500){ // Если прошло пол секунды
            PovorotnikiRightOff();                // Отключить поворотники
            timingPressButtonR=millis(); // Чтобы сразу не включался автоматический режим после выхода из меню при зажатой кнопке
            timingPressButtonL=millis(); // Чтобы сразу не включался автоматический режим после выхода из меню при зажатой кнопке. До пары
            OffPovorotniki = false; // Разрешить моргать поворотникам
        }
    }
  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
  // OffPovorotniki используются для того чтоб при выходе из главного меню небыло сразу морганий

    if (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==HIGH ){ PovorotnikiRightOff(); PovorotnikiLeftOff(); } //Чтоб при нажатии 2х кнопок не светило  // Один раз отключить правый поворотник

        // ============================================          Часть Простое нажатие клавиш        ============================================ 
    if(IntelligentMode == 0){ // Если intelligentmode выключен
        if(OffPovorotniki == false){ // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
      // <- Поворотник вправо  
        if(     (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW)  || AutomaticModeActivateR==true      ){ // Если кнопка право нажата
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

            if(PovorotOnRight == true){ // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                for(int i=0; i<13;++i){strip.SetPixelColor(i, color); }
                 strip.Show();              
            }
            else{  PovorotnikiRightOff();   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
                  
        }                                  // Если кнопка право нажата
        else{                          // Если кнопка право отпущена   
            if(AutomaticModeActivateR != true){ //Если включен автомат режим то убираем быстрое моргание при отпускании
                timingRightBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
                PovorotOnRight=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света             
                PovorotnikiRightOff();     // Если кнопка право отпущена // Один раз отключить правый поворотник
            }
        }                    
      // <- Поворотник вправо          
      // <- Поворотник влево
        if(digitalRead(RightButtonPin)==LOW && digitalRead(LeftButtonPin)==HIGH  || AutomaticModeActivateL==true  ){ // Если кнопка лево нажата
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

            if(PovorotOnLeft == true){ // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                for(int i=13; i<26;++i){
                    strip.SetPixelColor(i, color);
                }
                strip.Show();
            }
            else{                       // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ
                PovorotnikiLeftOff(); // turn off the pixels
            }
        }                                  // Если кнопка право нажата
        else{                          // Если кнопка лево отпущена 
            timingLeftBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
            PovorotOnLeft=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света  
            PovorotnikiLeftOff();     // Если кнопка лево отпущена // Один раз отключить правый поворотник
        }
      // <-
  
    }     // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
    }   
        // ============================================          Часть Простое нажатие клавиш        ============================================ 

    // ============================================          Часть Интеллигент режим         ============================================ 
    static int8_t OldPositionRightCountInt=1;
    static bool OneRazSavePRKOld_IntMode;

    static bool test;
    static bool RightPressedInt=false;
    static bool LeftPressedInt=true;

    Serial.print(" PositionRightCount:" );Serial.print(PositionRightCount);
    Serial.print(" OldPositionRightCountInt:" );Serial.print(OldPositionRightCountInt );
    Serial.print(" OneRazSavePRKOld_IntMode:" );Serial.print(OneRazSavePRKOld_IntMode );
    Serial.print(" test:" );Serial.print(test );
Serial.print(" RightPressedInt:" );Serial.print(RightPressedInt );
Serial.print(" LeftPressedInt:" );Serial.print(LeftPressedInt );

    Serial.print(" beginIntModeBlinkL:" );Serial.print(beginIntModeBlinkL);
    Serial.print(" beginIntModeBlinkR:" );Serial.print(beginIntModeBlinkR );


    Serial.println();
    //delay(2000);
    if(IntelligentMode == 1 ){
        if(OffPovorotniki == false){ // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
                 
              
            if( (digitalRead(RightButtonPin)==HIGH) && (digitalRead(LeftButtonPin)==LOW) ){  //Если зажата правая кнопка и не зажата левая
                //if (millis() - timingPressButtonR > 50 ){ // Вместо 500 подставьте нужное вам значение паузы 
                    Serial.println ("IntModeActivateR == true");  
                    //LeftPressedInt=false;
                    RightPressedInt=true;
                    // beginIntModeBlinkR=true; //Включение автоматического режима правого поворотника(Если в булях выставлено On)
                    
                //}
            }
            // /*
            if( (digitalRead(RightButtonPin)==LOW) && (digitalRead(LeftButtonPin)==HIGH) ){  //Если зажата левая кнопка и не зажата правая
                if (millis() - timingPressButtonL > 500 ){ // Вместо 500 подставьте нужное вам значение паузы 
                    Serial.println ("IntModeActivateL == true");  
                    LeftPressedInt=true;
                    
                }
            }
            // */
      
            if(RightPressedInt == true ) {//Если исполняется автоматический режим правого поворота
                // Один раз сохранить значение правого ползунка в переменную прошлого состояния              
               
                if(PositionRightCount > OldPositionRightCountInt){
                    beginIntModeBlinkR = !beginIntModeBlinkR;
                    OneRazSavePRKOld_IntMode=false;
                    RightPressedInt=false;
                }
               
                if(PositionRightCount < OldPositionRightCountInt){
                    if(beginIntModeBlinkR == 1){
                        beginIntModeBlinkR = !beginIntModeBlinkR;
                        
                    }
                    OldPositionRightCountInt = PositionRightCount;
                    
                        //beginIntModeBlinkR = false; // Отключить автоматический режим правого поворотника
                        //beginIntModeBlinkL = true; // Включить автоматический режим левого поворотника
                        //OneRazSavePRKOld_IntMode = false;
                }
               
                if(OneRazSavePRKOld_IntMode == false){
                    OldPositionRightCountInt = PositionRightCount;
                   
                    OneRazSavePRKOld_IntMode = true;     
                    RightPressedInt=false;
                }    
                // Один раз сохранить значение правого ползунка в переменную прошлого состояния
                   
            }
/*
            if(LeftPressedInt == true ) {//Если исполняется автоматический режим правого поворота
                // Один раз сохранить значение правого ползунка в переменную прошлого состояния              
               
                if(PositionRightCount < OldPositionRightCountInt){
                    // beginIntModeBlinkR = false; // Отключить автоматический режим правого поворотника                
                        //OneRazSavePRK_GE = false;
                        beginIntModeBlinkL = !beginIntModeBlinkL;
                    OneRazSavePRKOld_IntMode=false;
                }
               
                if(PositionRightCount > OldPositionRightCountInt){
                    if(beginIntModeBlinkL == 1){
                        beginIntModeBlinkL = !beginIntModeBlinkL;
                        
                    }
                    OldPositionRightCountInt = PositionRightCount;
                        //beginIntModeBlinkR = false; // Отключить автоматический режим правого поворотника
                        //beginIntModeBlinkL = true; // Включить автоматический режим левого поворотника
                        //OneRazSavePRKOld_IntMode = false;
                }
               
                if(OneRazSavePRKOld_IntMode == false){
                    OldPositionRightCountInt = PositionRightCount;
                    OneRazSavePRKOld_IntMode = true;     
                    LeftPressedInt=false;
                }    
                // Один раз сохранить значение правого ползунка в переменную прошлого состояния
                   
            }
           */
           /*
            if(beginIntModeBlinkL == true ) {//Если исполняется автоматический режим правого поворота
                // Один раз сохранить значение правого ползунка в переменную прошлого состояния
                if(OneRazSavePRK_GE2L == false){
                    OldPositionRightCountInt = PositionRightCount;
                    OneRazSavePRK_GE2L = true;     
                }    
                // Один раз сохранить значение правого ползунка в переменную прошлого состояния
               
                //Обработка нажатий в intelligent режиме левого поворотника
                if(PositionRightCount < OldPositionRightCountInt){
                    beginIntModeBlinkL = false; // Отключить автоматический режим правого поворотника
                    OneRazSavePRK_GE2L = false;
                }
                if(PositionRightCount > OldPositionRightCountInt){
                    beginIntModeBlinkL = false; // Отключить автоматический режим правого поворотника
                    beginIntModeBlinkR = true; // Включить автоматический режим левого поворотника
                    OneRazSavePRK_GE2L = false;
                }
                //Обработка нажатий в intelligent режиме левого поворотника
            }  
            */
            //Для правого поворота
            /*
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
            static int8_t CountBlinkOnIntMode;
            if(PovorotOnRight == true){ // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                for(int i=0; i<13;++i){strip.SetPixelColor(i, color); }
                 strip.Show();              
            }
            else{  PovorotnikiRightOff();   }                    // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ // turn off the pixels
             */     
        
        /*                                  // Если кнопка право нажата
        else{                          // Если кнопка право отпущена   
            if(AutomaticModeActivateR != true){ //Если включен автомат режим то убираем быстрое моргание при отпускании
                timingRightBlink = millis(); //Чтобы всегда при включении поворотника всегда начинать с включенного света
                PovorotOnRight=true;         //Чтобы всегда при включении поворотника всегда начинать с включенного света             
                PovorotnikiRightOff();     // Если кнопка право отпущена // Один раз отключить правый поворотник
            }
        }
        */
        }  
    }    
    // ============================================          Часть Интеллигент режим         ============================================

    // ============================================          Часть автоматический режим         ============================================ 
    if(AutomaticMode == 1 ){ // Если из настроек мы получили разрешение активировать автоматический режим
     
      if(OffPovorotniki == false){
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

      if( digitalRead(RightButtonPin)==LOW && digitalRead(LeftButtonPin)==LOW ){  timingPressButtonR = millis();timingPressButtonL = millis(); }
      static int8_t OldPositionRightCount;    
      
      if(AutomaticModeActivateR == true ) {//Если исполняется автоматический режим правого поворота
          // Один раз сохранить значение правого ползунка в переменную прошлого состояния
          if(OneRazSavePRK_GE == false){
              OldPositionRightCount = PositionRightCount;
              OneRazSavePRK_GE = true;     
          }    
          // Один раз сохранить значение правого ползунка в переменную прошлого состояния
              /*
              Serial.print("AutomaticModeActivateL: ");Serial.print(AutomaticModeActivateL);
              Serial.print(" AutomaticModeActivateR: ");Serial.print(AutomaticModeActivateR);
              Serial.print(" PositionRightCount: ");Serial.print(PositionRightCount);
              Serial.print(" OldPositionRightCount: ");Serial.println(OldPositionRightCount);
              */
              if(PositionRightCount > OldPositionRightCount){
                  AutomaticModeActivateR = false; // Отключить автоматический режим правого поворотника
                  OneRazSavePRK_GE = false;
              }
              if(PositionRightCount < OldPositionRightCount){
                  AutomaticModeActivateR = false; // Отключить автоматический режим правого поворотника
                  AutomaticModeActivateL = true; // Включить автоматический режим левого поворотника
                  OneRazSavePRK_GE = false;
              }
      }
      if(AutomaticModeActivateL == true ) {//Если исполняется автоматический режим правого поворота
          // Один раз сохранить значение правого ползунка в переменную прошлого состояния
          if(OneRazSavePRK_GE2L == false){
              OldPositionRightCount = PositionRightCount;
              OneRazSavePRK_GE2L = true;     
          }    
          // Один раз сохранить значение правого ползунка в переменную прошлого состояния
          /*
              Serial.print("AutomaticModeActivateL: ");Serial.print(AutomaticModeActivateL);
              Serial.print(" AutomaticModeActivateR: ");Serial.print(AutomaticModeActivateR);
              Serial.print(" PositionRightCount: ");Serial.print(PositionRightCount);
              Serial.print(" OldPositionRightCount: ");Serial.println(OldPositionRightCount);
              */
      //Обработка нажатий в автоматическом режиме левого поворотника
              if(PositionRightCount < OldPositionRightCount){
                  AutomaticModeActivateL = false; // Отключить автоматический режим правого поворотника
                  OneRazSavePRK_GE2L = false;
              }
              if(PositionRightCount > OldPositionRightCount){
                  AutomaticModeActivateL = false; // Отключить автоматический режим правого поворотника
                  AutomaticModeActivateR = true; // Включить автоматический режим левого поворотника
                  OneRazSavePRK_GE2L = false;
              }
      //Обработка нажатий в автоматическом режиме левого поворотника
      }
      
   } // if offpovorotniki == false
  }                 // Если из настроек мы получили разрешение активировать автоматический режим
    // ============================================          Часть автоматический режим         ============================================ 
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