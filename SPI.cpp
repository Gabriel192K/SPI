/* Dependencies */
#include "SPI.h"

/**
 * @brief Constructor: Initializes the SPI control, status, and data registers
 * @param spicr Pointer to the SPI control register
 * @param spisr Pointer to the SPI status register
 * @param spidr Pointer to the SPI data register
 */
__SPI__::__SPI__(volatile uint8_t* spicr, volatile uint8_t* spisr, volatile uint8_t* spidr)
{
    this->spicr = spicr;
    this->spisr = spisr;
    this->spidr = spidr;
}

/**
 * @brief Destructor: Resets the register pointers to NULL
 */
__SPI__::~__SPI__()
{
    this->spicr = NULL;
    this->spisr = NULL;
    this->spidr = NULL;
}

/**
 * @brief Begins SPI communication by setting the appropriate bits in the control registers
 * @return 1 if successful, 0 otherwise
 */
const uint8_t __SPI__::begin(void)
{
    if (this->began)
        return (0);

    this->began = 1;

    *this->spicr = *this->spicr | ((1 << SPE) | (1 << MSTR));
    *this->spisr = *this->spisr | (1 << SPI2X);

    return (1);
}

/**
 * @brief Transfers a single byte over SPI and returns the received byte
 * @param byte The byte to transfer
 * @return The received byte
 */
const uint8_t __SPI__::transfer(const uint8_t byte)
{
    *this->spidr = byte;
    // Introduce a small delay to prevent the wait loop from iterating at maximum speed
    asm volatile("nop");
    while (!(*this->spisr & (1 << SPIF)));
    return (*this->spidr);
}

/**
 * @brief Transfers a 16-bit word over SPI and returns the received word
 * @param word The word to transfer
 * @return The received word
 */
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

/**
 * @brief Transfers a 32-bit double word over SPI and returns the received double word
 * @param dword The double word to transfer
 * @return The received double word
 */
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

/**
 * @brief Ends SPI communication by clearing the appropriate bits in the control registers
 * @return 1 if successful, 0 otherwise
 */
const uint8_t __SPI__::end(void)
{
    if (!this->began)
        return (0);

    this->began = 0;

    *this->spicr = *this->spicr & ~((1 << SPE) | (1 << MSTR));
    *this->spisr = *this->spisr & ~(1 << SPI2X);

    return (1);
}

/**
 * @brief Sets the SPI clock divider to control the SPI clock speed
 * @param divider The clock divider value
 * @return 1 if successful, 0 otherwise
 */
const uint8_t __SPI__::setClockDivider(const uint8_t divider)
{
    if (!this->began)
        return (0);

    *this->spicr = (*this->spicr & ~0x03) | (divider & 0x03);
    return (1);
}

/**
 * @brief Sets the SPI data mode (clock polarity and phase)
 * @param mode The data mode (0-3)
 * @return 1 if successful, 0 otherwise
 */
const uint8_t __SPI__::setDataMode(const uint8_t mode)
{
    if (!this->began)
        return (0);

    *this->spicr = (*this->spicr & ~0x0C) | ((mode & 0x03) << 2);
    return (1);
}