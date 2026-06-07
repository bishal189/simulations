#define F_CPU 1000000UL // Define internal clock speed as 1 MHz for delay utilities
#include <avr/io.h>
#include <util/delay.h>

// 7-Segment Lookup Table for Common Anode Displays (Digits 0-9)
// In Common Anode, a '0' bit turns the segment ON, and a '1' bit turns it OFF.
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

// Initialize the Analog-to-Digital Converter peripheral
void ADC_init() {
    // Select internal 2.56V reference voltage source (REFS1=1, REFS0=1)
    // Pin PA0 is chosen by default as input channel (MUX4:0 = 00000)
    ADMUX = (1 << REFS1) | (1 << REFS0);
    
    // Enable the ADC module (ADEN=1)
    // Set division factor to 64 for clock prescaling (ADPS2=1, ADPS1=1)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

// Read the raw 10-bit digital value from a specific ADC channel
unsigned int ADC_read(unsigned char channel) {
    // Clear the existing channel selection bits safely, then set the new target channel
    ADMUX = (ADMUX & 0xE0) | (channel & 0x07);
    
    // Start the conversion process (ADSC=1)
    ADCSRA |= (1 << ADSC);
    
    // Hardware polling: Wait loop until the conversion finishes (ADSC returns to 0)
    while (ADCSRA & (1 << ADSC));
    
    // Return the combined 10-bit outcome register (ADCL + ADCH)
    return ADCW;
}

int main(void) {
    // Port System Setup
    DDRB = 0xFF;  // Configure all PORTB pins (PB0-PB6) as outputs for segment data lines
    DDRC = 0x07;  // Configure PC0, PC1, PC2 as outputs for digit control multiplex pins
    
    ADC_init();   // Turn on and configure ADC configurations
    
    unsigned int raw_adc = 0;
    unsigned int celsius_temp = 0;
    unsigned int fahrenheit_temp = 0; // Variable to hold converted Fahrenheit temperature
    unsigned char hundreds = 0;
    unsigned char tens = 0;
    unsigned char units = 0;
    
    while (1) {
        // 1. Fetch raw voltage reading from LM35 connected to channel 0 (PA0)
        raw_adc = ADC_read(0);
        
        // 2. Step 1: Linear Scaling to get Celsius base
        // Using Internal 2.56V reference: (raw_adc * 2.56V) / (1023 steps * 10mV/C)
        celsius_temp = (raw_adc * 256) / 1023;
        
        // 3. Step 2: Convert Celsius to Fahrenheit using high-speed integer math
        // Algebraic Formula: F = (C * 9 / 5) + 32
        fahrenheit_temp = ((celsius_temp * 9) / 5) + 32;
        
        // 4. Mathematical Digit Extraction from the Fahrenheit Value
        hundreds = fahrenheit_temp / 100;          // Isolate hundreds column
        tens     = (fahrenheit_temp % 100) / 10;   // Isolate tens column
        units    = fahrenheit_temp % 10;           // Isolate units column
        
        // 5. Time-Multiplexing Driving Loop
        // Refreshes the display columns rapidly to trick human persistence of vision (POV)
        for (int refresh = 0; refresh < 15; refresh++) {
            
            // --- Display Column 1: Hundreds Digit ---
            PORTB = 0xFF;               // Ghosting Prevention: Instantly clear segment signals
            PORTC = 0x01;               // Ground pin 1 line (PC0) to focus power on Digit 1
            PORTB = seg_code[hundreds]; // Output digit hex mapping 
            _delay_ms(4);               // Keep active briefly
            
            // --- Display Column 2: Tens Digit ---
            PORTB = 0xFF;               // Ghosting Prevention: Instantly clear segment signals
            PORTC = 0x02;               // Ground pin 2 line (PC1) to focus power on Digit 2
            PORTB = seg_code[tens];     // Output digit hex mapping
            _delay_ms(4);               // Keep active briefly
            
            // --- Display Column 3: Units Digit ---
            PORTB = 0xFF;               // Ghosting Prevention: Instantly clear segment signals
            PORTC = 0x04;               // Ground pin 3 line (PC2) to focus power on Digit 3
            PORTB = seg_code[units];    // Output digit hex mapping
            _delay_ms(4);               // Keep active briefly
        }
    }
    return 0;
}