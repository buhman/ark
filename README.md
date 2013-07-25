ark
===

A silly game (in the sense that it is appallingly horribly written) using sdl2

requirements
------------

No distribution that I know of packages [SDL2][], and [SDL2-ttf][] that I
know of, but these are required to run ark. For archlinux, you can find
both in the AUR: [aur/sdl2][], [aur/sdl2-ttf][]

[SDL2]: http://www.libsdl.org/
[SDL2-ttf]: http://www.libsdl.org/projects/SDL_ttf/

[aur/sdl2]: https://aur.archlinux.org/packages/sdl2/
[aur/sdl2-ttf]: https://aur.archlinux.org/packages/sdl2-ttf/

Once the dependencies are installed, (along with `glibc`, `gcc`, and
`make`) you'll be able to build ark.

building
--------

Just run `make`; this will create the `ark` binary.
