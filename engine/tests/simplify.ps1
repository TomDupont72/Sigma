. "$PSScriptRoot\common.ps1"

$tests = @(
    @{
        Name = "numeric sum"
        Args = @("simplify", "1+2+3")
        Expected = "6"
    },
    @{
        Name = "zero removed from sum"
        Args = @("simplify", "x+0")
        Expected = "x"
    },
    @{
        Name = "zero product removed from sum"
        Args = @("simplify", "0*x+y")
        Expected = "y"
    },
    @{
        Name = "one removed from product"
        Args = @("simplify", "x*1")
        Expected = "x"
    },
    @{
        Name = "power zero"
        Args = @("simplify", "x^0")
        Expected = "1"
    },
    @{
        Name = "like terms"
        Args = @("simplify", "x+x")
        Expected = "2x"
    },
    @{
        Name = "like terms with coefficients"
        Args = @("simplify", "2*x+3*x")
        Expected = "5x"
    },
    @{
        Name = "repeated product"
        Args = @("simplify", "x*x*x")
        Expected = "x^{3}"
    },
    @{
        Name = "numeric exponent product"
        Args = @("simplify", "x^2*x^3")
        Expected = "x^{5}"
    },
    @{
        Name = "symbolic exponent product"
        Args = @("simplify", "x^n*x^m")
        Expected = "x^{\left(m+n\right)}"
    },
    @{
        Name = "nested numeric powers"
        Args = @("simplify", "(x^2)^3")
        Expected = "x^{6}"
    },
    @{
        Name = "power distributes over product"
        Args = @("simplify", "(x*y)^2")
        Expected = "x^{2}y^{2}"
    },
    @{
        Name = "power quotient symbolic exponent"
        Args = @("simplify", "x^n/x")
        Expected = "x^{\left(n-1\right)}"
    },
    @{
        Name = "power quotient numeric exponent"
        Args = @("simplify", "x^3/x")
        Expected = "x^{2}"
    },
    @{
        Name = "ln exp inverse"
        Args = @("simplify", "ln(exp(x))")
        Expected = "x"
    },
    @{
        Name = "exp ln inverse"
        Args = @("simplify", "exp(ln(x))")
        Expected = "x"
    },
    @{
        Name = "sin zero plus cos zero"
        Args = @("simplify", "sin(0)+cos(0)")
        Expected = "1"
    },
    @{
        Name = "sin pi plus cos pi"
        Args = @("simplify", "sin(pi)+cos(pi)")
        Expected = "-1"
    },
    @{
        Name = "pythagorean identity"
        Args = @("simplify", "cos(x)^2+sin(x)^2")
        Expected = "1"
    },
    @{
        Name = "pythagorean identity with extra term"
        Args = @("simplify", "a+cos(x)^2+b+sin(x)^2")
        Expected = "a+b+1"
    },
    @{
        Name = "adjacent sums with dummy variables"
        Args = @("simplify", "sum(i,m,n,i^2)+sum(j,n+1,l,j^2)")
        Expected = "\displaystyle\sum_{i=m}^{l}i^{2}"
    },
    @{
        Name = "adjacent sums reversed"
        Args = @("simplify", "sum(j,n+1,l,j^2)+sum(i,m,n,i^2)")
        Expected = "\displaystyle\sum_{i=m}^{l}i^{2}"
    },
    @{
        Name = "adjacent sums with same variable"
        Args = @("simplify", "sum(i,1,3,i)+sum(i,4,n,i)")
        Expected = "\displaystyle\sum_{i=1}^{n}i"
    },
    @{
        Name = "adjacent sums keep unrelated term"
        Args = @("simplify", "a+sum(i,m,n,i^2)+sum(j,n+1,l,j^2)")
        Expected = "\displaystyle\sum_{i=m}^{l}i^{2}+a"
    },
    @{
        Name = "inclusive overlap sums are not merged"
        Args = @("simplify", "sum(i,m,n,i^2)+sum(j,n,l,j^2)")
        Expected = "\displaystyle\sum_{i=m}^{n}i^{2}+\displaystyle\sum_{j=n}^{l}j^{2}"
    },
    @{
        Name = "different sum bodies are not merged"
        Args = @("simplify", "sum(i,m,n,i^2)+sum(j,n+1,l,j^3)")
        Expected = "\displaystyle\sum_{i=m}^{n}i^{2}+\displaystyle\sum_{j=n+1}^{l}j^{3}"
    }
)

Invoke-EngineTests -Tests $tests
