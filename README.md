[![Imagine](https://github.com/hrkz/imagine/blob/gh-pages/images/imagine_render_100.png)](http://imagine-research.org)
=======================

[![Join the chat at https://gitter.im/hrkz/imagine](https://img.shields.io/gitter/room/nwjs/nw.js.svg?style=flat-square)](https://gitter.im/hrkz/imagine)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](LICENSE)

Imagine is a research-oriented simulation library based on computational graphs that provides many algorithms for light transport, fluids and rigid body dynamics.

It's written in modern c++17 with performance and modularity in mind.

## Features

* :globe_with_meridians: **Computational mathematics**
  * Expression template linear algebra (dense linear solvers and decompositions)
  * Projective geometry, space partitioning and surface representations (meshes, patches, implicit)
  * Ordinary differential equations solvers for implicit, explicit and symplectic systems
  * Nonlinear optimization with gradient descent, quasi-newton and least squares methods
* :video_game: **Interactive cross-platform environment**
  * Real-time physically based viewport with built-in voxel cone tracing, temporal antialiasing and many post-process techniques (bloom, color correction, motion blur, etc)
  * Virtual and augmented reality, including an advanced pose compositor, stereo rendering and monoscopic far field optimization
* :high_brightness: **Light transport**
* :wavy_dash: **Fluids and rigid body dynamics**

At the moment, the development is focused on a physically correct (spectral) renderer .
Next step will be to implement modern rendering techniques for the viewport in order to produces interesting results.

## License

Imagine is released under the [MIT](LICENSE) licence.
Please cite Imagine if it helps your research.
