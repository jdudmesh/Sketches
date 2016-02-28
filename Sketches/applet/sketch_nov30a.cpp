#include "Arduino.h"
void setup();
void loop();
//#include <SoftwareSerial.h>

// Definitions
#define INT1 1

#define STOPPED 0
#define RUNNING 1

#define APPLIANCE_OFF 0
#define APPLIANCE_ON 1

#define MODEM_RX 3
#define MODEM_TX 4
#define MODEM_POWER_BUTTON 2

#define INTERRUPT_PIN 1
#define TEST_PIN 0

#define AT_SIM_CHECK "AT+CPIN?" // check SIM card
#define AT_NETWORK_STATUS "AT+CGREG?" // check network status
#define AT_SIGNAL_QUALITY "AT+CSQ" // get signal quality
#define AT_IMEI_GET "AT+GSN" // get IMSEI

#define AT_GPRS_START "AT+CGATT=1" // start GPRS

#define AT_CNCONFIG_CONTYPE "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""
#define AT_CNCONFIG_SETAPN "AT+SAPBR=3,1,\"APN\",\"payandgo.o2.co.uk\""
#define AT_CNCONFIG_SETUSER "AT+SAPBR=3,1,\"USER\",\"payandgo\""
#define AT_CNCONFIG_SETPWD "AT+SAPBR=3,1,\"PWD\",\"password\""
#define AT_CNCONFIG_SETPROFILE "AT+SAPBR=1,1"

#define AT_HTTP_INIT "AT+HTTPINIT"
#define AT_HTTP_SETPROFILE "AT+HTTPPARA=\"CID\",1"
#define AT_HTTP_SETURL "AT+HTTPPARA=\"URL\",\"%s\""
#define AT_HTTP_GET "AT+HTTPACTION=0"
#define AT_HTTP_POST "AT+HTTPACTION=1"
#define AT_HTTP_READ "AT+HTTPREAD"
#define AT_HTTP_TERM "AT+HTTPTERM"
#define AT_HTTP_DATA "AT+HTTPDATA=%d,%d"

#define URL_LOG "http://someonetowatchover.me:8090/log?state=%d&imei=%s"

#define AT_GPRS_STOP "AT+CGATT=0" // stop GPRS

#define AT_GET_TIME "AT+CCLK?"
#define AT_SET_TIME "AT+CCLK=\"%s\"" //"13/11/25,18:38:00+00" set time

SoftwareSerial GPRS(MODEM_RX, MODEM_TX);
uint8_t lastState = APPLIANCE_OFF;
uint8_t currentState =  APPLIANCE_OFF;

long numTicks = 0;

#define ACTION_IDLE 0
#define ACTION_INIT 1
#define ACTION_START_SEND_STATUS_UPDATE 2

int action = ACTION_INIT;


ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
	// timer 1 monitors the appliance state and tiggers status updates
	currentState = numTicks == 0 ? STOPPED : RUNNING;
	if(currentState != lastState && action == ACTION_IDLE) {
		action = ACTION_START_SEND_STATUS_UPDATE;
	}

	numTicks = 0;
	lastState = currentState;  
}

void hardwareInterrupt() {
  numTicks++;
}

void sendStateUpdate(int state) {
}

void setup() 
{
	GPRS.begin(9600);
	
	pinMode(MODEM_POWER_BUTTON, OUTPUT);
	pinMode(INTERRUPT_PIN, INPUT);
	pinMode(TEST_PIN, OUTPUT);
	
	noInterrupts();
	
	// initialize timer1 to call the interrupt service routine at roughly 1Hz
	TCCR1 = (1<<CTC1) | (1<< CS13) | (1<< CS12) | (1<< CS11) | (1<< CS10);
	OCR1A = 255;
	TIMSK |= (1 << OCIE1A);
		
	interrupts();
}
 
void loop()
{
	switch(action) {
		case ACTION_INIT:
			GPRS.println("AT");
			delay_ms(50);
			break;
	}
}
