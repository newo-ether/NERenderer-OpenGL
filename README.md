# NERenderer-OpenGL
## A Path-Tracing Renderer on OpenGL

This is a GPU renderer written in C++, which uses path-tracing algorithm and is powerful enough to generate photorealisitic images. Unlike most of the GPU renderers which are mainly based on OpenCL, this renderer implements the entire path-tracing algorithm on OpenGL, which means that it supports much more varieties of devices than general GPU renderers. It partially enables the power of general computing on devices that doesn't support OpenCL library. Currently this renderer is still in developing.
