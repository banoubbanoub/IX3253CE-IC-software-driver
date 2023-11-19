#include <Arduino.h>
//#include <TA1268NTSCcolorTVIC.h>
#include <RemoteControll.h>
#include <ClockGenerator.h>
#include <SN761668PLL.h>
//#include <RotaryEncoder.h>
#include <Adafruit_SI5351.h>
#include <ESP_8_BIT_GFX.h>
#include <ESP32Display.h>


void setup() {
 Serial.begin(9600);
 Wire.begin(21, 22);
 IR_SetUP();
 RotaryEncoderSetUP();
 previceClock = digitalRead(clk);
 delay(20);
 SNTVTuner(fraquncy[0],osc);  
 delay(20);
 ClockSetUP();
 PLL(clockSpeed);
 delay(20);
 DeviseSetUP_TA1268();
DisplaySetUP();
       
}




void loop() {
currentClock = digitalRead(clk);
if(currentClock != previceClock){
RotaryEncoder();
DisplayEsp32(displaySelect);
}
previceClock = currentClock;

  if(irrecv.decode(&results)){
     switch (results.value)
      {
        case IR_UP: // up button for osc select
        SNTVTuner(fraquncy[frSelect],osc);
        osc++;
        if(osc >=3)osc=3;
        Serial.print("Osci: ");
        Serial.println(osc);
        break;

        case IR_DOWN: // DOWN button for osc select
        SNTVTuner(fraquncy[frSelect],osc);
        osc--;
        if(osc<=1)osc=1;
        Serial.print("Osci: ");
        Serial.println(osc);
        break;

        case IR_CHANNEL_UP:  //channel up
        SNTVTuner(fraquncy[frSelect],osc);
        fraquncy[frSelect]++;
        Serial.print("Fraquncy: ");
        Serial.println(fraquncy[frSelect]);
        break;

        case IR_CHANNEL_DOWN: // channel down
        SNTVTuner(fraquncy[frSelect],osc);
        fraquncy[frSelect]--;
        Serial.print("Fraquncy: ");
        Serial.println(fraquncy[frSelect]);
        break;

        case IR_RIGHT: // right button for frq select
        frSelect++;
        if(frSelect>1)frSelect=1;
        SNTVTuner(fraquncy[frSelect],osc);
        Serial.print("Fraquncy: ");
        Serial.println(fraquncy[frSelect]);
        break;

        case IR_LEFT: //left button for frq select
        frSelect--;
        if(frSelect <=0)frSelect=0;
        SNTVTuner(fraquncy[frSelect],osc);
        Serial.print("Fr: ");
        Serial.println(fraquncy[frSelect]);
        break;
 
        case IR_RECALL: //default settings
        osc =1;
        fraquncy[0] = 2030;
        valu[8] = 0x3f;
        SNTVTuner(fraquncy[0],osc);
         Serial.print("Fr: ");
        Serial.println(fraquncy[frSelect]);
       for(int x=0; x< 16;x++){
        setDeviceValue(slaveAddress,subAddres[x],valu[x]);
        Serial.print(functionName[x] );
        Serial.println(valu[x],HEX);
        delay(10);
       }
       break;
        
       case IR_VOLUME_UP://button #4
       valu[selectFun]++;
       setDeviceValue(slaveAddress,subAddres[selectFun],valu[selectFun]);
       Serial.print(functionName[selectFun] );
       Serial.println(valu[selectFun],HEX);
       break;

       case IR_VOLUME_DOWN: // button #5
       valu[selectFun]--;
       setDeviceValue(slaveAddress,subAddres[selectFun],valu[selectFun]);
       Serial.print(functionName[selectFun] );
       Serial.println(valu[selectFun],HEX);
       break;

       case IR_7://button #7
       highlightedLine++;
       selectFun++;
       if(selectFun >= 15)selectFun=15;
       if(selectFun<=0)selectFun=0;
       if(valu[selectFun]>0xff)valu[selectFun]=0xff;
       if(valu[selectFun]<=0)valu[selectFun]=0x00;
       if(highlightedLine>=15)highlightedLine=15;
       if(highlightedLine<=0)highlightedLine =0;
       setDeviceValue(slaveAddress,subAddres[selectFun],valu[selectFun]);
       Serial.print(functionName[selectFun] );
       Serial.println(valu[selectFun],HEX);
       break;

       case IR_8: //button #8
       selectFun--;
       highlightedLine--;
       if(selectFun >= 15)selectFun=15;
       if(selectFun<=0)selectFun=0;
       if(valu[selectFun]>0xff)valu[selectFun]=0xff;
       if(valu[selectFun]<=0)valu[selectFun]=0x00;
       if(highlightedLine>=15)highlightedLine=15;
       if(highlightedLine<=0)highlightedLine =0;
       setDeviceValue(slaveAddress,subAddres[selectFun],valu[selectFun]);
       Serial.print(functionName[selectFun] );
       Serial.println(valu[selectFun],HEX);
       break;

       case IR_0: //CLOCK G
       PLL(53);
       break;

       case IR_VIDEO_1: //for TV input
       valu[8] = 0x3f;
       osc = 0x01;
       fraquncy[0] =2030;
       SNTVTuner(fraquncy[0],osc);
       setDeviceValue(slaveAddress,subAddres[8],valu[8]);
       Serial.print(functionName[8] );
       Serial.println(valu[8],HEX);
       break;
       
       case IR_VIDEO_2:
       displaySelect =2;
       osc = 0x01;
      // fraquncy[1] =2030;
       SNTVTuner(fraquncy[frSelect],osc);
       break;

       case IR_MENU: //external video input
       displaySelect =1;
       valu[8] = valu[8]+0x40;
       setDeviceValue(slaveAddress,subAddres[8],valu[8]);
       Serial.print(functionName[8] );
       Serial.println(valu[8],HEX);
       break;
      }
      if(selectFun >= 15)selectFun=15;
      if(selectFun<=0)selectFun=0;
      if(valu[selectFun]>0xff)valu[selectFun]=0xff;
      if(valu[selectFun]<=0)valu[selectFun]=0x00;
      if(highlightedLine>=15)highlightedLine=15;
      if(highlightedLine<=0)highlightedLine =0;
   DisplayEsp32(displaySelect);
  }
   irrecv.resume();
   
}
