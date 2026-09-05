# Battery Voltage Monitor

An Arduino project that reads a battery's voltage through a resistor voltage divider and lights up a color-coded LED (green/yellow/red) to show its charge state, streaming every reading to the serial monitor as CSV.

Built as a personal project to get hands-on with C++ after finishing coursework in digital logic, an Arduino sketch (`.ino`) compiles as C++, with `setup()`/`loop()` standing in for `main()`.

> **Note:** the sketch's serial output uses Portuguese state names (`Verde`/`Amarelo`/`Vermelho`). This documentation is in English.

## How the circuit works

The circuit was designed and simulated in Tinkercad:

<img width="1185" height="420" alt="circuito_tinkercad" src="https://github.com/user-attachments/assets/31366af9-e97e-41fe-b635-7ab09a10a719" />

The power source in this project can reach up to 15 V, well above the 5 V an Arduino analog pin can safely read. A voltage divider made of two resistors in series (R1 = 20 kΩ, R2 = 10 kΩ) solves that: the midpoint between them connects to pin `A0`, and the voltage there is always `Vsource × R2 / (R1 + R2)`, which for these values works out to `Vsource / 3`. At the maximum 15 V, `A0` sees exactly 5 V, right at the Arduino's limit, never past it.

**Components used:**
- 1× Arduino Uno
- 1× Breadboard
- 5× Resistors (2 for the voltage divider, 3 as current limiters for the LEDs)
- 3× LEDs (green, yellow, red)
- 12× Jumper wires
- 1× Variable power source

## How the code works

Since `A0` only sees a scaled-down fraction of the real voltage, the sketch reverses the math to recover the source voltage from the raw ADC reading:

```cpp
float tensaoA0 = valorADC * (5.0 / 1023.0);
float tensaoFonte = tensaoA0 * ((R1 + R2) / R2);
```

That reconstructed voltage is then compared against two thresholds to decide which LED to light:

| Voltage range | LED |
|---|---|
| ≥ 13.9 V | Green |
| 12.2 V – 13.9 V | Yellow |
| < 12.2 V | Red |

On every loop, the sketch also prints the elapsed time, the computed voltage, and the current state over serial (`millis()/1000.0`, `tensaoFonte`, `estado`, comma-separated), so a run can be logged and reviewed afterward.

## Test results

The circuit was tested by letting the source voltage fall from 15 V to 0 V and logging the output. The LED followed the thresholds exactly:

| Time (s) | Voltage (V) | State | Note |
|---|---|---|---|
| 0.00 | 15.00 | Verde (green) | Test start |
| 11.55 | 14.00 | Verde (green) | Last reading above the yellow threshold |
| 12.05 | 13.61 | Amarelo (yellow) | Crosses below 13.9 V |
| 13.06 | 12.60 | Amarelo (yellow) | Last reading above the red threshold |
| 13.56 | 11.91 | Vermelho (red) | Crosses below 12.2 V |
| 23.10 | 0.00 | Vermelho (red) | Test end — source depleted |

The full 47-row log is in [`data/resultado_teste.csv`](data/resultado_teste.csv). It's semicolon-separated with comma decimals (`;` / `,`), the format Excel produces by default under Brazilian regional settings when the raw serial output is opened there; the sketch itself prints plain comma-separated values.

## Getting started

1. Open `battery_monitor.ino` in the Arduino IDE (or import it into a Tinkercad project).
2. Wire the circuit as shown above.
3. Upload the sketch to an Arduino Uno.
4. Open the Serial Monitor at 9600 baud to watch the live readings.

## Project structure

```
.
├── battery_monitor.ino
├── README.md
├── LICENSE
├── .gitignore
├── images/
│   └── circuito_tinkercad.png
└── data/
    └── resultado_teste.csv
```

## Possible improvements

- Average several ADC samples per reading to smooth out sensor noise
- Add hysteresis around the two thresholds so the LED doesn't flicker if the voltage hovers right at a boundary
- Replace the `String` state variable with a `char*` or `enum` to avoid heap fragmentation on long-running sessions

## Author

Pedro Rangel — Electronics and Computer Engineering student at UFRJ

## License

Released under the [MIT License](LICENSE).
