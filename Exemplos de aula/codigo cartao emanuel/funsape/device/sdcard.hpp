//! \file           sdcard.hpp
//! \brief          Declares SDCard class

#ifndef SDCARD_HPP
#define SDCARD_HPP

#include "../../fatFs/ff.h"
#include "../../fatFs/mmc_avr.h"

#include "../peripheral/timer2.hpp"

class SDCard
{
private:
    FATFS *_card; // Volume unit
    FRESULT _err            : 5; // Error Codes (20 values)
    bool_t _verbose          : 1; // If false, only prints error codes
    FIL _current_file;
public:
    SDCard(bool_t verbose);

    //! \brief      Sets the pins the card is connected to
    bool_t set_pin(pinLocation_e Pin);

    //! \brief      Mounts the SDCard
    //! \note       This function is essentialy a wrapper for f_mount,
    //!         with added error tracking for convenience
    bool_t init(FATFS *card, const TCHAR *path, BYTE init_opt);

    //! \brief      Opens a file and stores it in _current_file
    bool_t open(const TCHAR *path, BYTE mode);

    //! \brief      Closes a given file
    //! \note       If no arguments are passed or if path is empty,
    //!         closes _current_file
    bool_t close(const TCHAR *path);

    //! \brief      Writes to _current_file
    bool_t write(void *str, uint8_t size, UINT *written);

    //! \brief      Syncs _current_file
    bool_t sync();

    //! \brief      Writes to _current_file and immediatly syncs it
    bool_t write_and_sync(void *str, uint8_t size, UINT *written);

    //! \brief      Reads whole file and stores it in buffer
    bool_t read(void *buffer, uint8_t size, UINT *bytes_read);

    //! \brief      Reads a single line from a file (text only)
    //! \note       Wrapper for f_gets, which wasn't implemented
    char *read_line(TCHAR *buff, int len);

    //! \brief      Sets the read/write pointer to the beginning of _current_file
    //! \note       Could cause crashes, but should be fine
    inline bool_t reset_pointer() {
        _current_file.fptr = 0;
        return true;
    }

    //! \brief      Sets the pointer to the end of _current_file
    //! \note       NOT IMPLEMENTED
    inline bool_t set_pointer_eof() {
        // ?
    }

    //! \brief      Sets the pointer to the beginning of a current line
    //! \note       NOT IMPLEMENTED
    inline bool_t set_pointer_line_start() {
        // ?
    }
};

#endif
