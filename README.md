<p align="center">
  <a href="https://github.com/SpaceYoshi/traffic-tactician/" rel="noopener">
 <img width=400px height=400px src="logo.png" alt="Project logo"></a>
</p>

<h3 align="center">Traffic Tactician</h3>

<div align="center">

  [![Stars](https://img.shields.io/github/stars/SpaceYoshi/traffic-tactician.svg)](https://github.com/SpaceYoshi/traffic-tactician/stargazers)
  [![GitHub Issues](https://img.shields.io/github/issues/SpaceYoshi/traffic-tactician.svg)](https://github.com/SpaceYoshi/traffic-tactician/issues)
  [![GitHub Pull Requests](https://img.shields.io/github/issues-pr/SpaceYoshi/traffic-tactician.svg)](https://github.com/SpaceYoshi/traffic-tactician/pulls)
  [![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> A traffic controller training program in C++ using <a href="https://opencv.org/">OpenCV</a> & <a href="https://www.opengl.org/">OpenGL</a>.
    <br> 
</p>

## 📝 Table of Contents
- [About](#about)
- [Built Using](#built_using)
- [Authors](#authors)

## 🧐 About <a name = "about"></a>
Traffic Tactician is a training program made in C++ for a uni assignment. The software uses <a href="https://opencv.org/">OpenCV</a> & <a href="https://www.opengl.org/">OpenGL</a>.


⚠ Note: Make sure to add an environment variable called "OPENCV" that leads to to your OpenCV build folder (e.g. *C:\opencv\opencv\build*).

⚠ Note: Make sure to download the *pose_iter_440000.caffemodel* from the [CMU School of Computer Science](https://www.cs.cmu.edu/), and put it in *${SolutionDir}\TrafficTactician\pose\coco\\*.

⚠ Note: In case of getting the "could not find opencv_world490d.lib" error, ensure you have installed (the right version of) OpenCV correctly, and have configured it in your system environment variables. Alternatively, you can drag the .dll files manually into *${SolutionDir}\TrafficTactician\x64\${ConfigurationName}*.

⚠ Note: Make sure to build the solution when making changes to *settings.json* to ensure having the updated header file.

## ⛏️ Built Using <a name = "built_using"></a>
- [OpenGL](https://opengl.org/) - The Industry's Foundation for High Performance Graphics
- [OpenCV](https://opencv.org/) - World's biggest computer vision library
- [GLFW](https://www.glfw.org/) - Open Source, multi-platform library for OpenGL
- [GLM](https://github.com/g-truc/glm/) - OpenGL Mathematics (GLM)
- [GLEW](https://github.com/nigels-com/glew/) - The OpenGL Extension Wrangler Library
- [Easylogging++](https://github.com/abumq/easyloggingpp/) - Single header efficient logging library for C++ applications
- [JSON](https://github.com/nlohmann/json/) - JSON for Modern C++
- [json2hpp](https://github.com/KaixoCode/json2hpp/) - CMake module that converts a json file straight into a C++ header containing a constexpr object

## ✍️ Authors <a name = "authors"></a>
- [@OnsPetruske](https://github.com/pkg-dot-zip)
- [@SpaceYoshi](https://github.com/SpaceYoshi)
- [@lrstolk](https://github.com/lrstolk)
- [@Mosu5](https://github.com/Mosu5)
- [@Sam-D-C](https://github.com/Sam-D-C)
- [@Yuichi135](https://github.com/Yuichi135)

See also the list of [contributors](https://github.com/SpaceYoshi/traffic-tactician/contributors) who participated in this project.
