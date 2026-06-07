/*
 * SKEE3223 - Problem 1: Digital Thermometer in Celsius
 *
 * Hardware:
 *   LM35DZ       -> PA0 (ADC0)
 *   Segments a-g -> PORTB (PB0-PB6)
 *   Digit select -> PORTD (PD0=hundreds, PD1=tens, PD2=units)
 *
 * Clock: 8 MHz
 */

#include <avr/io.h>
#include <util/delay.h>

/* Common cathode patterns for digits 0-9 (PB0-PB6 = segments a-g) */
static const uint8_t SEG_LUT[10] = {
    0b00111111, /* 0 */
    0b00000110, /* 1 */
    0b01011011, /* 2 */
    0b01001111, /* 3 */
    0b01100110, /* 4 */
    0b01101101, /* 5 */
    0b01111101, /* 6 */
    0b00000111, /* 7 */
    0b01111111, /* 8 */
    0b01101111  /* 9 */
};

#define DIGIT_OFF      0x07
#define DIGIT_HUNDREDS 0x06
#define DIGIT_TENS     0x05
#define DIGIT_UNITS    0x03

static uint8_t digit_h;
static uint8_t digit_t;
static uint8_t digit_u;
static uint8_t scan_pos;

static void gpio_init(void)
{
    DDRB  = 0xFF;
    DDRD |= 0x07;
    PORTD |= DIGIT_OFF;
}

static void adc_init(void)
{
    ADMUX  = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

static uint16_t adc_read(void)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;
    return ADC;
}

static uint16_t read_temperature(void)
{
    uint32_t sum = 0;

    for (uint8_t i = 0; i < 8; i++) {
        sum += adc_read();
        _delay_ms(2);
    }

    return (uint16_t)((sum / 8) * 500 / 1024);
}

static void update_digits(uint16_t temp)
{
    if (temp > 199)
        temp = 199;

    digit_h = temp / 100;
    digit_t = (temp / 10) % 10;
    digit_u = temp % 10;
}

static void show_digit(uint8_t pattern, uint8_t select)
{
    PORTD |= DIGIT_OFF;
    PORTB  = pattern;
    PORTD  = (PORTD & 0xF8) | select;
}

static void display_scan(void)
{
    PORTD |= DIGIT_OFF;

    switch (scan_pos) {
    case 0:
        if (digit_h)
            show_digit(SEG_LUT[digit_h], DIGIT_HUNDREDS);
        break;
    case 1:
        show_digit(SEG_LUT[digit_t], DIGIT_TENS);
        break;
    default:
        show_digit(SEG_LUT[digit_u], DIGIT_UNITS);
        break;
    }

    scan_pos++;
    if (scan_pos > 2)
        scan_pos = 0;
}

static void refresh_display(void)
{
    for (uint8_t i = 0; i < 50; i++) {
        display_scan();
        _delay_ms(1);
    }
}

int main(void)
{
    gpio_init();
    adc_init();

    while (1) {
        update_digits(read_temperature());
        refresh_display();
    }
}
