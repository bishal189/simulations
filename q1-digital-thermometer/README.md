# Q1 — Digital Thermometer in Celsius

**MCU:** ATmega32 | **Sensor:** LM35DZ | **Display:** 3× 7-segment (common cathode)

Follow this guide **wire by wire** in Proteus. Every connection is numbered.

---

## Step 1 — Place Components on Proteus Canvas

Place components in this layout (left to right):

```
+------------------------------------------------------------------+
|                                                                  |
|   [POWER]          [ATmega32]         [7-Seg] [7-Seg] [7-Seg]   |
|   TERMINAL         (centre)           (Hundreds)(Tens) (Units) |
|                                                                  |
|   [LM35]           [CRYSTAL]                                     |
|                    [C1][C2]                                      |
|                    [R1 10k]                                      |
|                                                                  |
+------------------------------------------------------------------+
```

### Component list — search and place these in Proteus

| Label | Proteus search name | Quantity |
|-------|---------------------|----------|
| U1 | `ATMEGA32` | 1 |
| U2 | `LM35` | 1 |
| DISP1 | `7SEG-COM-CATHODE-GRN` | 1 (Hundreds) |
| DISP2 | `7SEG-COM-CATHODE-GRN` | 1 (Tens) |
| DISP3 | `7SEG-COM-CATHODE-GRN` | 1 (Units) |
| Y1 | `CRYSTAL` (8 MHz) | 1 |
| C1, C2 | `CAP` (22 pF) | 2 |
| C3 | `CAP` (100 nF) | 1 |
| R1 | `RES` (10 kΩ) | 1 |
| PWR | `POWER` or `TERMINAL` | 1 |

Set **POWER** terminals: `+5V` and `GND`

---

## Step 2 — ATmega32 Pin Map (reference)

```
                    ATmega32 (U1)
              ┌─────────────────────┐
         PB0  │ 1               40 │  PA0  (ADC0)  ← LM35
         PB1  │ 2               39 │  PA1
         PB2  │ 3               38 │  PA2
         PB3  │ 4               37 │  PA3
         PB4  │ 5               36 │  PA4
         PB5  │ 6               35 │  PA5
         PB6  │ 7               34 │  PA6
         PB7  │ 8               33 │  PA7
    RESET  │ 9               32 │  AREF
         VCC  │10               31 │  GND
         GND  │11               30 │  AVCC
       XTAL2  │12               29 │  PC7
       XTAL1  │13               28 │  PC6
         PD0  │14               27 │  PC5
         PD1  │15               26 │  PC4
         PD2  │16               25 │  PC3
         PD3  │17               24 │  PC2
         PD4  │18               23 │  PC1
         PD5  │19               22 │  PC0
         PD6  │20               21 │  PD7
              └─────────────────────┘
```

---

## Step 3 — Connect Every Wire (follow in order)

### POWER wires

| Wire | From | To |
|------|------|----|
| W1 | POWER `+5V` | U1 pin 10 (VCC) |
| W2 | POWER `+5V` | U1 pin 30 (AVCC) |
| W3 | POWER `+5V` | U2 pin 1 (V+) |
| W4 | POWER `+5V` | R1 pin 1 |
| W5 | POWER `GND` | U1 pin 11 (GND) |
| W6 | POWER `GND` | U1 pin 31 (GND) |
| W7 | POWER `GND` | U2 pin 3 (GND) |
| W8 | POWER `GND` | C1 pin 2 |
| W9 | POWER `GND` | C2 pin 2 |
| W10 | POWER `GND` | C3 pin 2 |

### RESET wire

| Wire | From | To |
|------|------|----|
| W11 | R1 pin 2 | U1 pin 9 (RESET) |

### Decoupling capacitor

| Wire | From | To |
|------|------|----|
| W12 | POWER `+5V` | C3 pin 1 |
| W13 | C3 pin 2 | POWER `GND` |

Place C3 close to U1 VCC pin.

### Clock circuit

| Wire | From | To |
|------|------|----|
| W14 | U1 pin 13 (XTAL1) | Y1 pin 1 |
| W15 | U1 pin 12 (XTAL2) | Y1 pin 2 |
| W16 | Y1 pin 1 | C1 pin 1 |
| W17 | C1 pin 2 | POWER `GND` |
| W18 | Y1 pin 2 | C2 pin 1 |
| W19 | C2 pin 2 | POWER `GND` |

```
         GND
          |
         [C1 22pF]
          |
U1 pin13 -+---[ Y1 8MHz ]---+- U1 pin12
          |                  |
         [C2 22pF]          |
          |                  |
         GND                GND
```

### LM35 sensor

| Wire | From | To |
|------|------|----|
| W20 | U2 pin 2 (Vout) | U1 pin 40 (PA0) |

(W1, W3, W7 already connect LM35 power and ground)

```
    +5V ---- W3 ---- U2 pin1 (V+)
                      |
                   [LM35]
                      |
    PA0 ---- W20 ---- U2 pin2 (Vout)
                      |
    GND ---- W7 ---- U2 pin3 (GND)
```

---

## Step 4 — Seven-Segment Display Pin Layout

Each display (`7SEG-COM-CATHODE-GRN`) has these pins:

```
         ┌───┐
      f  │   │  b
         │ a │
      e  │ g │  c
         │ d │
         └───┘
            COM (common cathode)
```

| Display pin | Name |
|-------------|------|
| a | Top segment |
| b | Top-right segment |
| c | Bottom-right segment |
| d | Bottom segment |
| e | Bottom-left segment |
| f | Top-left segment |
| g | Middle segment |
| COM | Common cathode (digit enable) |

**Do not connect `dp` (decimal point) — leave it unconnected.**

---

## Step 5 — Segment Bus (shared by all 3 displays)

Connect the **same segment letter** from all 3 displays together, then to one PORTB pin.

```
DISP1.a ──┐
DISP2.a ──┼── W21 ── U1 pin 1  (PB0)
DISP3.a ──┘

DISP1.b ──┐
DISP2.b ──┼── W22 ── U1 pin 2  (PB1)
DISP3.b ──┘

DISP1.c ──┐
DISP2.c ──┼── W23 ── U1 pin 3  (PB2)
DISP3.c ──┘

DISP1.d ──┐
DISP2.d ──┼── W24 ── U1 pin 4  (PB3)
DISP3.d ──┘

DISP1.e ──┐
DISP2.e ──┼── W25 ── U1 pin 5  (PB4)
DISP3.e ──┘

DISP1.f ──┐
DISP2.f ──┼── W26 ── U1 pin 6  (PB5)
DISP3.f ──┘

DISP1.g ──┐
DISP2.g ──┼── W27 ── U1 pin 7  (PB6)
DISP3.g ──┘
```

### Segment wire table

| Wire | From | To | ATmega32 |
|------|------|----|----------|
| W21 | DISP1.a + DISP2.a + DISP3.a (bus) | U1 pin 1 | PB0 |
| W22 | DISP1.b + DISP2.b + DISP3.b (bus) | U1 pin 2 | PB1 |
| W23 | DISP1.c + DISP2.c + DISP3.c (bus) | U1 pin 3 | PB2 |
| W24 | DISP1.d + DISP2.d + DISP3.d (bus) | U1 pin 4 | PB3 |
| W25 | DISP1.e + DISP2.e + DISP3.e (bus) | U1 pin 5 | PB4 |
| W26 | DISP1.f + DISP2.f + DISP3.f (bus) | U1 pin 6 | PB5 |
| W27 | DISP1.g + DISP2.g + DISP3.g (bus) | U1 pin 7 | PB6 |

**In Proteus:** Click segment `a` on DISP1, then click `a` on DISP2, then click `a` on DISP3 — they share one wire. Then connect that wire to PB0.

---

## Step 6 — Digit Select (one COM per display)

| Wire | From | To | Display |
|------|------|----|---------|
| W28 | DISP1 COM | U1 pin 14 | Hundreds (PD0) |
| W29 | DISP2 COM | U1 pin 15 | Tens (PD1) |
| W30 | DISP3 COM | U1 pin 16 | Units (PD2) |

**Rule:** PD pin = LOW → that digit turns ON | PD pin = HIGH → digit OFF

---

## Step 7 — Complete Circuit (all wires drawn)

```
                              +5V (POWER)
                               |
           +-------------------+-------------------+-------------------+
           |                   |                   |                   |
          W3                  W1                  W2                 W4
           |                   |                   |                   |
        ┌──┴──┐           ┌────┴─────────────────┴────┐             [R1 10k]
        │LM35 │           │        ATmega32 (U1)       │                |
        │ U2  │           │                            │               W11
        └──┬──┘           │  PB0(1)──W21──┐           │                |
           |               │  PB1(2)──W22──┤           │                |
          W20              │  PB2(3)──W23──┤  SEGMENT  │         RESET(9)
           |               │  PB3(4)──W24──┤   BUS     │                |
    PA0(40)├───────────────│  PB4(5)──W25──┤           │                |
           |               │  PB5(6)──W26──┤           │                |
          W7               │  PB6(7)──W27──┘           │                |
           |               │                            │                |
          GND              │  PD0(14)──W28──►DISP1 COM  │                |
                           │  PD1(15)──W29──►DISP2 COM  │                |
                           │  PD2(16)──W30──►DISP3 COM  │                |
                           │                            │                |
                           │  XTAL1(13)──[Y1]──XTAL2(12)│                |
                           │              |    |        │                |
                           │            [C1]  [C2]     │                |
                           │              |    |        │                |
                           │             GND  GND       │                |
                           │  VCC(10)  GND(11)          │                |
                           └────────────┬───────────────┘                |
                                        |                                |
                                       GND                               |
                                                                         |
    SEGMENT BUS (W21-W27)                                                |
         |                                                               |
    ┌────┴────────────────────────────────────┐                         |
    |         |         |         |           |                         |
    |    a    |    b    |    c    |    d      |                         |
    |    f    |    g    |    e    |           |                         |
    |         |         |         |           |                         |
    |      [DISP1]   [DISP2]   [DISP3]        |                         |
    |      Hundreds   Tens     Units         |                         |
    |         |         |         |           |                         |
    |       W28       W29       W30           |                         |
    |         |         |         |           |                         |
    |       PD0       PD1       PD2           |                         |
    └─────────────────────────────────────────┘                         |
                                                                          |
    +5V ─────────────────────────────────────────────────────────────────+
```

---

## Step 8 — Wire Connection Checklist

Tick each wire after connecting in Proteus:

```
POWER & RESET
 [ ] W1   +5V  → U1 VCC (pin 10)
 [ ] W2   +5V  → U1 AVCC (pin 30)
 [ ] W3   +5V  → LM35 V+ (pin 1)
 [ ] W4   +5V  → R1 (10k)
 [ ] W5   GND  → U1 GND (pin 11)
 [ ] W6   GND  → U1 GND (pin 31)
 [ ] W7   GND  → LM35 GND (pin 3)
 [ ] W8   GND  → C1
 [ ] W9   GND  → C2
 [ ] W10  GND  → C3
 [ ] W11  R1   → U1 RESET (pin 9)
 [ ] W12  +5V  → C3
 [ ] W13  C3   → GND

CLOCK
 [ ] W14  U1 XTAL1 (pin 13) → Crystal
 [ ] W15  U1 XTAL2 (pin 12) → Crystal
 [ ] W16  Crystal → C1 → GND
 [ ] W17  C1 → GND
 [ ] W18  Crystal → C2 → GND
 [ ] W19  C2 → GND

LM35
 [ ] W20  LM35 Vout (pin 2) → U1 PA0 (pin 40)

SEGMENTS (bus all 3 displays first, then to MCU)
 [ ] W21  a bus → U1 PB0 (pin 1)
 [ ] W22  b bus → U1 PB1 (pin 2)
 [ ] W23  c bus → U1 PB2 (pin 3)
 [ ] W24  d bus → U1 PB3 (pin 4)
 [ ] W25  e bus → U1 PB4 (pin 5)
 [ ] W26  f bus → U1 PB5 (pin 6)
 [ ] W27  g bus → U1 PB6 (pin 7)

DIGIT SELECT
 [ ] W28  DISP1 COM → U1 PD0 (pin 14)
 [ ] W29  DISP2 COM → U1 PD1 (pin 15)
 [ ] W30  DISP3 COM → U1 PD2 (pin 16)
```

**Total wires: 30**

---

## Step 9 — Load Hex File and Run

### Compile

```bash
sudo apt install gcc-avr avr-libc binutils-avr
cd firmware
make
```

Output: `firmware/digital_thermometer.hex`

### Load into Proteus

1. Double-click **ATmega32 (U1)**
2. **Program File** → `digital_thermometer.hex`
3. **Clock Frequency** → `8000000`
4. Click OK

### Run simulation

1. Press Play ▶
2. Double-click **LM35** → set **Temperature**
3. Check display

| LM35 Temperature | Display Shows |
|------------------|---------------|
| 25 °C            | 25            |
| 37 °C            | 37            |
| 99 °C            | 99            |

---

## Temperature Formula

```
LM35 output = 10 mV per °C
Temperature (°C) = ADC_value × 500 / 1024
```

| Temperature | LM35 Voltage | ADC Value |
|-------------|--------------|-----------|
| 25 °C       | 250 mV       | 51        |
| 37 °C       | 370 mV       | 76        |
| 99 °C       | 990 mV       | 203       |

---

## Report Checklist

- [ ] Proteus schematic screenshot
- [ ] Simulation at 25 °C, 37 °C, 99 °C
- [ ] Flowchart (`docs/flowchart.md`)
- [ ] C code listing (`firmware/main.c`)
- [ ] Abstract, Introduction, Methodology, Results, Conclusion
