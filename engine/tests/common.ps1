$ErrorActionPreference = "Stop"

$Script:EngineRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$Script:App = Join-Path $Script:EngineRoot "app.exe"

function Assert-EngineOutput {
    param(
        [Parameter(Mandatory = $true)]
        [string] $Name,

        [Parameter(Mandatory = $true)]
        [string[]] $Args,

        [Parameter(Mandatory = $true)]
        [string] $Expected
    )

    if (-not (Test-Path $Script:App)) {
        throw "Missing engine binary: $Script:App"
    }

    $actual = & $Script:App @Args

    if ($actual -ne $Expected) {
        Write-Host "FAIL: $Name" -ForegroundColor Red
        Write-Host "  command:  app.exe $($Args -join ' ')"
        Write-Host "  expected: $Expected"
        Write-Host "  actual:   $actual"
        return $false
    }

    Write-Host "PASS: $Name" -ForegroundColor Green
    return $true
}

function Invoke-EngineTests {
    param(
        [Parameter(Mandatory = $true)]
        [array] $Tests
    )

    $failed = 0

    foreach ($test in $Tests) {
        $passed = Assert-EngineOutput `
            -Name $test.Name `
            -Args $test.Args `
            -Expected $test.Expected

        if (-not $passed) {
            $failed++
        }
    }

    if ($failed -gt 0) {
        throw "$failed test(s) failed"
    }
}
