# SimulIDE 0.4.15 Testing Infrastructure - Implementation Summary

## Overview
Created parallel infrastructure to test if ATmega128 UART TX bug exists in SimulIDE 0.4.15-SR10 (legacy version) compared to SimulIDE 1.1.0-SR1 (current version with confirmed bug).

## Implementation Date
**October 18, 2025**

## Files Created/Modified

### 1. New Script: `tools/simulide/cli-simulide-0415.ps1`
**Purpose:** Launch SimulIDE 0.4.15-SR10 with legacy circuit file
**Features:**
- Auto-detects SimulIDE 0.4.15-SR10 installation
- Uses Simulator0415.simu circuit file
- Auto-updates circuit with project HEX file
- Prevents version conflicts (detects running SimulIDE)
- Identical workflow to cli-simulide.ps1
- Provides testing guidance in console output

**Key Differences from cli-simulide.ps1:**
- Only searches for 0.4.15 version paths
- Uses Simulator0415.simu instead of Simulator110.simu
- Enhanced warning about version conflicts
- Testing-focused console messages

### 2. New Documentation: `docs/simulide/SIMULIDE_VERSION_COMPARISON.md`
**Purpose:** Complete testing guide for comparing SimulIDE versions
**Contents:**
- Quick start instructions for both versions
- Step-by-step testing procedure
- Comparison table template
- Troubleshooting guide
- Command reference
- Expected outcomes for different scenarios

### 3. New Summary: `SIMULIDE_0415_SETUP_SUMMARY.md`
**Purpose:** Quick reference at workspace root
**Contents:**
- Setup status confirmation
- Quick testing commands
- Testing checklist
- Expected results scenarios
- File location reference
- Important notes

### 4. Modified: `.vscode/tasks.json`
**Added Tasks:**
1. **"Simulate in SimulIDE 0.4.15 (Legacy)"**
   ```json
   {
       "label": "Simulate in SimulIDE 0.4.15 (Legacy)",
       "command": "cli-simulide-0415.ps1 -ProjectDir ${fileDirname}"
   }
   ```

2. **"Build and Simulate 0415 (Legacy Test)"**
   ```json
   {
       "label": "Build and Simulate 0415 (Legacy Test)",
       "dependsOn": [
           "Build Current Project",
           "Simulate in SimulIDE 0.4.15 (Legacy)"
       ]
   }
   ```

### 5. Updated: `docs/simulide/SIMULIDE_KNOWN_ISSUES.md`
**Changes:**
- Added reference to 0.4.15 testing infrastructure
- Updated workaround section with testing option
- Added links to new documentation

## Directory Structure

```
W:\soc3050code\
├── SIMULIDE_0415_SETUP_SUMMARY.md          (NEW - Quick reference)
├── .vscode\
│   └── tasks.json                          (MODIFIED - Added 2 new tasks)
├── docs\simulide\
│   ├── SIMULIDE_VERSION_COMPARISON.md      (NEW - Complete testing guide)
│   └── SIMULIDE_KNOWN_ISSUES.md            (UPDATED - Added 0415 reference)
└── tools\simulide\
    ├── cli-simulide-0415.ps1               (NEW - 0.4.15 launcher)
    ├── cli-simulide.ps1                    (UNCHANGED - 1.1.0 launcher)
    ├── Simulator0415.simu                  (EXISTING - 0.4.15 circuit)
    ├── Simulator0415_backup.simu           (AUTO-CREATED - Backup)
    ├── Simulator110.simu                   (EXISTING - 1.1.0 circuit)
    ├── SimulIDE_0.4.15-SR10_Win64\         (EXISTING - Old version)
    │   └── bin\simulide.exe
    └── SimulIDE_1.1.0-SR1_Win64\           (EXISTING - New version)
        └── bin\simulide.exe
```

## Usage Examples

### Via VS Code Tasks
```
1. Open project file (e.g., Serial_Communications/Main.c)
2. Press Ctrl+Shift+P
3. Type "Tasks: Run Task"
4. Select "Build and Simulate 0415 (Legacy Test)"
```

### Via PowerShell
```powershell
# From project directory
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1

# From anywhere with explicit project path
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1 -ProjectDir "projects\Serial_Communications"

# Skip building (faster testing)
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1 -BuildFirst $false
```

## Testing Workflow

### Phase 1: Test Legacy Version
```powershell
# 1. Close any running SimulIDE
Stop-Process -Name simulide -Force -ErrorAction SilentlyContinue

# 2. Launch 0.4.15
cd W:\soc3050code\projects\Serial_Communications
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1

# 3. In SimulIDE 0.4.15:
#    - Click Play
#    - Open SerialPort/SerialTerm
#    - Observe UART TX output
#    - Test UART RX input
```

### Phase 2: Test Current Version
```powershell
# 1. Close SimulIDE 0.4.15
Stop-Process -Name simulide -Force -ErrorAction SilentlyContinue

# 2. Launch 1.1.0
W:\soc3050code\tools\simulide\cli-simulide.ps1

# 3. In SimulIDE 1.1.0:
#    - Click Play
#    - Open SerialTerminal
#    - Compare behavior with 0.4.15
```

### Phase 3: Document Results
Update `SIMULIDE_VERSION_COMPARISON.md` with findings:
- Does 0.4.15 UART TX work?
- Is there a regression in 1.1.0?
- Which version should be recommended?

## Script Features

### Automatic Detection
- ✅ Finds SimulIDE 0.4.15-SR10 in workspace
- ✅ Locates Simulator0415.simu circuit file
- ✅ Detects project directory and Main.hex
- ✅ Auto-selects most recent project if not in project dir

### Safety Features
- ✅ Prevents running multiple SimulIDE versions simultaneously
- ✅ Creates backup of circuit file before modification
- ✅ Validates all paths before launching
- ✅ Provides clear error messages with solutions

### User Guidance
- ✅ Color-coded console output
- ✅ Testing instructions in launch message
- ✅ Comparison guidance (how to test 1.1.0 next)
- ✅ Circuit file path displayed

## Testing Goals

### Primary Goal
Determine if ATmega128 UART TX functionality works in SimulIDE 0.4.15-SR10

### Success Criteria
**If 0.4.15 UART TX works:**
- ✅ Confirms 1.1.0 has regression bug
- ✅ Provides workaround for students
- ✅ Justifies recommending 0.4.15 for serial projects

**If 0.4.15 UART TX also fails:**
- ⚠️ Suggests circuit configuration issue
- ⚠️ Requires deeper investigation
- ⚠️ May indicate long-standing ATmega128 simulation gap

## Known Constraints

### Version Isolation
- Only ONE SimulIDE version should run at a time
- Script automatically detects and prevents conflicts
- Shared configuration files may cause issues

### Circuit File Separation
- Simulator0415.simu is ONLY for 0.4.15
- Simulator110.simu is ONLY for 1.1.0
- DO NOT manually swap files between versions

### Hardware Reference
- Hardware (COM3) always works correctly
- Use hardware as "ground truth" for testing
- Simulator results must be compared to hardware behavior

## Related Issues

### Original Refactoring Request
**User Request:** "I renamed Simulator.simu as Simulator110.simu. Refactor related scripts."
- ✅ Completed: All references updated across 46+ locations
- ✅ Verified: Hardware testing confirms code correctness
- ✅ Discovered: SimulIDE 1.1.0 UART TX bug during verification

### UART TX Bug Discovery
**Issue Found:** ATmega128 UART transmission not working in SimulIDE 1.1.0-SR1
- ✅ Documented: SIMULIDE_KNOWN_ISSUES.md
- ✅ Tested: Multiple configurations (SerialPort, SerialTerminal)
- ✅ Confirmed: Hardware works, simulator fails
- 🔬 Now testing: Legacy version for comparison

## Next Steps

1. **Run Comparison Test**
   - Test Serial_Communications project in both versions
   - Document UART TX/RX behavior differences
   - Update comparison table

2. **Update Documentation**
   - Fill in SIMULIDE_VERSION_COMPARISON.md results
   - Update SIMULIDE_KNOWN_ISSUES.md with findings
   - Add recommendation to course materials

3. **Student Guidance** (based on results)
   - If 0.4.15 works: Provide dual-version setup instructions
   - If 0.4.15 fails: Emphasize hardware-only testing
   - Update lab guides with appropriate warnings

## Technical Notes

### PowerShell Script Compatibility
- Uses PowerShell 5.1+ compatible syntax
- Works in both pwsh.exe and powershell.exe
- No external dependencies required
- Path handling uses System.IO for reliability

### Circuit File Format
- Both versions use XML-based .simu format
- Program path uses forward slashes: `/`
- Auto_Load attribute must be "true" for auto-loading
- MCU itemtype="MCU" required for firmware loading

### Build Integration
- Reuses existing cli-build-project.ps1
- Automatically finds Main.hex after build
- No changes needed to build infrastructure
- Works with existing AVR toolchain

## Verification

### Script Tested Successfully
```powershell
# Test run performed: 2025-10-18
cd W:\soc3050code\projects\Serial_Communications
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1 -BuildFirst $false

# Results:
# ✅ SimulIDE 0.4.15 found correctly
# ✅ Simulator0415.simu located
# ✅ Circuit updated with HEX path
# ✅ Backup created automatically
# ✅ SimulIDE launched successfully (PID: 5552)
# ✅ Version conflict detection working
```

### VS Code Tasks Verified
- ✅ Tasks appear in Task Runner
- ✅ Command paths correct
- ✅ Dependency order correct

## Documentation Cross-References

| Document | Purpose | Audience |
|----------|---------|----------|
| `SIMULIDE_0415_SETUP_SUMMARY.md` | Quick start | All users |
| `SIMULIDE_VERSION_COMPARISON.md` | Testing guide | Testers |
| `SIMULIDE_KNOWN_ISSUES.md` | Bug documentation | All users |
| `SIMULIDE_GUIDE.md` | General usage | Students |
| `cli-simulide-0415.ps1` | Script source | Developers |

## Success Metrics

- ✅ Infrastructure complete and functional
- ✅ Scripts tested and working
- ✅ Documentation comprehensive
- ✅ VS Code integration seamless
- ⏳ Testing results: Pending
- ⏳ Recommendation: Pending testing results

---

**Status:** ✅ **IMPLEMENTATION COMPLETE**  
**Next Action:** Run comparison test between SimulIDE versions  
**Created:** October 18, 2025  
**Author:** GitHub Copilot (requested by user)
