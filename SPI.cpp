/* Dependencies */
#include "SPI.h"

__SPI__::__SPI__(volatile uint8_t* spicr, volatile uint8_t* spisr, volatile uint8_t* spidr)
{
    this->spicr = spicr;
    this->spisr = spisr;
    this->spidr = spidr;
}

__SPI__::~__SPI__()
{
    this->spicr = NULL;
    this->spisr = NULL;
    this->spidr = NULL;
}

const uint8_t __SPI__::begin(void)
{
    if (this-> began)
        return (0);

    this->began = 1;

    *this->spicr = *this->spicr | ((1 << SPE) | (1 << MSTR));
    *this->spisr = *this->spisr | (1 << SPI2X);

    return (1);
}
const uint8_t __SPI__::transfer(const uint8_t byte)
{
    *this->spidr = byte;
    /*
     * The following NOP introduces a small delay that can prevent the wait
     * loop form iterating when running at the maximum speed. This gives
     * about 10% more speed, even if it seems counter-intuitive. At lower
     * speeds it is unnoticed.
     */
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    return (*this->spidr);
}

const uint16_t __SPI__::transfer(const uint16_t word)
{
    union
    {
        uint16_t val;
        struct
        {
            uint8_t lsb;
            uint8_t msb;
        };
    } in, out;

    in.val = word;

    *this->spidr = in.msb;
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    out.msb = *this->spidr;

    *this->spidr = in.lsb;
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    out.lsb = *this->spidr;

    return (out.val);
}

const uint32_t __SPI__::transfer(const uint32_t dword)
{
    union
    {
        uint32_t val;
        struct
        {
            uint8_t lsbh;
            uint8_t lsbl;
            uint8_t msbh;
            uint8_t msbl;
        };
    } in, out;

    in.val = dword;

    *this->spidr = in.msbl;
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    out.msbl = *this->spidr;

    *this->spidr = in.msbh;
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    out.msbh = *this->spidr;

    *this->spidr = in.lsbl;
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    out.lsbl = *this->spidr;

    *this->spidr = in.lsbh;
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    out.lsbh = *this->spidr;

    return (out.val);
}

const uint8_t __SPI__::end(void)
{
    if (!this->began)
        return (0);

    this->began = 0;

    *this->spicr = *this->spicr & ~((1 << SPE) | (1 << MSTR));
    *this->spisr = *this->spisr & ~(1 << SPI2X);

    return (1);
}