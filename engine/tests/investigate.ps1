. "$PSScriptRoot\common.ps1"

# This suite is intentionally not called by run.ps1.
# Put suspected bugs or desired future behavior here before moving them into
# simplify.ps1, derive.ps1, or display.ps1.

$tests = @(
    @{
        Name = "future quotient rule full simplification"
        Args = @("derive", "x/(1+x)", "x")
        Expected = "\displaystyle\frac{1}{\left(1+x\right)^{2}}"
    },
    @{
        Name = "future chain rule compact trig"
        Args = @("derive", "sin(x^2)", "x")
        Expected = "2x\textup{cos}\left(x^{2}\right)"
    },
    @{
        Name = "future pythagorean identity reversed powers"
        Args = @("simplify", "sin(x)^2+cos(x)^2")
        Expected = "1"
    }
)

Invoke-EngineTests -Tests $tests
