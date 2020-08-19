#include <Arduino.h>
// SK6812
#include <NeoPixelBus.h>
// SK6812
const uint16_t PixelCount = 26; // this example assumes 4 pixels, making it smaller will cause a failure
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
 
    static bool PovorotOnRight; // Буль меняет состояние в зависимости от тика таймера выше
    static bool PovorotOnLeft;
  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
    static unsigned long timingRightBlink; // Используется для таймера моргания правого поворотника
    static unsigned timingLeftBlink;  // Используется для таймера моргания левого поворотника
    if(OffPovorotniki == true){ // Если поворотникам нельзя моргать
        if(millis() - timingOffPovorotniki >500){ // Если прошло пол секунды
            PovorotnikiRightOff();                  // Отключить поворотники
            OffPovorotniki = false; // Разрешить моргать поворотникам
        }
    }
  // Блок ответственный за то чтоб при выходе из меню не моргали поворотники
  
  
    if (digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==HIGH ){PovorotnikiRightOff(); } //Чтоб при нажатии 2х кнопок не светило  // Один раз отключить правый поворотник

    if(OffPovorotniki == false){ // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
        if(digitalRead(RightButtonPin)==HIGH && digitalRead(LeftButtonPin)==LOW){ // Если кнопка право нажата
            RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal = SpeedPovorotnikBlink*100;
            TempInvertVal= map( (TempInvertVal),1000,3000,1000,300); // Изменяем чтоб при увеличении значения чаще моргал поворотник
            //Serial.print("TempInvertVal:");Serial.println(TempInvertVal);Serial.print("MAP TempInvertVal:");Serial.println(TempInvertVal);
            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще

            if (millis() - timingRightBlink > TempInvertVal ){ // Таймер отсчёта включения и выключения правого поворотника
                PovorotOnRight = !PovorotOnRight;
                timingRightBlink = millis(); 
            }

            if(PovorotOnRight == true){ // Если включен по таймеру буль светится правому поворотнику то зажечься ЖЁЛТОМУ
                for(int i=0; i<13;++i){
                    strip.SetPixelColor(i, color);
                }
                strip.Show();
            }
            else{                       // Если выключен по таймеру буль светится правому поворотнику то диоды ПОГАСЛИ
                PovorotnikiRightOff(); // turn off the pixels
            }
        }                                  // Если кнопка право нажата
        else{                          // Если кнопка право отпущена   
            PovorotnikiRightOff();     // Если кнопка право отпущена // Один раз отключить правый поворотник
        }                            
      // <- Поворотник влево
        if(digitalRead(RightButtonPin)==LOW && digitalRead(LeftButtonPin)==HIGH){ // Если кнопка право нажата
            RgbColor color = RgbColor(200, 255, 0); //Создали жёлтый

            // Блок инвертирующий значение скорости моргания чтоб при увеличении значения поворотник моргал чаще
            uint16_t TempInvertVal = SpeedPovorotnikBlink*100;
            TempInvertVal= map( (TempInvertVal),1000,3000,1000,300); // Изменяем чтоб при увеличении значения чаще моргал поворотник
            //Serial.print("TempInvertVal:");Serial.println(TempInvertVal);Serial.print("MAP TempInvertVal:");Serial.println(TempInvertVal);
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
        else{                          // Если кнопка право отпущена   
            PovorotnikiLeftOff();     // Если кнопка право отпущена // Один раз отключить правый поворотник
        }
      // <-
  
    }     // Если мы только что не вышли из главного меню то можно моргать поворотниками (Если можно моргать поворотником)
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