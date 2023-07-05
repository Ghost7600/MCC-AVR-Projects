
#ifndef __ZFM20_HPP
#define __ZFM20_HPP

#include "funsape/globalDefines.hpp"
#include "kbitbang.hpp"
#include "funsape/peripheral/usart0.hpp"

class Zfm20
{
public:
    enum class SByteValues {
        BYTE1        = 0xEF,
        BYTE0        = 0x02,
    };
    enum class Id {
        COMMAND     = 0x01,
        DATA        = 0x02,
        ACKNOWLEDGE = 0x07,
        END_OF_DATA = 0x08
    };

    enum class Rcv_Status {
        OK                      = 0x00,
        RCV_ERROR               = 0x01,
        NO_FINGER               = 0x02,
        IMG_ENTRY_FAILURE       = 0x03,
        MESSY_IMG               = 0x06,
        TOO_FEW_FEATURES        = 0x07,
        NO_MATCH                = 0x08,
        NO_FP_FOUND             = 0x09,
        MUST_CHECK_PWD          = 0x21,
    };

    Zfm20();

    uint8_t receive_pkg(Usart0* usart);

    bool_t init(
            cuint8_t bufferSize_p = 40
    );
    bool_t setAddress(
            cuint32_t deviceAddress_p
    );

    bool_t createPackage(
            Id packageId_p,
            uint8_t *packageData_p,
            uint8_t packageSize_p
    );

    bool_t sendPackage (Usart0 * usart);

    bool_t checkFingerPrint (Usart0* usart);

    void debug(void);

private:
    uint8_t         *_packageData;
    uint8_t         _packageIndex;
    uint8_t         _packageLength;
    uint8_t         _packageMaxSize;
    bool_t          _initialized;
    Error           _lastError;
    uint32_t        _deviceAddress;
    uint8_t         _packet[];

};


#endif
