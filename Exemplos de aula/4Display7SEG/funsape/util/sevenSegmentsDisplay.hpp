/* =============================================================================
 * Project:         FunSAPE AVR8++ Integrated Library
 * File:            sevenSegmentsDisplay.hpp
 * Description:     Seven segments display decoder
 * Author:          Leandro Schwarz
 * Version:         202303
 * Last edition:    2023-03-19
 * Purpose:         Seven segments display controller with support to common
 *                      anode and common cathode displays. Special characters
 *                      dash (0xFD), U (0xFE) and display off (0xFF) were also
 *                      implented
 * ========================================================================== */

// =============================================================================
// Include guard (START)
// =============================================================================

#ifndef __FUNSAPE_AVR8_LIB_SEVEN_SEGMENTS_DISPLAY_HPP
#define __FUNSAPE_AVR8_LIB_SEVEN_SEGMENTS_DISPLAY_HPP   202303

// =============================================================================
// Header file dependencies
// =============================================================================

//     //////////////////     LIBRARY DEPENDENCIES     //////////////////     //
#include "../globalDefines.hpp"
#ifndef __FUNSAPE_AVR8_LIB_GLOBAL_DEFINES_HPP
#   error Global definitions file must be corrupted or missing!
#elif __FUNSAPE_AVR8_LIB_GLOBAL_DEFINES_HPP != 202303
#    error "Version mismatch between header file and library dependency (globalDefines.hpp)!"
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

#define DISPLAY_DASH                    0xFD
#define DISPLAY_U                       0xFE
#define DISPLAY_OFF                     0xFF

// =============================================================================
// New data types
// =============================================================================

typedef enum displayType_t {
    COMMON_ANODE                        = 0,
    COMMON_CATHODE                      = 1
} displayType_t;

// =============================================================================
// Function declarations
// =============================================================================

uint8_t hexToSevenSegments(              // Segments configuration with format 0bPGFEDCBA
        uint8_t hexValue_p,             // Hex number or special character value
        displayType_t displayType_p,    // Display type (use enumerations at displayType_t)
        bool_t point_p                  // Turns point on/off
);

// =============================================================================
// Include guard (END)
// =============================================================================

#endif  // __FUNSAPE_AVR8_LIB_SEVEN_SEGMENTS_DISPLAY_HPP

// =============================================================================
// END OF FILE: sevenSegmentsDisplay.hpp
// =============================================================================
