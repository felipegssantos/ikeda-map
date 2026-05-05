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

## Split-step method

Let us supress the index $m$ since this discussion apply to each $A^m$, irrespective of the others.

In order to solve the NLSE, we use the split-step method. That consists in intercalating steps of the linear and nonlinear parts of the NLSE. The linear of the equation is that given by $\gamma = 0$,

$$
\frac{\partial A}{\partial z} = -\frac{\alpha}{2} A - i \frac{\beta_2}{2} \frac{\partial^2 A}{\partial t^2}
$$

which has an exact solution in the frequency domain. Allowing $\tilde{A}(z, \omega)$ to be the Fourier transform (in time) for $A(z, t)$, we have

$$
\frac{\partial \tilde{A}}{\partial z} = \left(-\frac{\alpha}{2} + i \omega^2 \frac{\beta_2}{2}\right) \tilde{A}
$$

whose solution is

$$
\tilde{A}(z, \omega) = e^{k (z - z_0)} \tilde{A}(z_0, \omega)
$$

where

$$
k = -\frac{\alpha}{2} + i \omega^2 \frac{\beta_2}{2}
$$

and $z_0$ corresponds to some initial condition. We can perform the linear step from $z_0$ to $z = z_0 + h$ for some small $h$ with a small error. Taking the inverse Fourier transform of $\tilde{A}(z, \omega)$, we get $A(z, t)$ finishes the linear propagation.

For the nonlinear step, we look at all the terms in the NLSE neglected so far:

$$
\frac{\partial A}{\partial z} = i \gamma |A|^2 A
$$

which for a small length step $h$, gives the approximate solution

$$
A(z + h, t) = e^{i \gamma |A(z, t)^2| h} A(z, t)
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
