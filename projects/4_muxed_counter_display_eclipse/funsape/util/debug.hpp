//!
//! \file           debug.hpp
//! \brief          Debug support to Funsape AVR8 Library
//! \author         Leandro Schwarz (bladabuska+funsapeavr8lib@gmail.com)
//! \date           2023-04-05
//! \version        23.04
//! \copyright      license
//! \details        This file provides extended debug support to all modules of
//!                     the Funsape AVR8 Library
//! \todo           This file is just a stub. Must implement the full file.
//!

// =============================================================================
// Include guard (START)
// =============================================================================

#ifndef __DEBUG_HPP
#define __DEBUG_HPP                     2304

// =============================================================================
// Dependencies
// =============================================================================

//     /////////////////     GLOBAL DEFINITIONS FILE    /////////////////     //
#include "../globalDefines.hpp"
#if !defined(__GLOBAL_DEFINES_HPP)
#   error "Global definitions file is corrupted!"
#elif __GLOBAL_DEFINES_HPP != __DEBUG_HPP
#   error "Version mismatch between file header and global definitions file!"
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
// Public functions declarations
// =============================================================================

// NONE

// =============================================================================
// Debug Class
// =============================================================================

class Debug
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------
public:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------
public:
    Debug(
            void
    );
    ~Debug(
            void
    );

    // -------------------------------------------------------------------------
    // Methods -----------------------------------------------------------------
public:
    //     ///////////////////     CONFIGURATION     ////////////////////     //
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------
private:
    // NONE
}; // class Debug

// =============================================================================
// Inlined class functions
// =============================================================================

// NONE

// =============================================================================
// External global variables
// =============================================================================

extern Debug debug;

// =============================================================================
// Include guard (END)
// =============================================================================

#endif  // __DEBUG_HPP

// =============================================================================
// END OF FILE
// =============================================================================


#define debugMessage(errorCode, module)         {}
#define debugMark(string, module)               {}
