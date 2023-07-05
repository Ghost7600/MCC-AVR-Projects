// *INDENT-OFF*
/* =============================================================================
 * Project:
 * File name:       circularBuffer.hpp
 * Module:          Generic data-wide circular buffer module
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H                     __BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

#include "globalDefines.h"

// =============================================================================
// Platform verification
// =============================================================================

// NONE

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// NONE
// *INDENT-ON*
// *INDENT-OFF*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// CircularBuffer - Class declaration
// =============================================================================

template<typename T>
class CircularBuffer
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Operators overloading ---------------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    CircularBuffer(
        void
    );
    CircularBuffer(
        cuint16_t   bufferSize_p,
        bool        overwrite_p     = false
    );
    // TODO: Implement destructor function
    ~CircularBuffer(
        void
    );

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    uint16_t getFreeSpace(
        void
    );
    Error getLastError(
        void
    );
    uint16_t getOccupation(
        void
    );
    bool init(
        cuint16_t   bufferSize_p    = 20,
        bool        overwrite_p     = false
    );

    bool isEmpty(
        void
    );
    bool isFull(
        void
    );
    bool isInitialized(
        void
    );

    //     //////////////////    DATA MANIPULATION     //////////////////     //
    bool flush(
        bool bypassProtection_p = false
    );
    bool pop(
        T *data_p,
        bool keepData_p = false
    );
    bool pop(
        T *bufData_p,
        cuint16_t bufSize_p,
        bool keepData_p = false
    );
    bool push(
        T data_p
    );
    bool push(
        T *bufData_p,
        cuint16_t bufSize_p
    );

    //     ///////////////////     BUFFER CONTROL     ///////////////////     //
    void setBlockRead(
        bool block_p
    );
    void setBlockWrite(
        bool block_p
    );

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool _isLocked(
        void
    );
    bool _isReadProtected(
        void
    );
    bool _isWriteProtected(
        void
    );

    //     /////////////    BUFFER POINTER MANIPULATION     /////////////     //
    void _moveBothPointers(
        void
    );
    void _moveReadPointer(
        void
    );
    void _moveWritePointer(
        void
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     ///////////////////////     STATUS     ///////////////////////     //
    bool        _empty                  : 1;
    bool        _full                   : 1;
    bool        _initialized            : 1;
    bool        _locked                 : 1;
    bool        _overwriting            : 1;
    bool        _rdProtected            : 1;
    bool        _wrProtected            : 1;

    //     ////////////////////    CONFIGURATION     ////////////////////     //
    uint16_t    _maxSize;

    //     /////////////////////    BUFFER DATA     /////////////////////     //
    T           *_data;
    uint16_t    _rdIndex;
    uint16_t    _wrIndex;
    uint16_t    _occupation;

    //     ///////////////////     ERROR MESSAGES     ///////////////////     //
    Error       _lastError;

protected:
    // NONE

};  // class CircularBuffer

// =============================================================================
// CircularBuffer - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// =============================================================================
// CircularBuffer - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief Construct a new Circular Buffer<T>::Circular Buffer object
 * @details
 * @tparam      T
 */
template<typename T> CircularBuffer<T>::CircularBuffer(void)
{
    // Locking procedure
    this->_locked = true;                       // Locks circular buffer
    this->_rdProtected = true;                  // Protects against read operations
    this->_wrProtected = true;                  // Protects against write operations

    // Reset data members
    this->_data = nullptr;
    this->_empty = true;
    this->_full = false;
    this->_initialized = false;
    this->_maxSize = 0;
    this->_occupation = 0;
    this->_overwriting = false;
    this->_rdIndex = 0;
    this->_wrIndex = 0;

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

/**
 * @brief Construct a new Circular Buffer<T>::Circular Buffer object
 * @details
 * @tparam      T
 * @param[in]   bufferSize_p
 * @param[in]   overwrite_p
 */
template<typename T> CircularBuffer<T>::CircularBuffer(cuint16_t bufferSize_p, bool overwrite_p)
{
    // Locking procedure
    this->_locked = true;                       // Locks circular buffer
    this->_rdProtected = true;                  // Protects against read operations
    this->_wrProtected = true;                  // Protects against write operations

    // Reset data members
    this->_data = nullptr;
    this->_empty = true;
    this->_full = false;
    this->_initialized = false;
    this->_maxSize = 0;
    this->_occupation = 0;
    this->_overwriting = false;
    this->_rdIndex = 0;;
    this->_wrIndex = 0;

    // CHECK FOR ERROR - Zero-sized buffer
    if(bufferSize_p == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        return;
    }
    // CHECK FOR ERROR - Size too small
    if(bufferSize_p < 2) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        return;
    }

    // Allocate memory
    this->_data = (T *)calloc(bufferSize_p, sizeof(T));
    if(!isPointerValid(this->_data)) {
        // Returns error
        this->_lastError = Error::MEMORY_ALLOCATION;
        return;
    }

    // Update data members
    this->_initialized = true;
    this->_maxSize = bufferSize_p;
    this->_overwriting = overwrite_p;

    // Unlock procedure
    this->_locked = false;                      // Unlocks circular buffer
    this->_rdProtected = false;                 // Allows read operations
    this->_wrProtected = false;                 // Allows write operations

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

/**
 * @brief Destroy the Circular Buffer<T>::Circular Buffer object
 * @details
 * @tparam      T
 */
template<typename T> CircularBuffer<T>::~CircularBuffer(void)
{
    // Locking procedure
    this->_locked = true;                       // Locks circular buffer
    this->_rdProtected = true;                  // Protects against read operations
    this->_wrProtected = true;                  // Protects against write operations

    // Is there some memory allocated to buffer?
    if(isPointerValid(this->_data)) {
        free(this->_data);
    }
    this->_data = nullptr;                          // Clears pointer

    // Reset data members
    this->_empty = true;
    this->_full = false;
    this->_initialized = false;
    this->_maxSize = 0;
    this->_occupation = 0;
    this->_overwriting = false;
    this->_rdIndex = 0;
    this->_wrIndex = 0;

    // Returns successfully (but do NOT unlock)
    this->_lastError = Error::NONE;
    return;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 * @details
 * @tparam      T
 * @return
 */
template<typename T> uint16_t CircularBuffer<T>::getFreeSpace(void)
{
    // Returns amount of free space
    this->_lastError = Error::NONE;
    return (this->_maxSize - this->_occupation);
}

/**
 * @brief
 * @details
 * @tparam      T
 * @return
 */
template<typename T> Error CircularBuffer<T>::getLastError(void)
{
    // Returns successfully
    return this->_lastError;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @return
 */
template<typename T> uint16_t CircularBuffer<T>::getOccupation(void)
{
    // Returns amount of occupied space
    this->_lastError = Error::NONE;
    return this->_occupation;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @param[in]   bufferSize_p
 * @param[in]   overwrite_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::init(cuint16_t bufferSize_p, bool overwrite_p)
{
    // CHECK FOR ERROR - Zero-sized buffer
    if(bufferSize_p == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        return false;
    }
    // CHECK FOR ERROR - Size too small
    if(bufferSize_p < 2) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        return false;
    }

    // Locking procedure
    this->_locked = true;                       // Locks circular buffer
    this->_rdProtected = true;                  // Protects against read operations
    this->_wrProtected = true;                  // Protects against write operations

    // Deallocate memory (if needed)
    if(isPointerValid(this->_data)) {
        free(this->_data);
    }
    this->_data = nullptr;                          // Clears pointer

    // Reset data members
    this->_empty = true;
    this->_full = false;
    this->_initialized = false;
    this->_maxSize = 0;
    this->_occupation = 0;
    this->_overwriting = false;
    this->_rdIndex = 0;;
    this->_wrIndex = 0;

    // Allocate memory
    this->_data = (T *)calloc(bufferSize_p, sizeof(T));
    if(!isPointerValid(this->_data)) {
        // Returns error
        this->_lastError = Error::MEMORY_ALLOCATION;
        return false;
    }

    // Update data members
    this->_initialized = true;
    this->_maxSize = bufferSize_p;
    this->_overwriting = overwrite_p;

    // Unlock procedure
    this->_locked = false;                      // Unlocks circular buffer
    this->_rdProtected = false;                 // Allows read operations
    this->_wrProtected = false;                 // Allows write operations

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::isEmpty(void)
{
    // Returns whether buffer is empty or not
    this->_lastError = Error::NONE;
    return this->_empty;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::isFull(void)
{
    // Returns whether buffer is full or not
    this->_lastError = Error::NONE;
    return this->_full;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::isInitialized(void)
{
    // CHECK FOR ERROR - peripheral not initialized
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

//     ////////////////////    DATA MANIPULATION     ////////////////////     //

/**
 * @brief
 * @details
 * @tparam      T
 * @param[in]   bypassProtection_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::flush(bool bypassProtection_p)
{
    // CHECK FOR ERROR - Not initialized
    if(!this->isInitialized()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Locked
    if(this->_isLocked()) {
        // Returns error
        return false;
    }
    // Must bypass read and write protection?
    if(!bypassProtection_p) {                   // No, lets see if any protection is on!
        // CHECK FOR ERROR - Read protected
        if(this->_isReadProtected()) {
            // Returns error
            return false;
        }
        // CHECK FOR ERROR - Write protected
        if(this->_isWriteProtected()) {
            // Returns error
            return false;
        }
    }

    // Locking procedure
    this->_locked = true;                       // Locks circular buffer
    this->_rdProtected = true;                  // Protects against read operations
    this->_wrProtected = true;                  // Protects against write operations

    // Flushes data
    this->_empty = true;
    this->_full = false;
    this->_occupation = 0;
    this->_rdIndex = 0;;
    this->_wrIndex = 0;

    // Unlock procedure (also resets protection)
    this->_locked = false;                      // Unlocks circular buffer
    this->_rdProtected = false;                 // Allows read operations
    this->_wrProtected = false;                 // Allows write operations

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @param[out]  data_p
 * @param[in]   keepData_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::pop(T *data_p, bool keepData_p)
{
    // CHECK FOR ERROR - Argument is a NULL pointer
    if(!isPointerValid(data_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }
    // CHECK FOR ERROR - Not initialized
    if(!this->isInitialized()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Locked
    if(this->_isLocked()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Read protected
    if(this->_isReadProtected()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Buffer empty
    if(this->_empty) {
        // Returns error
        this->_lastError = Error::BUFFER_EMPTY;
        return false;
    }

    // Locking procedure
    this->_locked = true;                       // Locks circular buffer

    // Gets data
    *data_p = this->_data[this->_rdIndex];      // Retrieves data
    // Must move pointer?
    if(!keepData_p) {                           // Yes, pointer must be moved!
        this->_moveReadPointer();                       // Moves pointer
    }

    // Unlocking procedure
    this->_locked = false;                      // Unlocks circular buffer

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @param[out]  bufData_p
 * @param[in]   bufSize_p
 * @param[in]   keepData_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::pop(T *bufData_p, cuint16_t bufSize_p, bool keepData_p)
{
    // CHECK FOR ERROR - Argument is a NULL pointer
    if(!isPointerValid(bufData_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }
    // CHECK FOR ERROR - Buffer is zero-sized
    if(bufSize_p == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        return false;
    }
    // CHECK FOR ERROR - Not initialized
    if(!this->isInitialized()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Locked
    if(this->_isLocked()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Read protected
    if(this->_isReadProtected()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Not enough elements to get from buffer
    if(this->_occupation < bufSize_p) {
        // Returns error
        this->_lastError = Error::BUFFER_NOT_ENOUGH_ELEMENTS;
        return false;
    }

    // Locking procedure
    this->_locked = true;                       // Locks circular buffer

    // Keep record of current buffer status
    bool        emptyOld                = this->_empty;
    bool        fullOld                 = this->_full;
    uint16_t    occupationOld           = this->_occupation;
    uint16_t    rdIndexOld              = this->_rdIndex;

    // Get elements from buffer
    for(uint16_t i = 0; i < bufSize_p; i++) {
        bufData_p[i] = this->_data[this->_rdIndex]; // Retrieves data
        this->_moveReadPointer();                   // Moves READ pointer
    }

    // Must keep old data?
    if(keepData_p) {                                // Yes, load old status!
        this->_rdIndex = rdIndexOld;
        this->_occupation = occupationOld;
        this->_empty = emptyOld;
        this->_full = fullOld;
    }

    // Unlocking procedure
    this->_locked = false;                      // Unlocks circular buffer

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @param[in]   data_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::push(T data_p)
{
    // CHECK FOR ERROR - Not initialized
    if(!this->isInitialized()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Locked
    if(this->_isLocked()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Write protected
    if(this->_isWriteProtected()) {
        return false;
    }

    // Locking procedure
    this->_locked = true;                       // Locks circular buffer

    // Is buffer full?
    if(!this->_full) {                          // No, normal operation!
        this->_data[this->_wrIndex] = data_p;           // Stores data
        this->_moveWritePointer();                      // Moves WRITE pointer only
    } else {                                    // Yes, must check what to do!
        // Is overwriting allowed?
        if(this->_overwriting) {                        // Yes, overwrites old data!
            this->_data[this->_wrIndex] = data_p;               // Stores data
            this->_moveBothPointers();                          // Moves BOTH pointers
        } else {                                        // No, trow error!
            // Returns error
            this->_locked = false;              // Unlocking procedure before exit
            this->_lastError = Error::BUFFER_FULL;
            return false;
        }
    }

    // Unlocking procedure
    this->_locked = false;                      // Unlocks circular buffer

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @param[in]   bufData_p
 * @param[in]   bufSize_p
 * @return true
 * @return false
 */
template<typename T> bool CircularBuffer<T>::push(T *bufData_p, cuint16_t bufSize_p)
{
    // CHECK FOR ERROR - Argument is a NULL pointer
    if(!isPointerValid(bufData_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }
    // CHECK FOR ERROR - Buffer is zero-sized
    if(bufSize_p == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        return false;
    }
    // CHECK FOR ERROR - Not initialized
    if(!this->isInitialized()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Buffer is locked
    if(this->_isLocked()) {
        // Returns error
        return false;
    }
    // CHECK FOR ERROR - Write protected
    if(this->_isWriteProtected()) {
        // Returns error
        return false;
    }

    // Locking procedure
    this->_locked = true;                       // Locks circular buffer

    uint16_t freeSpace = this->_maxSize - this->_occupation;
    // Is there enough space?
    if(freeSpace >= bufSize_p) {                    // Yes, normal operation!
        for(uint16_t i = 0; i < bufSize_p; i++) {
            this->_data[this->_wrIndex] = bufData_p[i]; // Stores data
            this->_moveWritePointer();                  // Moves WRITE pointer
        }
    } else {                                    // No, must check what to do!
        // Is overwriting allowed?
        if(this->_overwriting) {                        // Yes, split operation!
            for(uint16_t i = 0; i < bufSize_p; i++) {
                this->_data[this->_wrIndex] = bufData_p[i];     // Stores data
                // Is buffer full?
                if(this->_full) {                               // Yes, move BOTH pointers!
                    this->_moveBothPointers();
                } else {                                        // No, move WRITE pointer only!
                    this->_moveWritePointer();
                }
            }
        } else {                                        // No, trow error!
            // Returns error
            this->_locked = false;              // Unlocking procedure before exit
            this->_lastError = Error::BUFFER_NOT_ENOUGH_SPACE;
            return false;
        }
    }

    // Unlocking procedure
    this->_locked = false;                      // Unlocks circular buffer

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

//     /////////////////////     BUFFER CONTROL     /////////////////////     //

/**
 * @brief
 * @details
 * @tparam      T
 * @param[in]   block_p
 */
template<typename T> void CircularBuffer<T>::setBlockRead(bool block_p)
{
    // Update data members
    this->_rdProtected = block_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @param[in]   block_p
 */
template<typename T> void CircularBuffer<T>::setBlockWrite(bool block_p)
{
    // Update data members
    this->_wrProtected = block_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

/**
 * @brief
 * @details
 * @tparam      T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_isLocked(void)
{
    // Check locking status
    if(this->_locked) {
        // Returns error
        this->_lastError = Error::LOCKED;
        return true;
    }

    // Returns successfully (false - not locked)
    this->_lastError = Error::NONE;
    return false;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_isReadProtected(void)
{
    // Check protection status
    if(this->_rdProtected) {
        // Returns error
        this->_lastError = Error::READ_PROTECTED;
        return true;
    }

    // Returns successfully (false - not protected)
    this->_lastError = Error::NONE;
    return false;
}

/**
 * @brief
 * @details
 * @tparam      T
 * @return true
 * @return false
 */
template<typename T> bool inlined CircularBuffer<T>::_isWriteProtected(void)
{
    // Check protection status
    if(this->_wrProtected) {
        // Returns error
        this->_lastError = Error::WRITE_PROTECTED;
        return true;
    }

    // Returns successfully (false - not protected)
    this->_lastError = Error::NONE;
    return false;
}

//     ///////////////    BUFFER POINTER MANIPULATION     ///////////////     //

/**
 * @brief
 * @details
 * @tparam      T
 */
template<typename T> void inlined CircularBuffer<T>::_moveBothPointers(void)
{
    // Move buffer pointers
    this->_wrIndex++;                           // Increments write pointer
    this->_wrIndex %= this->_maxSize;           // Resolves write pointer overflow
    this->_rdIndex++;                           // Increments read pointer
    this->_rdIndex %= this->_maxSize;           // Resolves read pointer overflow
    this->_full = (this->_occupation == this->_maxSize);    // Resolves full status
    this->_empty = (this->_occupation == 0);    // Resolves empty status

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

/**
 * @brief
 * @details
 * @tparam      T
 */
template<typename T> void inlined CircularBuffer<T>::_moveReadPointer(void)
{
    // Move buffer pointers
    this->_rdIndex++;                           // Increments read pointer
    this->_rdIndex %= this->_maxSize;           // Resolves read pointer overflow
    this->_occupation--;                        // Decreases occupation number
    this->_full = false;                        // Not full anymore
    this->_empty = (this->_occupation == 0);    // Resolves empty status

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

/**
 * @brief
 * @details
 * @tparam      T
 */
template<typename T> void inlined CircularBuffer<T>::_moveWritePointer(void)
{
    // Move buffer pointers
    this->_wrIndex++;                           // Increments write pointer
    this->_wrIndex %= this->_maxSize;           // Resolves write pointer overflow
    this->_occupation++;                        // Increases occupation number
    this->_full = (this->_occupation == this->_maxSize);    // Resolves full status
    this->_empty = false;                       // Not empty anymore

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_CIRCULAR_BUFFER_HPP

// =============================================================================
// END OF FILE
// =============================================================================
