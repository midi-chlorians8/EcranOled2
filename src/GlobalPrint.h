// /*
#ifndef GlobalPrint_H
#define GlobalPrint_H
#include <Arduino.h>
void GlobalPrint(){
//    Serial.print(" MenuLayer:" );Serial.print(MenuLayer );
//Serial.print(" PositionUpCount:" );   Serial.print(PositionUpCount );
Serial.print(" PositionRightCount:" );Serial.print(PositionRightCount );
Serial.print(" TimePressToOnAutoMode:" );Serial.print(TimePressToOnAutoMode );
Serial.print(" OffPovorotniki:" );Serial.print(OffPovorotniki );
//Serial.print(" AutomaticModeActivateL:" );Serial.print(AutomaticModeActivateL );
Serial.print(" AutomaticModeActivateR:" );Serial.print(AutomaticModeActivateR );

//Serial.print(" PovorotOnRight:" );Serial.print(PovorotOnRight);
Serial.print(" CountStepTiming:" );Serial.print(CountStepTiming );
Serial.println();
}

#endif

// */