# Q1 — Digital Thermometer in Celsius

**MCU:** ATmega32 | **Sensor:** LM35DZ | **Display:** 3× 7-segment (common cathode)

---

## PART A — ADD ALL COMPONENTS FIRST (no wiring yet)

Add everything below before connecting any wire.

---

### Step 1 — Add main components

Click **P** (Pick Device), search, place on canvas:

| Place on canvas | Search in Proteus | Qty | Label |
|-----------------|-------------------|-----|-------|
| Microcontroller | `ATMEGA32` | 1 | U1 |
| Temperature sensor | `LM35` | 1 | U2 |
| 7-segment display | `7SEG-COM-CATHODE-GRN` | 1 | DISP1 (Hundreds) |
| 7-segment display | `7SEG-COM-CATHODE-GRN` | 1 | DISP2 (Tens) |
| 7-segment display | `7SEG-COM-CATHODE-GRN` | 1 | DISP3 (Units) |
| Power supply | `POWER` | 1 | PWR |

Arrange like this:

```
[PWR]     [U1 ATmega32]     [DISP1] [DISP2] [DISP3]
          [U2 LM35]
```

Right-click each display → rotate so digits face the same way.  
Place DISP1 (left) = Hundreds, DISP2 (middle) = Tens, DISP3 (right) = Units.

---

### Step 2 — Add clock components

Place near U1 ATmega32:

| Search in Proteus | Qty | Label | Set value |
|-------------------|-----|-------|-----------|
| `CRYSTAL` | 1 | Y1 | **8MHz** |
| `CAP` | 1 | C1 | **22p** |
| `CAP` | 1 | C2 | **22p** |

```
      [C1]          [C2]
       |             |
  [U1]---[Y1 Crystal]---
```

---

### Step 3 — Add resistors and capacitors

| Search in Proteus | Qty | Label | Set value |
|-------------------|-----|-------|-----------|
| `RES` | 1 | R1 | **10k** |
| `RES` | 7 | R2, R3, R4, R5, R6, R7, R8 | **220** |
| `CAP` | 1 | C3 | **100n** |
| `CAP` | 1 | C4 | **100n** |

Place R1 near U1 RESET pin.  
Place R2–R8 between U1 PORTB and the displays.  
Place C3 near U1 VCC. Place C4 near U1 AREF.

---

### Step 4 — Set component values (double-click each part)

| Component | Property | Value |
|-----------|----------|-------|
| Y1 | Frequency | `8MHz` |
| C1, C2 | Capacitance | `22p` |
| C3, C4 | Capacitance | `100n` |
| R1 | Resistance | `10k` |
| R2–R8 | Resistance | `220` |

**Stop here. Make sure all parts are on the canvas before wiring.**

---

## PART B — WIRING (after all components are placed)

Connect wires in this order. Use **green wire** for signals, **red** for +5V, **blue** for GND.

---

### Step 5 — Power wires (+5V and GND)

**+5V connections:**

```
PWR +5V  →  U1 pin 10  (VCC)
PWR +5V  →  U1 pin 30  (AVCC)     ← needed for ADC
PWR +5V  →  U2 pin 1   (LM35 V+)
PWR +5V  →  R1 pin 1
PWR +5V  →  C3 pin 1
```

**GND connections:**

```
PWR GND  →  U1 pin 11  (GND)
PWR GND  →  U1 pin 31  (GND)
PWR GND  →  U2 pin 3   (LM35 GND)
PWR GND  →  C1 pin 2
PWR GND  →  C2 pin 2
PWR GND  →  C3 pin 2
PWR GND  →  C4 pin 2
```

---

### Step 6 — Reset wire

```
R1 pin 2  →  U1 pin 9  (RESET)
```

(+5V already connected to R1 pin 1 in Step 5)

---

### Step 7 — AREF capacitor

```
U1 pin 32 (AREF)  →  C4 pin 1
(C4 pin 2 already connected to GND in Step 5)
```

---

### Step 8 — Clock wires

```
U1 pin 13 (XTAL1)  →  Y1 pin 1
U1 pin 12 (XTAL2)  →  Y1 pin 2
Y1 pin 1           →  C1 pin 1
Y1 pin 2           →  C2 pin 1
(C1 and C2 other pins already go to GND)
```

```
         GND
          |
         [C1]
          |
pin13 ----+----[ Y1 ]----+---- pin12
          |              |
         GND            [C2]
                          |
                         GND
```

---

### Step 9 — LM35 sensor wire

```
U2 pin 2 (Vout)  →  U1 pin 40 (PA0)
```

(V+ and GND already done in Step 5)

```
+5V → U2 pin1 (V+)
       [LM35]
PA0 ← U2 pin2 (Vout)
GND → U2 pin3 (GND)
```

---

### Step 10 — Display segment wires

Join the **same letter** on all 3 displays, then connect through a 220Ω resistor to PORTB.

| Join these 3 pins together | Through resistor | To ATmega32 |
|----------------------------|------------------|-------------|
| DISP1.a + DISP2.a + DISP3.a | R2 | U1 pin 1 (PB0) |
| DISP1.b + DISP2.b + DISP3.b | R3 | U1 pin 2 (PB1) |
| DISP1.c + DISP2.c + DISP3.c | R4 | U1 pin 3 (PB2) |
| DISP1.d + DISP2.d + DISP3.d | R5 | U1 pin 4 (PB3) |
| DISP1.e + DISP2.e + DISP3.e | R6 | U1 pin 5 (PB4) |
| DISP1.f + DISP2.f + DISP3.f | R7 | U1 pin 6 (PB5) |
| DISP1.g + DISP2.g + DISP3.g | R8 | U1 pin 7 (PB6) |

**How to wire in Proteus:**
1. Click `a` on DISP1 → click `a` on DISP2 → click `a` on DISP3 (they share one wire)
2. Connect that wire → R2 → U1 pin 1
3. Repeat for b, c, d, e, f, g

**Leave `dp` (decimal point) unconnected on all displays.**

---

### Step 11 — Display digit select wires (COM pin)

| From | To | Shows |
|------|----|-------|
| DISP1 COM | U1 pin 14 (PD0) | Hundreds |
| DISP2 COM | U1 pin 15 (PD1) | Tens |
| DISP3 COM | U1 pin 16 (PD2) | Units |

**Do NOT connect COM to GND.** COM goes to PD0, PD1, PD2 only.

---

### Step 12 — Wiring checklist

Tick when done:

```
POWER
 [ ] +5V → U1 VCC (pin 10)
 [ ] +5V → U1 AVCC (pin 30)
 [ ] +5V → LM35 V+ (pin 1)
 [ ] +5V → R1, C3
 [ ] GND → U1 GND (pins 11, 31)
 [ ] GND → LM35 GND (pin 3)
 [ ] GND → C1, C2, C3, C4

RESET & AREF
 [ ] R1 → U1 RESET (pin 9)
 [ ] U1 AREF (pin 32) → C4 → GND

CLOCK
 [ ] U1 XTAL1 (pin 13) → Crystal → XTAL2 (pin 12)
 [ ] Crystal → C1, C2 → GND

LM35
 [ ] LM35 Vout (pin 2) → U1 PA0 (pin 40)

DISPLAY SEGMENTS
 [ ] a bus → R2 → PB0 (pin 1)
 [ ] b bus → R3 → PB1 (pin 2)
 [ ] c bus → R4 → PB2 (pin 3)
 [ ] d bus → R5 → PB3 (pin 4)
 [ ] e bus → R6 → PB4 (pin 5)
 [ ] f bus → R7 → PB5 (pin 6)
 [ ] g bus → R8 → PB6 (pin 7)

DISPLAY DIGIT SELECT
 [ ] DISP1 COM → PD0 (pin 14)
 [ ] DISP2 COM → PD1 (pin 15)
 [ ] DISP3 COM → PD2 (pin 16)
```

---

## PART C — LOAD CODE AND RUN

### Step 13 — Compile hex file

```bash
sudo apt install gcc-avr avr-libc binutils-avr
cd firmware
make
```

Output: `firmware/digital_thermometer.hex`

### Step 14 — Load hex into Proteus

1. Double-click **U1 ATmega32**
2. **Program File** → browse to `digital_thermometer.hex`
3. **Clock Frequency** → `8000000`
4. Click OK

### Step 15 — Run simulation

1. Press **Play** ▶
2. Double-click **LM35 (U2)** → set **Temperature**
3. Check the 3 displays

| Set LM35 to | Display shows |
|-------------|---------------|
| 25 °C | 25 |
| 37 °C | 37 |
| 99 °C | 99 |

---

## ATmega32 pin map (quick reference)

```
         PB0-pin1  ....  PA0-pin40 ← LM35
         PB1-pin2
         ...
         PB6-pin7  ....  segments a-g
         RESET-pin9
         VCC-pin10, GND-pin11
         XTAL2-pin12, XTAL1-pin13
         PD0-pin14 ← DISP1 COM (Hundreds)
         PD1-pin15 ← DISP2 COM (Tens)
         PD2-pin16 ← DISP3 COM (Units)
         AVCC-pin30 ← +5V
         AREF-pin32 ← C4 → GND
```

---

## Troubleshooting

| Problem | Fix |
|---------|-----|
| Display blank | Load `.hex`, set clock to `8000000` |
| Display blank | Use `7SEG-COM-CATHODE-GRN` (not anode) |
| Wrong digits | Wire a→PB0, b→PB1, c→PB2, d→PB3, e→PB4, f→PB5, g→PB6 |
| COM to GND | COM must go to PD0, PD1, PD2 only |
| Temperature always 0 | Connect pin 30 (AVCC) to +5V |
| Wrong temperature | Double-click LM35 → change Temperature |

---

## Temperature formula

```
LM35: 10 mV per °C
Temperature (°C) = ADC_value × 500 / 1024
```

---

## Report checklist

- [ ] Proteus schematic screenshot
- [ ] Simulation at 25 °C, 37 °C, 99 °C
- [ ] Flowchart (`docs/flowchart.md`)
- [ ] C code (`firmware/main.c`)
- [ ] Abstract, Introduction, Methodology, Results, Conclusion
