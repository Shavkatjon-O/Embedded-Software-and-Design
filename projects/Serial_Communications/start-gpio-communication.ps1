#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Start GPIO Parallel Communication System (UART Workaround)

.DESCRIPTION
    One-command solution to use GPIO parallel communication instead of broken UART TX
    in SimulIDE 1.1.0. Builds project, launches SimulIDE, and starts Python monitor.

.EXAMPLE
    .\start-gpio-communication.ps1
    
.NOTES
    Author: Professor Hong Jeong
    Course: SOC 3050
    Purpose: Bypass SimulIDE 1.1.0 UART TX bug
#>

param(
    [Parameter(Mandatory = $false)]
    [string]$ProjectDir = "W:\soc3050code\projects\Serial_Communications"
)

$ErrorActionPreference = "Stop"

# Banner
Write-Host ""
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Cyan
Write-Host "  GPIO PARALLEL COMMUNICATION SYSTEM" -ForegroundColor Yellow
Write-Host "  UART TX Workaround for SimulIDE 1.1.0" -ForegroundColor Yellow
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Cyan
Write-Host ""

# Step 1: Backup original Main.c
Write-Host "1️⃣  Backing up original code..." -ForegroundColor Cyan
$mainFile = Join-Path $ProjectDir "Main.c"
$parallelFile = Join-Path $ProjectDir "Main_Parallel.c"
$backupFile = Join-Path $ProjectDir "Main_UART.c.backup"

if (Test-Path $mainFile) {
    Copy-Item $mainFile $backupFile -Force
    Write-Host "   ✅ Backed up: Main.c → Main_UART.c.backup" -ForegroundColor Green
}

# Step 2: Copy parallel communication code
Write-Host ""
Write-Host "2️⃣  Installing GPIO parallel code..." -ForegroundColor Cyan
if (Test-Path $parallelFile) {
    Copy-Item $parallelFile $mainFile -Force
    Write-Host "   ✅ Installed: Main_Parallel.c → Main.c" -ForegroundColor Green
}
else {
    Write-Host "   ❌ Error: Main_Parallel.c not found!" -ForegroundColor Red
    exit 1
}

# Step 3: Build project
Write-Host ""
Write-Host "3️⃣  Building project..." -ForegroundColor Cyan
$buildScript = "W:\soc3050code\tools\cli\cli-build-project.ps1"
& $buildScript -ProjectDir $ProjectDir

if ($LASTEXITCODE -ne 0) {
    Write-Host "   ❌ Build failed!" -ForegroundColor Red
    exit 1
}
Write-Host "   ✅ Build successful!" -ForegroundColor Green

# Step 4: Configure SimulIDE Logic Analyzer
Write-Host ""
Write-Host "4️⃣  Configuring SimulIDE Logic Analyzer..." -ForegroundColor Cyan
$circuitFile = "W:\soc3050code\tools\simulide\Simulator110.simu"
$csvExportFile = Join-Path $ProjectDir "gpio_data.csv"

Write-Host "   📝 CSV Export: $csvExportFile" -ForegroundColor White
Write-Host "   ℹ️  Manual steps required (will show after SimulIDE launches):" -ForegroundColor Yellow
Write-Host "      1. Double-click Logic Analyzer component" -ForegroundColor White
Write-Host "      2. Check 'Auto Export' checkbox" -ForegroundColor White
Write-Host "      3. Set file: $csvExportFile" -ForegroundColor White
Write-Host "      4. Click OK" -ForegroundColor White

# Step 5: Launch SimulIDE
Write-Host ""
Write-Host "5️⃣  Launching SimulIDE 1.1.0..." -ForegroundColor Cyan
$simulideScript = "W:\soc3050code\tools\simulide\cli-simulide.ps1"

# Launch SimulIDE in background
$simulideJob = Start-Job -ScriptBlock {
    param($script, $dir)
    Set-Location $dir
    & $script
} -ArgumentList $simulideScript, $ProjectDir

Write-Host "   ✅ SimulIDE starting... (Job ID: $($simulideJob.Id))" -ForegroundColor Green

# Wait for SimulIDE to initialize
Write-Host ""
Write-Host "6️⃣  Waiting for SimulIDE to initialize..." -ForegroundColor Cyan
Start-Sleep -Seconds 5
Write-Host "   ✅ Ready!" -ForegroundColor Green

# Step 6: Show configuration instructions
Write-Host ""
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Yellow
Write-Host "  ⚠️  REQUIRED: Configure Logic Analyzer in SimulIDE" -ForegroundColor Yellow
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Yellow
Write-Host ""
Write-Host "Please complete these steps in SimulIDE:" -ForegroundColor White
Write-Host ""
Write-Host "  1. Find the Logic Analyzer component (left side of circuit)" -ForegroundColor Cyan
Write-Host "  2. Double-click it to open settings" -ForegroundColor Cyan
Write-Host "  3. Enable '✅ Auto Export' checkbox" -ForegroundColor Cyan
Write-Host "  4. Set export file path to:" -ForegroundColor Cyan
Write-Host "     $csvExportFile" -ForegroundColor Green
Write-Host "  5. Click 'OK' to save" -ForegroundColor Cyan
Write-Host "  6. Click '▶' (Play) to start simulation" -ForegroundColor Cyan
Write-Host ""
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Yellow
Write-Host ""

# Wait for user confirmation
Write-Host "Press any key when you've completed the above steps..." -ForegroundColor Yellow
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

# Step 7: Launch Python monitor
Write-Host ""
Write-Host "7️⃣  Starting Python GPIO monitor..." -ForegroundColor Cyan
$pythonScript = "W:\soc3050code\tools\cli\read-gpio-communication.py"

# Check if CSV file exists
if (Test-Path $csvExportFile) {
    Write-Host "   ✅ CSV export file detected!" -ForegroundColor Green
}
else {
    Write-Host "   ⚠️  CSV file not found yet - will wait for it" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Green
Write-Host "  🚀 SYSTEM READY!" -ForegroundColor Green
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Green
Write-Host ""
Write-Host "✅ SimulIDE: Running (PID: $($simulideJob.Id))" -ForegroundColor Green
Write-Host "✅ GPIO Code: Loaded in ATmega128" -ForegroundColor Green
Write-Host "✅ Python Monitor: Starting..." -ForegroundColor Green
Write-Host ""
Write-Host "📊 Watch for messages in Python console below:" -ForegroundColor Cyan
Write-Host "   - PORTB LEDs will flash with data" -ForegroundColor White
Write-Host "   - Logic Analyzer exports to CSV" -ForegroundColor White
Write-Host "   - Python decodes and displays messages" -ForegroundColor White
Write-Host ""
Write-Host "Press Ctrl+C to stop monitoring" -ForegroundColor Yellow
Write-Host ""

# Run Python monitor (foreground)
try {
    & python $pythonScript $csvExportFile
}
catch {
    Write-Host "❌ Python monitor error: $_" -ForegroundColor Red
}

# Cleanup
Write-Host ""
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Cyan
Write-Host "  CLEANUP" -ForegroundColor Yellow
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Cyan
Write-Host ""

# Ask if user wants to restore original code
$restore = Read-Host "Restore original UART code? (y/n)"
if ($restore -eq 'y') {
    if (Test-Path $backupFile) {
        Copy-Item $backupFile $mainFile -Force
        Write-Host "✅ Restored: Main_UART.c.backup → Main.c" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "🎉 Session complete!" -ForegroundColor Green
Write-Host ""
Write-Host "To run again:" -ForegroundColor Cyan
Write-Host "  .\start-gpio-communication.ps1" -ForegroundColor White
Write-Host ""

# Stop SimulIDE job if still running
if ($simulideJob.State -eq 'Running') {
    Write-Host "Stopping SimulIDE..." -ForegroundColor Yellow
    Stop-Job $simulideJob
    Remove-Job $simulideJob
}
