// /*
#ifndef GlobalPrint_H
#define GlobalPrint_H
#include <Arduino.h>
void GlobalPrint(){
    Serial.print(" MenuLayer:" );Serial.print(MenuLayer );
Serial.print(" PositionUpCount:" );   Serial.print(PositionUpCount );
Serial.print(" PositionRightCount:" );Serial.print(PositionRightCount ); 
Serial.print(" EnterOnTheAutoMode:" );Serial.print(EnterOnTheAutoMode);
// Serial.print(" TimeDoublePressedStep:" );Serial.print(TimeDoublePressedStep);
//Serial.print(" CountStepTiming:" );Serial.print(CountStepTiming );
Serial.println();
}

#endif

// */