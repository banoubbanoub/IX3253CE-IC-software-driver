#include<Arduino.h>
#include<Wire.h>
const byte  I2C_multiplexerAddress = 0x70;
const byte slaveAddress = 0x44;

const byte subAddres[16] ={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,};

uint8_t valu[16] ={0x32,0x41,0x28,0xcb,0x08,0x00,0x2f,0x20,0x3f,0x3f,0x7f,0x00,0x00,0x00,0x00,0x00,};

const String functionName[17] = {"Color","Tint","Brightness","Uni-color","Sharpness","Vertical, Horizontal phase",
"Audio Attenuator","Don'T USE","RF AGC","Vertical Amplitue","PIF VCO","R CUT Off","G CUT Off","B CUTT Off",
"G Gain","B Gain",};

int selectFun =0;

void setDeviceValue(int slaveAddress, byte subAddress, byte data) {
  Wire.beginTransmission(I2C_multiplexerAddress);  // TCA9548A address  I2C IC
  Wire.write(1 << 1);// send byte to select bus
  Wire.endTransmission();

  Wire.beginTransmission(slaveAddress); // Start communication with the device
  Wire.write(subAddress); // Send the subaddress
  Wire.write(data); // Send data for the specific function
  Wire.endTransmission(); // End the transmission 

   // if (error != 0) {
    //Serial.print("Error in I2C communication. Error code: ");
    //Serial.println(error);
    // Handle the error as needed
 // }
}
void DeviseSetUP_TA1268(){
   for(int x=0; x< 16;x++){
      setDeviceValue(slaveAddress,subAddres[x],valu[x]);
      Serial.print(functionName[x] );
      Serial.println(valu[x],HEX);
        delay(10);
      }
}
void TunerDisplay(uint8_t f, uint8_t os){
Serial.print("freq:");
Serial.println(f);
Serial.println("os:");
}
