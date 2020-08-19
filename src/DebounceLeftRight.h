#include <Arduino.h>
// Начнем с директив препроцессора. ADD_H – это произвольное уникальное имя (обычно используется имя заголовочного файла)
#ifndef DebounceLeftRight_H
#define DebounceLeftRight_H

#define DownButtonPin 2
#define UpButtonPin 4
#define RightButtonPin 26
#define LeftButtonPin 32

//Комплект обработки кнопок

bool buttonState4;             // the current reading from the input pin
bool lastButtonState4 = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime4 = 0;  // the last time the output pin was toggled
const uint8_t debounceDelay4 = 5;    // the debounce time; increase if the output flickers

bool MinusUP4 = false;
bool OneRazFirstTimePressed4=false;
unsigned long TimePressed4;
unsigned long FirstTimePressed4;

bool buttonState2;             // the current reading from the input pin
bool lastButtonState2 = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
const uint8_t debounceDelay2 = 5;    // the debounce time; increase if the output flickers

bool MinusUP2 = false;
bool OneRazFirstTimePressed2=false;
unsigned long TimePressed2;
unsigned long FirstTimePressed2;

bool buttonState26;             // the current reading from the input pin
bool lastbuttonState26 = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime26 = 0;  // the last time the output pin was toggled
const uint8_t debounceDelay26 = 17;    // the debounce time; increase if the output flickers

bool buttonState32;             // the current reading from the input pin
bool lastbuttonState32 = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime32 = 0;  // the last time the output pin was toggled
const uint8_t debounceDelay32 = 17;    // the debounce time; increase if the output flickers
//Комплект обработки кнопок

// А это уже содержимое заголовочного файла
//int add(int x, int y); // прототип функции add() (не забывайте точку с запятой в конце!)
 void Debounce(const int8_t buttonPin,bool& buttonState,bool& lastButtonState,unsigned long& lastDebounceTime,uint8_t debounceDelay);
 //void DebounceV2(const int8_t buttonPin);

 void PinMode();
// Заканчиваем директивой препроцессора
#endif


