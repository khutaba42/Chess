## On macOS:
I assume you have Homebrew downloaded and downloaded SDL2 through it (using `brew install sdl2`) and make sure gcc is also downloaded (`brew install gcc`). 
1. change the `WORKING_DIR` value to the path of the project's path on your machine.
2. run the `./run ` script (make sure the terminal is in the dir the run file is in).

## On Linux: 
### ! (tested on Ubuntu-22.04.3 running on an amd64 proccessor).
the run that runs on macOS also works here with some adjustments, I assume you have Homebrew downloaded and downloaded SDL2 through it (using `brew install sdl2`) and make sure gcc is also downloaded (`brew install gcc`). 
1. change the `WORKING_DIR` value to the path of the project's path on your machine.
2. change the `FRAMEWORK` value to
  ```bash
  FRAMEWORK="
      -I/home/linuxbrew/.linuxbrew/include \
      -I/home/linuxbrew/.linuxbrew/lib \
      -lSDL2 
  "
  ```
3. run the `./run ` script (make sure the terminal is in the dir the run file is in).
4. if you got an error along the lines of `libSDL2-2.0.so.0: cannot open shared object file` then in the terminal type the command `sudo apt install libsdl2-dev` and optionally remove the line `-I/home/linuxbrew/.linuxbrew/lib \` from the value of `FRAMEWORK` then run the `./run ` script.
