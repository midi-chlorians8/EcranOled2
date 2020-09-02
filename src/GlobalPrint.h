// /*
#ifndef GlobalPrint_H
#define GlobalPrint_H
#include <Arduino.h> 
extern int8_t CountBlinkOnIntModeR;
void GlobalPrint(){

Serial.print(" MenuLayer:" );Serial.print(MenuLayer );
Serial.print(" PositionUpCount:" );   Serial.print(PositionUpCount );


Serial.print(" PositionRightCount:" );Serial.print(PositionRightCount );
    /*
Serial.print(" saveBlink3_1:" );Serial.print(saveBlink3_1 );

Serial.print(" OldPositionRightCount(Auto):" );   Serial.print(OldPositionRightCount );

//Serial.print(" OldPositionRightCount:" );Serial.print(OldPositionRightCount);
Serial.print(" OldPositionRightCountInt:" );Serial.print(OldPositionRightCountInt);
*/
Serial.print(" AutomaticModeActivateR:" );Serial.print(AutomaticModeActivateR ); 
Serial.print(" IntelligentMode:" );Serial.print(IntelligentMode);
 Serial.print(" CountBlinkOnIntModeR:" );Serial.print(CountBlinkOnIntModeR);
Serial.print(" OnSound:" );Serial.print(OnSound );

Serial.print(" HowLongTimeBeep)/10:" );Serial.print(float(HowLongTimeBeep)/10 ,1 );

Serial.print(" HowLongTimeBeep*100:" );Serial.print(HowLongTimeBeep*100);
//Serial.print(" Perebor:" );Serial.print(Perebor ); 
/*
Serial.print(" PovorotOnLeft(int):" );Serial.print(PovorotOnLeft);
 Serial.print(" CountBlinkOnIntModeL:" );Serial.print(CountBlinkOnIntModeL);
Serial.print(" beginIntModeBlinkL:" );Serial.print(beginIntModeBlinkL );
*/
Serial.println();
}

#endif

// */