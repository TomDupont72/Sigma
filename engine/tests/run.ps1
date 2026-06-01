$ErrorActionPreference = "Stop"

$suites = @(
    "simplify.ps1",
    "derive.ps1",
    "display.ps1"
)

foreach ($suite in $suites) {
    Write-Host ""
    Write-Host "== $suite ==" -ForegroundColor Cyan
    & (Join-Path $PSScriptRoot $suite)
}

Write-Host ""
Write-Host "All test suites passed." -ForegroundColor Green
