---
title: "Neuro-Inspired Computing on the SoC FPAA"
excerpt: "Undergraduate research implementing Hopfield networks on a reconfigurable analog chip (FPAA) to solve NP-hard Max-Cut problems with analog annealing.<br/>*Georgia Tech-Europe — Spring 2026*"
collection: portfolio
---

**Role:** Undergraduate Research Assistant  
**Advisor:** Dr. Alexandre Locquet · Georgia Institute of Technology (Georgia Tech-Europe), Metz, France  
**Period:** Spring 2026  
**Tech:** FPAA (SoC 3.0 A), analog computing, CADSP/Xcos, Scilab, React/JavaScript, Digilent Analog Discovery

---

Neuro-inspired analog computing research on the **SoC FPAA** (Field-Programmable Analog Array) — a reconfigurable analog chip that acts as a "universal analog breadboard," where circuits are specified in software and the chip programs itself. The work implemented **Hopfield networks** on the FPAA to solve the **Max-Cut** problem (NP-hard) and experimentally investigated **analog annealing** to escape local energy minima.

**Key contributions:**

- **Hopfield-on-FPAA implementation** — mapped Max-Cut onto a Hopfield network: one neuron per graph node, weights constructed so connected nodes are penalized for sharing a partition. Stable states correspond to high-quality graph cuts.
- **Differential-pair conductance encoding** — encoded network weights into the FPAA's fixed 8×6 weight matrix using positive/negative conductance row pairs. Current magnitudes ranged from ~1 pA (no connection) to 100–300 nA (recurrent weights) to µA (forcing biases), within a <10 µA per-neuron budget.
- **Analog annealing experiments** — applied external kick signals through weight-matrix input columns to perturb the energy landscape and let the network reconverge. Swept kick parameters on a 4-neuron network: waveform shape, rise time, amplitude (~2.5 V most effective), duration (50–300 µs), and target neuron (most profound impact on resulting state).
- **Hopfield Weight Matrix Designer** — built an interactive React/JavaScript single-file web tool that lets users define a graph (nodes, edges, initial states, forcing), automatically computes the Hopfield weight matrix, and generates the Scilab FPAA configuration. This automated a previously manual matrix-derivation step and supports SVG graph export.
- **Circuit bring-up** — learned the FPAA toolchain (CADSP/Xcos → hardware config → program chip) by building and verifying simple circuits (e.g. low-pass filter validated with a Bode sweep) using a Digilent Analog Discovery as waveform generator and oscilloscope.

**Publication:** *Neuro-Inspired Computing on the SoC FPAA* — Spring 2026 Undergraduate Research Report, Georgia Tech-Europe.
