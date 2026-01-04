# Script untuk push project ke GitHub
# Run: .\push-to-github.ps1

Write-Host "🚀 IoT Smart Parking System - GitHub Push Script" -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host ""

# Set repository URL
$repo = "https://github.com/ajay0896/iot-smart-parking-system-firebase.git"

# Check if git is initialized
if (-Not (Test-Path ".git")) {
    Write-Host "📦 Initializing Git repository..." -ForegroundColor Yellow
    git init
    Write-Host "✓ Git initialized" -ForegroundColor Green
    Write-Host ""
}

# Add remote
Write-Host "🔗 Setting up remote repository..." -ForegroundColor Yellow
git remote remove origin 2>$null
git remote add origin $repo
Write-Host "✓ Remote added: $repo" -ForegroundColor Green
Write-Host ""

# Stage all files
Write-Host "📝 Staging files..." -ForegroundColor Yellow
git add .
Write-Host "✓ Files staged" -ForegroundColor Green
Write-Host ""

# Show status
Write-Host "📊 Git Status:" -ForegroundColor Yellow
git status --short
Write-Host ""

# Commit
$commitMessage = Read-Host "Enter commit message (default: 'Initial commit - IoT Smart Parking System')"
if ([string]::IsNullOrWhiteSpace($commitMessage)) {
    $commitMessage = "Initial commit - IoT Smart Parking System"
}

Write-Host "💾 Creating commit..." -ForegroundColor Yellow
git commit -m "$commitMessage"
Write-Host "✓ Commit created" -ForegroundColor Green
Write-Host ""

# Set branch to main
Write-Host "🌿 Setting branch to main..." -ForegroundColor Yellow
git branch -M main
Write-Host "✓ Branch set to main" -ForegroundColor Green
Write-Host ""

# Push to GitHub
Write-Host "⬆️  Pushing to GitHub..." -ForegroundColor Yellow
Write-Host "Repository: $repo" -ForegroundColor Cyan
Write-Host ""

$push = Read-Host "Ready to push? (y/n)"
if ($push -eq "y" -or $push -eq "Y") {
    git push -u origin main --force
    Write-Host ""
    Write-Host "✅ Successfully pushed to GitHub!" -ForegroundColor Green
    Write-Host "🔗 View at: https://github.com/ajay0896/iot-smart-parking-system-firebase" -ForegroundColor Cyan
} else {
    Write-Host "❌ Push cancelled" -ForegroundColor Red
}

Write-Host ""
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host "Done! 🎉" -ForegroundColor Green
