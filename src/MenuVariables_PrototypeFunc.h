#ifndef MenuVar_ProtF_H
#define MenuVar_ProtF_H

#include <Arduino.h>
// Переменные для работы меню
int16_t MenuLayer = 1; //Начальный 0 слой где видны все пункты. 2 - вход в меню CirclY
int16_t PositionUpCount=5; //Ползунок по вертикали
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

bool OneRazPosition1_6=false; 
bool OneRazGalochka1_6=false; // Используется для того чтоб галочка ставилась напротив текущего значения
bool saveBlink1_6=false;
bool saveBlink_sensOnValue1_6=false;

bool OneRazPosition1_9=false;  //!!
bool OneRazGalochka1_9=false; // Используется для того чтоб галочка ставилась напротив текущего значения
bool saveBlink1_9=false;
bool saveBlink_sensOnValue1_9=false;
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
bool saveBlink3_0=false;
bool OneRazGalochka3_0=false; // Один раз завести правильное значение из меню
int16_t old_PositionUpCount3_0;
bool saveBlink_EcoBright3_0;

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


bool saveBlink3_6=false;    
int16_t old_HowLongTimeBeep; // Старое значение
int16_t old_PositionUpCount3_6;
bool OneRazPosition3_6 = false; // Один раз завести правильное значение из меню
bool saveBlink_sensOnValue3_6 = false;

bool saveBlink3_7=false;    
int16_t old_HowLongTimeBeepMute; // Старое значение
int16_t old_PositionUpCount3_7;
bool OneRazPosition3_7 = false; // Один раз завести правильное значение из меню
bool saveBlink_sensOnValue3_7 = false;
// Переменные для вкладки 3

// Переменные для вкладки 4
bool saveBlink4_1=false;
bool OneRazGalochka4_1=false; // Один раз завести правильное значение из меню
int16_t old_PositionUpCount4_1;
bool saveBlink_EcoBright4_1;
// Переменные для вкладки 4

// Переменные для вкладки 5 
bool saveBlink5_1=false;
bool OneRazGalochka5_1=false; // Один раз завести правильное значение из меню
int16_t old_PositionUpCount5_1;
bool saveBlink_EcoBright5_1;

bool saveBlink5_2=false;
bool OneRazPosition5_2=false; // Один раз завести правильное значение из меню
int16_t old_BrightnessDayLight;
int16_t old_PositionUpCount5_2;
bool saveBlink_sensOnValue5_2;

bool saveBlink5_3=false;
bool OneRazPosition5_3=false; // Один раз завести правильное значение из меню
int16_t old_BrightnessInEcoMode;
int16_t old_PositionUpCount5_3;
bool saveBlink_sensOnValue5_3;

bool saveBlink5_4=false;
bool OneRazPosition5_4=false; // Один раз завести правильное значение из меню
int16_t old_FadingWhiteWhenTurning;
int16_t old_PositionUpCount5_4;
bool saveBlink_sensOnValue5_4;

bool saveBlink5_5=false;
bool OneRazPosition5_5=false; // Один раз завести правильное значение из меню
int16_t old_BrightnessOnActivatedPassingLights;
int16_t old_PositionUpCount5_5;
bool saveBlink_sensOnValue5_5;
// Переменные для вкладки 5

void DayLight();

void SaveBlink1_1();
void SaveBlink1_2();
void SaveBlink1_3(); 
void SaveBlink1_4(); 
void SaveBlink1_5(); 
void SaveBlink1_6();
void SaveBlink1_9();
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
void SaveBlink3_0();
void SaveBlink3_1();
void SaveBlink3_11();
void SaveBlink3_2();
void SaveBlink3_21();
void SaveBlink3_6();
void SaveBlink3_7();
void SaveBlink4_1();
void SaveBlink5_1();
void SaveBlink5_2();
void SaveBlink5_3();
void SaveBlink5_4();
void SaveBlink5_5();


#endif