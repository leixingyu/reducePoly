<div align="center">
<h1 align="center">Maya Reduce Poly Plugin</h1>

  <p align="center">
    A simple plugin for visualizing mesh reduction in Maya
    <br />
    <a href="https://youtu.be/juwPpeyG7yg">Full Demo</a>
  </p>
</div>

## About The Project

The algorithm is based on a paper from 1998: [A Simple, Fast, and Effective Polygon Reduction Algorithm](http://pds26.egloos.com/pds/201402/12/11/gdmag.pdf)
which collapses vertices with the lowest cost one by one. 
This gives a great visualization of the poly reduction process. 

The limits are that it takes a lot of time to process high poly mesh, and it only works for triangular meshes. 
A possible optimization is to compute to desire poly-count, store the vertex order and then re-construct the mesh all at once, 
but that would lose the slowly-reduced visualization.

<div align="center">
  <img src="https://i.imgur.com/WyGO59u.gif" alt="reduce">
</div>

## Getting Started

> The Plugin is compiled for Maya 2018

1. [Download](https://github.com/leixingyu/reduce-poly/releases/tag/v0.1.0) the latest plugin release


2. Load the `reduceCommand.mll` in Maya through Plug-in Manager


## Usage

The plugin is a custom command plugin, once the plugin has loaded, 
you should be able to run it with `maya.cmds`. 

I strongly suggest
using a mesh under 3000 vert-count as the algorithm is not optimized yet.
The plugin also only supports triangle meshes.

1. Select the mesh you want to reduce in the scene


2. Execute using `reduceCmd()` of desired reduce rate using `percentage` flag.
The `percentage` is how much vertices you want the mesh to remain compared to 
the original. (example: A mesh with 1000 verts will be reduced to
200 verts after running with`percentage=20`)

    ```python
    import maya.cmds as cmds
    cmds.reduceCmd(percentage=50)
    ```
