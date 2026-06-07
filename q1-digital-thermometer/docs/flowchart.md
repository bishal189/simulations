# Software Flowchart

## Pseudocode

```
START
    Set PORTB as output (segments)
    Set PORTD bits 0-2 as output (digit select)
    Initialize ADC on channel 0
    Discard first ADC reading

    LOOP forever
        Read ADC 8 times and sum
        Temperature = (sum × 500 + 4096) / 8192

        hundreds = temperature / 100
        tens     = (temperature / 10) % 10
        units    = temperature % 10

        Repeat 60 times
            If hundreds > 0, show hundreds digit
            If hundreds or tens > 0, show tens digit
            Show units digit
            Delay 1 ms per digit
        End repeat
    END LOOP
END
```

## Flowchart

```mermaid
flowchart TD
    A([Start]) --> B[Init PORTB and PORTD]
    B --> C[Init ADC channel 0]
    C --> D[Read and average ADC 8 times]
    D --> E["Temp = (sum × 500 + 4096) / 8192"]
    E --> F[Split into hundreds, tens, units]
    F --> G[Multiplex scan 3 digits]
    G --> H{60 cycles done?}
    H -->|No| G
    H -->|Yes| D
```
