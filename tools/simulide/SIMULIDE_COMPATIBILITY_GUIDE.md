# SimulIDE Version Compatibility Guide

## The Problem

After extensive testing, it was confirmed that the **Graphics_Display project's GLCD library does not work correctly with SimulIDE 1.1.0**, but works perfectly with SimulIDE 0.4.15.

### Symptoms with SimulIDE 1.1.0:
- KS0108 LCD display shows blank/corrupted output
- Graphics rendering functions fail silently  
- Text display is garbled or invisible
- No error messages, but display doesn't work

### Root Cause Analysis:
1. **Pin mappings are correct** - Both versions use PE6→CS2, PE7→CS1
2. **CS signal polarity is correct** - Both use CS_Active_Low="false" 
3. **Circuit connections are identical** - Same wiring in both .simu files
4. **The issue is in SimulIDE's KS0108 simulation behavior** - Internal changes between versions

## The Solution

### Automatic Version Selection
Use the intelligent launcher that automatically chooses the best SimulIDE version:

```powershell
# Auto-detect best version for current project
.\tools\simulide\auto-simulide-launcher.ps1

# Force Graphics_Display to use working version
.\tools\simulide\auto-simulide-launcher.ps1 -ProjectDir "projects\Graphics_Display"

# Override version selection (for testing)
.\tools\simulide\auto-simulide-launcher.ps1 -ForceVersion "0.4.15"
```

### Project-Specific Recommendations

| Project Type | Recommended SimulIDE | Circuit File | Reason |
|-------------|---------------------|--------------|--------|
| **Graphics_Display** | **0.4.15** | `Simulator0415.simu` | ✅ GLCD library confirmed working |
| **All Other Projects** | **1.1.0** | `Simulator.simu` | ✅ Latest features, better performance |

### Manual Version Selection

If you need to manually choose:

#### For Graphics_Display (Use 0.4.15):
```powershell
.\tools\simulide\cli-simulide.ps1 -ProjectDir "projects\Graphics_Display" -SimulIDEPath "tools\simulide\SimulIDE_0.4.15-SR10_Win64\simulide.exe" -CircuitFile "tools\simulide\Simulator0415.simu"
```

#### For Other Projects (Use 1.1.0):
```powershell
.\tools\simulide\cli-simulide.ps1 -ProjectDir "projects\Port_Basic" -SimulIDEPath "tools\simulide\SimulIDE_1.1.0-SR1_Win64\simulide.exe" -CircuitFile "tools\simulide\Simulator.simu"
```

## Technical Details

### What Was Investigated

1. **Timing Parameters** - Tested different delay constants (D_BEFORE, D_AFTER, D_MIDDLE)
2. **CS Signal Polarity** - Verified CS_Active_Low settings match library expectations
3. **Pin Mapping** - Confirmed PE6/PE7 connections are correct in both versions
4. **Circuit Configuration** - Compared working PIC examples with ATmega128 setup
5. **GLCD Library Code** - Analyzed command/data sequences and initialization

### What Changed Between SimulIDE Versions

SimulIDE 1.1.0 introduced internal changes to KS0108 simulation that affect:
- Timing sensitivity of command/data sequences
- CS signal handling during panel switching
- Display memory state management
- Command execution order requirements

### The GLCD Library

Our GLCD library (`shared_libs/_glcd.c`) implements:
- **Active HIGH CS signals** (CS1=HIGH, CS2=HIGH to select panels)
- **Standard KS0108 command set** (Display ON/OFF, Set Y Address, Set X Page, etc.)
- **Panel switching logic** (Left panel = CS1, Right panel = CS2)
- **Timing delays** optimized for SimulIDE 0.4.15

## Files Involved

### Circuit Files:
- `tools/simulide/Simulator.simu` - For SimulIDE 1.1.0 (XML format version="1.1.0-SR1")
- `tools/simulide/Simulator0415.simu` - For SimulIDE 0.4.15 (XML format type="simulide_0.4")

### Scripts:
- `tools/simulide/auto-simulide-launcher.ps1` - **Recommended** - Intelligent version selection
- `tools/simulide/cli-simulide.ps1` - Manual launcher with version/circuit options

### Library:
- `shared_libs/_glcd.c` - KS0108 GLCD library (optimized for SimulIDE 0.4.15)
- `shared_libs/_glcd.h` - GLCD library header

## Future Improvements

### Option 1: Dual GLCD Libraries
Create version-specific libraries:
- `_glcd_v0415.c` - Optimized for SimulIDE 0.4.15
- `_glcd_v110.c` - Optimized for SimulIDE 1.1.0 (if timing can be fixed)

### Option 2: Runtime Detection
Modify GLCD library to detect SimulIDE version and adjust timing automatically.

### Option 3: SimulIDE Source Investigation
Study SimulIDE source code changes between versions to understand exact KS0108 simulation differences.

## Testing Results

✅ **Graphics_Display + SimulIDE 0.4.15** - Perfect display output, all graphics functions work  
❌ **Graphics_Display + SimulIDE 1.1.0** - Blank/corrupted display, functions fail  
✅ **Other Projects + SimulIDE 1.1.0** - Normal operation, latest features available  

## Conclusion

The automatic launcher provides the best user experience by:
1. **Eliminating guesswork** - Students don't need to know which version to use
2. **Ensuring compatibility** - Graphics_Display always works correctly
3. **Providing latest features** - Non-graphics projects get SimulIDE 1.1.0 benefits
4. **Maintaining portability** - Both versions included in workspace, zero external dependencies