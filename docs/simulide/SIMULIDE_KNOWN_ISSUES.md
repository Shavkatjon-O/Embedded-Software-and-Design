# SimulIDE Known Issues and Workarounds

## SimulIDE 1.1.0-SR1 Critical Bugs

### 1. ATmega128 UART TX Transmission Bug ⚠️ **CRITICAL**

**Issue**: ATmega128 UART transmission (TX) does not work in SimulIDE 1.1.0-SR1
- ✅ UART RX works (simulator receives data from external sources)
- ❌ UART TX fails (MCU cannot transmit data back)
- ✅ Digital I/O works (LEDs, buttons, etc.)
- ✅ Hardware works perfectly

**Symptoms**:
- MCU program executes correctly (LEDs blink, code runs)
- No data appears from MCU UART transmission
- SerialPort/SerialTerm Input window remains empty
- No startup messages from MCU
- Tested with both SerialPort and SerialTerm components - both fail

**Affected Configuration**:
```xml
<!-- Both components fail for UART TX -->
<item itemtype="SerialPort" ... />
<item itemtype="SerialTerm" ... />
```

**Root Cause**: 
SimulIDE 1.1.0-SR1 has incomplete/buggy implementation of ATmega128 USART1 TX functionality. This is similar to the ELPM bug that was already patched.

**Related Issues**:
- GitHub Issue #59: "Crashes after a while when connected via an external serial channel"
- ATmega128 UART peripheral simulation is incomplete

**Testing Evidence**:
- Date discovered: 2025-10-18
- Same code works on hardware (COM3)
- SerialTerm receives typed input correctly
- MCU executes other code correctly (LEDs work)
- **✅ CONFIRMED: SimulIDE 0.4.15 UART TX works perfectly**
- **❌ CONFIRMED: SimulIDE 1.1.0 UART TX is completely broken**
- Conclusion: **Regression bug** introduced in 1.1.0

**Comparison Testing Results (October 18, 2025)**:
| Feature | SimulIDE 0.4.15 | SimulIDE 1.1.0 |
|---------|----------------|----------------|
| UART TX | ✅ Working | ❌ Broken |
| UART RX | ✅ Working | ✅ Working |
| Evidence | "Hello hong" visible in Uart2 component | No output in SerialTerm |

**Workaround**:
✅ **USE SimulIDE 0.4.15-SR10 for serial communication projects**
- Tested and confirmed working
- Run: `tools\simulide\cli-simulide-0415.ps1`
- UART TX and RX both functional
- More stable for serial testing

**TESTING WITH OLDER VERSION**:
✅ **SimulIDE 0.4.15-SR10 testing COMPLETE - UART TX works!**
- Scripts created to test if older version works better
- Run: `tools\simulide\cli-simulide-0415.ps1`
- See: `TEST_RESULTS_SIMULIDE_0415.md` for detailed results
- See: `docs\simulide\SIMULIDE_VERSION_COMPARISON.md` for testing guide
- See: `SIMULIDE_0415_SETUP_SUMMARY.md` for quick start

**RECOMMENDED SOLUTION**: 
✅ **Use SimulIDE 0.4.15 for serial communication projects**
- Run: `tools\simulide\cli-simulide-0415.ps1`
- UART TX confirmed working
- UART RX confirmed working
- Perfect for Serial_Communications, UART-based sensors, debugging, etc.

**For Non-Serial Projects:**
- Use SimulIDE 1.1.0 (has ELPM fix for ATmega128)
- Good for: Port I/O, Timers, ADC, PWM, I2C, SPI
- Run: `tools\simulide\cli-simulide.ps1`

**Hardware Testing:**
- Hardware (COM3): 100% reliable, fully functional
- Use for final verification and grading

**Alternative**: If 0.4.15 testing proves successful, use old SimulIDE version for serial projects

---

### 2. ELPM Instruction Bug (Already Patched)

**Issue**: ELPM (Extended Load Program Memory) instruction not properly implemented.

**Status**: ✅ Fixed with custom patch in project

---

## Alternative Solutions

### Option A: Use SimulIDE 0.4.15
- Older version may not have these bugs
- Circuit file: `Simulator0415.simu`
- Less features but more stable serial communication

### Option B: Hardware Testing Only
- Physical ATmega128 board on COM3
- 100% reliable
- Recommended for student testing

### Option C: Built-in Serial Monitor
- Use SerialPort with `Port=""` and `SerialMon="true"`
- No external COM port connection
- Limited to SimulIDE window only

---

## Current Configuration

**Simulator110.simu**: 
- Configured for built-in serial monitor (workaround applied)
- No external COM port to avoid bug

**Hardware Testing**:
- COM3: Physical board ✅ Works perfectly
- Recommended for all serial communication labs

---

## References

- SimulIDE Issue #59: https://github.com/eeTools/SimulIDE-dev/issues/59
- SimulIDE Version: 1.1.0-SR1 (Released 2024-03-25)
- Bug discovered: 2025-10-18

---

## For Students

**Important**: When testing serial communication projects:

1. **Primary Method**: Use hardware (COM3) for actual testing
2. **Secondary Method**: Use SimulIDE built-in serial monitor for visual verification
3. **Don't use**: External COM port bridges (COM4↔COM5) - known to have bugs

Your code works correctly - the issue is with SimulIDE's external COM port implementation!
