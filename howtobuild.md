#talk about how to build this project.

# Introduction #
we need to build this project use ccmake.


# Details #

1, check out all code.

2, goto the <root source code directory>, which contains "3rdParties, cmake, CMakeLists.txt, document, Projects, Sources".

3, mkdir -p build/X86.HongFei

4, cd build/X86.HongFei

5, ccmake ../..

6, config it, the begin build.

7, you need first build ffmpeg
> make ds\_gnupkg\_config

> make ds\_gunpkg\_build

8, after build ffmpeg, you can build application.
> make

9, after build, you can find executables under bin/ and lib/.

10, then, you can run sdlplayer or others.