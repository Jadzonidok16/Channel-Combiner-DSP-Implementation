<img width="443" height="329" alt="Scheme" src="https://github.com/user-attachments/assets/2aa9374c-f456-40ad-9ad2-daa432d840f6" />
# Audio Channel Combination - DSP Implementation

## Project Overview
The objective of this project is to implement a multi-channel audio combination system based on a specific hardware-oriented schematic and control table[cite: 1, 2]. [cite_start]The system utilizes 2nd order IIR filters to process and mix signals across various output modes[cite: 2].

## Development Phases
[cite_start]The development is structured into four distinct stages (Models) to ensure a reliable transition from a high-level reference to hardware-optimized code[cite: 4]:

* [cite_start]**Model 0 (Reference Model):** A floating-point implementation focused on algorithmic correctness rather than performance[cite: 9, 10]. [cite_start]It serves as the baseline for verification of subsequent optimizations[cite: 11].
* [cite_start]**Model 1 (Algorithmic Improvement):** Optimizes Model 0 by implementing direct memory addressing instead of array indexing and reducing function argument overhead by utilizing global variables[cite: 16, 17, 18]. [cite_start]It produces bit-exact output compared to Model 0[cite: 19].
* [cite_start]**Model 2 (Platform Optimization):** Tailored for the Cirrus Logic DSP platform[cite: 20, 21]. [cite_start]Floating-point arithmetic is replaced with fixed-point emulation using specific classes to ensure hardware compatibility[cite: 21, 22].
* [cite_start]**Model 3 (Hardware Porting):** The final stage involves porting to the target platform, splitting variables into dual-access memory regions, and rewriting the core processing function in Assembly for maximum resource efficiency[cite: 24, 25].
* [cite_start]**Final Integration:** The Assembly-optimized core is integrated into the operating system environment, handling system functions like "PreKick," "PostKick," and "Block/Brick," while incorporating MCV user controls[cite: 26, 27, 28].

## Verification & Testing
[cite_start]System integrity is verified at each stage by comparing the output of each model against its predecessor[cite: 30, 31]:
* [cite_start]**PCMCompare.exe:** Used for sample-by-sample bit-accuracy analysis[cite: 32].
* [cite_start]**run_test.py:** A script used to automate the testing process across different audio signals[cite: 32].
* **Audacity:** Utilized for visual verification and frequency spectrum analysis of the processed signals[cite: 12, 33].

## Performance Profiling
The following MIPS consumption was measured for the processing function[cite: 34]:
* [cite_start]**6172** - C implementation (without debug info)[cite: 35].
* [cite_start]**6902** - Assembly implementation with unoptimized IIR[cite: 36].
* **5174** - Assembly implementation with optimized IIR[cite: 37].

## Setup & Configuration
The project is configured for Visual Studio with the following specifications[cite: 38]:
* [cite_start]**Preprocessor Definitions:** `_CRT_SECURE_NO_WARNINGS`[cite: 39, 40].
* [cite_start]**Language Standards:** C++14 (iso 14) and C11 (iso 11)[cite: 43, 44].
* **Conformance Mode:** Set to `No`[cite: 41, 42].
