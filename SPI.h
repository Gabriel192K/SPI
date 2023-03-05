#ifndef __SPI_H__
#define __SPI_H__

/* Dependecies */
#include "Core/Core.h"

class __SPI__
{
    public:
        __SPI__(volatile uint8_t* spcr, volatile uint8_t* spsr, volatile uint8_t* spdr);
        ~__SPI__();
        void    begin(void);
        void    write(uint8_t data);
        void    write(uint16_t data);
        uint8_t read (void);
        void    end  (void);
    private:
        volatile uint8_t* spcr;
        volatile uint8_t* spsr;
        volatile uint8_t* spdr;
};

#if defined(__AVR_ATmega328P__)
#define SS_DDR DDRB   /* <Slave Select> Data Direction Register */
#define SS_DOR DORB   /* <Slave Select> Data Output Register */
#define SS_DIR DIRB   /* <Slave Select> Data Input Register */
#define SS_BIT 2      /* <Slave Select> Bit */
#define MOSI_DDR DDRB /* <Master-Out Slave-In> Data Direction Register */
#define MOSI_DOR DORB /* <Master-Out Slave-In> Data Output Register */
#define MOSI_DIR DIRB /* <Master-Out Slave-In> Data Input Register */
#define MOSI_BIT 3    /* <Master-Out Slave-In> Bit */
#define MISO_DDR DDRB /* <Master-In Slave-Out> Data Direction Register */
#define MISO_DOR DORB /* <Master-In Slave-Out> Data Output Register */
#define MISO_DIR DIRB /* <Master-In Slave-Out> Data Input Register */
#define MISO_BIT 4    /* <Master-In Slave-Out> Bit */
#define SCLK_DDR DDRB /* <Serial Clock> Data Direction Register */
#define SCLK_DOR DORB /* <Serial Clock> Data Output Register */
#define SCLK_DIR DIRB /* <Serial Clock> Data Input Register */
#define SCLK_BIT 5    /* <Serial Clock> Bit */
extern __SPI__ SPI;
#else
#error "No SPI bus implementation"
#endif



#endif
