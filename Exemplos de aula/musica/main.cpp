//Definições
#define F_CPU 16000000UL

//Includes
#include "funsape/globalDefines.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "notes.hpp"


int main()
{
    //Configure sounder pin as output
    setBit(DDRB, PB1);

    //configure TIMER 1
    timer1.init(Timer1::Mode::CTC_OCRA, Timer1::ClockSource::DISABLED);
    timer1.setOutputMode(Timer1::OutputMode::TOGGLE_ON_COMPARE, Timer1::OutputMode::NORMAL);

    timer1.setCounterValue(0);
    timer1.setClockSource(notesPrescaler[(uint8_t)Note::NOTE_A4]);
    timer1.setCompareAValue(notesOcrValue[(uint8_t)Note::NOTE_A4]);

    while(1) {
        Note music[] = {}
                playnotes(music);
        timer1.setCounterValue(0);
        timer1.setClockSource(notesPrescaler[(uint8_t)Note::NOTE_A4]);
        timer1.setCompareAValue(notesOcrValue[(uint8_t)Note::NOTE_A4]);

    }

    return 0;
}
