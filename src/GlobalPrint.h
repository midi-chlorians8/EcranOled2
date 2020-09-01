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
Serial.print(" PovorotOnRight(int):" );Serial.print(PovorotOnRight);
 Serial.print(" CountBlinkOnIntModeR:" );Serial.print(CountBlinkOnIntModeR);
Serial.print(" beginIntModeBlinkR:" );Serial.print(beginIntModeBlinkR );

/*
Serial.print(" AutomaticModeActivateL:" );Serial.print(AutomaticModeActivateL ); 
Serial.print(" PovorotOnLeft(int):" );Serial.print(PovorotOnLeft);
 Serial.print(" CountBlinkOnIntModeL:" );Serial.print(CountBlinkOnIntModeL);
Serial.print(" beginIntModeBlinkL:" );Serial.print(beginIntModeBlinkL );
*/
Serial.println();
}

#endif

// */