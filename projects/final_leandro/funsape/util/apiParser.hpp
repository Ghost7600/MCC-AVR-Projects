//!
//! \file           apiParser.hpp
//! \brief          API Parser for the FunSAPE AVR8 Library
//! \author         Leandro Schwarz (bladabuska+funsapeavr8lib@gmail.com)
//! \date           2023-06-18
//! \version        23.04
//! \copyright      license
//! \details        API Parser for the FunSAPE AVR8 Library
//! \todo           Todo list
//!

// =============================================================================
// Include guard (START)
// =============================================================================

#ifndef __API_PARSER_HPP
#define __API_PARSER_HPP                2304

// =============================================================================
// Dependencies
// =============================================================================

//     /////////////////     GLOBAL DEFINITIONS FILE    /////////////////     //
#include "../globalDefines.hpp"
#if !defined(__GLOBAL_DEFINES_HPP)
#    error "Global definitions file is corrupted!"
#elif __GLOBAL_DEFINES_HPP != __API_PARSER_HPP
#    error "Version mismatch between file header and global definitions file!"
#endif

//     //////////////////     LIBRARY DEPENDENCIES     //////////////////     //
#include "../util/debug.hpp"
#if !defined(__DEBUG_HPP)
#   error "Header file (debug.hpp) is corrupted!"
#elif __DEBUG_HPP != __API_PARSER_HPP
#   error "Version mismatch between header file and library dependency (debug.hpp)!"
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Interrupt callback functions
// =============================================================================

// NONE

// =============================================================================
// ApiParser Class
// =============================================================================

//!
//! \brief          ApiParser class
//! \details        ApiParser class
//!
class ApiParser
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------
private:

    //     ////////////////    FRAME STATES MACHINE     /////////////////     //
    //!
    //! \brief          Brief description
    //! \details        Long description
    //!
    enum class State {
        START_DELIMITER = 0,
        ID,
        DESTINATION_ADDRESS,
        SOURCE_ADDRESS,
        PAYLOAD_SIZE,
        PAYLOAD_DATA,
        CHECKSUM,
        READY
    };

public:
    friend ApiParser::State &operator++(ApiParser::State &auxEnum_p);
    friend ApiParser::State operator++(ApiParser::State &auxEnum_p, int auxInt_p);
    friend ApiParser::State &operator--(ApiParser::State &auxEnum_p);
    friend ApiParser::State operator--(ApiParser::State &auxEnum_p, int auxInt_p);

public:
    ApiParser(
            uint8_t payloadMaxSize_p = 255,
            bool_t useEscapeChar_p = false
    );
    Error getLastError(
            void
    );
    bool_t rxAddData(
            uint8_t data_p
    );
    void rxFreePackage(
            void
    );
    bool_t rxGetDestinationAddress(
            uint8_t *destinationAddress_p
    );
    bool_t rxGetId(
            uint8_t *frameId_p
    );
    bool_t rxGetPayload(
            uint8_t *payloadData_p,
            uint8_t *payloadSize_p,
            uint8_t maxChars_p
    );
    bool_t rxGetSourceAddress(
            uint8_t *sourcAddress_p
    );
    bool_t rxIsReady(
            void
    );
    bool_t txCreatePackage(
            uint8_t *packageData_p,
            uint16_t *packageSize_p,
            uint16_t maxChars_p
    );
    bool_t txSetDestinationAddress(
            uint8_t destinationAddress_p
    );
    bool_t txSetFrameId(
            uint8_t id_p,
            bool_t autoIncrement_p
    );
    bool_t txSetPayload(
            uint8_t *payload_p,
            uint8_t size_p
    );
    bool_t txSetSourceAddress(
            uint8_t sourceAddress_p
    );

private:
    bool_t _rxProcessEscapeCharacter(
            void
    );
    bool_t _rxProcessFrameFormat(
            void
    );
    bool_t _rxProcessNextState(
            void
    );
    bool_t _rxProcessRawAddData(
            void
    );
    void _rxResetPackage(
            void
    );
    bool_t _txAddData(
            uint8_t data_p
    );
    bool_t _txEncodePackage(
            void
    );

private:
    //     /////////////////     STATUS AND CONTROL     /////////////////     //
    bool_t          _initialized                        : 1;
    Error           _lastError;
    uint8_t         _payloadMaxSize;
    bool_t          _useEscapeChar                      : 1;
    uint16_t        _rawMaxSize                         : 10;

    //     /////////////////     RAW RX DATA BUFFER     /////////////////     //
    uint8_t         _rxRawChecksum;
    uint8_t         *_rxRawData;
    bool_t          _rxRawEscape                        : 1;
    uint16_t        _rxRawIndex                         : 10;
    bool_t          _rxRawReady                         : 1;
    uint16_t        _rxRawSize                          : 10;
    State           _rxRawState;

    //     /////////////////     RAW TX DATA BUFFER     /////////////////     //
    uint8_t         *_txRawData;
    uint16_t        _txRawIndex                         : 10;
    uint16_t        _txRawSize                          : 10;

    //     ///////////////     DECODED RX DATA BUFFER     ///////////////     //
    uint8_t         _rxDecodedAuxData;
    uint8_t         _rxDecodedDestinyAddress;
    uint8_t         _rxDecodedId;
    uint8_t         *_rxDecodedPayloadData;
    uint8_t         _rxDecodedPayloadIndex;
    uint8_t         _rxDecodedPayloadSize;
    uint8_t         _rxDecodedSourceAddress;

    //     ///////////////     DECODED TX DATA BUFFER     ///////////////     //
    uint8_t         _txDecodedDestinyAddress;
    uint8_t         _txDecodedId;
    bool_t          _txDecodedIdAutoIncrement;
    uint8_t         *_txDecodedPayloadData;
    uint8_t         _txDecodedPayloadIndex;
    uint8_t         _txDecodedPayloadSize;
    uint8_t         _txDecodedSourceAddress;
}; // class ApiParser

inline ApiParser::State &operator++(ApiParser::State &auxEnum_p)
{
    auxEnum_p = (auxEnum_p == ApiParser::State::READY)
            ? ApiParser::State::START_DELIMITER
            : (ApiParser::State)(static_cast<uint8_t>(auxEnum_p) + 1);
    return auxEnum_p;
}

inline ApiParser::State operator++(ApiParser::State &auxEnum_p, int auxInt_p)
{
    ApiParser::State auxEnum_p2(auxEnum_p);
    ++auxEnum_p;
    return auxEnum_p2;
}

inline ApiParser::State &operator--(ApiParser::State &auxEnum_p)
{
    auxEnum_p = (auxEnum_p == ApiParser::State::START_DELIMITER)
            ? ApiParser::State::READY
            : (ApiParser::State)(static_cast<uint8_t>(auxEnum_p) - 1);
    return auxEnum_p;
}

inline ApiParser::State operator--(ApiParser::State &auxEnum_p, int auxInt_p)
{
    ApiParser::State auxEnum_p2(auxEnum_p);
    --auxEnum_p;
    return auxEnum_p2;
}

#endif // __API_PARSER_HPP
