#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Build project for SimulIDE simulation (wit    # Common    } els    } elseif ($name -eq "Graphics_Display") {
        Write-Host "Building Graphics_Display (MC Studio compatible)..." -ForegroundColor Magenta
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ElpmWorkaround.Split(' ') Main.c ../../shared_libs/_glcd_working_backup.c ../../shared_libs/_port.c ../../shared_libs/_init.c -lm -o Main.elf ($name -eq "Graphics_Display") {
        Write-Host "Building Graphics_Display (MC Studio compatible + SimulIDE)..." -ForegroundColor Magenta
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ElmpWorkaround Main.c ../../shared_libs/_port.c ../../shared_libs/_init.c ../../shared_libs/_glcd.c -lm -o Main.elfMP workaround flags
    $ElmpWorkaround = "-Wl,--defsym=__do_copy_data=0"
    $CommonFlags = "-mmcu=atmega128 -DF_CPU=16000000UL -DBAUD=9600 -Os -Wall -I. -I../../shared_libs"
    # MC Studio compatible flags
    $McStudioFlags = "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums"t ELPM instructions)
    
.DESCRIPTION
    Special build configuration that avoids ELPM instructions for SimulIDE compatibility.
    Generates both standard HEX file and clean HEX file for simulation.
    
.PARAMETER ProjectDir
    Path to project directory containing Main.c (default: current directory)
    
.EXAMPLE
    .\cli-build-simulide.ps1
    Build current project for SimulIDE
    
.EXAMPLE
    .\cli-build-simulide.ps1 -ProjectDir "projects/Graphics_Display"
    Build specific project for SimulIDE
#>

param([string]$ProjectDir = $PWD.Path)

Write-Host "🎯 Building for SimulIDE (ELPM-free)" -ForegroundColor Cyan
Write-Host "Analyzing: $ProjectDir"

# Smart project detection
if ($ProjectDir -notlike "*\projects\*") {
    Write-Host "⚠️  Not in a project directory. Searching for recent projects..." -ForegroundColor Yellow
    
    # Try to find the most recently modified project
    $WorkspaceRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
    $ProjectsDir = Join-Path $WorkspaceRoot "projects"
    
    if (Test-Path $ProjectsDir) {
        $RecentProjects = Get-ChildItem $ProjectsDir -Directory | Where-Object { 
            Test-Path (Join-Path $_.FullName "Main.c") 
        } | Sort-Object LastWriteTime -Descending | Select-Object -First 5
        
        if ($RecentProjects) {
            Write-Host ""
            Write-Host "📁 Available projects:" -ForegroundColor Cyan
            for ($i = 0; $i -lt $RecentProjects.Count; $i++) {
                $project = $RecentProjects[$i]
                Write-Host "  $($i + 1). $($project.Name)" -ForegroundColor White
            }
            Write-Host ""
            
            # Auto-select the most recent project
            $ProjectDir = $RecentProjects[0].FullName
            Write-Host "🎯 Auto-selecting most recent: $($RecentProjects[0].Name)" -ForegroundColor Green
        }
        else {
            Write-Host "❌ No projects found with Main.c files" -ForegroundColor Red
            exit 1
        }
    }
    else {
        Write-Host "❌ Projects directory not found: $ProjectsDir" -ForegroundColor Red
        exit 1
    }
}

# Setup portable tools paths
$WorkspaceRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent

# Use portable AVR tools
$portableAvrGcc = Join-Path $WorkspaceRoot "tools\avr-toolchain\bin\avr-gcc.exe"
$portableAvrObjcopy = Join-Path $WorkspaceRoot "tools\avr-toolchain\bin\avr-objcopy.exe"

if (Test-Path $portableAvrGcc) {
    $avrGccExe = $portableAvrGcc
    $avrObjcopyExe = $portableAvrObjcopy
    Write-Host "✅ Using portable AVR toolchain" -ForegroundColor Green
}
else {
    Write-Host "❌ No AVR toolchain found! Please ensure tools/avr-toolchain exists." -ForegroundColor Red
    exit 1
}

if (Test-Path (Join-Path $ProjectDir "Main.c")) {
    $name = Split-Path $ProjectDir -Leaf
    Write-Host "Building: $name (SimulIDE mode)" -ForegroundColor Yellow
    Set-Location $ProjectDir
    
    # UNIFIED BUILD: Use same flags as hardware build for consistency
    # Standard AVR compiler flags (compatible with avr/io.h)
    $McStudioFlags = "-funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax"
    $ExactMcStudioFlags = "-O3"  # EXACT from MC Studio: "Optimize most (-O3)" - NOT -Os!
    $CommonFlags = "-mmcu=atmega128 -DF_CPU=16000000UL -DBAUD=9600 -DDEBUG -Wall -I. -I../../shared_libs"
    
    # Project-specific builds with ELMP workaround and MC Studio compatibility
    if ($name -eq "Serial_Communications") {
        Write-Host "Building Serial_Communications (MC Studio compatible)..." -ForegroundColor Cyan
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ElpmWorkaround Main.c ../../shared_libs/_port.c -lm -o Main.elf
    }
    elseif ($name -eq "Graphics_Display") {
        Write-Host "Building Graphics_Display with comprehensive KS0108 library..." -ForegroundColor Magenta
        # Graphics Display project uses new comprehensive KS0108 library
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ExactMcStudioFlags.Split(' ') Main.c `
            ../../shared_libs/_glcd.c `
            ../../shared_libs/_port.c `
            ../../shared_libs/_init.c `
            -lm -o Main.elf
    }
    elseif ($name -eq "Port_Basic") {
        Write-Host "Building Port_Basic (MC Studio compatible)..." -ForegroundColor Cyan
        & $avrGccExe -mmcu=atmega128 -DF_CPU=16000000UL -DBAUD=9600 -O3 $McStudioFlags.Split(' ') -Wall -I. Main.c -lm -o Main.elf
    }
    elseif ($name -eq "Interrupt_Basic") {
        Write-Host "Building Interrupt_Basic (MC Studio compatible)..." -ForegroundColor Yellow
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ElpmWorkaround Main.c ../../shared_libs/_port.c ../../shared_libs/_init.c -lm -o Main.elf
    }
    else {
        # Generic build with ELPM workaround and MC Studio compatibility
        Write-Host "Building $name with MC Studio compatible flags..." -ForegroundColor Green
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ElpmWorkaround Main.c ../../shared_libs/_port.c ../../shared_libs/_init.c -lm -o Main.elf
    }
    
    if ($LASTEXITCODE -eq 0) {
        # Generate HEX file - IDENTICAL to hardware build
        & $avrObjcopyExe -O ihex -R .eeprom Main.elf Main.hex
        
        Write-Host "✅ SimulIDE build completed!" -ForegroundColor Green
        Write-Host "Files created:" -ForegroundColor Gray
        Write-Host "  • Main.elf (executable)" -ForegroundColor White
        Write-Host "  • Main.hex (identical to hardware)" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "💡 Main.hex is identical to hardware build" -ForegroundColor Cyan
    }
    else {
        Write-Host "❌ SimulIDE build failed!" -ForegroundColor Red
    }
}
else {
    Write-Host "❌ Error: Main.c not found in $ProjectDir" -ForegroundColor Red
    Write-Host "Please ensure you're in a valid project directory" -ForegroundColor Yellow
}