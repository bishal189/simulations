/*
 * SKEE3223 — Problem 1: Digital Thermometer in Celsius
 * MCU: ATmega32 @ 8 MHz
 * Sensor: LM35DZ on ADC0 (PA0) — 10 mV/°C
 * Display: 3× common-cathode 7-segment, multiplexed
 *
 * PORTB = segment pattern (a..g on PB0..PB6)
 * PORTD = digit select (PD0=hundreds, PD1=tens, PD2=units, active LOW)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* ── 7-segment patterns for digits 0-9 (common cathode, bits a-g) ── */
const uint8_t SEGMENT_LUT[10] = {
    0b00111111,  /* 0 */
    0b00000110,  /* 1 */
    0b01011011,  /* 2 */
    0b01001111,  /* 3 */
    0b01100110,  /* 4 */
    0b01101101,  /* 5 */
    0b01111101,  /* 6 */
    0b00000111,  /* 7 */
    0b01111111,  /* 8 */
    0b01101111   /* 9 */
};

/* Digit enable masks on PORTD (active LOW) */
#define DIGIT_HUNDREDS  0b11111110
#define DIGIT_TENS      0b11111101
#define DIGIT_UNITS     0b11111100
#define DIGIT_OFF       0b11111111

static volatile uint8_t digit_h = 0;
static volatile uint8_t digit_t = 0;
static volatile uint8_t digit_u = 0;
static volatile uint8_t scan_index = 0;

/* ── ADC init: single conversion, prescaler 64 → 125 kHz @ 8 MHz ── */
static void adc_init(void)
{
    ADMUX  = (1 << REFS0);              /* AVCC reference, ADC0 */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); /* prescaler 64 */
}

/* Read 10-bit ADC result (0–1023) */
static uint16_t adc_read(void)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;
    return ADC;
}

/*
 * LM35: 10 mV/°C, Vref = 5 V
 * Temp = (ADC × 5.0 / 1024) / 0.01 = ADC × 500 / 1024
 */
static uint16_t read_temperature_c(void)
{
    uint32_t sum = 0;
    uint8_t i;

    /* Average 8 samples for stable reading */
    for (i = 0; i < 8; i++) {
        sum += adc_read();
        _delay_ms(2);
    }

    return (uint16_t)((sum / 8UL) * 500UL / 1024UL);
}

static void split_digits(uint16_t value, uint8_t *h, uint8_t *t, uint8_t *u)
{
  if (value > 199) value = 199;  /* clamp to 3-digit range */

    *h = (uint8_t)(value / 100);
    *t = (uint8_t)((value / 10) % 10);
    *u = (uint8_t)(value % 10);
}

/* Display one digit — call rapidly in loop for multiplex persistence */
static void display_scan(void)
{
    uint8_t pattern;
    uint8_t enable;

    PORTD |= DIGIT_OFF;  /* blank all digits first (reduce ghosting) */

    switch (scan_index) {
    case 0:
        pattern = (digit_h == 0) ? 0 : SEGMENT_LUT[digit_h];
        enable  = DIGIT_HUNDREDS;
        break;
    case 1:
        pattern = SEGMENT_LUT[digit_t];
        enable  = DIGIT_TENS;
        break;
    default:
        pattern = SEGMENT_LUT[digit_u];
        enable  = DIGIT_UNITS;
        break;
    }

    PORTB = pattern;
    PORTD = (PORTD & 0xF8) | (enable & 0x07);

    scan_index++;
    if (scan_index > 2)
        scan_index = 0;
}

int main(void)
{
    uint16_t temp_c;

    DDRB  = 0xFF;   /* segments output */
    DDRD  = 0x07;   /* PD0..PD2 digit select */
    PORTD = DIGIT_OFF;

    adc_init();

    while (1) {
        uint8_t h, t, u;

        temp_c = read_temperature_c();
        split_digits(temp_c, &h, &t, &u);
        digit_h = h;
        digit_t = t;
        digit_u = u;

        /* Multiplex ~3 ms per full cycle (1 ms × 3 digits) */
        for (uint8_t i = 0; i < 50; i++) {
            display_scan();
            _delay_ms(1);
        }
    }

    return 0;
}
