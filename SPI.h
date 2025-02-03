#ifndef __SPI_H__
#define __SPI_H__

/* Dependencies */
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

class __SPI__
{
    public:
        /**
         * @brief Constructor: Initializes the SPI control, status, and data registers
         * @param spicr Pointer to the SPI control register
         * @param spisr Pointer to the SPI status register
         * @param spidr Pointer to the SPI data register
         */
        __SPI__(volatile uint8_t* spicr, volatile uint8_t* spisr, volatile uint8_t* spidr);
        
        /**
         * @brief Destructor: Resets the register pointers to NULL
         */
        ~__SPI__();
        
        /**
         * @brief Begins SPI communication by setting the appropriate bits in the control registers
         * @return 1 if successful, 0 otherwise
         */
        const uint8_t  begin(void);
        
        /**
         * @brief Transfers a single byte over SPI and returns the received byte
         * @param byte The byte to transfer
         * @return The received byte
         */
        const uint8_t  transfer(const uint8_t byte);
        
        /**
         * @brief Transfers a 16-bit word over SPI and returns the received word
         * @param word The word to transfer
         * @return The received word
         */
        const uint16_t transfer(const uint16_t word);
        
        /**
         * @brief Transfers a 32-bit double word over SPI and returns the received double word
         * @param dword The double word to transfer
         * @return The received double word
         */
        const uint32_t transfer(const uint32_t dword);
        
        /**
         * @brief Ends SPI communication by clearing the appropriate bits in the control registers
         * @return 1 if successful, 0 otherwise
         */
        const uint8_t  end(void);
        
        /**
         * @brief Sets the SPI clock divider to control the SPI clock speed
         * @param divider The clock divider value
         * @return 1 if successful, 0 otherwise
         */
        const uint8_t  setClockDivider(const uint8_t divider);

        /**
         * @brief Sets the SPI data mode (clock polarity and phase)
         * @param mode The data mode (0-3)
         * @return 1 if successful, 0 otherwise
         */
        const uint8_t  setDataMode(const uint8_t mode);
    
    private:
        // Pointers to the SPI control, status, and data registers
        volatile uint8_t* spicr; ///< Pointer to the SPI control register
        volatile uint8_t* spisr; ///< Pointer to the SPI status register
        volatile uint8_t* spidr; ///< Pointer to the SPI data register
        
        uint8_t began; ///< Flag to indicate whether SPI communication has begun
};

// Define the SPI object for specific AVR microcontrollers
#if defined(__AVR_ATmega328__)   ||\
    defined(__AVR_ATmega328P__)  ||\
    defined(__AVR_ATmega328PB__) ||
    defined(__AVR_ATmega2560__)
    extern __SPI__ SPI;
#endif

#endif
