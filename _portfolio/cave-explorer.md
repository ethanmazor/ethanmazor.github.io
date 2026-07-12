---
title: "Embedded Cave Explorer Game"
excerpt: "Handheld dungeon crawler on an ARM Cortex-M3 microcontroller — sparse hash-table world map, differential UART renderer, C/C++.<br/>*Georgia Tech — Fall 2025*"
collection: portfolio
---

Source files: [GitHub folder](https://github.com/ethanmazor/ethanmazor.github.io/tree/main/projects/embedded-cave-explorer)

---

**Course:** Programming for Hardware/Software Systems · Georgia Tech · Fall 2025  
**Platform:** NXP LPC1768 (ARM Cortex-M3, 96 MHz, 64 KB SRAM) · mbed HAL · uLCD-144 serial display  
**Tech:** C, C++, UART, hash tables, embedded systems

---

A fully playable handheld dungeon/cave explorer game running on an NXP LPC1768 ARM Cortex-M3 microcontroller. The game engine, sparse map data structure, differential renderer, and all game logic were written from scratch in C/C++ on top of the mbed HAL.

**Key engineering highlights:**

- **Sparse world map** — custom separately-chained hash table keyed by packed `(x,y)` coordinates. Only occupied tiles consume memory, fitting a large multi-map game world within 64 KB SRAM — far more efficient than a dense 2D array for a sparse cave.
- **Differential renderer** — dirty-tile draw algorithm redraws only tiles that changed since the previous frame, dispatched through per-tile function pointers. Cuts UART traffic enough to sustain a 10 Hz refresh over a 115200-baud serial link.
- **Fixed-timestep game loop** — hardware `Timer` measures per-frame update time and pads to exactly 100 ms, enforcing a deterministic 10 Hz update rate under real-time constraints.
- **Function-pointer polymorphism** — each `MapItem` carries a `DrawFunc` pointer for rendering and behavior dispatch without virtual functions or RTTI overhead in a memory-constrained environment.
- **Polled input + software debounce** — pushbuttons and a 5-way navigation switch read via `DigitalIn`/`BusIn` with ~180 ms debounce guards, maintaining responsive control in a single-threaded loop.

## Demo

<div style="margin: 1em 0;">
  <a href="https://gtvault-my.sharepoint.com/:v:/g/personal/emazor7_gatech_edu/IQD1xz1DCWc0Rr_5C5jOqDVzAYGT6blx_ixtYFk__zXgBQY?e=mQE8YK&nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJTdHJlYW1XZWJBcHAiLCJyZWZlcnJhbFZpZXciOiJTaGFyZURpYWxvZy1MaW5rIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXcifX0%3D" target="_blank" rel="noopener">
    ▶ Watch Demo Video
  </a>
</div>
