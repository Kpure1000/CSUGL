# CSUGL框架基本介绍

> [Central South University Graphics Library – CSUGL](https://gitee.com/Kpure1000/CSUGL)，这是中南大学计算机学院计算机图形学大作业依赖的图形学框架，以OpenGL为基础的渲染引擎。由于部分模板未实例化，不方便导出为动态链接，生成目标暂为静态库。欢迎加入[贡献](#contribute)！

### 构建

推荐使用CMake进行构建

#### 1. 生成静态库

- CMake
``` sh
mkdir build
cd build
cmake ..
```
- 根据系统和构建工具，选择使用make或者nmake等工具进行编译和链接
  如果在Windows中，推荐使用MSVC以及nmake
- 将生成的静态链接库，移动至相应位置，方便自行配置链接
- 最后，不要忘记将src作为include目录

#### 2. 作为子项目

将CSUGL作为子项目，只需要在你的项目CMakeLists中加入一下

```cmake 
 # add sub cmake configure path
add_subdirectory(your_3rd_parts_path/CSUGL)
 # add header path of CSUGL
include_directories(your_3rd_parts_path/CSUGL/src)
```

### 依赖

* OpenGL图形渲染——[GLAD](https://glad.dav1d.de/)
* 跨平台窗口渲染——[GLFW](https://www.glfw.org/)
* 图形学数学运算——[GLM](https://glm.g-truc.net/0.9.9/index.html)
* 图片加载——[STB](https://www.stbi.net/)
* 模型加载——[Assimp](https://github.com/assimp/assimp)
* GUI渲染——[ImGUI](https://github.com/ocornut/imgui)
* 日志输出——[glog](https://github.com/google/glog)

__ TBC ...__

### 特性

* C++17：使用了少量C++17特性
* 基础封装：将GLFW的大部分功能进行封装，例如```Window```，```Event```，```Input```等
* 渲染小部分封装：封装了shader，三个基本的object (vao, vbo, ibo)
* 内存自动管理：你将不几乎会在项目中用到delete，而是使用封装好的C++原生智能指针进行更加安全的内存管理。

__ TBC ...__

### 项目组织

- src
	- core
		- core: 基础定义
		- window: 窗口，封装GLFWWindow
		- application: 应用程序，管理window
		- transform: 基本变换，包括平移旋转缩放
		- event: 事件
		- input: 输入
		- file_system: 文件系统，目前只实现了文件路径名处理
		- camera: 相机系统
	- render
		- shader: 加载和使用shader
		- light: 光照封装**（准备取消这个封装）**
		- vertex_array: GL 的 Vertex Array 封装
		- buffer: GL 的 Vertex Buffer 和 Index Buffer 封装
	- utils
		- singleton: 单例模板
		- thread_pool: 简单的线程池
		- model_loader: 模型元数据加载器
		- asset_manager: 资源管理器模板

__TBC ...__

### TODO

- 渲染
    - [x] 窗口
    - [x] shader
    - [x] 基本变换
    - [x] 基本相机
    - [ ] 封装各类缓冲和渲染对象（VBO，EBO，FBO、VAO、RBO等）
    - [ ] 封装渲染对象（模型）
    - [ ] 材质基本封装
    - [ ]  分批渲染（渲染优化）
    - [ ] 粒子效果（实例渲染）
    - [ ] 层（Layer，不仅仅是渲染深度分层，还有各类功能作用的层级，例如碰撞层次）
    - [ ] 自定义管线流程（引入render pass）
- 物理
	
	- [ ] 3D或2D物理封装（Bullet、Box2D）
- 动画
    - [ ] 自由变形（FFD）
    - [ ] 骨骼绑定
	- [ ] 动画加载、渲染
- 其他媒体
    - [ ] 声音库
    - [ ] 网络库
- GUI
  - [ ] 各类panel封装
    - [ ] Project（asset与folder）
    - [ ] Hierarchy（场景对象管理）
    - [ ] Scene（场景编辑窗口）
      - [ ] Gizmo封装
    - [ ] Render（渲染结果窗口）
    - [ ] Inspector（详细信息）
  - [ ] 鼠标拖拽实现
- 框架
  - [x] 应用程序
  - [ ] 文件系统
  - [ ] 系统配置、项目配置
  - [ ] 程序脚本系统
  - [ ] ECS框架（如果实现了高效的ECS以及脚本系统，以后可以将功能实现在扩展库中，类似Unity，CSUGL将作为动态链接核心库减少开发）
- 示例demo
    - [ ] 智能指针
    - [ ] 线程池
    - [ ] 窗口创建和三角形绘制
    - [ ] 变换和FPS相机
    - [ ] 纹理加载、绘制
    - [ ] 输入、事件
    - [ ] 模型加载
    - [x] 阴影渲染
    - [ ] 实时纹理绘制

__TBC ...__

### 使用方法

[Usage](https://gitee.com/Kpure1000/CSUGL/blob/master/doc/USAGE.md)

### <b id="contribute">贡献 </b>

基本要求：

- 了解C++项目组织原理
- 了解Git的基本使用
- 热爱图形学、不畏困难、愿意肝代码

加入contribution十分简单。推荐使用gitee进行贡献：

1. 登录gitee
2. __fork__ 本仓库
3. 克隆fork的仓库到本地，进行代码编写
4. 提交修改至远程，在gitee中提交pull request
5. 等待CSUGL管理员通过pr、合并修改，完成一次贡献

### 最后

> 图形学是一个十分有趣的计算机学分支。你初次了解这个学科，可能是通过3A游戏的炫酷特效，或是迪士尼的动画渲染，或者是想做一款只属于自己的“Indie Game”。的确，当你实现了一个新的光影效果，或者花了大几个小时离线渲染完了一张图，这种 强烈的__”眼见为实“ __的成就感，前所未有。
>
> 但一个无法否认的事实是，图形学编程的__代码量__很大，需要掌握一定的计算机__底层优化__知识，甚至需要了解__GPU__的硬件构造。并且在实时渲染中，还存在着许多__奇技淫巧__，我们称之为__trick__。如果能够坚持下来，相信你的整体代码水平和思考问题的能力可以提升一个层次。
>
> 希望你能够热爱图形学，热爱游戏，热爱生活。
