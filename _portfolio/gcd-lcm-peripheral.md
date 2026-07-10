---
title: "SCOMP GCD/LCM Hardware Peripheral"
excerpt: "Memory-mapped GCD/LCM co-processor peripheral in VHDL, integrated into the SCOMP 16-bit processor on a Cyclone V FPGA.<br/>*Georgia Tech — Fall 2025*"
collection: portfolio
---

**Course:** Introduction to Systems and Computer Organization (SCOMP) · Georgia Tech · Fall 2025  
**Platform:** Intel Cyclone V FPGA · Quartus Prime  
**Tech:** VHDL, FSM design, memory-mapped I/O, FPGA synthesis

---

A memory-mapped GCD/LCM co-processor peripheral integrated into the SCOMP 16-bit educational processor I/O bus. The peripheral is fully synthesized and deployed on an Intel Cyclone V FPGA.

**Key engineering highlights:**

- **Memory-mapped I/O interface** — peripheral sits at addresses `0x90–0x94` on the SCOMP I/O bus with custom address decoding logic; defines the full hardware-software interface.
- **6-state Moore FSM** — `IDLE → LOAD → COMPARE → A_GT_B / A_LT_B → DONE` executing the Euclidean subtraction algorithm with dual GCD/LCM output modes from the same hardware datapath.
- **Tri-state bus sharing** — `lpm_bustri` drives `IO_DATA` only when addressed (`IO_READ` asserted + correct `IO_ADDR`), presenting high-impedance otherwise to prevent bus contention.
- **In-progress lock register** — `IN_PROGRESS` signal blocks `START` writes during computation, protecting intermediate register values from corruption by concurrent processor accesses.
- **LCM extension** — single `MODE` bit selects LCM output: `OUTPUT = (A × B) / GCD`, with division-by-zero protection and 16-bit width to accommodate `LCM(255, 254) = 32,385`.

## Demo

<div style="margin: 1em 0; position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden;">
  <iframe
    src="https://www.youtube.com/embed/X5V4lPkyZoM"
    style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; border: 0;"
    allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
    allowfullscreen>
  </iframe>
</div>
