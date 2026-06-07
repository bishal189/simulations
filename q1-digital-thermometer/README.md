# Q1 — Digital Thermometer in Celsius

**Course:** SKEE3223 Microprocessor | **Sensor:** LM35DZ | **MCU:** ATmega32 | **Display:** 3× 7-segment

---

## What You Must Deliver (from assignment)

| Item | What to submit |
|------|----------------|
| Interface circuit | Full schematic in **Proteus** |
| C program | Written in IDE, compiled to **.hex** |
| Simulation | Proteus run with sample outputs (screenshots) |
| Report | Abstract, Introduction, Methodology, Results, Conclusion |
| Software design | Flowchart or pseudocode |
| Code listing | Full C code with comments |

> **Note:** The assignment header says *ARM Cortex-M4 / STM32CubeIDE*, but **Problem 1 explicitly says ATmega32**. Proteus has excellent ATmega32 support — use ATmega32 unless your lecturer says otherwise.

---

## How the System Works

```
LM35DZ ──(analog 10 mV/°C)──► ATmega32 ADC (PA0)
                                      │
                              Convert to °C
                                      │
                         Split into 3 digits (H T U)
                                      │
                    Multiplex scan ──► 3× 7-segment display
```

**LM35DZ:** Output voltage = **10 mV × temperature (°C)**  
At 25 °C → 250 mV | At 100 °C → 1000 mV (1 V)

**Formula (5 V reference, 10-bit ADC):**

```
Temperature (°C) = ADC_reading × 500 / 1024
```

---

## Proteus Circuit (step-by-step)

### Components to place

1. **ATmega32** (or ATMEGA32L)
2. **LM35DZ** (search: `LM35`)
3. **3× 7SEG-COM-CATHODE** (or COM-ANODE — match your code)
4. **CRYSTAL** 8 MHz + 2× 22 pF capacitors
5. **RES** 10 kΩ (reset pull-up)
6. **CAP** 100 nF (VCC decoupling)
7. **POT-HG** (optional — to test ADC without LM35)

### Power & clock

- VCC = 5 V, GND common
- Crystal on XTAL1/XTAL2 with 22 pF caps to GND
- RESET pin via 10 kΩ to VCC

### LM35 wiring

| LM35 pin | Connect to |
|----------|------------|
| V+       | +5 V       |
| Vout     | **PA0 / ADC0** (pin 40) |
| GND      | GND        |

### 7-segment wiring (common cathode)

**Segment bus (shared by all 3 digits)** — connect to **PORTB**:

| Segment | PORTB pin | ATmega32 pin |
|---------|-----------|--------------|
| a       | PB0       | 1            |
| b       | PB1       | 2            |
| c       | PB2       | 3            |
| d       | PB3       | 4            |
| e       | PB4       | 5            |
| f       | PB5       | 6            |
| g       | PB6       | 7            |

**Digit select (common cathode → GND when ON)** — **PORTD**:

| Digit        | PORTD pin | ATmega32 pin | Shows |
|--------------|-----------|--------------|-------|
| Hundreds     | PD0       | 14           | 0–1   |
| Tens         | PD1       | 15           | 0–9   |
| Units        | PD2       | 16           | 0–9   |

- Common cathode of each display → corresponding PD pin (through 220 Ω if needed)
- When PDx = **LOW**, that digit is **ON**

### Load the hex file in Proteus

1. Double-click ATmega32 → **Program File** → browse to `digital_thermometer.hex`
2. Set **Clock Frequency** = `8000000` (8 MHz)
3. Run simulation ▶

### Test temperature in Proteus

- Double-click **LM35** → set **Temperature** property (e.g. 25, 37, 99)
- Or use **POT** on ADC pin and adjust voltage: **V = °C × 0.01**

---

## Build the firmware

### Option A — AVR-GCC (recommended for ATmega32)

```bash
cd firmware
make
# Output: digital_thermometer.hex
```

Requires: `avr-gcc`, `avr-libc`, `avr-objcopy`

```bash
sudo apt install gcc-avr avr-libc binutils-avr
```

### Option B — Atmel Studio / Microchip Studio

Create ATmega32 project → add `main.c` → Build → use generated `.hex` in Proteus.

---

## Report flowchart (paste into report)

See `docs/flowchart.md`

---

## Simulation screenshots to capture

1. Circuit at 25 °C → display shows **025** or **25** (leading zero on hundreds)
2. Circuit at 37 °C → **037**
3. Circuit at 99 °C → **099**
4. Close-up of ADC / LM35 properties panel
