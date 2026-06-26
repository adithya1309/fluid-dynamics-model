# Burgers' Equation Simulator

A small C++ program that solves the **1D viscous Burgers' equation** with finite differences and renders the solution live using [raylib](https://www.raylib.com/). Watch a smooth sine wave steepen into a near-shock and then get smoothed out by viscosity. Hopefully built up to full navier-stokes in the future.

<!-- Add a screenshot or GIF here, e.g.:
![Burgers' equation steepening](docs/screenshot.png)
-->

## The physics

Burgers' equation couples nonlinear advection with diffusion:

$$\frac{\partial u}{\partial t} + u\,\frac{\partial u}{\partial x} = \nu\,\frac{\partial^2 u}{\partial x^2}$$

- **Advection** ($u\,\partial u / \partial x$) is nonlinear: faster parts of the wave overtake slower parts, steepening the front into a shock.
- **Diffusion** ($\nu\,\partial^2 u / \partial x^2$) smooths gradients and prevents the shock from becoming truly discontinuous.

The viscosity $\nu$ sets the balance between the two: small $\nu$ → sharp shocks, large $\nu$ → smooth blobs.

## Numerical method

| Aspect | Choice |
| --- | --- |
| Domain | $x \in [0, 1)$, periodic |
| Grid | Uniform, $N = 200$ points, spacing $\Delta x = 1/N$ |
| Initial condition | $u(x, 0) = \sin(2\pi x)$ |
| Advection term | First-order **upwind** difference (direction set by the sign of $u$) |
| Diffusion term | Second-order **central** difference |
| Time integration | Explicit **forward Euler** |

Per time step, each grid point is updated as:

$$u_i^{\,\text{new}} = u_i + \Delta t\left(-\,u_i\,\frac{\partial u}{\partial x}\bigg|_i + \nu\,\frac{u_{i+1} - 2u_i + u_{i-1}}{\Delta x^2}\right)$$

The new values are written into a scratch array and swapped in, so every point is updated from a consistent previous state. Periodic boundaries are handled with modular indexing (`(i-1+N) % N`, `(i+1) % N`).

### Stability (CFL)

Explicit forward Euler is only stable if $\Delta t$ stays below both limits:

- Advection: $\Delta t \le \dfrac{\Delta x}{\max|u|} = \dfrac{0.005}{1} = 0.005$
- Diffusion: $\Delta t \le \dfrac{\Delta x^2}{2\nu} = \dfrac{0.005^2}{2(0.01)} = 0.00125$

The default $\Delta t = 0.0002$ is comfortably under both (~6× margin on the tighter diffusion limit).

## Build & run

### Prerequisites
- A C++ toolchain and **raylib**. On Windows this project was built with [MSYS2](https://www.msys2.org/) (UCRT64 environment):
  ```bash
  pacman -S mingw-w64-ucrt-x86_64-gcc \
            mingw-w64-ucrt-x86_64-make \
            mingw-w64-ucrt-x86_64-pkg-config \
            mingw-w64-ucrt-x86_64-raylib
  ```
- Run all build commands from the **MSYS2 UCRT64 shell**.

### Build
```bash
make          # or: mingw32-make
```
Then run:
```bash
make run      # builds if needed, then launches
```
`make clean` removes the binary.

> Without `make`, compile directly:
> ```bash
> g++ src/burgers.cpp -o burgers.exe $(pkg-config --cflags --libs raylib)
> ```

## Controls

| Key | Action |
| --- | --- |
| `R` | Reset the wave to its initial sine and zero the clock |
| `Esc` | Quit |

## Tweaking the simulation

All parameters live at the top of [`src/burgers.cpp`](src/burgers.cpp):

| Constant | Meaning | Try |
| --- | --- | --- |
| `nu` | Viscosity $\nu$ | `0.001` (sharp shock) … `0.05` (very smooth) |
| `N` | Grid resolution | Higher = sharper, slower |
| `dt` | Time step | Keep below the CFL limits above |
| `stepsPerFrame` | Physics steps per drawn frame | Controls animation speed |

## Possible extensions

- Compare against the exact Cole–Hopf analytic solution to validate accuracy.
- Try other initial conditions (Gaussian, square pulse).
- Switch to the inviscid case ($\nu = 0$) with a conservative scheme.
- Higher-order time integration (RK4) or flux limiters for the advection term.

## License

MIT
