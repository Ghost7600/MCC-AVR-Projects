#include "sdcard.hpp"



SDCard::SDCard(bool_t verbose = false)
{
    timer2.setCompareAValue(155);
    timer2.init(Timer2::Mode::NORMAL, Timer2::ClockSource::PRESCALER_1024);
    timer2.activateCompareAInterrupt();

    _verbose = verbose;

    sei();
}

bool_t SDCard::set_pin(pinLocation_e Pin)
{
    sdCardCreate(Pin);
    return true;
}

bool_t SDCard::init(FATFS *card, const TCHAR *path, BYTE init_opt)
{
    _card = card;
    _err = f_mount(card, path, init_opt);
    if(_err) {
        fprintf(stderr, "Erro %u ao montar disco\r\n", (uint8_t) _err);
        return false;
    }

    if(_verbose) {
        fprintf(stderr, "Disco montado com sucesso\r\n");
    }

    return true;
}

bool_t SDCard::open(const TCHAR *path, BYTE mode)
{
    _err = f_open(&_current_file, path, mode);
    if(_err) {
        fprintf(stderr, "Erro %u ao abrir arquivo %s\r\n", (uint8_t)_err, path);
        return false;
    }

    if(_verbose) {
        fprintf(stderr, "Arquivo %s aberto com sucesso\r\n", path);
    }

    return true;
}

bool_t SDCard::close(const TCHAR *path)
{
    _err = f_close(&_current_file);

    if(_err) {
        fprintf(stderr, "Erro %u ao fechar arquivo\r\n", (uint8_t)_err);
        return false;
    }

    if(_verbose) {
        fprintf(stderr, "Arquivo fechado com sucesso\r\n");
    }

    return true;
}

bool_t SDCard::write(void *str, uint8_t size, UINT *written)
{
    _err = f_write(&_current_file, str, size, written);
    if(_err) {
        fprintf(stderr, "Erro %u ao escrever dados\r\n", (uint8_t) _err);
        return false;
    }

    if(_verbose) {
        fprintf(stderr, "Arquivo escrito com sucesso\r\n");
    }

    return true;
}

bool_t SDCard::sync()
{
    _err = f_sync(&_current_file);

    if(_err) {
        fprintf(stderr, "Erro %u ao salvar dados\r\n", (uint8_t) _err);
        return false;
    }

    if(_verbose) {
        fprintf(stderr, "Arquivo salvo com sucesso\r\n");
    }

    return true;
}

bool_t SDCard::write_and_sync(void *str, uint8_t size, UINT *written)
{
    bool_t ret = 0;
    ret |= write(str, size, written);
    ret |= sync();
    return ret;
}

bool_t SDCard::read(void *buffer, uint8_t size, UINT *bytes_read)
{
    _err = f_read(&_current_file, buffer, size, bytes_read);

    if(_err) {
        fprintf(stderr, "Erro %u ao ler dados\r\n", (uint8_t) _err);
        return false;
    }

    if(_verbose) {
        fprintf(stderr, "Arquivo lido com sucesso\r\n");
    }

    return true;
}
// f_gets is not implemented
char *SDCard::read_line(TCHAR *buff, int len)
{
    return f_gets(buff, len, &_current_file);
}

// Handles periodic functions
void timer2CompareACallback(void)
{
    mmc_disk_timerproc();
}
