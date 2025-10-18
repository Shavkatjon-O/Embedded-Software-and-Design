# Which SimulIDE Version Should I Use?

## Quick Answer

### For Serial Communication Projects → Use 0.4.15 ✅
```powershell
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1
```

### For Everything Else → Use 1.1.0 ✅
```powershell
W:\soc3050code\tools\simulide\cli-simulide.ps1
```

---

## Detailed Version Comparison

| Project Type | SimulIDE 0.4.15 | SimulIDE 1.1.0 | Recommended |
|--------------|-----------------|----------------|-------------|
| **Serial_Communications** | ✅ **Works perfectly** | ❌ UART TX broken | **0.4.15** |
| **I2C_Master_Basic** | ✅ Works | ✅ Works | 1.1.0 |
| **I2C_RTC_DS1307** | ✅ Works | ✅ Works | 1.1.0 |
| **I2C_Sensors_Multi** | ✅ Works | ✅ Works | 1.1.0 |
| **SPI_Master_Basic** | ✅ Works | ✅ Works | 1.1.0 |
| **SPI_EEPROM_Memory** | ✅ Works | ✅ Works | 1.1.0 |
| **SPI_Multi_Device** | ✅ Works | ✅ Works | 1.1.0 |
| **Port_Basic** | ✅ Works | ✅ Works | 1.1.0 |
| **Port_Assembly** | ✅ Works | ✅ Works | 1.1.0 |
| **Timer_Basic** | ✅ Works | ✅ Works | 1.1.0 |
| **PWM_Motor_DC** | ✅ Works | ✅ Works | 1.1.0 |
| **PWM_Motor_Servo** | ✅ Works | ✅ Works | 1.1.0 |
| **PWM_Motor_Stepper** | ✅ Works | ✅ Works | 1.1.0 |
| **ADC_Basic** | ✅ Works | ✅ Works | 1.1.0 |
| **Joystick** | ✅ Works | ✅ Works | 1.1.0 |
| **CDS_Light_Sensor** | ✅ Works | ✅ Works | 1.1.0 |
| **Accelerometer** | ✅ Works | ✅ Works | 1.1.0 |
| **LCD_Character_Basic** | ✅ Works | ✅ Works | 1.1.0 |
| **LCD_Advanced_Features** | ✅ Works | ✅ Works | 1.1.0 |
| **LCD_Sensor_Dashboard** | ✅ Works | ✅ Works | 1.1.0 |
| **Graphics_Display** | ⚠️ May fail | ✅ **ELPM fixed** | **1.1.0** |
| **Keypad_Matrix_Basic** | ✅ Works | ✅ Works | 1.1.0 |
| **Keypad_Advanced_Debounce** | ✅ Works | ✅ Works | 1.1.0 |
| **Keypad_Calculator_App** | ✅ Works | ✅ Works | 1.1.0 |
| **Interrupt** | ✅ Works | ✅ Works | 1.1.0 |
| **Watchdog_System_Reset** | ✅ Works | ✅ Works | 1.1.0 |
| **Watchdog_Fail_Safe** | ✅ Works | ✅ Works | 1.1.0 |
| **Power_Sleep_Modes** | ✅ Works | ✅ Works | 1.1.0 |
| **Power_LowPower_Sensors** | ✅ Works | ✅ Works | 1.1.0 |
| **Power_Wakeup_Optimization** | ✅ Works | ✅ Works | 1.1.0 |
| **ELPM_Test** | ❌ ELPM broken | ✅ **ELPM fixed** | **1.1.0** |

---

## Known Issues by Version

### SimulIDE 0.4.15-SR10 (Legacy - 2020)

**✅ Strengths:**
- ✅ **UART/Serial communication works perfectly**
- ✅ Stable and well-tested
- ✅ Good for educational serial projects
- ✅ All basic peripherals work

**❌ Weaknesses:**
- ❌ **ELPM instruction broken** (affects large programs >64KB)
- ❌ Graphics_Display project may fail
- ❌ Older component library
- ❌ No recent bug fixes

**Use for:**
- Serial_Communications project
- UART debugging
- Any project using serial terminal
- Projects that fit in lower 64KB memory

---

### SimulIDE 1.1.0-SR1 (Current - 2024)

**✅ Strengths:**
- ✅ **ELPM instruction fixed** (Professor Hong's patch applied)
- ✅ Graphics_Display works correctly
- ✅ Updated component library
- ✅ Better UI and features
- ✅ All digital I/O, timers, ADC, I2C, SPI work

**❌ Weaknesses:**
- ❌ **ATmega128 UART TX completely broken** (regression bug)
- ❌ No serial output from MCU
- ❌ SerialTerminal component shows no TX data
- ❌ External serial channel crashes (Issue #59)

**Use for:**
- All non-serial projects
- Graphics_Display (needs ELPM fix)
- I2C, SPI, Timers, ADC, PWM
- Digital I/O and interrupts
- Power management projects

---

## How to Switch Versions

### Using VS Code Tasks (Easiest)

1. Open your project file (e.g., `Main.c`)
2. Press `Ctrl+Shift+P`
3. Type "Tasks: Run Task"
4. Choose:
   - **"Build and Simulate Current Project"** → Uses 1.1.0
   - **"Build and Simulate 0415 (Legacy Test)"** → Uses 0.4.15

### Using PowerShell Commands

```powershell
# For Serial Projects (use 0.4.15)
cd W:\soc3050code\projects\Serial_Communications
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1

# For Other Projects (use 1.1.0)
cd W:\soc3050code\projects\Graphics_Display
W:\soc3050code\tools\simulide\cli-simulide.ps1
```

### Important: Close Between Versions!

**⚠️ NEVER run both versions simultaneously**
```powershell
# Always close SimulIDE before switching
Stop-Process -Name simulide -Force -ErrorAction SilentlyContinue
Start-Sleep -Seconds 1
```

---

## Testing Evidence (October 18, 2025)

### Proof: SimulIDE 0.4.15 UART TX Works

**Test Setup:**
- Project: Serial_Communications
- Code: UART1 transmission using UDR1 register
- Test: Send "Hello hong" and "Once upon a time"

**Results:**
- ✅ **SimulIDE 0.4.15**: Text visible in Uart2 component
- ✅ **VS Code Serial Monitor**: Output received via COM4
- ❌ **SimulIDE 1.1.0**: SerialTerminal shows nothing

**Conclusion:**
SimulIDE 1.1.0 has a **regression bug** in ATmega128 UART TX. Use 0.4.15 for serial projects.

---

## Recommendations for Different Users

### Students Learning Serial Communication
```
→ Use SimulIDE 0.4.15
  Reason: Need to see UART output for learning
  Script: cli-simulide-0415.ps1
```

### Students Learning Graphics (GLCD)
```
→ Use SimulIDE 1.1.0
  Reason: ELPM instruction required for graphics library
  Script: cli-simulide.ps1
```

### Students Learning I2C/SPI/Timers/ADC
```
→ Use SimulIDE 1.1.0
  Reason: Better component library, no known issues
  Script: cli-simulide.ps1
```

### Debugging Any Project
```
→ Use HARDWARE (COM3)
  Reason: Simulator bugs don't affect hardware
  Method: "Build and Program Current Project" task
```

---

## Project-Specific Quick Reference

| If You're Working On... | Use This Command |
|-------------------------|------------------|
| Serial_Communications | `cli-simulide-0415.ps1` |
| Graphics_Display | `cli-simulide.ps1` |
| ELPM_Test | `cli-simulide.ps1` |
| LCD_Sensor_Dashboard (with UART debug) | `cli-simulide-0415.ps1` |
| I2C_RTC_DS1307 | `cli-simulide.ps1` |
| SPI_EEPROM_Memory | `cli-simulide.ps1` |
| Timer_Basic | `cli-simulide.ps1` |
| PWM_Motor_* | `cli-simulide.ps1` |
| ADC_Basic | `cli-simulide.ps1` |
| Interrupt | `cli-simulide.ps1` |
| Port_Basic | `cli-simulide.ps1` |
| Watchdog_* | `cli-simulide.ps1` |

**Default rule:** If it doesn't use serial output, use 1.1.0

---

## Hardware is Always Best

**For Final Testing and Grading:**
```powershell
# Always verify on hardware
W:\soc3050code\tools\cli\cli-program-project.ps1 -ProjectDir . -Programmer arduino -Port COM3
```

**Why Hardware?**
- ✅ No simulator bugs
- ✅ Real-world behavior
- ✅ Accurate timing
- ✅ True peripheral operation
- ✅ What professors grade on

**SimulIDE is for:**
- Visual understanding
- Quick testing
- Circuit verification
- Learning peripheral connections

---

## Need Help?

### Check These Files:
- `TEST_RESULTS_SIMULIDE_0415.md` - Test evidence
- `docs/simulide/SIMULIDE_KNOWN_ISSUES.md` - All known bugs
- `docs/simulide/SIMULIDE_VERSION_COMPARISON.md` - Detailed comparison
- `SIMULIDE_0415_SETUP_SUMMARY.md` - Quick setup guide

### Common Questions:

**Q: Why does 0.4.15 show "Uart2" but code uses UART1?**
A: Labeling quirk in 0.4.15. The component connects to UART1 pins (PD2/PD3) correctly.

**Q: Can I use 0.4.15 for Graphics_Display?**
A: Not recommended. ELPM instruction is broken in 0.4.15, graphics library won't work.

**Q: Which version for I2C sensor with UART debugging?**
A: Use 0.4.15 if you need to see UART debug output. I2C works in both versions.

**Q: How do I know which version is running?**
A: Check window title. Also: `Get-Process simulide | Select-Object Path`

---

**Last Updated:** October 18, 2025  
**Status:** ✅ Tested and verified  
**Summary:** Use 0.4.15 for serial, 1.1.0 for everything else, hardware for final testing
