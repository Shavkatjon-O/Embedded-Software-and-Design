# SimulIDE Version Comparison Guide

## Purpose
Compare UART/serial communication behavior between SimulIDE versions:
- **SimulIDE 0.4.15-SR10** (Legacy version)
- **SimulIDE 1.1.0-SR1** (Current version with known UART TX bug)

## Quick Start

### Testing with SimulIDE 0.4.15 (Legacy)
```powershell
# Navigate to your project
cd W:\soc3050code\projects\Serial_Communications

# Option 1: Run VS Code Task
# Press Ctrl+Shift+P → "Tasks: Run Task" → "Build and Simulate 0415 (Legacy Test)"

# Option 2: Run script directly
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1 -ProjectDir .
```

### Testing with SimulIDE 1.1.0 (Current)
```powershell
# Option 1: Run VS Code Task
# Press Ctrl+Shift+P → "Tasks: Run Task" → "Build and Simulate Current Project"

# Option 2: Run script directly
W:\soc3050code\tools\simulide\cli-simulide.ps1 -ProjectDir .
```

## File Structure

```
tools/simulide/
├── SimulIDE_0.4.15-SR10_Win64/     # Legacy version
│   └── simulide.exe
├── SimulIDE_1.1.0-SR1_Win64/       # Current version
│   └── simulide.exe
├── Simulator0415.simu              # Circuit for 0.4.15
├── Simulator110.simu               # Circuit for 1.1.0
├── cli-simulide-0415.ps1           # Launch 0.4.15 (NEW)
└── cli-simulide.ps1                # Launch 1.1.0 (Default)
```

## VS Code Tasks Available

### New Tasks (for version comparison):
1. **"Simulate in SimulIDE 0.4.15 (Legacy)"**
   - Launches old version with Simulator0415.simu
   - Does NOT auto-build (assumes already built)
   
2. **"Build and Simulate 0415 (Legacy Test)"**
   - Builds project first
   - Then launches SimulIDE 0.4.15
   - Best for testing UART functionality

### Existing Tasks (unchanged):
3. **"Build and Simulate Current Project"**
   - Uses SimulIDE 1.1.0-SR1
   - Default simulation workflow

## Testing Procedure

### Step 1: Test in SimulIDE 0.4.15
```powershell
# 1. Close any running SimulIDE instances
Get-Process simulide -ErrorAction SilentlyContinue | Stop-Process -Force

# 2. Build and launch with 0.4.15
cd W:\soc3050code\projects\Serial_Communications
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1

# 3. In SimulIDE:
#    - Click Play button
#    - Open SerialPort or SerialTerminal component
#    - Observe UART TX output
#    - Test typing input (UART RX)
```

### Step 2: Test in SimulIDE 1.1.0
```powershell
# 1. Close SimulIDE 0.4.15
Get-Process simulide -ErrorAction SilentlyContinue | Stop-Process -Force

# 2. Launch with 1.1.0
W:\soc3050code\tools\simulide\cli-simulide.ps1

# 3. In SimulIDE:
#    - Click Play button
#    - Open SerialTerminal component
#    - Compare behavior with 0.4.15
```

### Step 3: Document Results
Compare the following between versions:

| Feature | SimulIDE 0.4.15 | SimulIDE 1.1.0 |
|---------|----------------|----------------|
| **UART TX (MCU → PC)** | ✅ **WORKING** | ❌ Not working |
| **UART RX (PC → MCU)** | ✅ Working | ✅ Working |
| **LED Response** | ✅ Working | ✅ Working |
| **Startup Messages** | ✅ **Visible** | ❌ Not visible |
| **Menu Display** | ✅ **Visible** | ❌ Not visible |

**✅ TEST COMPLETED: October 18, 2025**
**Result:** SimulIDE 0.4.15 UART TX works perfectly! SimulIDE 1.1.0 has regression bug.

## Expected Outcomes

### ✅ TEST RESULT: 0.4.15 UART TX Works! (Confirmed October 18, 2025)
**Root cause confirmed**: SimulIDE 1.1.0-SR1 regression bug
- ✅ SimulIDE 0.4.15 UART TX works perfectly
- ✅ Evidence: "Hello hong" and "Once upon a time" visible in Uart2 component
- ✅ VS Code Serial Monitor also shows output
- ❌ SimulIDE 1.1.0 UART TX completely non-functional
- **Recommendation**: **Use SimulIDE 0.4.15 for ALL serial communication projects**
- Document findings in SIMULIDE_KNOWN_ISSUES.md ✅ Done
- Update recommendation: "Use 0.4.15 for serial testing" ✅ Done
- Consider reporting to SimulIDE developers

**Note:** SimulIDE 0.4.15 labels the component as "Uart2" but it actually connects to UART1 pins (PD2/PD3). This is just a labeling quirk.

### If 0.4.15 UART TX Also Fails:
⚠️ **Circuit configuration issue**: Both versions affected
- Review circuit file (Simulator0415.simu vs Simulator110.simu)
- Check ATmega128 pin connections
- Verify SerialPort/SerialTerminal settings

**Status:** This scenario did NOT occur - 0.4.15 works perfectly!

## Important Notes

### Version Conflicts
⚠️ **Never run both SimulIDE versions simultaneously!**
- They may share configuration files
- Process detection prevents accidental overlap
- Always close one version before launching the other

### Circuit Files
- `Simulator0415.simu` → For SimulIDE 0.4.15 **ONLY**
- `Simulator110.simu` → For SimulIDE 1.1.0 **ONLY**
- Scripts automatically select correct circuit
- Do **NOT** manually swap circuit files between versions

### Hardware Testing
✅ **Hardware (COM3) always works correctly**
- UART TX: ✅ Working perfectly
- UART RX: ✅ Working perfectly
- Use hardware as "ground truth" reference

## Troubleshooting

### "SimulIDE 0.4.15 executable not found"
```powershell
# Check if directory exists
Test-Path "W:\soc3050code\tools\simulide\SimulIDE_0.4.15-SR10_Win64\simulide.exe"

# If missing, extract SimulIDE_0.4.15-SR10_Win64.zip to tools/simulide/
```

### "Simulator0415.simu not found"
```powershell
# Check if circuit file exists
Test-Path "W:\soc3050code\tools\simulide\Simulator0415.simu"

# If missing, create from Simulator110.simu or restore from backup
```

### "SimulIDE is already running"
```powershell
# Close all SimulIDE instances
Get-Process simulide -ErrorAction SilentlyContinue | Stop-Process -Force
Start-Sleep -Seconds 1
```

### Serial Component Not Showing Output
1. Check MCU is running (LEDs blinking)
2. Double-click SerialPort or SerialTerminal
3. Verify baud rate: 9600
4. Check TX pin connection: PD3 (PORTD bit 3)

## Command Reference

### PowerShell Commands
```powershell
# Launch 0.4.15 with current project
.\tools\simulide\cli-simulide-0415.ps1

# Launch 0.4.15 with specific project
.\tools\simulide\cli-simulide-0415.ps1 -ProjectDir "projects\Serial_Communications"

# Launch without rebuilding (faster testing)
.\tools\simulide\cli-simulide-0415.ps1 -BuildFirst $false

# Specify custom circuit
.\tools\simulide\cli-simulide-0415.ps1 -CircuitFile "custom\circuit.simu"

# Kill all SimulIDE instances
Stop-Process -Name simulide -Force -ErrorAction SilentlyContinue
```

### VS Code Task Shortcuts
```
Ctrl+Shift+B           → Build Current Project
Ctrl+Shift+P           → Open Command Palette
  → "Tasks: Run Task"  → See all available tasks
```

## Related Documentation

- `docs/simulide/SIMULIDE_KNOWN_ISSUES.md` - Known bugs in SimulIDE 1.1.0
- `docs/simulide/SIMULIDE_GUIDE.md` - General SimulIDE usage
- `tools/simulide/cli-simulide-0415.ps1` - Legacy version launcher script
- `tools/simulide/cli-simulide.ps1` - Current version launcher script

## Reporting Results

After testing both versions, please update:

1. **SIMULIDE_KNOWN_ISSUES.md**
   - Add section "Version Comparison Results"
   - Document which version works for UART TX
   - Include testing date and project used

2. **This Guide (SIMULIDE_VERSION_COMPARISON.md)**
   - Fill in the comparison table
   - Add any new findings or workarounds

---

**Created:** January 2025  
**Purpose:** Test if UART TX bug exists in SimulIDE 0.4.15 vs 1.1.0  
**Status:** Ready for testing
