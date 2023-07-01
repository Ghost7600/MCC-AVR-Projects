// Controller 1 - Keypad/Transmitter Code

//---DEFINIÇÔES---//
#define F_CPU 16000000UL

//---INCLUDES---//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/int0.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/device/keypad.hpp"
#include "funsape/peripheral/usart0.hpp"
#include "Adafruit_Fingerprint.h"

//global variables
vbool_t timer0flag;

void bitbang(uint8_t tecla)
{
    timer0flag = 1;
    timer0.clearCompareAInterruptRequest();
    timer0.setCounterValue(0);
    clrBit(PORTD, PD2);
    timer0.activateCompareAInterrupt();
    while(timer0flag){}
    //delayUs(104);
    timer0.deactivateCompareAInterrupt();
    timer0flag = 1;
    for(uint8_t i = 0; i < 8; i++) {
        if(isBitSet(tecla, i)) {
            setBit(PORTD, PD2);
            timer0.activateCompareAInterrupt();
            while(timer0flag) {
                doNop();
            }
            timer0.deactivateCompareAInterrupt();
            timer0flag = 1;
        }
        else {
            clrBit(PORTD, PD2);
            timer0.activateCompareAInterrupt();
            while(timer0flag) {
                doNop();
            }
            timer0.deactivateCompareAInterrupt();
            timer0flag = 1;
        }
    }
    setBit(PORTD, PD2);
    timer0.activateCompareAInterrupt();
    while(timer0flag) {
    }
    timer0.deactivateCompareAInterrupt();
}

void sendstring (char string[64]){
    int i =0;
    while (string[i] != '\0'){
        bitbang(string[i]);
        i++;
    }
    bitbang('\n');
}

char uartgetchar(){
    // Local variables
    uint8_t aux8;
    bool auxb;


    // Waits until last reception ends
    waitUntilBitIsSet(UCSR0A, RXC0);

    // Checks reception erros
    aux8 = UCSR0A;

    // Stores 8th bit (if data size in 9 bits)
    auxb = 0;
    // if(this->_dataSize == DataSize::DATA_9_BITS) {
    //     *data_p = (isBitSet(UCSR0B, RXB80)) ? (1 << 8) : 0;
    // }

    // Gets received byte
    auxb |= UDR0;

    return aux8;
}

void usartgetstring (char string[]){
    char ch;
    int i=0;
    while ((ch = uartgetchar()) != '\0'){
        string[i]=ch;
        i++;
    }
}


void usartsendstring (char string[64]){
    int i =0;
    while (string[i] != '\0'){
        while (!(UCSR0A & (1<<UDRE0)));
        UDR0 = string[i];
        i++;
    }
    printf("fim de usartsend ");
}
uint8_t getFingerprintID(Adafruit_Fingerprint* finger) {
     sendstring(" entrou na get finger id");
  uint8_t p = finger->getImage();
  sendstring("depois da getimage");
  switch (p) {
    case FINGERPRINT_OK:
      sendstring("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      sendstring("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      sendstring("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      sendstring("Imaging error");
      return p;
    default:
      sendstring("Unknown error");
      return p;
  }

  // OK success!

  p = finger->image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      sendstring("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      sendstring("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      sendstring("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      sendstring("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      sendstring("Could not find fingerprint features");
      return p;
    default:
      sendstring("Unknown error");
      return p;
  }

  // OK converted!
  p = finger->fingerSearch();
  sendstring("after fingersearch");
  if (p == FINGERPRINT_OK) {
    sendstring("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    sendstring("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    sendstring("Did not find a match");
    return p;
  } else {
    sendstring("Unknown error");
    return p;
  }

  // found a match!
  sendstring("Found ID #");
  sendstring(" with confidence of ");

  return finger->fingerID;
}




// void prepusartfp (void){
//     Usart0 usart(Usart0::BaudRate::BAUD_RATE_57600,Usart0::Mode::ASYNCHRONOUS);
// }

int getFingerprintIDez(Adafruit_Fingerprint* finger) {
  uint8_t p = finger->getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger->image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger->fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  printf("Found ID #"); printf("%d", finger->fingerID);
  printf(" with confidence of "); printf("%d", finger->confidence);
  return finger->fingerID;
}

//---MAIN---//
int main()
{
    //////////////////// Configuração Usart de hardware para comunicação com o sensor ///////////////////
    //Usart0 usart0(Usart0::BaudRate::BAUD_RATE_57600,Usart0::Mode::ASYNCHRONOUS);
    Usart0 usart0(Usart0::BaudRate::BAUD_RATE_57600,Usart0::Mode::ASYNCHRONOUS);
    usart0.setDataSize(Usart0::DataSize::DATA_8_BITS);
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&usart0, 0);
    usart0.stdio();
    usart0.enableReceiver();
    usart0.enableTransmitter();
    usart0.init();
    char teste1[] = "qwertyuiop";
    timer0flag = 1;
    //Config Keypad
    uint8_t tecla = 0;

    // Enabling interrupts
    sei();

//////////////////////// Configuração do USART de software para bitbang de debugacao/////////////////////
    //Timer Configuration
    DDRD = 0xFF;
    setBit(PORTD, PD2);
    Timer0 timer0;
    timer0.setMode(Timer0::Mode::CTC_OCRA);
    timer0.setClockSource(Timer0::ClockSource::PRESCALER_8);
    timer0.setCompareAValue(208);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    delayMs(1000);

    char msg[] = "yey man";
    char rcvmsg[64];
    int i=0;
    while(1) {
        // sendstring("dentro do while");
        //getFingerprintID(&finger);
        // int i =0;
        usartsendstring("Ola, mandando coisas ");
        // usartgetstring(rcvmsg);
        usartsendstring(rcvmsg);
        rcvmsg[i] = i;
        //printf("%s",usart0._lastError);
        // while (msg[i] != '\0'){
        //     bitbang(msg[i]);
        //     i++;
        // }
        // usartsendstring(" fim de while sendstring ");
        bitbang('\n');
        delayMs(500);
    }

    return 0;
}

void timer0CompareACallback(void)
{
    timer0flag = 0;
    // cplBit(PORTD, PD2);
}
