# Poly Reduction Visualization

I was looking through some algorithm for generating LODs and find this paper from 1998: A Simple, Fast, and Effective Polygon Reduction Algorithm [http://pds26.egloos.com/pds/201402/12...]
The idea is to collapse vertices one by one. I successfully implement this in Maya, however, it takes a lot of time to process high poly mesh. 

A optimization is to compute to desire poly-count, store the vertex order and then re-construct the mesh all at once at the end.

Demo:

![](https://github.com/sheldonlei/reducePoly/blob/master/polyreduce.gif)

Full Video: https://youtu.be/juwPpeyG7yg
