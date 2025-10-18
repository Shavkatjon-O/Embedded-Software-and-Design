# CRITICAL FINDING: SimulIDE 0.4.15 UART TX Works!

## Test Results - October 18, 2025

### ✅ **SimulIDE 0.4.15-SR10: UART TX WORKING**

**Evidence:**
- Screenshot shows Uart2 component displaying transmitted data
- "Hello hong" and "Once upon a time" visible in component
- VS Code Serial Monitor (COM4) also shows output
- **Conclusion: UART transmission fully functional in 0.4.15**

### 🔍 Component Naming Confusion

**Important Note:**
- Code uses **UART1** registers (UCSR1, UDR1, UBRR1)
- SimulIDE 0.4.15 circuit has component labeled **"Uart2"**
- This is a **labeling issue**, not a functional problem
- The "Uart2" component in 0.4.15 actually connects to UART1 pins (PD2/PD3)

### 📊 Version Comparison Results

| Feature | SimulIDE 0.4.15 | SimulIDE 1.1.0 |
|---------|----------------|----------------|
| **UART TX (MCU → PC)** | ✅ **WORKING** | ❌ **BROKEN** |
| **UART RX (PC → MCU)** | ✅ Working | ✅ Working |
| **LED Response** | ✅ Working | ✅ Working |
| **Startup Messages** | ✅ **Visible** | ❌ Not visible |
| **Menu Display** | ✅ **Visible** | ❌ Not visible |

### 🎯 Confirmed: SimulIDE 1.1.0 Regression Bug

**Conclusion:**
This definitively proves that SimulIDE 1.1.0-SR1 has a **regression bug** in ATmega128 UART TX functionality. The same code that works perfectly in 0.4.15 fails completely in 1.1.0.

### ✅ Recommended Solution

**For Students:**
1. **Use SimulIDE 0.4.15-SR10 for serial communication projects**
   - Run: `W:\soc3050code\tools\simulide\cli-simulide-0415.ps1`
   - UART TX works perfectly
   - More stable for serial testing

2. **Use SimulIDE 1.1.0-SR1 for non-serial projects**
   - Has ELPM fix for ATmega128
   - Better for memory-intensive projects
   - Use for digital I/O, timers, ADC, etc.

3. **Always verify on hardware (COM3)**
   - Hardware is the ultimate "ground truth"
   - Use for final testing and grading

### 🔧 Circuit File Component Names

**Simulator0415.simu (0.4.15):**
```xml
<item itemtype="Uart2" ... />  <!-- Actually UART1 - labeling quirk -->
```

**Simulator110.simu (1.1.0):**
```xml
<item itemtype="SerialTerm" ... />  <!-- UART1 - but TX broken -->
```

### 📝 Action Items

1. ✅ Update SIMULIDE_KNOWN_ISSUES.md with test results
2. ✅ Update course documentation to recommend 0.4.15 for serial
3. ✅ Add warning in project launcher about version selection
4. ✅ Create student guide: "Which SimulIDE version to use?"
5. 🔲 Report bug to SimulIDE developers (optional)

### 🎓 Student Instructions

**For Serial Communication Projects:**
```powershell
# Always use 0.4.15 version
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1 -ProjectDir "projects\Serial_Communications"
```

**For Other Projects (Port, Timer, ADC, etc.):**
```powershell
# Use 1.1.0 version (has ELPM fix)
W:\soc3050code\tools\simulide\cli-simulide.ps1 -ProjectDir "projects\Port_Basic"
```

### 🐛 Bug Report Summary

**Title:** ATmega128 UART TX regression in SimulIDE 1.1.0-SR1

**Description:**
- ATmega128 UART1 transmission works in 0.4.15-SR10
- Same code fails completely in 1.1.0-SR1
- UART RX works in both versions
- Only TX is affected

**Affected:**
- SimulIDE 1.1.0-SR1
- ATmega128 microcontroller
- UART/USART transmission (TX)

**Workaround:**
- Use SimulIDE 0.4.15-SR10 for serial projects
- Or test on physical hardware

---

**Test Date:** October 18, 2025  
**Tester:** Professor Hong Jeong  
**Status:** ✅ **CONFIRMED - Use 0.4.15 for serial projects**
