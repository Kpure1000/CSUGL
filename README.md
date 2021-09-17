# CSUGL框架基本介绍

> [Central South University Graphics Library – CSUGL](https://gitee.com/Kpure1000/CSUGL)，这是中南大学计算机学院计算机图形学大作业依赖的图形学框架，以OpenGL框架为主，其他媒体框架为辅。由于部分模板未实例化，生成目标为静态库。

### 依赖

* OpenGL图形渲染——[GLAD](https://glad.dav1d.de/)
* 跨平台窗口渲染——[GLFW](https://www.glfw.org/)
* 图形学数学运算——[GLM](https://glm.g-truc.net/0.9.9/index.html)
* 图片加载——[STB](https://www.stbi.net/)
* 模型加载——[Assimp](https://github.com/assimp/assimp)
* GUI渲染——[ImGUI](https://github.com/ocornut/imgui)
* 日志输出——[glog](https://github.com/google/glog)

### 特性

* C++17：使用了少量C++17特性
* 基础封装：将GLFW的大部分功能进行封装，例如```Window```，```Event```，```Input```等
* 渲染未封装：除了Shader外，大部分涉及OpenGL渲染的代码并未封装，供学习者灵活使用
* 内存自动管理：你将不几乎会在项目中用到delete，而是使用封装好的C++原生智能指针进行更加安全的内存管理。
* TODO
