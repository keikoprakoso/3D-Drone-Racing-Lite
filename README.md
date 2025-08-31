# 3D Drone Racing Lite

A high-performance 3D drone racing simulator built with C++, OpenGL, and Bullet Physics. Features advanced camera controls, realistic physics simulation, and professional-grade graphics.

## 🎮 Features

### Core Features
- **Realistic Physics**: Bullet Physics integration with accurate drone dynamics
- **Advanced Camera System**: Mouse look controls with smooth camera following
- **Professional Controls**: WASD movement, Q/E vertical control, SPACE thrust
- **Mission System**: 5-ring racing courses with progress tracking
- **Performance Monitoring**: Real-time FPS display and performance metrics

### Enhanced Features
- **Debug Visualization**: Physics collision shapes and wireframe rendering (F1)
- **Dynamic Environment**: Animated sky background with atmospheric effects
- **Enhanced Lighting**: Dual light sources with realistic attenuation
- **Smooth Rendering**: 60+ FPS with VSync and optimized geometry
- **Data Logging**: Flight telemetry saved to CSV for analysis

## 🚀 Quick Start

### Prerequisites
- **macOS** (currently optimized for macOS with Metal)
- **CMake** 3.16+
- **C++17** compatible compiler
- **OpenGL** 3.3+ support
- **GLFW**, **GLM**, and **Bullet Physics** libraries

### Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/keikoprakoso/3D-Drone-Racing-Lite.git
   cd 3D-Drone-Racing-Lite
   ```

2. **Install dependencies (macOS with Homebrew):**
   ```bash
   brew install cmake glfw glm bullet
   ```

3. **Build the project:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

4. **Run the simulator:**
   ```bash
   ./drone-sim
   ```

## 🎯 Controls

### Basic Movement
- **WASD**: Forward/Left/Backward/Right movement
- **Q/E**: Up/Down (vertical movement)
- **SPACE**: Additional upward thrust

### Camera Controls
- **Arrow Keys**: Orbit camera around drone
- **Right Mouse + Drag**: First-person mouse look
- **C**: Reset camera to default position

### Special Functions
- **R**: Reset drone and mission
- **F1**: Toggle physics debug visualization
- **F2**: Toggle performance info display

## 🏗️ Architecture

### Core Components
- **Renderer**: OpenGL rendering pipeline with shader management
- **Physics**: Bullet Physics integration with collision detection
- **Controls**: Input handling and thrust calculation
- **Mission**: Race course management and progress tracking

### File Structure
```
3D-Drone-Racing-Lite/
├── src/
│   ├── main.cpp              # Application entry point
│   ├── renderer/
│   │   ├── renderer.h        # OpenGL rendering interface
│   │   └── renderer.cpp      # Rendering implementation
│   ├── physics/
│   │   ├── physics.h         # Physics world interface
│   │   ├── physics.cpp       # Bullet physics integration
│   │   └── debug_drawer.*    # Physics visualization
│   ├── controls/
│   │   ├── controls.h        # Input handling interface
│   │   └── controls.cpp      # Control implementation
│   └── mission/
│       ├── mission.h         # Mission management interface
│       └── mission.cpp       # Race logic implementation
├── assets/
│   └── shaders/              # GLSL shader files
├── include/                  # External library headers
├── data/                     # Log files and data storage
└── build/                    # Build artifacts (generated)
```

## 🔧 Technical Details

### Dependencies
- **GLFW**: Window and input management
- **GLAD**: OpenGL function loading
- **GLM**: Mathematics library for 3D transformations
- **Bullet Physics**: Rigid body dynamics and collision detection

### Performance
- **Target FPS**: 60+ with VSync
- **Memory Usage**: Efficient resource management
- **Platform**: Optimized for macOS with Metal backend

### Rendering Pipeline
1. **Vertex Processing**: Position, normal, and texture coordinate handling
2. **Physics Integration**: Real-time transform synchronization
3. **Lighting**: Phong illumination with attenuation
4. **Post-processing**: Atmospheric effects and performance monitoring

## 🎨 Visual Features

### Lighting System
- Primary directional light with realistic shadows
- Secondary fill light for depth perception
- Distance-based light attenuation
- Dynamic ambient lighting

### Geometry
- High-resolution sphere drone model (16x16 divisions)
- Detailed ring obstacles with torus geometry
- Ground plane with proper normal mapping
- Optimized vertex buffer objects (VBOs)

### Effects
- Animated gradient sky background
- Real-time performance metrics in window title
- Debug wireframe visualization
- Smooth camera interpolation

## 📊 Data & Analytics

### Flight Logging
- **CSV Export**: Position, velocity, and thrust data
- **Real-time Monitoring**: FPS and mission progress
- **Performance Analysis**: Frame time and render statistics

### Debug Features
- **Physics Visualization**: Collision shapes and AABBs
- **Wireframe Mode**: Geometry debugging
- **Performance Overlay**: Real-time system metrics

## 🚀 Future Enhancements

### Planned Features
- **AI Opponents**: Intelligent drone competitors
- **Multiplayer**: Online racing with other players
- **Advanced Physics**: Aerodynamics and weather systems
- **VR Support**: Oculus/Meta Quest integration
- **Procedural Tracks**: Infinite race course generation

### Technical Roadmap
- **Phase 1**: AI opponents and advanced physics
- **Phase 2**: Multiplayer networking
- **Phase 3**: VR/AR integration
- **Phase 4**: Machine learning flight assistance

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Bullet Physics**: For realistic physics simulation
- **GLFW**: For cross-platform window management
- **OpenGL**: For high-performance graphics rendering
- **GLM**: For mathematical operations

## 📞 Support

For questions, issues, or contributions:
- **GitHub Issues**: Report bugs and request features
- **Pull Requests**: Submit improvements and fixes
- **Discussions**: Share ideas and get help

---

**Happy Flying!** 🛸

*Built with ❤️ using modern C++ and OpenGL*