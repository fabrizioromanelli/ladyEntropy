# Lady Entropy
OpenGL will create a beautiful [Schotter](https://collections.vam.ac.uk/item/O221321/schotter-print-nees-georg/) art for ya!

# Compile
```g++ rose.c++ -lglut -lGL -o rose```
```g++ lady_entropy.c++ -lglut -lGL -o lentroy```

# Run
```./rose```

```./lentropy -display :0```

# Options
```-type``` Let select the program to be used to display or to write to a file. It can be one of the following: ```display``` or ```file```.
Example: ```-type display```.

```-row``` Let select the number of rows for the matrix of cubes. Must be a positive integer.
Example: ```-row 10```.

```-col``` Let select the number of columns for the matrix of cubes. Must be a positive integer.
Example: ```-col 20```.

```-size``` Let select the cube size. Must be a positive floating number.
Example: ```-size 0.03```.

# Tricks
In order to save images at high resolution, you can fake X display on Linux systems. Just install xvfb and then run on a separate console the following command:
``` Xvfb :1 -screen 0 1920x1080x24+32 ```
This will create a virtual screen that can be used on the synth program to create a huge window useful to save images from OpenGL. It's quite a big fake, but it works ok! ;)
So, after creating the virtual screen, just run:
```./lentropy -display :1```
