// /*
#ifndef GlobalPrint_H
#define GlobalPrint_H
#include <Arduino.h> 
extern int8_t CountBlinkOnIntModeR;
void GlobalPrint(){
    Serial.print(" MenuLayer:" );Serial.print(MenuLayer );
Serial.print(" PositionUpCount:" );   Serial.print(PositionUpCount );
Serial.print(" PositionRightCount:" );Serial.print(PositionRightCount );
Serial.print(" OldPositionRightCount:" );Serial.print(OldPositionRightCount);
Serial.print(" OldPositionRightCountInt:" );Serial.print(OldPositionRightCountInt);

Serial.print(" AutomaticModeActivateR:" );Serial.print(AutomaticModeActivateR ); 
Serial.print(" PovorotOnRight(int):" );Serial.print(PovorotOnRight);
 Serial.print(" CountBlinkOnIntModeR:" );Serial.print(CountBlinkOnIntModeR);
Serial.print(" beginIntModeBlinkR:" );Serial.print(beginIntModeBlinkR );
Serial.println();
}

#endif

// */