#include <RH_RF95.h>

class Radio
{
    public:
        Radio(uint8_t rst, uint8_t cst, uint8_t intr, double freq);
        uint8_t receive();
        uint8_t send(void *buffer, uint8_t size);
        uint8_t *receive_buffer;
        void reset(unsigned int duration);
    private:
        RH_RF95 *rf95;
        uint8_t r_len;
        uint8_t reset_pin;
};

