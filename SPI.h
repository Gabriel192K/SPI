#ifndef __SPI_H__
#define __SPI_H__

/* Dependencies */
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

class __SPI__
{
    public:
        __SPI__(volatile uint8_t* spicr, volatile uint8_t* spisr, volatile uint8_t* spidr);
        ~__SPI__();
        const uint8_t  begin   (void);
        const uint8_t  transfer(const uint8_t byte);
        const uint16_t transfer(const uint16_t word);
        const uint32_t transfer(const uint32_t dword);
        const uint8_t  end     (void);
    private:
        volatile uint8_t* spicr;
        volatile uint8_t* spisr;
        volatile uint8_t* spidr;
        uint8_t began;
};

#if defined(__AVR_ATmega328__)   ||\
    defined(__AVR_ATmega328P__)  ||\
    defined(__AVR_ATmega328PB__) ||
    defined(__AVR_ATmega2560__)
    extern __SPI__ SPI;
#endif

#endif
