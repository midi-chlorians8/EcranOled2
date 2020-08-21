// /*
#ifndef GlobalPrint_H
#define GlobalPrint_H
#include <Arduino.h>
void GlobalPrint(){
    Serial.print(" MenuLayer:" );Serial.print(MenuLayer );
Serial.print(" PositionUpCount:" );   Serial.print(PositionUpCount );
Serial.print(" PositionRightCount:" );Serial.print(PositionRightCount );
Serial.print(" TimePressToOnAutoMode:" );Serial.print(TimePressToOnAutoMode );
Serial.print(" OffPovorotniki:" );Serial.print(OffPovorotniki );
//Serial.print(" millis() - timingOffPovorotniki:" );Serial.print(millis() - timingOffPovorotniki );
Serial.print(" AutomaticModeActivateR:" );Serial.print(AutomaticModeActivateR );

Serial.print(" AutomaticMode:" );Serial.print(AutomaticMode);
Serial.print(" SpeedPovorotnikBlink:" );Serial.print(SpeedPovorotnikBlink );
Serial.println();
}

#endif

// */