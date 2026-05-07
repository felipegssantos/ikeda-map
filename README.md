# Ikeda map solver

## Problem

We want to solve the nonlinear Schrodinger equation (NLSE):

$$
\frac{\partial A^m}{\partial z} = -\frac{\alpha}{2} A^m - i \frac{\beta_2}{2} \frac{\partial^2 A^m}{\partial t^2} + i \gamma |A^m|^2 A^m
$$

for the fields $A^m(z, t)$, $m=1,2,\ldots$, with $t \in \R$ and $z \in [0, L]$, subject to the boundary conditions

$$
A^{m+1}(0, t) = \sqrt{\theta} A_{in} + e^{i \phi_0} \sqrt{1 - \theta} A^m(L, t)
$$

where:
* $m$ is the round-trip index
* $\alpha$ is the propagation loss;
* $\beta_2$ is the dispersion;
* $\gamma$ is the (third-order) nonlinear coupling coefficient;
* $A_{in}$ is the pumping field;
* $L$ is the round-trip length;
* $\phi_0$ is a detuning parameter;
* $\theta \in [0, 1]$ is an energy coupling parameter.

### Notation

In the following, we drop the index $m$ when discussing the NLSE, since this equation does not couple different fields with different $m$.

### Renormalizing

$$
\frac{\partial A}{\partial z} = -\frac{\alpha}{2} A - i \frac{\beta_2}{2} \frac{\partial^2 A}{\partial t^2} + i \gamma |A|^2 A
$$

Substituting $z = \xi z_0$ and $t = \tau t_0$,

$$
\frac{1}{z_0} \frac{\partial A}{\partial \xi} = -\frac{\alpha}{2} A - i \frac{\beta_2}{2 t_0^2} \frac{\partial^2 A}{\partial \tau^2} + i \gamma |A|^2 A
\\[5pt]
\frac{\partial A}{\partial \xi} = -\frac{\alpha z_0}{2} A - i \frac{\beta_2 z_0}{2 t_0^2} \frac{\partial^2 A}{\partial\tau^2} + i \gamma z_0 |A|^2 A
$$

Then we can choose $z_0$ and $t_0$ such that

$$
\alpha z_0 = \left|\frac{\beta_2 z_0}{t_0^2}\right| = 1
\\[5pt]
\implies
z_0 = \frac{1}{\alpha},
\quad
t_0 = \sqrt{\frac{|\beta_2|}{\alpha}}
$$

(recall that $\beta_2$ can be positive or negative) so the NLSE becomes

$$
\frac{\partial A}{\partial\xi} = -\frac{1}{2} A \pm \frac{i}{2} \frac{\partial^2 A}{\partial\tau^2} + i \kappa |A|^2 A
$$

where $\kappa = \dfrac{\gamma}{\alpha}$ and the $\pm$ sign depends on the sign of $\beta_2$.

## Split-step method

In order to solve the NLSE, we use the split-step method. That consists in intercalating steps of the linear and nonlinear parts of the NLSE. The linear of the equation is that given by $\kappa = 0$,

$$
\frac{\partial A}{\partial \xi} = -\frac{1}{2} A - \frac{i}{2} \frac{\partial^2 A}{\partial \tau^2}
$$

which has an exact solution in the frequency domain. Allowing $\tilde{A}(\xi, \omega)$ to be the Fourier transform (w.r.t. time \tau) for $A(\xi, \tau)$, we have

$$
\frac{\partial \tilde{A}}{\partial \xi} = \frac{1}{2} (i \omega^2 - 1) \tilde{A}
$$

whose solution is

$$
\tilde{A}(\xi, \omega) = e^{k (\xi - \xi_0)} \tilde{A}(\xi_0, \omega)
$$

where

$$
k = \frac{1}{2} (i \omega^2 - 1)
$$

and $\xi_0$ corresponds to some initial condition. We can perform the linear step from $\xi_0$ to $\xi = \xi_0 + \delta$ for some small $\delta$ with a small error. Taking the inverse Fourier transform of $\tilde{A}(\xi, \omega)$, we get $A(\xi, \tau)$ finishes the linear propagation.

For the nonlinear step, we consider only the effect of the nonlinear term in the NLSE

$$
\frac{\partial A}{\partial \xi} = i \kappa |A|^2 A
$$

which for a small length step $\delta$, gives the approximate solution

$$
A(\xi + \delta, \tau) = e^{i \kappa |A(\xi, \tau)^2| h} A(\xi, \tau)
$$

A "leap-frog" approach can be used in order to make convergence scale with $h^3$ instead of $h$. That would be to take a nonlinear step of length $h/2$, then a linear step of length $h$, and finally another nonlinear step of length $h/2$, as below:

$$
A_1 = e^{i \kappa |A(\xi, \tau)|^2 h / 2} A(\xi, \tau)
\\[5pt]
A_2 = \mathcal{F}^{-1}\left\{ e^{k h} \mathcal{F}[A_1] \right\}
\\[5pt]
A(\xi + h, \tau) = e^{i \kappa |A_2|^2 h / 2} A_2
$$

### Test cases for NLSE

Bright soliton

$$
A(\xi, \tau) = a \operatorname{sech} [a (\xi - v \tau)] e^{i (k \xi - \omega \tau)}
$$

## Compiling

To link against fftw, I must run:

```bash
clang++ <CPP_FILES> -o <OUTPUT> /usr/local/lib/libfftw3.a
```

which should go into a makefile.

## Utils

Output from installing fftw-3.3.11:

----------------------------------------------------------------------
Libraries have been installed in:
   /usr/local/lib

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
