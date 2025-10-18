# SimulIDE Testing Infrastructure - Visual Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                    SimulIDE Version Comparison                      │
│                         Testing Setup                                │
└─────────────────────────────────────────────────────────────────────┘

┌────────────────────┐                    ┌────────────────────┐
│  SimulIDE 0.4.15   │                    │  SimulIDE 1.1.0    │
│   (Legacy Test)    │                    │  (Current/Bug)     │
└────────────────────┘                    └────────────────────┘
         │                                          │
         │                                          │
         ▼                                          ▼
┌─────────────────────┐              ┌─────────────────────┐
│ cli-simulide-0415   │              │ cli-simulide.ps1    │
│      .ps1           │              │   (Existing)        │
│   (NEW SCRIPT)      │              │                     │
└─────────────────────┘              └─────────────────────┘
         │                                          │
         │                                          │
         ▼                                          ▼
┌─────────────────────┐              ┌─────────────────────┐
│ Simulator0415.simu  │              │ Simulator110.simu   │
│  (Legacy Circuit)   │              │ (Current Circuit)   │
└─────────────────────┘              └─────────────────────┘
         │                                          │
         └──────────────────┬───────────────────────┘
                            │
                            ▼
                   ┌──────────────┐
                   │  Main.hex    │
                   │  (Project)   │
                   └──────────────┘
```

## Quick Command Reference

```powershell
┌────────────────────────────────────────────────────────────┐
│  Test OLD Version (0.4.15)                                 │
├────────────────────────────────────────────────────────────┤
│  W:\soc3050code\tools\simulide\cli-simulide-0415.ps1      │
└────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────┐
│  Test NEW Version (1.1.0)                                  │
├────────────────────────────────────────────────────────────┤
│  W:\soc3050code\tools\simulide\cli-simulide.ps1           │
└────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────┐
│  Stop All SimulIDE                                         │
├────────────────────────────────────────────────────────────┤
│  Stop-Process -Name simulide -Force                        │
└────────────────────────────────────────────────────────────┘
```

## VS Code Task Menu

```
Ctrl+Shift+P → "Tasks: Run Task"

┌─────────────────────────────────────────────────────┐
│  BUILD TASKS                                        │
├─────────────────────────────────────────────────────┤
│  ⚙️  Build Current Project                          │
│  📦 Build and Program Current Project               │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│  SIMULATION TASKS                                   │
├─────────────────────────────────────────────────────┤
│  🎮 Build and Simulate Current Project (1.1.0)      │
│  🔬 Build and Simulate 0415 (Legacy Test)   ⭐NEW   │
│  🚀 Simulate in SimulIDE (Current) (1.1.0)          │
│  🕰️  Simulate in SimulIDE 0.4.15 (Legacy)   ⭐NEW   │
└─────────────────────────────────────────────────────┘
```

## Testing Workflow

```
┌──────────────────────────────────────────────────────────────┐
│  Step 1: Test in SimulIDE 0.4.15                            │
└──────────────────────────────────────────────────────────────┘
   │
   ├─► Close any running SimulIDE
   │
   ├─► Run: cli-simulide-0415.ps1
   │
   ├─► In SimulIDE:
   │   • Click Play ▶️
   │   • Open SerialPort/SerialTerm 📟
   │   • Check UART TX output 📤
   │   • Test UART RX input 📥
   │
   └─► Record Results ✍️

┌──────────────────────────────────────────────────────────────┐
│  Step 2: Test in SimulIDE 1.1.0                             │
└──────────────────────────────────────────────────────────────┘
   │
   ├─► Close SimulIDE 0.4.15
   │
   ├─► Run: cli-simulide.ps1
   │
   ├─► In SimulIDE:
   │   • Click Play ▶️
   │   • Open SerialTerminal 📟
   │   • Compare UART TX ⚠️ (Known bug)
   │   • Compare UART RX ✅ (Working)
   │
   └─► Record Results ✍️

┌──────────────────────────────────────────────────────────────┐
│  Step 3: Document & Decide                                   │
└──────────────────────────────────────────────────────────────┘
   │
   ├─► Update: SIMULIDE_VERSION_COMPARISON.md
   │
   ├─► Fill comparison table:
   │   ┌────────────────┬────────┬────────┐
   │   │    Feature     │ 0.4.15 │ 1.1.0  │
   │   ├────────────────┼────────┼────────┤
   │   │ UART TX        │   ?    │   ❌   │
   │   │ UART RX        │   ?    │   ✅   │
   │   │ LEDs/GPIO      │   ?    │   ✅   │
   │   └────────────────┴────────┴────────┘
   │
   └─► Decide Recommendation:
       • If 0.4.15 works → Use 0.4.15 for serial
       • If 0.4.15 fails → Hardware only
```

## File Structure Map

```
W:\soc3050code\
│
├─📄 SIMULIDE_0415_SETUP_SUMMARY.md          ⭐ Quick Reference
├─📄 IMPLEMENTATION_SIMULIDE_0415_TESTING.md ⭐ Full Details
│
├─📁 .vscode\
│   └─📄 tasks.json                          ✏️ Modified (2 new tasks)
│
├─📁 docs\simulide\
│   ├─📄 SIMULIDE_VERSION_COMPARISON.md      ⭐ Testing Guide (NEW)
│   ├─📄 SIMULIDE_KNOWN_ISSUES.md            ✏️ Updated (Added 0415 ref)
│   └─📄 SIMULIDE_GUIDE.md                   📖 General usage
│
├─📁 projects\
│   └─📁 Serial_Communications\              🎯 Test Project
│       ├─📄 Main.c
│       └─📄 Main.hex                        🎯 Target for both versions
│
└─📁 tools\simulide\
    ├─📄 cli-simulide-0415.ps1               ⭐ NEW SCRIPT (0.4.15)
    ├─📄 cli-simulide.ps1                    📜 Existing (1.1.0)
    │
    ├─📄 Simulator0415.simu                  🔵 Circuit for 0.4.15
    ├─📄 Simulator0415_backup.simu           💾 Auto-created backup
    ├─📄 Simulator110.simu                   🔵 Circuit for 1.1.0
    │
    ├─📁 SimulIDE_0.4.15-SR10_Win64\
    │   └─📁 bin\
    │       └─📄 simulide.exe                🕰️ Old Version
    │
    └─📁 SimulIDE_1.1.0-SR1_Win64\
        └─📁 bin\
            └─📄 simulide.exe                🆕 Current Version
```

## Expected Testing Results

```
┌──────────────────────────────────────────────────────────────┐
│  Scenario A: 0.4.15 UART TX Works ✅                        │
└──────────────────────────────────────────────────────────────┘
   Conclusion: SimulIDE 1.1.0 has REGRESSION BUG
   
   Action Plan:
   ✅ Use 0.4.15 for serial communication projects
   ✅ Update course materials to recommend 0.4.15
   ✅ Report bug to SimulIDE developers (GitHub issue)
   ✅ Document workaround in student guides

┌──────────────────────────────────────────────────────────────┐
│  Scenario B: 0.4.15 UART TX Also Fails ❌                   │
└──────────────────────────────────────────────────────────────┘
   Conclusion: Long-standing ATmega128 UART simulation issue
   
   Action Plan:
   ⚠️ Compare circuit files (Simulator0415 vs Simulator110)
   ⚠️ Check UART pin connections and settings
   ⚠️ Review serial component configurations
   ✅ Emphasize HARDWARE testing (COM3 always works)
   ✅ Update course: "SimulIDE serial = visual only"
```

## Known Good vs Known Bad

```
┌─────────────────────────────────────────────────────────────┐
│  ✅ KNOWN WORKING                                           │
└─────────────────────────────────────────────────────────────┘
   • Hardware (COM3): UART TX + RX perfect
   • SimulIDE 1.1.0: UART RX working
   • SimulIDE 1.1.0: Digital I/O (LEDs, buttons)
   • SimulIDE 1.1.0: Program execution
   • Both versions: Circuit loading

┌─────────────────────────────────────────────────────────────┐
│  ❌ KNOWN BROKEN                                            │
└─────────────────────────────────────────────────────────────┘
   • SimulIDE 1.1.0: ATmega128 UART TX
   • SimulIDE 1.1.0: External serial channels (Issue #59)

┌─────────────────────────────────────────────────────────────┐
│  ❓ TO BE TESTED                                            │
└─────────────────────────────────────────────────────────────┘
   • SimulIDE 0.4.15: ATmega128 UART TX ← PRIMARY GOAL
   • SimulIDE 0.4.15: ATmega128 UART RX
   • SimulIDE 0.4.15: Overall stability for serial
```

## Safety Checklist

```
Before Running Tests:
┌─────────────────────────────────────────────────────────────┐
│  ⚠️ IMPORTANT SAFETY CHECKS                                 │
└─────────────────────────────────────────────────────────────┘
   ☐ Close ALL SimulIDE instances before switching versions
   ☐ Do NOT run 0.4.15 and 1.1.0 simultaneously
   ☐ Verify correct circuit file for each version
   ☐ Have backup of working code on hardware
   ☐ Document each test result immediately
   ☐ Take screenshots if UART TX works in 0.4.15!

During Testing:
┌─────────────────────────────────────────────────────────────┐
│  📋 TESTING PROTOCOL                                         │
└─────────────────────────────────────────────────────────────┘
   ☐ Use same project (Serial_Communications) for both
   ☐ Same HEX file tested in both versions
   ☐ Test UART TX first (primary concern)
   ☐ Test UART RX second (verify functionality)
   ☐ Check LED behavior (verify code execution)
   ☐ Try typing in serial terminal
   ☐ Look for startup messages from MCU

After Testing:
┌─────────────────────────────────────────────────────────────┐
│  📝 DOCUMENTATION REQUIRED                                   │
└─────────────────────────────────────────────────────────────┘
   ☐ Update SIMULIDE_VERSION_COMPARISON.md
   ☐ Fill in comparison table completely
   ☐ Add screenshots/evidence if helpful
   ☐ Write recommendation based on results
   ☐ Update SIMULIDE_KNOWN_ISSUES.md if needed
```

## Quick Troubleshooting

```
❌ "SimulIDE 0.4.15 executable not found"
   → Check: tools\simulide\SimulIDE_0.4.15-SR10_Win64\bin\simulide.exe
   → Solution: Extract ZIP if missing

❌ "Simulator0415.simu not found"
   → Check: tools\simulide\Simulator0415.simu
   → Solution: Copy from Simulator110.simu if needed

❌ "SimulIDE is already running"
   → Run: Stop-Process -Name simulide -Force
   → Wait 1 second, try again

❌ "No serial output in either version"
   → Test hardware first (COM3) to verify code
   → Check MCU is running (LEDs should blink)
   → Verify UART pins connected (PD2/PD3)
```

---

**Status:** ✅ Infrastructure Complete  
**Ready:** Yes - Ready for testing  
**Next:** Run comparison test and document results

