. "$PSScriptRoot\common.ps1"

$tests = @(
    @{
        Name = "sum and product precedence"
        Args = @("display", "x+y*z")
        Expected = "x+yz"
    },
    @{
        Name = "sum factor needs parentheses"
        Args = @("display", "(x+y)*z")
        Expected = "\left(x+y\right)z"
    },
    @{
        Name = "fraction denominator product"
        Args = @("display", "x/(y*z)")
        Expected = "\displaystyle\frac{x}{yz}"
    },
    @{
        Name = "fraction numerator sum has no outer parentheses"
        Args = @("display", "(1+x^(n+1))/(1+x)")
        Expected = "\displaystyle\frac{1+x^{\left(n+1\right)}}{1+x}"
    },
    @{
        Name = "fraction product keeps sum factor parentheses"
        Args = @("display", "(n+k)*x^(n+k)/x")
        Expected = "\displaystyle\frac{\left(n+k\right)x^{\left(n+k\right)}}{x}"
    },
    @{
        Name = "sin argument"
        Args = @("display", "sin(x+y)")
        Expected = "\textup{sin}\left(x+y\right)"
    },
    @{
        Name = "sum notation"
        Args = @("display", "sum(i,1,n,i^2)")
        Expected = "\displaystyle\sum_{i=1}^{n}i^{2}"
    },
    @{
        Name = "prod notation"
        Args = @("display", "prod(i,1,n,i)")
        Expected = "\displaystyle\prod_{i=1}^{n}i"
    },
    @{
        Name = "binomial notation"
        Args = @("display", "binom(n,k)")
        Expected = "\displaystyle\binom{n}{k}"
    },
    @{
        Name = "pi notation"
        Args = @("display", "pi")
        Expected = "\pi"
    }
)

Invoke-EngineTests -Tests $tests
