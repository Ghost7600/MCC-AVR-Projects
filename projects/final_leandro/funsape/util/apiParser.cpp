
#include "apiParser.hpp"

cuint8_t constStartDelimiter = 0x7E;

ApiParser::ApiParser(uint8_t _payloadMaxSizep, bool_t useEscapeChar_p)
{
    // Resets data members - Status and Control
    this->_initialized                  = false;
    this->_lastError                    = Error::NOT_INITIALIZED;
    this->_payloadMaxSize               = 0;
    this->_rawMaxSize                   = 0;
    this->_useEscapeChar                = false;
    // Resets data members - Rx Raw Data
    this->_rxRawChecksum                = 0xFF;
    this->_rxRawData                    = nullptr;
    this->_rxRawEscape                  = false;
    this->_rxRawIndex                   = 0;
    this->_rxRawReady                   = false;
    this->_rxRawSize                    = 0;
    this->_rxRawState                   = State::START_DELIMITER;
    // Resets data members - Tx Raw Data
    this->_txRawData                    = nullptr;
    this->_txRawIndex                   = 0;
    this->_txRawSize                    = 0;
    // Resets data members - Decoded Rx Raw Data
    this->_rxDecodedAuxData             = 0;
    this->_rxDecodedDestinyAddress      = 0;
    this->_rxDecodedId                  = 0;
    this->_rxDecodedPayloadData         = nullptr;
    this->_rxDecodedPayloadIndex        = 0;
    this->_rxDecodedPayloadSize         = 0;
    this->_rxDecodedSourceAddress       = 0;
    // Resets data members - Decoded Tx Raw Data
    this->_txDecodedDestinyAddress      = 0;
    this->_txDecodedId                  = 0;
    this->_txDecodedIdAutoIncrement     = false;
    this->_txDecodedPayloadData         = nullptr;
    this->_txDecodedPayloadIndex        = 0;
    this->_txDecodedPayloadSize         = 0;
    this->_txDecodedSourceAddress       = 0;

    // Checks for errors
    if(_payloadMaxSizep < 2) {
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        return;
    }

    // Updates data members
    this->_useEscapeChar = useEscapeChar_p;
    this->_payloadMaxSize = _payloadMaxSizep;
    this->_rawMaxSize = 5 + (uint16_t)_payloadMaxSizep; // (ID+DA+SA+SZ+CH)+PL
    if(useEscapeChar_p) {
        this->_rawMaxSize *= 2;                         // Double for escapes
    }
    this->_rawMaxSize++;                                // Add START

    // Allocates memory => Raw Rx Data
    this->_rxRawData = (uint8_t *)calloc(this->_rawMaxSize, sizeof(uint8_t));
    if(!isPointerValid(this->_rxRawData)) {
        // Returns error
        this->_lastError = Error::MEMORY_ALLOCATION;
        return;
    }
    // Allocates memory => Raw Tx Data
    this->_txRawData = (uint8_t *)calloc(this->_rawMaxSize, sizeof(uint8_t));
    if(!isPointerValid(this->_txRawData)) {
        // Returns error
        free(this->_rxRawData);
        this->_lastError = Error::MEMORY_ALLOCATION;
        return;
    }
    // Allocates memory => Decoded Rx Data
    this->_rxDecodedPayloadData = (uint8_t *)calloc(this->_payloadMaxSize, sizeof(uint8_t));
    if(!isPointerValid(this->_rxDecodedPayloadData)) {
        // Returns error
        free(this->_rxRawData);
        free(this->_txRawData);
        this->_lastError = Error::MEMORY_ALLOCATION;
        return;
    }
    // Allocates memory => Decoded Tx Data
    this->_txDecodedPayloadData = (uint8_t *)calloc(this->_payloadMaxSize, sizeof(uint8_t));
    if(!isPointerValid(this->_txDecodedPayloadData)) {
        // Returns error
        free(this->_rxRawData);
        free(this->_txRawData);
        free(this->_rxDecodedPayloadData);
        this->_lastError = Error::MEMORY_ALLOCATION;
        return;
    }
    this->_initialized = true;

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

// =============================================================================
// Class public methods
// =============================================================================

Error ApiParser::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

bool_t ApiParser::txCreatePackage(uint8_t *packageData_p, uint16_t *packageSize_p, uint16_t maxChars_p)
{
    // Checks for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if((!isPointerValid(packageData_p)) || (!isPointerValid(packageSize_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }

    // Resets Raw Tx data members
    this->_txRawIndex = 0;
    this->_txRawSize = 0;

    // Encodes raw package
    if(!this->_txEncodePackage()) {
        // Returns error
        return false;
    }

    // Transfer data
    for(uint16_t i = 0; i < this->_txRawSize; i++) {
        if(i == maxChars_p) {
            // Returns error
            this->_lastError = Error::BUFFER_FULL;
            return false;
        }
        packageData_p[i] = this->_txRawData[i];
    }
    *packageSize_p = this->_txRawSize;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::txSetDestinationAddress(uint8_t destinationAddress_p)
{
    // Checks for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }

    // Updates data members
    this->_txDecodedDestinyAddress = destinationAddress_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::txSetFrameId(uint8_t frameId_p, bool_t autoIncrement_p)
{
    // Checks for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }

    // Updates data members
    this->_txDecodedId = frameId_p;
    this->_txDecodedIdAutoIncrement = autoIncrement_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::txSetPayload(uint8_t *payload_p, uint8_t size_p)
{
    // Checks for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(size_p < 6) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        return false;
    }
    if(size_p > this->_payloadMaxSize) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
        return false;
    }
    if(!isPointerValid(payload_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }

    // Updates data members
    this->_txDecodedPayloadSize = size_p;
    for(uint8_t i = 0; i < size_p; i++) {
        this->_txDecodedPayloadData[i] = payload_p[i];
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::txSetSourceAddress(uint8_t sourceAddess_p)
{
    // Checks for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }

    // Updates data members
    this->_txDecodedSourceAddress = sourceAddess_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

// ////////////////////////////////////   RX   ////////////////////////////////////

bool_t ApiParser::rxAddData(uint8_t data_p)
{
    // Checks for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(this->_rxRawReady) {
        // Returns error
        this->_lastError = Error::PACKAGE_AWAITING;
        return false;
    }
    if(this->_rxRawState == ApiParser::State::READY) {
        // Returns error
        this->_lastError = Error::PACKAGE_AWAITING;
        return false;
    }

    // Updates auxData
    this->_rxDecodedAuxData = data_p;

    // Process escape character
    if(!this->_rxProcessEscapeCharacter()) {      // Could be an error, checking...
        if(this->_lastError == Error::NONE) {     // Not an error, return successful
            return true;
        } else {                                 // It is a really an error
            return false;
        }
    } // No error detected, continuing process

    // Check frame format
    if(!_rxProcessFrameFormat()) {                    // Frame error
        return false;
    }

    // Put data in raw package
    if(!this->_rxProcessRawAddData()) {
        return false;
    }

    // Evaluates states machine next state
    return this->_rxProcessNextState();
}

void ApiParser::rxFreePackage(void)
{
    this->_rxRawReady = false;
    this->_rxResetPackage();
}

bool_t ApiParser::rxGetDestinationAddress(uint8_t *destinationAddress_p)
{
    // Check for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(!isPointerValid(destinationAddress_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }

    // Updates data members
    *destinationAddress_p = this->_rxDecodedDestinyAddress;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::rxGetId(uint8_t *frameId_p)
{
    // Check for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(!isPointerValid(frameId_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }

    // Updates data members
    *frameId_p = this->_rxDecodedId;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::rxGetPayload(uint8_t *payloadData_p, uint8_t *payloadSize_p, uint8_t maxChars_p)
{
    // Check for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if((!isPointerValid(payloadData_p)) || (!isPointerValid(payloadSize_p))) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }

    // Retrieve payload size
    *payloadSize_p = this->_rxDecodedPayloadSize;

    // Retrieve payload data
    for(uint8_t i = 0; i < this->_rxDecodedPayloadSize; i++) {
        if(i == maxChars_p) {
            // Returns error
            this->_lastError = Error::BUFFER_FULL;
            return false;
        }
        payloadData_p[i] = this->_rxDecodedPayloadData[i];
    }

    // Returns successfuly
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::rxGetSourceAddress(uint8_t *sourceAddress_p)
{
    // Check for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(sourceAddress_p == NULL) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }

    // Updates data members
    *sourceAddress_p = this->_rxDecodedSourceAddress;

    // Returns successfuly
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::rxIsReady(void)
{
    // Returns value
    return this->_rxRawReady;
}

bool_t ApiParser::_txEncodePackage(void)
{
    // Local variables
    uint8_t auxChecksum = 0xFF;

    // Checks for errors - Zero payload size
    if(this->_txDecodedPayloadSize == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        return false;
    }

    // Updates data members
    this->_txRawSize = 0;
    this->_txRawIndex = 0;
    this->_txDecodedPayloadIndex = 0;

    // Creates raw package
    // Start delimiter
    this->_txRawData[this->_txRawIndex++] = constStartDelimiter;
    auxChecksum = 0xFF;

    // Package ID
    if(!this->_txAddData(this->_txDecodedId)) {
        // Returns error
        return false;
    }
    auxChecksum -= this->_txDecodedId;
    if(this->_txDecodedIdAutoIncrement) {
        this->_txDecodedId = (this->_txDecodedId == 255) ? 1 : (this->_txDecodedId + 1);
    }

    // Destination address
    if(!this->_txAddData(this->_txDecodedDestinyAddress)) {
        // Returns error
        return false;
    }
    auxChecksum -= this->_txDecodedDestinyAddress;

    // Source address
    if(!this->_txAddData(this->_txDecodedSourceAddress)) {
        // Returns error
        return false;
    }
    auxChecksum -= this->_txDecodedSourceAddress;

    // Payload size
    if(!this->_txAddData(this->_txDecodedPayloadSize)) {
        // Returns error
        return false;
    }
    auxChecksum -= this->_txDecodedPayloadSize;

    // Payload data
    for(uint8_t i = 0; i < this->_txDecodedPayloadSize; i++) {
        if(!this->_txAddData(this->_txDecodedPayloadData[i])) {
            // Returns error
            return false;
        }
        auxChecksum -= this->_txDecodedPayloadData[i];
    }

    // Checksum
    if(!this->_txAddData(auxChecksum)) {
        // Returns error
        return false;
    }

    // Final package flags setup
    this->_txRawSize = this->_txRawIndex;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::_txAddData(uint8_t data_p)
{
    // Checks for errors - Package size
    if((this->_txRawIndex + 2) >= this->_rawMaxSize) {
        // Returns error
        this->_lastError = Error::BUFFER_FULL;
        return false;
    }

    // Adds data to tx buffer
    if(!this->_useEscapeChar) {         // Escape character is DISABLED
        this->_txRawData[this->_txRawIndex++] = data_p;
    } else {                            // Escape character is ENABLED
        switch(data_p) {
        case 0x7E:
        case 0x7D:
        case 0x11:
        case 0x13:
            this->_txRawData[this->_txRawIndex++] = 0x7D;
            this->_txRawData[this->_txRawIndex++] = data_p ^ 0x20;
            break;
        default:
            this->_txRawData[this->_txRawIndex++] = data_p;
            break;
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::_rxProcessEscapeCharacter(void)
{
    // Escape character is not in use!
    if(!this->_useEscapeChar) {
        // Returns successfully
        this->_lastError = Error::NONE;
        return true;
    }

    // Escape character found!
    if(this->_rxDecodedAuxData == 0x7D) {
        // Check for error
        if(this->_rxRawEscape) {        // Last character was an escape!
            // Resets package
            this->_rxResetPackage();
            // Returns error
            this->_lastError = Error::FRAME_ERROR;
            return false;
        }
        // Mark as escape character
        this->_rxRawEscape = true;
        this->_rxProcessRawAddData();
        // Procedure was successful but, must indicate that the
        // function should end, because no next state must be
        // evaluated and no further data must be added to buffers
        // So, Error::NONE and false must be checked at caller function
        this->_lastError = Error::NONE;
        return false;
    }

    // Checks for misplaced start delimiter
    if(this->_rxDecodedAuxData == constStartDelimiter) {
        if(this->_rxRawState != State::START_DELIMITER) {
            // Start delimiter must not happen outside State::START_DELIMITER
            this->_rxResetPackage();
            this->_lastError = Error::FRAME_ERROR;
            return false;
        }
    }

    // Normal character found!
    if(this->_rxRawEscape) {         // It must be escaped?
        this->_rxDecodedAuxData ^= 0x20;
        this->_rxRawEscape = false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::_rxProcessFrameFormat(void)
{
    switch(this->_rxRawState) {
    case State::START_DELIMITER:
        if(this->_rxDecodedAuxData != constStartDelimiter) {
            this->_lastError = Error::FRAME_ERROR;
            this->_rxResetPackage();
            return false;
        }
        break;
    case State::PAYLOAD_SIZE:
        if(this->_rxDecodedAuxData == 0) {
            this->_lastError = Error::FRAME_ERROR;
            this->_rxResetPackage();
            return false;
        }
        break;
    case State::PAYLOAD_DATA:
        if((this->_rxDecodedPayloadIndex + 1) == this->_payloadMaxSize) {
            this->_rxResetPackage();
            this->_lastError = Error::BUFFER_FULL;
            return false;
        }
        break;
    case State::CHECKSUM:
        if(this->_rxDecodedAuxData != this->_rxRawChecksum) {    // Checksum error
            this->_rxResetPackage();
            this->_lastError = Error::CHECKSUM_ERROR;
            return false;
        }
    default:
        break;
    }

    // Everything OK
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::_rxProcessNextState(void)
{
    if(this->_rxRawEscape) {
        return true;
    }

    switch(this->_rxRawState) {
    case State::START_DELIMITER:
        this->_rxRawChecksum = 0xFF;
        this->_rxRawState++;
        break;
    case State::ID:
        this->_rxDecodedId = this->_rxDecodedAuxData;
        this->_rxRawChecksum -= this->_rxDecodedAuxData;
        this->_rxRawState++;
        break;
    case State::DESTINATION_ADDRESS:
        this->_rxDecodedDestinyAddress = this->_rxDecodedAuxData;
        this->_rxRawChecksum -= this->_rxDecodedAuxData;
        this->_rxRawState++;
        break;
    case State::SOURCE_ADDRESS:
        this->_rxDecodedSourceAddress = this->_rxDecodedAuxData;
        this->_rxRawChecksum -= this->_rxDecodedAuxData;
        this->_rxRawState++;
        break;
    case State::PAYLOAD_SIZE:
        this->_rxDecodedPayloadIndex = 0;
        this->_rxDecodedPayloadSize = this->_rxDecodedAuxData;
        this->_rxRawChecksum -= this->_rxDecodedAuxData;
        this->_rxRawState++;
        break;
    case State::PAYLOAD_DATA:
        this->_rxDecodedPayloadData[this->_rxDecodedPayloadIndex++] = this->_rxDecodedAuxData;
        this->_rxRawChecksum -= this->_rxDecodedAuxData;
        if(this->_rxDecodedPayloadIndex == this->_rxDecodedPayloadSize) {
            this->_rxRawState++;
        }
        break;
    case State::CHECKSUM:
        this->_rxRawReady = true;
        this->_rxRawSize = this->_rxRawIndex;
        this->_rxRawState = State::READY;
        break;
    default:
        // Unreachable code area
        this->_rxResetPackage();
        this->_lastError = Error::UNKNOWN;
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t ApiParser::_rxProcessRawAddData(void)
{
    if((this->_rxRawIndex + 1) == this->_rawMaxSize) {
        // Resets package
        this->_rxResetPackage();
        // Returns error
        this->_lastError = Error::BUFFER_FULL;
        return false;
    }

    // Updates data members
    this->_rxRawData[this->_rxRawIndex++] = this->_rxDecodedAuxData;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

void ApiParser::_rxResetPackage(void)
{
    // Resets package
    this->_rxDecodedPayloadSize = 0;
    this->_rxDecodedPayloadIndex = 0;
    this->_rxRawChecksum = 0xFF;
    this->_rxRawSize = 0;
    this->_rxRawIndex = 0;
    this->_rxRawReady = false;
    this->_rxRawState = State::START_DELIMITER;
    this->_rxRawEscape = false;
}
