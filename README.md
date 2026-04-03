# Turing Machine Simulator

A C++ implementation of a Turing machine simulator that reads machine definitions from configuration files and executes them on input strings.

## Overview

This project implements a working Turing machine with:
- **Configurable states** (starting and final states)
- **Customizable alphabet** (input symbols)
- **Transition rules** defining state changes, symbol rewrites, and tape head movement
- **Fixed-size tape** (1024 bytes)
- **Input validation** ensuring the entire input is consumed

## Building

### Prerequisites
- CMake 3.10+
- C++17 compatible compiler (g++, clang, etc.)
- Ninja (or Make)

### Build Steps

```bash
cd /path/to/Turing
mkdir -p build
cd build
cmake ..
ninja
```

The executable `Turing` will be created in the `build/` directory.

## Usage

```bash
./build/Turing <config_file> <input_string>
```

### Arguments
- `<config_file>`: Path to the Turing machine configuration file
- `<input_string>`: The string to test against the machine

### Return Codes
- `0`: Input **ACCEPTED** (machine reached final state after consuming all input)
- `1`: Input **REJECTED** (invalid transition, incomplete input, or reached boundary)
- `2`: **ERROR** (invalid configuration file)

## Configuration File Format

The configuration file uses section-based syntax:

```
# Comments start with # or ;

.states
[FLAGS] state_name

.alphabet
symbol1 symbol2 symbol3 ...

.transitions
from_state input_symbol to_state write_symbol direction
```

### State Definitions

- `S state_name` — Mark as **Starting state** (required, only one)
- `F state_name` — Mark as **Final state** (accepting state)
- `state_name` — Regular state (no flags)

### Alphabet

List all symbols the machine can read and write (space-separated). Common choices:
- `a b c` for string matching patterns
- `0 1` for binary operations
- Custom characters for other tasks

### Transitions

Format: `from_state input_symbol to_state write_symbol direction`

- `from_state`: Current state
- `input_symbol`: Symbol read from tape
- `to_state`: Next state
- `write_symbol`: Symbol to write on tape
- `direction`: Movement (`L` for left, `R` for right)

## Example: Accepts a^n b^n Pattern

**File: `example.txt`**
```
# Turing Machine - accepts strings like "ab", "aabb", "aaabbb", etc.
# Pattern: one or more a's followed by one or more b's

.states
S reading_a
reading_b
F reading_b

.alphabet
a b

.transitions
reading_a a reading_a a R
reading_a b reading_b b R
reading_b b reading_b b R
```

### Run Examples

```bash
# Accept cases
./build/Turing example.txt "ab"       # ✓ ACCEPTED
./build/Turing example.txt "aabb"     # ✓ ACCEPTED
./build/Turing example.txt "aaabbb"   # ✓ ACCEPTED

# Reject cases
./build/Turing example.txt "ba"       # ✗ REJECTED
./build/Turing example.txt "a"        # ✗ REJECTED (no b's)
./build/Turing example.txt "aabaa"    # ✗ REJECTED (extra input)
```

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── includes/
│   ├── Turing.h            # Main Turing machine class
│   └── parser.h            # Configuration file parser
├── src/
│   ├── Turing.cpp          # Implementation
│   ├── parser.cpp          # Parser implementation
│   ├── main.cpp            # Entry point
│   └── test.txt            # Example configuration
└── build/                  # Build output directory
    └── Turing              # Executable
```

## How It Works

### Initialization
1. Parser reads the configuration file (`.states`, `.alphabet`, `.transitions`)
2. Validates state definitions and transitions
3. Stores the Turing machine definition

### Execution (`run()`)
1. Places input string on the tape, fills rest with null bytes (`\0` - blank symbol)
2. Starts at the initial state, position 0
3. **Main loop:**
   - Check if in final state AND reached end of input → **ACCEPT**
   - Read symbol at current tape position
   - Look up transition for (current_state, symbol)
   - If no transition found → **REJECT**
   - Execute transition: write symbol, move tape head (L/R), change state
   - If pointer goes out of bounds → **REJECT**
4. Repeat until acceptance or rejection

### Input Validation
The machine only accepts if:
1. It reaches a **final state** (marked with `F`)
2. It has consumed the **entire input** (pointer beyond input length)
3. No other rejection condition occurred

## Limitations

- **Fixed tape size**: 1024 bytes (adjustable via `BAND_SIZE`)
- **No infinite loop detection**: Machine may run indefinitely on cyclic transitions
- **Deterministic only**: Supports single transitions per (state, symbol) pair
- **Blank symbol**: Uses null bytes (`\0`) after the input to mark the end of input on the tape

## Future Enhancements

- [ ] Non-deterministic transitions
- [ ] Configurable tape size
- [ ] Execution trace/debugging mode
- [ ] Multiple tape support
- [ ] Infinite loop detection

## License

This project was created as an educational implementation of Turing machine theory.
