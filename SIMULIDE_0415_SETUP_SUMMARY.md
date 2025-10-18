# SimulIDE Version Testing - Quick Summary

## ✅ Setup Complete

### New Files Created
1. **`tools/simulide/cli-simulide-0415.ps1`**
   - Launches SimulIDE 0.4.15-SR10 with Simulator0415.simu
   - Auto-builds project and updates circuit file
   - Prevents version conflicts

2. **`docs/simulide/SIMULIDE_VERSION_COMPARISON.md`**
   - Complete testing guide
   - Comparison procedures
   - Troubleshooting steps

### New VS Code Tasks
Access via: `Ctrl+Shift+P` → "Tasks: Run Task"

1. **"Simulate in SimulIDE 0.4.15 (Legacy)"**
   - Launch old version only (no build)
   
2. **"Build and Simulate 0415 (Legacy Test)"**
   - Build + Launch old version
   - **Recommended for testing**

## 🚀 Quick Start Testing

### Test in Old Version (0.4.15)
```powershell
# From Serial_Communications project directory
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1
```

### Test in New Version (1.1.0)
```powershell
# From Serial_Communications project directory
W:\soc3050code\tools\simulide\cli-simulide.ps1
```

## 📋 Testing Checklist

### Step 1: Test SimulIDE 0.4.15
- [ ] Close any running SimulIDE
- [ ] Run: `cli-simulide-0415.ps1`
- [ ] Click Play in SimulIDE
- [ ] Open SerialPort/SerialTerminal component
- [ ] **Check:** Do you see UART TX output? (startup messages, menu)
- [ ] **Check:** Does UART RX work? (type and echo)
- [ ] **Record results** in comparison table

### Step 2: Test SimulIDE 1.1.0
- [ ] Close SimulIDE 0.4.15
- [ ] Run: `cli-simulide.ps1`
- [ ] Click Play in SimulIDE
- [ ] Open SerialTerminal component
- [ ] **Compare:** UART TX behavior vs 0.4.15
- [ ] **Compare:** UART RX behavior vs 0.4.15

### Step 3: Document Findings
Update `SIMULIDE_VERSION_COMPARISON.md` with results:
- Does 0.4.15 UART TX work? Yes/No
- Does 1.1.0 UART TX work? **No (confirmed bug)**
- Which version should students use for serial projects?

## 🎯 Expected Results

### Scenario A: 0.4.15 UART TX Works ✅
**Conclusion:** SimulIDE 1.1.0 has a regression bug
**Action:**
- Recommend students use 0.4.15 for serial projects
- Update course documentation
- Report bug to SimulIDE developers

### Scenario B: 0.4.15 UART TX Also Fails ❌
**Conclusion:** Circuit configuration issue
**Action:**
- Compare Simulator0415.simu vs Simulator110.simu
- Check ATmega128 UART pin connections
- Review serial component settings

## 📁 File Locations

```
W:\soc3050code\
├── .vscode\
│   └── tasks.json                          (Updated: Added 0415 tasks)
├── docs\simulide\
│   ├── SIMULIDE_VERSION_COMPARISON.md      (NEW: Full testing guide)
│   └── SIMULIDE_KNOWN_ISSUES.md            (Existing: Bug documentation)
└── tools\simulide\
    ├── cli-simulide-0415.ps1               (NEW: 0.4.15 launcher)
    ├── cli-simulide.ps1                    (Existing: 1.1.0 launcher)
    ├── Simulator0415.simu                  (Circuit for 0.4.15)
    ├── Simulator110.simu                   (Circuit for 1.1.0)
    ├── SimulIDE_0.4.15-SR10_Win64\         (Old version binary)
    │   └── bin\simulide.exe
    └── SimulIDE_1.1.0-SR1_Win64\           (New version binary)
        └── bin\simulide.exe
```

## 🔧 Command Quick Reference

```powershell
# Launch 0.4.15 from anywhere
W:\soc3050code\tools\simulide\cli-simulide-0415.ps1 -ProjectDir "projects\Serial_Communications"

# Launch 1.1.0 from anywhere
W:\soc3050code\tools\simulide\cli-simulide.ps1 -ProjectDir "projects\Serial_Communications"

# Kill all SimulIDE instances
Stop-Process -Name simulide -Force -ErrorAction SilentlyContinue

# Check which version is running
Get-Process simulide | Select-Object Id, Path
```

## ⚠️ Important Notes

### Never Mix Versions
- Always close one SimulIDE before launching the other
- Scripts automatically prevent conflicts
- Configuration files may be shared

### Circuit Files Are Version-Specific
- `Simulator0415.simu` → Only for 0.4.15
- `Simulator110.simu` → Only for 1.1.0
- Scripts automatically select correct file

### Hardware Is Always Correct
- COM3 hardware works perfectly for both TX and RX
- Use hardware as reference/"ground truth"
- Simulator bugs don't affect hardware

## 📚 Full Documentation

For complete details, see:
- **Testing Guide:** `docs/simulide/SIMULIDE_VERSION_COMPARISON.md`
- **Known Issues:** `docs/simulide/SIMULIDE_KNOWN_ISSUES.md`
- **General Usage:** `docs/simulide/SIMULIDE_GUIDE.md`

---

**Status:** ✅ Ready for testing  
**Next Step:** Run both versions and compare UART behavior  
**Goal:** Determine if 0.4.15 can be used as workaround for 1.1.0 UART TX bug
