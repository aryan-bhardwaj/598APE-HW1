# 598APE-HW1


This program assumes the following are installed on your machine:
* A working C++ compiler (g++ is assumed)
* make
* ImageMagick (for importing and exporting non-ppm images)
* FFMpeg (for exporting movies from image sequences)

In order to install the correct packages to run the program, run either:
```bash
sudo bash ./install.sh
```
or
```bash
chmod +x install.sh
sudo ./install.sh
```

To compile the program run:
```bash
make -j
```

To clean existing build artifacts run:
```bash
make clean
```

In order to run benchmarking tests, run
```bash
hyperfine "COMMAND"
```
Make sure to include the quotation marks! For example, to benchmark the Piano Room, you would run
```bash
hyperfine "./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500"
```
and you can do the same with the remaining scenes. For Globe, Sphere, and Elephant, we ran our benchmarks with a 1-frame output (as in, we set the output frame flag to 1).

## Important Commit Hashs
The following list of commit hashes represent snapshots of our fundamental/major optimizations. 

**Baseline code**: 04f1a833f78bb146c2cda83b92920a98fe30e6a3<br>
**Basic branch, cache, logic changes**: 5dc3b7f6e59b35c587b6b0c50741858a5a52f2aa <br>
**Linked list change**: 6d120c6a70feca4c5251216bd195fe329851ce45 <br>
**Fast inverse square root**: edd66a49389bf696b41b51d7259e65562d161c1b <br>
**Removing insertion sort**: 03c4ed4c33f4b66ad1776ba7738b7704198c2af7 <br>
**Adding OpenMP parallelism**: 3c12127c62855c86882872da8c4bb6f6f73be37e<br>
**BVH**: 09272da8e7d0fa78bd4688ac36aea3b4973f2d4c <br>

---
---
The raytracer program here is general and can be used to generate any number of different potential scenes.

Once compiled, one can call the raytracer program as follows:
```bash
./main.exe --help
# Prints the following
# Usage ./main.exe [-H <height>] [-W <width>] [-F <framecount>] [--movie] [--no-movie] [--png] [--ppm] [--help] [-o <outfile>] [-i <infile>] [-a <animationfile>]
```

The raytracer program takes a scene file (a text file ending in .ray) and generates an image or sequence of images corresponding to the specified scene.

One can tune the height, width, and format of the image being generated with optional command line arguments. For example, let's generate an 500x500 image corresponding to the scene in `inputs/pianoroom.ray`, in PPM format.

```bash
./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
```

As we run the program, we see the following output:
```
Done Frame       0|
Total time to create images=1.334815 seconds
```

We have placed timer code surrounding the main computational loop inside main.cpp. It is your goal to reduce this runtime as much as possible, while maintaining or increasing the complexity (i.e. resolution, number of frames) of the scene.

Here we see that the image took 1.3 seconds to run and produced a result in `output/pianoroom.ppm`. Input and output of images is already handled by the library. In particular, the PPM format (see https://en.wikipedia.org/wiki/Netpbm for an example), represents images as text for data -- which makes it easy to input and output without the use of a library. However, as this is not the most efficient, this application uses the tool ImageMagick tool to convert to and from the PPM formats.

## Input Programs
This project contains three (arguably four) input programs for you to optimize.

### PianoRoom

A simple room with a reflecting checkerboard floor, a stairwell, a sphere, a circular rug, and a mirror ref.

Here we want to produce the highest resolution single image of this format, as fast as possible. The relevant command for producing an output is:

```bash
./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
```

### Globe

A video of the Earth floating on top of a sea with a sky in the background. The Earth and clouds are rotating (in opposite directions), and the sea beneath reflects the scene above, and moves.

Here we want to produce the highest resolution video, as fast as possible. The relevant command for producing an output is:

```bash
./main.exe -i inputs/globe.ray --ppm  -a inputs/globe.animate --movie -F 24 
```

Here, as we are generating multiple frames, the extra command `-a <animationfile>` is used to pass in a sequence of commands to generate subsequent frames.

The number of frames we wish to generate (24) is passed in as `-F <numframes>`.

Here we will produce 24 individual images for each frame. To produce a playable movie out of these images, the `--movie` command will call a program called FFMpeg to produce a playable video.

### Elephant

A mesh of objects. In practical graphics applications, designing a primitive for each possible object is too complex. Instead, one builds up a mesh of triangles to represent the object being shown. Given sufficiently many triangles, we can represent arbitrarily complex structures. Here, we wish to make a video circling around a Mesh object which we import.

The simple version of this program is generated by the following command:
```bash
./main.exe -i inputs/elephant.ray --ppm  -a inputs/elephant.animate --movie -F 24 -W 100 -H 100 -o output/sphere.mp4 
```

Note the reduced resolution (as the initial unoptimized code can be somewhat slow).

This initial mesh represents a sphere with 3168 triangles.

Here we produce a video in which we have the camera circles around the object.

If we inspect the input file `inputs/elephant.ray` we see that it loads the mesh from two files, as defined by the line
```
data/x.txt 1586 data/f.txt 3168 -1.58 -.43 2.7
```

The goal here is to speed up the program sufficiently to make a high resolution circle of the elephant mesh (found in `data/elepx.txt` and `data/elepf.txt`), which contains 111748 triangles. One can edit the `.ray` file and comment out the sphere mesh and replace it with `data/elepx.txt 62779 data/elepf.txt 111748 -1.58 -.43 2.7` (this is done in `inputs/realelephant.ray`).
