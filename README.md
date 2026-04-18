<img width="443" height="329" alt="Scheme" src="https://github.com/user-attachments/assets/2aa9374c-f456-40ad-9ad2-daa432d840f6" />

# Audio Channel Combination - DSP Implementation

## Project Overview
The objective of this project is to implement a multi-channel audio combination system based on a specific hardware-oriented schematic and control table. The system utilizes 2nd order IIR filters to process and mix signals across various output modes.

## Development Phases
The development is structured into four distinct stages (Models) to ensure a reliable transition from a high-level reference to hardware-optimized code:

* **Model 0 (Reference Model):** A floating-point implementation focused on algorithmic correctness rather than performance. It serves as the baseline for verification of subsequent optimizations.
* **Model 1 (Algorithmic Improvement):** Optimizes Model 0 by implementing direct memory addressing instead of array indexing and reducing function argument overhead by utilizing global variables. It produces bit-exact output compared to Model 0.
* **Model 2 (Platform Optimization):** Tailored for the Cirrus Logic DSP platform. Floating-point arithmetic is replaced with fixed-point emulation using specific classes to ensure hardware compatibility.
* **Model 3 (Hardware Porting):** The final stage involves porting to the target platform, splitting variables into dual-access memory regions, and rewriting the core processing function in Assembly for maximum resource efficiency.
* **Final Integration:** The Assembly-optimized core is integrated into the operating system environment, handling system functions like "PreKick," "PostKick," and "Block/Brick," while incorporating MCV user controls.

## Verification & Testing
System integrity is verified at each stage by comparing the output of each model against its predecessor:
* **PCMCompare.exe:** Used for sample-by-sample bit-accuracy analysis.
* **run_test.py:** A script used to automate the testing process across different audio signals.
* **Audacity:** Utilized for visual verification and frequency spectrum analysis of the processed signals.

## Performance Profiling
The following MIPS consumption was measured for the processing function:
* **6172** - C implementation (without debug info).
* **6902** - Assembly implementation with unoptimized IIR.
* **5174** - Assembly implementation with optimized IIR.

## Setup & Configuration
The project is configured for Visual Studio with the following specifications:
* **Preprocessor Definitions:** `_CRT_SECURE_NO_WARNINGS`
* **Language Standards:** C++14 (iso 14) and C11 (iso 11)
* **Conformance Mode:** Set to `No`
