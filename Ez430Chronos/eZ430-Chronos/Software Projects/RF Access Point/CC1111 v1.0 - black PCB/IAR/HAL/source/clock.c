/******************************************************************************
    Filename: clock.c

    This file defines clock related functions for the CC1110/CC2510 family
    of RF system-on-chips from Texas Instruments.

******************************************************************************/

/******************************************************************************
 * INCLUDES
 */

#include "clock.h"



/*******************************************************************************
 * PUBLIC FUNCTIONS
 */


/******************************************************************************
* @fn  clockSetMainSrc
*
* @brief  Function for setting the main system clock source.
*         The function turns off the clock source that is not being used.
*         TICKSPD is set to the same frequency as the source.
*
* @param  uint8 source (one of CLOCK_SRC_HFRC or CLOCK_SRC_XOSC)
*
* @return void
*
******************************************************************************/
void clockSetMainSrc(uint8 source)
{
    register uint8 osc32k_bm = CLKCON & CLKCON_OSC32K_BM;

    // Source can have the following values:
    // CLOCK_SRC_XOSC   0x00  /*  High speed Crystal Oscillator (XOSC) */
    // CLOCK_SRC_HFRC   0x01  /*  Low power RC Oscillator (HFRC) */

    if (source == CLOCK_SRC_HFRC)
    {
        SLEEP &= ~SLEEP_OSC_PD_BM;       // power up both oscillators
        while (!IS_HFRC_STABLE());// wait until the oscillator is stable

        CLKCON = (osc32k_bm | CLKCON_OSC_BM | TICKSPD_DIV_2 | CLKSPD_DIV_2);
        while (CLKCON != (osc32k_bm | CLKCON_OSC_BM | TICKSPD_DIV_2 | CLKSPD_DIV_2));

        SLEEP |= SLEEP_OSC_PD_BM;        // power down the unused oscillator
    }
    else if (source == CLOCK_SRC_XOSC)
    {
        SLEEP &= ~SLEEP_OSC_PD_BM;       // power up both oscillators
        while (!IS_XOSC_STABLE());// wait until the XOSC is stable

        CLKCON = (osc32k_bm | TICKSPD_DIV_1 | CLKSPD_DIV_1);
        while (CLKCON != (osc32k_bm | TICKSPD_DIV_1 | CLKSPD_DIV_1));

        SLEEP |= SLEEP_OSC_PD_BM;        // power down the unused oscillator
    }
}
