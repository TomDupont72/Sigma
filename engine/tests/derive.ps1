. "$PSScriptRoot\common.ps1"

$tests = @(
    @{
        Name = "derive constant"
        Args = @("derive", "5", "x")
        Expected = "0"
    },
    @{
        Name = "derive variable"
        Args = @("derive", "x", "x")
        Expected = "1"
    },
    @{
        Name = "derive other variable"
        Args = @("derive", "y", "x")
        Expected = "0"
    },
    @{
        Name = "derive sum"
        Args = @("derive", "x+x^2", "x")
        Expected = "2x+1"
    },
    @{
        Name = "derive product with constant variable"
        Args = @("derive", "x*y", "x")
        Expected = "y"
    },
    @{
        Name = "derive repeated variable product"
        Args = @("derive", "x*x", "x")
        Expected = "2x"
    },
    @{
        Name = "derive quotient by independent variable"
        Args = @("derive", "x/y", "x")
        Expected = "\displaystyle\frac{1}{y}"
    },
    @{
        Name = "derive numeric power"
        Args = @("derive", "x^3", "x")
        Expected = "3x^{2}"
    },
    @{
        Name = "derive symbolic power"
        Args = @("derive", "x^n", "x")
        Expected = "nx^{\left(n-1\right)}"
    },
    @{
        Name = "derive symbolic sum exponent"
        Args = @("derive", "x^(n+k)", "x")
        Expected = "\left(k+n\right)x^{\left(k+n-1\right)}"
    },
    @{
        Name = "derive sin"
        Args = @("derive", "sin(x)", "x")
        Expected = "\textup{cos}\left(x\right)"
    },
    @{
        Name = "derive cos"
        Args = @("derive", "cos(x)", "x")
        Expected = "-\textup{sin}\left(x\right)"
    },
    @{
        Name = "derive ln"
        Args = @("derive", "ln(x)", "x")
        Expected = "\displaystyle\frac{1}{x}"
    },
    @{
        Name = "derive exp"
        Args = @("derive", "exp(x)", "x")
        Expected = "\textup{e}^{x}"
    }
)

Invoke-EngineTests -Tests $tests
