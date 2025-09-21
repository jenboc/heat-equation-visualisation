# Heat Equation Visualisation
Repository for (my blog post)[] about visualising the heat equation.
Contains implementations for the Crank-Nicholson scheme and series truncation
methods for solving the following:
$$
    \frac{\partial u}{\partial t} = \kappa \frac{\partial u}{\partial x}
$$
on \\(t \ge 0\\), \\(x \in (0, L)\\) subject to:
1. \\(u(0,t) = u(L, t) = 0\\)
2. \\(u(x,0) = f(x)\\)

Repository also contains scripts used to create the visualisations in the
article.
