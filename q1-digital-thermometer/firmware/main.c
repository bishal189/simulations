#define F_CPU 1000000UL 
#include <avr/io.h>
#include <util/delay.h>

// 7-Segment Lookup Table for Common Anode Displays (Digits 0-9)
unsigned char seg_code[] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};

void ADC_init() {
    ADMUX = (1 << REFS1) | (1 << REFS0); // Internal 2.56V Reference
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Prescaler 64
}

unsigned int ADC_read(unsigned char channel) {
    ADMUX = (ADMUX & 0xE0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADCW;
}

int main(void) {
    DDRB = 0xFF;  // PORTB as output for segments
    DDRC = 0x07;  // PC0, PC1, PC2 as output for digits
    
    ADC_init();   
    
    unsigned int raw_adc = 0;
    unsigned int celsius_temp = 0;
    unsigned char hundreds = 0;
    unsigned char tens = 0;
    unsigned char units = 0;
    
    while (1) {
        raw_adc = ADC_read(0);
        
        // Direct Celsius Calculation
        celsius_temp = (raw_adc * 256) / 1023;
        
        // Extract digits from the Celsius value
        hundreds = celsius_temp / 100;          
        tens     = (celsius_temp % 100) / 10;   
        units    = celsius_temp % 10;           
        
        // Multiplexing Display Loop
        for (int refresh = 0; refresh < 15; refresh++) {
            // Digit 1
            PORTB = 0xFF;               
            PORTC = 0x01;               
            PORTB = seg_code[hundreds]; 
            _delay_ms(4);               
            
            // Digit 2
            PORTB = 0xFF;               
            PORTC = 0x02;               
            PORTB = seg_code[tens];     
            _delay_ms(4);               
            
            // Digit 3
            PORTB = 0xFF;               
            PORTC = 0x04;               
            PORTB = seg_code[units];    
            _delay_ms(4);               
        }
    }
    return 0;
}