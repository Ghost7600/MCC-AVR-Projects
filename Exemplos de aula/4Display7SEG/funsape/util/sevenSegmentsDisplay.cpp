/* =============================================================================
 * Project:         FunSAPE AVR8++ Integrated Library
 * File:            sevenSegmentsDisplay.cpp
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
// Header file dependencies
// =============================================================================

#include "sevenSegmentsDisplay.hpp"
#ifndef __FUNSAPE_AVR8_LIB_SEVEN_SEGMENTS_DISPLAY_HPP
#   error Header file must be corrupted or missing!
#elif __FUNSAPE_AVR8_LIB_SEVEN_SEGMENTS_DISPLAY_HPP != 202303
#   error Version mismatch between header and source files.
#endif

// =============================================================================
// Function definitions
// =============================================================================

/* -----------------------------------------------------------------------------
 * Function:    hexToSevenSegments
 * Purpose:     Seven segments display decoder
 * Arguments:   hexValue        hex number or special character value
 *              displayType     Display type (use enumerations at displayType_t)
 *              point           ON (TRUE) or OFF (FALSE)
 * Returns:     segments configuration with format 0bPGFEDCBA
 * -------------------------------------------------------------------------- */

uint8_t hexToSevenSegments(uint8_t hexValue_p, displayType_t displayType_p, bool_t point_p)
{
    uint8_t auxsegments = 0;

    switch(hexValue_p) { // PGFEDCBA
    case 0x00:          auxsegments = 0b00111111;  break;
    case 0x01:          auxsegments = 0b00000110;  break;
    case 0x02:          auxsegments = 0b01011011;  break;
    case 0x03:          auxsegments = 0b01001111;  break;
    case 0x04:          auxsegments = 0b01100110;  break;
    case 0x05:          auxsegments = 0b01101101;  break;
    case 0x06:          auxsegments = 0b01111101;  break;
    case 0x07:          auxsegments = 0b00000111;  break;
    case 0x08:          auxsegments = 0b01111111;  break;
    case 0x09:          auxsegments = 0b01101111;  break;
    case 0x0A:          auxsegments = 0b01110111;  break;
    case 0x0B:          auxsegments = 0b01111100;  break;
    case 0x0C:          auxsegments = 0b00111001;  break;
    case 0x0D:          auxsegments = 0b01011110;  break;
    case 0x0E:          auxsegments = 0b01111001;  break;
    case 0x0F:          auxsegments = 0b01110001;  break;
    case DISPLAY_DASH:  auxsegments = 0b01000000;  break;  // Dash
    case DISPLAY_U:     auxsegments = 0b00111110;  break;  // U
    case DISPLAY_OFF:   auxsegments = 0b00000000;  break;  // OFF
    }
    if(point_p) {
        auxsegments |= (1 << 7);
    }
    auxsegments = (displayType_p == displayType_t::COMMON_CATHODE) ? auxsegments : (~auxsegments);

    // Returns successfully
    return auxsegments;
}

// =============================================================================
// END OF FILE: sevenSegmentsDisplay.cpp
// =============================================================================
