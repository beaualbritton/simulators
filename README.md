# simulators

Suite of hardware simulators I developed to deepen my understanding of **systems programming** and the **boundary between hardware and software**.  
These projects cover cache design, instruction pipelining, branch prediction, and GPU acceleration, with performance analysis and visualizations.

---





## Projects

- **Cache Simulator (C++)**  
  - Set-associative and direct-mapped designs  
  - Models **write-back** and **write-through** logic

<div align="center">
  <img src="https://beaualbritton.site/images/cache.gif"alt="demo" >
</div>

- **RISC-V Instruction Pipeline (C & Python)**  
  - Pipeline simulator in both languages  
  - Compares **accuracy** and **runtime performance**
    
<div align="center">
  <img src="https://beaualbritton.site/images/instruction.gif"alt="demo" >
</div>

- **Branch Predictor (C++ & Python)**  
  - N-bit predictor implementations (0–3 bit) in C++
  - Visualized with Python's **matplotlib** performance charts  
<div align="center">
  <img src="https://beaualbritton.site/images/branching.gif"alt="demo" >
</div>

- **CUDA Parallelism (CUDA C++)**  
  - Matrix-vector multiplication and vector addition kernels  
  - Demonstrates **GPU speedup vs CPU**  

<div align="center">
  <img src="https://beaualbritton.site/images/cuda.png"alt="demo" >
</div>
