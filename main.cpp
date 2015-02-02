#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>



// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library

OneWire	oneWire(10);	// on pin 10 (a 4.7K resistor is necessary)
DallasTemperature DT(&oneWire);

#define TEMP_PRECISION 9
#define TEMP_CONVERSION_DELAY 750 / (1 << (12 - TEMP_PRECISION))
#define TEMP_COUNT 6
int sensors=0;

//static int16_t tempRaw[TEMP_COUNT]={DEVICE_DISCONNECTED_RAW,DEVICE_DISCONNECTED_RAW,DEVICE_DISCONNECTED_RAW,DEVICE_DISCONNECTED_RAW,DEVICE_DISCONNECTED_RAW,DEVICE_DISCONNECTED_RAW};
static int16_t tempRaw[TEMP_COUNT]={
	0x07d0
	,0x0191
	,0x0008
	,0x0001
	,0xfff8
	,0xfe6f
};
static double tempC[TEMP_COUNT]={75.1,72.02,60.003,40.0004,30.0,28.12345};
static DeviceAddress DA[TEMP_COUNT]={
	{0x28,0xe7,0x5f,0x60,0x05,0x00,0x00,0xed},
	{0x28,0x11,0xda,0x5f,0x05,0x00,0x00,0xe6},
	{0x28,0x01,0xc6,0x60,0x05,0x00,0x00,0x32},
	{0x28,0xe6,0x53,0x60,0x05,0x00,0x00,0xfb},
	{0x28,0x49,0x5c,0x61,0x05,0x00,0x00,0x63},
	{0x28,0x1c,0x04,0x60,0x05,0x00,0x00,0x82}
};


void setup(void) {
	Serial.begin(9600);
	pinMode(9,OUTPUT);
	pinMode(8,INPUT);
	pinMode(7,INPUT);
	pinMode(6,INPUT);
	pinMode(5,INPUT);
	DT.begin();
	DT.setWaitForConversion(true);
	sensors=DT.getDeviceCount();
	if(sensors>TEMP_COUNT){
		sensors=TEMP_COUNT;
	}
	for(int i=0;i<sensors;i++) {
		DT.getAddress(DA[i],i);
		DT.setResolution(DA[i],TEMP_PRECISION);
		tempRaw[i] = DT.getTemp(DA[i]);
	}
}

void getTemperatures(void) {
	DT.requestTemperatures();
	for (int i = 0; i < sensors; i++) {
		tempRaw[i] = DT.getTemp(DA[i]);
		tempC[i]=(float)tempRaw[i] * 0.0625;
	}
}

void loop(void) {
	if(Serial.available()>0) {
		getTemperatures();
		for(int i=0;i<sensors;i++) {
			Serial.print(tempC[i],DEC);
			Serial.print(',');
		}
		Serial.print(digitalRead(8),DEC);
		Serial.print(',');
		Serial.print(digitalRead(7),DEC);
		Serial.print(',');
		Serial.print(digitalRead(6),DEC);
		Serial.print(',');
		Serial.print(digitalRead(5),DEC);
		Serial.print('\n');
		while(Serial.available()>0) {
			Serial.read();
		}
	}
}
int main(void)
{
	init();

#if defined(USBCON)
	USBDevice.attach();
#endif
	
	setup();
		
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
				
	return 0;
}

