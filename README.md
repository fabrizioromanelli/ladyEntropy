# Rose
OpenGL will create a beautiful rose for ya. And also some fine digital geometric art!

# Compile
```g++ rose.c++ -lglut -lGL -o rose```
```g++ synth_art.c++ -lglut -lGL -o synth```

# Run
```./rose```
```./synth -display :0```

# Tricks
In order to save images at high resolution, you can fake X display on Linux systems. Just install xvfb and then run on a separate console the following command:
``` Xvfb :1 -screen 0 1920x1080x24+32 ```
This will create a virtual screen that can be used on the synth program to create a huge window useful to save images from OpenGL. It's quite a big fake, but it works ok! ;)
So, after creating the virtual screen, just run:
```./synth -display :1```