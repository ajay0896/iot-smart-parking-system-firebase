# Script untuk reorganisasi project structure
# Run: .\reorganize-project.ps1

Write-Host "📁 Project Reorganization Script" -ForegroundColor Cyan
Write-Host "=================================" -ForegroundColor Cyan
Write-Host ""

# Define file movements
$movements = @(
    # Arduino files
    @{ From = "ESP32_GATE_MASUK.ino"; To = "arduino/ESP32_GATE_MASUK/ESP32_GATE_MASUK.ino" },
    @{ From = "ESP32_GATE_KELUAR.ino"; To = "arduino/ESP32_GATE_KELUAR/ESP32_GATE_KELUAR.ino" },
    @{ From = "ESP32_HARDWARE_TEST.ino"; To = "arduino/ESP32_HARDWARE_TEST/ESP32_HARDWARE_TEST.ino" },
    
    # Documentation files
    @{ From = "ARCHITECTURE.md"; To = "docs/ARCHITECTURE.md" },
    @{ From = "FIREBASE_SETUP.md"; To = "docs/FIREBASE_SETUP.md" },
    @{ From = "ESP32_GUIDE.md"; To = "docs/ESP32_GUIDE.md" },
    @{ From = "QUICK_START.md"; To = "docs/QUICK_START.md" },
    @{ From = "QUICK_START_GUIDE.md"; To = "docs/QUICK_START_GUIDE.md" },
    @{ From = "TROUBLESHOOTING.md"; To = "docs/TROUBLESHOOTING.md" },
    @{ From = "TROUBLESHOOTING_AUTH.md"; To = "docs/TROUBLESHOOTING_AUTH.md" },
    @{ From = "PANDUAN_TESTING.md"; To = "docs/PANDUAN_TESTING.md" },
    @{ From = "PANDUAN_UPDATE_SOLUSI2.md"; To = "docs/PANDUAN_UPDATE_SOLUSI2.md" },
    @{ From = "USER_LOGIN_GUIDE.md"; To = "docs/USER_LOGIN_GUIDE.md" },
    @{ From = "ADMIN_GUIDE.md"; To = "docs/ADMIN_GUIDE.md" },
    @{ From = "BEGINNER_GUIDE.md"; To = "docs/BEGINNER_GUIDE.md" },
    @{ From = "CHECKLIST_PROJECT.md"; To = "docs/CHECKLIST_PROJECT.md" },
    @{ From = "FIREBASE_AUTH_SETUP.md"; To = "docs/FIREBASE_AUTH_SETUP.md" },
    @{ From = "FIX_WEBSITE_MODE.md"; To = "docs/FIX_WEBSITE_MODE.md" },
    
    # Firebase files
    @{ From = "firebase_rules_update.json"; To = "firebase/database-rules.json" },
    @{ From = "firebase_update_users_structure.json"; To = "firebase/database-structure.json" },
    @{ From = "database_dummy.json"; To = "firebase/database-dummy.json" }
)

# Create directories
$directories = @("arduino/ESP32_GATE_MASUK", "arduino/ESP32_GATE_KELUAR", "arduino/ESP32_HARDWARE_TEST", "docs", "firebase", "public/css", "public/js")

Write-Host "📂 Creating directories..." -ForegroundColor Yellow
foreach ($dir in $directories) {
    if (-Not (Test-Path $dir)) {
        New-Item -Path $dir -ItemType Directory -Force | Out-Null
        Write-Host "  ✓ Created: $dir" -ForegroundColor Green
    }
}
Write-Host ""

# Move files
Write-Host "📦 Moving files..." -ForegroundColor Yellow
$moved = 0
$skipped = 0

foreach ($movement in $movements) {
    if (Test-Path $movement.From) {
        # Create parent directory if not exists
        $parentDir = Split-Path $movement.To -Parent
        if (-Not (Test-Path $parentDir)) {
            New-Item -Path $parentDir -ItemType Directory -Force | Out-Null
        }
        
        # Move file
        Move-Item -Path $movement.From -Destination $movement.To -Force
        Write-Host "  ✓ Moved: $($movement.From) → $($movement.To)" -ForegroundColor Green
        $moved++
    } else {
        Write-Host "  ⊘ Skipped (not found): $($movement.From)" -ForegroundColor Gray
        $skipped++
    }
}

Write-Host ""
Write-Host "=================================" -ForegroundColor Cyan
Write-Host "✅ Reorganization complete!" -ForegroundColor Green
Write-Host "   Files moved: $moved" -ForegroundColor Cyan
Write-Host "   Files skipped: $skipped" -ForegroundColor Yellow
Write-Host ""

# Clean up old documentation files
Write-Host "🧹 Cleaning up old files..." -ForegroundColor Yellow
$oldFiles = @("CHANGES.md", "CHECKLIST.md", "FINAL_MANIFEST.md", "INDEX.md", "SOLUTION.md", "START_HERE.md", "SUMMARY_PERBAIKAN.md", "UPDATE_SUMMARY.md", "QUICK_REFERENCE.md", "index-old.html")

foreach ($file in $oldFiles) {
    if (Test-Path $file) {
        Remove-Item $file -Force
        Write-Host "  ✓ Removed: $file" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "🎉 Project structure is now organized!" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "1. Review the new structure" -ForegroundColor White
Write-Host "2. Test the website: cd public && python -m http.server 8000" -ForegroundColor White
Write-Host "3. Run: .\push-to-github.ps1 to push to GitHub" -ForegroundColor White
