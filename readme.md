# Sorting Visualizer (C++)

This project is a **terminal-based sorting visualizer** written in C++. It uses ANSI escape codes to animate popular sorting algorithms directly in the terminal.

---

## Features

- **Sorting Algorithms Implemented:**
  - Bubble Sort
  - Insertion Sort
  - Selection Sort
  - Merge Sort
  - Quick Sort
  - Heap Sort
- **Interactive Controls:**
  - `1..6` → Choose sorting algorithm
  - `s` → Shuffle array
  - `v` → Toggle numeric values below bars
  - `+` / `-` → Increase / decrease animation delay
  - `n` → Change array size (5–200)
  - `q` → Quit program
- **Customizable:**
  - Adjustable delay
  - Adjustable array size
  - Option to display or hide numeric values

---

## Requirements

- **Compiler:** Any modern C++17 compiler (tested with `g++`)
- **Platform:**
  - Linux or macOS terminal (ANSI escape codes supported)
  - Windows users can run this in WSL or a terminal supporting ANSI sequences (e.g., Windows Terminal)

---

## Build Instructions

1. Clone or download this repository.
2. Compile the program using:
   ```bash
   g++ -std=c++17 -O2 -pthread sort_visualizer.cpp -o sortviz
   ```
3. Run the program:
   ```bash
   ./sortviz
   ```

---

## Usage

1. When you run the program, a randomized array will be displayed as vertical bars.
2. Open the menu in the console and choose an option:
   - Example: Press `5` to run **Quick Sort**.
3. Watch the array get sorted step by step with animations.

---

## Example Output

```
Sorting Visualizer (n=50, delay=20ms)
Bubble Sort: comparing j and j+1

██████ █████ ███ ██████ ███ ...
```

(Bars update live as sorting progresses.)

---

## Notes

- Increase array size (`n`) for smoother bar animations.
- Use smaller sizes if you want to track numbers precisely.
- If output looks broken, ensure your terminal supports ANSI escape codes.

---

## License

This project is free to use, modify, and distribute for educational purposes.

