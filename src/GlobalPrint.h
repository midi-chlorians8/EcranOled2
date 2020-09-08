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
Serial.print(" PovorotOnRight:" );Serial.print(PovorotOnRight);
 Serial.print(" CountBlinkOnIntModeR:" );Serial.print(CountBlinkOnIntModeR);
Serial.print(" OnSound:" );Serial.print(OnSound );

Serial.print(" BrightnessDayLight" );Serial.print(BrightnessDayLight );

Serial.print(" BrightnessInEcoMode255:" );Serial.print(BrightnessInEcoMode255); //L -1 ;B - 0;
Serial.print(" BrightnessInEcoMode:" );Serial.print(BrightnessInEcoMode);
Serial.println();
// Serial.print(" AftherOn:" );Serial.print(AftherOn);
//Serial.print(" Perebor:" );Serial.print(Perebor ); 
/*


Serial.print(" beginIntModeBlinkL:" );Serial.print(beginIntModeBlinkL );
*/
//Serial.println();
}

#endif

// */