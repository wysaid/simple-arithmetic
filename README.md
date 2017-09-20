# simple-arithmetic

## c++ version

A tiny & high-performance calculation class without other dependencies.  
You can use it simply like:
```
string s = "(1+1+1+1) ^ tan(((2+2)+(3-2)) * 0.5)";
ArithmeticExpression expression(s);
if(expression)
{
    printf("%s = %g\n", s.c_str(), expression.value());
}
```
or with variable `x` and `y`:
```
string s = "cos(x^2) + sin(y)^2";
ArithmeticExpression expression(s);
if(expression)
{
    expression.setX(0.8);
    expression.setY(1.5);
    printf("%s = %g\n", s.c_str(), expression.value());
}
```

### Demo

### ScreenShot

[![Tool](https://raw.githubusercontent.com/wysaid/simple-arithmetic/gh-pages/screenshot/screenshot2.png "wysaid")](http://arithmetic.wysaid.org)

## javascript version

A simple demo that just draw the equation curve

View it online: [http://arithmetic.wysaid.org](http://arithmetic.wysaid.org "wysaid.org")

### ScreenShot ##

[![Tool](https://raw.githubusercontent.com/wysaid/simple-arithmetic/gh-pages/screenshot/screenshot.jpg "wysaid")](http://arithmetic.wysaid.org)
