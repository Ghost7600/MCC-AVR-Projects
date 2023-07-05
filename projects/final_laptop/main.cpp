// Controller 1 - Keypad/Transmitter Code

//---DEFINIÇÔES---//
#define F_CPU 16000000UL

//---INCLUDES---//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/int1.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "funsape/peripheral/usart0.hpp"
#include "Adafruit_Fingerprint.h"
#include "kbitbang1.hpp"
#include "klib/adafp.hpp"

//global variables




unsigned char uartgetchar()
{
    // Local variables
    uint8_t aux8;
    uint8_t auxb;


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

    // Debug verbose, send byte back
    // while (!(UCSR0A & (1<<UDRE0)));
    // UDR0 = auxb;

    return auxb;
}

void usartgetstring(char string[64])
{
    unsigned char ch;
    int i = 0;
    memset(string, '\0', sizeof(string));
    usartsendstring("usartgetstring \n");
    while((ch = uartgetchar()) != '\0') {
        string[i] = ch;
        i++;
    }
    while(i < 65) {
        string[i] = '\0';
        i++;
    }
}

uint8_t getFingerprintID(Adafruit_Fingerprint *finger)
{
    sendstring(" entrou na get finger id ");
    uint8_t p = finger->getImage();
    sendstring("depois da getimage");
    switch(p) {
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
    switch(p) {
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
    if(p == FINGERPRINT_OK) {
        sendstring("Found a print match!");
    } else if(p == FINGERPRINT_PACKETRECIEVEERR) {
        sendstring("Communication error");
        return p;
    } else if(p == FINGERPRINT_NOTFOUND) {
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

int getFingerprintIDez(Adafruit_Fingerprint *finger)
{
    uint8_t p = finger->getImage();
    if(p != FINGERPRINT_OK) {
        return -1;
    }

    p = finger->image2Tz();
    if(p != FINGERPRINT_OK) {
        return -1;
    }

    p = finger->fingerFastSearch();
    if(p != FINGERPRINT_OK) {
        return -1;
    }

    // found a match!
    sendstring("Found ID #"); usart0.write(finger->fingerID);
    sendstring(" with confidence of "); usart0.write(finger->confidence);
    return finger->fingerID;
}

//---MAIN---//
int main()
{
    //////////////////// Configuração Usart de hardware para comunicação com o sensor ///////////////////
    //Usart0 usart0(Usart0::BaudRate::BAUD_RATE_57600,Usart0::Mode::ASYNCHRONOUS);
    Usart0 kusart0(Usart0::BaudRate::BAUD_RATE_57600, Usart0::Mode::ASYNCHRONOUS, Usart0::FrameFormat::FRAME_FORMAT_8_N_1);
    kusart0.setDataSize(Usart0::DataSize::DATA_8_BITS);
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&kusart0, 0);
    kusart0.stdio();
    kusart0.enableReceiver();
    kusart0.enableTransmitter();
    char teste1[] = "qwertyuiop";
    //Config Keypad
    uint8_t tecla = 0;

    // Enabling interrupts
    sei();

//////////////////////// Configuração do USART de software para bitbang de debugacao/////////////////////
    //Timer Configuration
    DDRD = 0xFF;
    setBit(PORTD, PD3);
    Timer1 timer1;
    timer1.setMode(Timer1::Mode::CTC_OCRA);
    timer1.setClockSource(Timer1::ClockSource::PRESCALER_8);
    timer1.setCompareAValue(208);
    sendstring("Booted UP!");
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    delayMs(1000);

    char msg[] = "yey man \n";
    char rcvmsg[8];
    unsigned char chh = 'a';
    uint8_t tester = 0;
    int i = 0;
    memset(rcvmsg, '\0', sizeof(rcvmsg));
    sendstring("hello");

    // set the data rate for the sensor serial port
    //finger.begin(57600);
//   delayMs(5);
//   if (finger.verifyPassword()) {
//     sendstring("Found fingerprint sensor!");
//   } else {
//     sendstring("Did not find fingerprint sensor :(");
//     while (1) { delayMs(1); }
//   }
    finger.LEDcontrol(false);
    // delayMs(300);
    // finger.LEDcontrol(1);
    // sendstring("Reading sensor parameters");
    // delayMs(20);
    // finger.getParameters();
    // sprintf(rcvmsg, "%02X", finger.status_reg);
    // sendstring("Status: 0x"); sendstring(rcvmsg);
    // delayMs(20);
    // bitbang(finger.status_reg);
    // sprintf(rcvmsg, "%02X", finger.system_id);
    // sendstring("Sys ID: 0x"); sendstring(rcvmsg);
    // sprintf(rcvmsg, "%02X", finger.capacity);
    // sendstring("Capacity: "); sendstring(rcvmsg);
    // sprintf(rcvmsg, "%02X", finger.security_level);
    // sendstring("Security level: "); sendstring(rcvmsg);
    // sprintf(rcvmsg, "%02X", finger.device_addr);
    // sendstring("Device address: "); sendstring(rcvmsg);
    // sprintf(rcvmsg, "%02X", finger.packet_len);
    // sendstring("Packet len: "); sendstring(rcvmsg);
    // sprintf(rcvmsg, "%02X", finger.baud_rate);
    // sendstring("Baud rate: "); sendstring(rcvmsg);

    // finger.getTemplateCount();

    // if(finger.templateCount == 0) {
    //     sendstring("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    // } else {
    //     sendstring("Waiting for valid finger...");
    //     sendstring("Sensor contains ");
    //     sprintf(rcvmsg, "%02X", finger.templateCount);
    //     sendstring(rcvmsg); sendstring(" templates");
    // }


    while(1) {
        sendstring("teste");
        sendstring("\n");
        getFingerprintID(&finger);
        //mandando endereo do sensor
        //signalbegin();
        // int i =0;
        delayMs(200);
        // usartsendstring("Ola, mandando coisas \n");
        // usart0.read(&tester);
        // usart0.write(tester);
        //usart0.write('a');
        // usartgetstring(rcvmsg);
        // delayMs(1500);
        // usartsendstring(rcvmsg);
        // sendstring(rcvmsg);
        // delayMs(500);
    }

    return 0;
}
