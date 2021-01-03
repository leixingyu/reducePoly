## Poly Reduction Visualization

### Introduction

The algorithm is based on a paper from 1998: A Simple, Fast, and Effective Polygon Reduction Algorithm [http://pds26.egloos.com/pds/201402/12/11/gdmag.pdf]

The algorithm collapses vertices one by one, which is great to visualize the process of vertex-reduction. 

The limits are that it takes a lot of time to process high poly mesh, and it only works for triangluar meshes. A possible optimization is to compute to desire poly-count, store the vertex order and then re-construct the mesh all at once, but that would lose the slowly-reduced process.

### How To:

1. load the reduceCommand.mll

2. select the mesh you want to reduce (make sure it's full triangle with a mesh count less than 3,000)

3. execute the following command with the desired reduce rate
```python
import maya.cmds as cmds
cmds.reduceCmd(percentage=50)
```

### Demo:

![](https://www.xingyulei.website/assets/reducePoly.gif)

Full Video: https://youtu.be/juwPpeyG7yg

[Test Scene Download](https://www.xingyulei.website/assets/reducePolyTest.mb)
