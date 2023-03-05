#include "SPI.h"

/*********************************************
Function: __SPI__()
Purpose:  Constructor to __SPI__ class
Input:    SPI registers
Return:   None
*********************************************/
__SPI__::__SPI__(volatile uint8_t* spcr, volatile uint8_t* spsr, volatile uint8_t* spdr)
{
    this->spcr = spcr;
    this->spsr = spsr;
    this->spdr = spdr;
}

void __SPI__::begin(void)
{
    pinMode(&SS_DDR, SS_BIT, OUTPUT);     /* Configure <Slave Select> as OUTPUT */
    pinMode(&MOSI_DDR, MOSI_BIT, OUTPUT); /* Configure <Master-Out Slave-In> as OUTPUT */
    pinMode(&MISO_DDR, MISO_BIT, INPUT);  /* Configure <Master-In Slave-Out> as INPUT */
    pinMode(&SCLK_DDR, SCLK_BIT, OUTPUT); /* Configure <Serial Clock> as OUTPUT */

    #if defined(__AVR_ATmega328P__)
    *this->spcr |= (1 << SPE) | (1 << MSTR);
    *this->spsr |= (1 << SPI2X);
    #endif
}

void __SPI__::write(uint8_t data)
{
    *this->spdr = data;                  /* Load data into <SPDR> */
    while(!(*this->spsr & (1 << SPIF))); /* Wait until interrupt flag is set */
}

void __SPI__::write(uint16_t data)
{
    this->write((uint8_t)(data >> 8)); /* Write byte */
    this->write((uint8_t)data);        /* Write byte */
}

uint8_t __SPI__::read(void)
{
    while(!(*this->spsr & (1 << SPIF))); /* Wait until interrupt flag is set */
    return(*this->spdr);                 /* Return <SPDR> */
}

void __SPI__::end(void)
{
    pinMode(&SS_DDR, SS_BIT, INPUT);     /* Configure <Slave Select> as INPUT */
    pinMode(&MOSI_DDR, MOSI_BIT, INPUT); /* Configure <Master-Out Slave-In> as INPUT */
    pinMode(&MISO_DDR, MISO_BIT, INPUT); /* Configure <Master-In Slave-Out> as INPUT */
    pinMode(&SCLK_DDR, SCLK_BIT, INPUT); /* Configure <Serial Clock> as INPUT */

    #if defined(__AVR_ATmega328P__)
    *this->spcr = 0;
    *this->spsr = 0;
    #endif
}

#if defined(__AVR_ATmega328P__)
__SPI__ SPI = __SPI__(&SPCR, &SPSR, &SPDR);
#endif