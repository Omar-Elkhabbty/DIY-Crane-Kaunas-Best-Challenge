# 🏗️ Smart Remote-Controlled Crane System — EBEC Challenge

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/)
[![Event: EBEC Challenge](https://img.shields.io/badge/Event-EBEC_Kaunas-orange.svg)](https://www.bestkaunas.org/)

An embedded hardware and mechanical engineering project developed for the **EBEC (European BEST Engineering Competition) Challenge — "Tower of Babel"**. 

The goal of this challenge was to design, build, and mathematically model a fully remote-controlled motorized crane system using restricted physical materials within a strict **48-hour time limit**.

---


## 📌 Project Objectives & Competition Rules

* **Mission:** Safely lift and place payload objects to 4 target heights ($15\text{ cm}$, $25\text{ cm}$, $35\text{ cm}$, and maximum reachable height) onto specified target spots.
* **Constraints:** 
  * 100% remote-controlled (no human intervention or physical contact allowed during operation).
  * Materials restricted strictly to those provided in the challenge kit (cardboard, paper, duct tape, skewers, thread, rubber bands, cable ties).
  * 8-hour total design and construction timeframe.

---

## 📐 Mechanical Engineering & Physics Calculations

To prevent tipping and structural failure under load, the crane's geometry and mass distribution were optimized using static equilibrium principles.

### 1. Static Equilibrium Equations
For the crane structure to remain stable under maximum payload lift:

$$\sum F_x = 0, \quad \sum F_y = 0, \quad \sum \tau = 0$$

### 2. Counterweight & Moment Balance
Taking the pivot point $O$ at the base frame edge, the balancing equation between counterweight $m_{\text{cw}}$ and load $m_{\text{load}}$ is expressed as:

$$m_{\text{cw}} \cdot d_{\text{cw}} + m_{\text{base}} \cdot d_{\text{base}} \ge (m_{\text{boom}} \cdot d_{\text{boom}}) + (m_{\text{gripper}} + m_{\text{payload}}) \cdot d_{\text{payload}}$$

Where:
* $m_{\text{payload}}$: Weight of the object being lifted.
* $d_{\text{payload}}$: Moment arm length from pivot $O$ to the load center of mass.
* $m_{\text{cw}}$: Counterweight mass located at distance $d_{\text{cw}}$ on the rear side.

### 3. Motor Shaft Torque Calculation
The required winch motor torque $\tau_{\text{motor}}$ to hoist the payload vertically at speed $v$ is given by:

$$\tau_{\text{hoist}} = R_{\text{spool}} \cdot (m_{\text{payload}} + m_{\text{gripper}}) \cdot g \cdot \frac{1}{\eta}$$

Where:
* $R_{\text{spool}}$: Radius of the hoisting thread spool.
* $g$: Acceleration due to gravity ($9.81\text{ m/s}^2$).
* $\eta$: Mechanical efficiency factor of the pulley system ($\eta \approx 0.85$).

---

## 🔌 Embedded Electronics & Circuitry

The control electronics consist of an **ESP32 microcontroller** interfacing with a **Joy-It SBC-MotoDriver2** board driving 4 DC gear motors.

### System Components:
1. **ESP32 Microcontroller Board:** Generates control signals, PWM motor speeds, and manages wireless logic communication.
2. **Joy-It SBC-MotoDriver2 (v2.0):** Dual/Quad motor driver board routing power and control signals to actuators.
3. **4x TT DC Geared Motors:**
   * Motor 1-1 / Motor 1-2: Base movement / Boom elevation drive.
   * Motor 2-1 / Motor 2-2: Winch drum hoisting & Gripper actuation.
4. **Power Supply:** 4x AAA 1.5V batteries connected in series providing a $6.0\text{V DC}$ total power bus.

### Pinout Mapping:

```
+-------------------+----------------------------+
| ESP32 GPIO Pin    | Joy-It MotoDriver Pin      |
+-------------------+----------------------------+
| GPIO / PWM Out 1  | Motor 1 Control Input (A)  |
| GPIO / PWM Out 2  | Motor 1 Control Input (B)  |
| GPIO / PWM Out 3  | Motor 2 Control Input (A)  |
| GPIO / PWM Out 4  | Motor 2 Control Input (B)  |
| 3.3V / 5V         | VCC Logic                  |
| GND               | Common Ground (GND)        |
+-------------------+----------------------------+
```

---

## 🕹️ Control Logic & Operations

The embedded control logic handles multi-axis execution required for precise object picking and placement:

```
                 [ User Remote Commands ]
                            │
                            ▼
                     [ ESP32 Logic ]
                            │
        ┌───────────────────┼───────────────────┐
        ▼                   ▼                   ▼
 [ Frame Drive ]     [ Winch Hoist ]    [ Gripper Claw ]
 (Forward / Rev)      (Up / Down)        (Open / Close)
```

1. **Horizontal Mobility:** Precise linear positioning towards the object zone.
2. **End-Effector Gripping:** Actuation of cardboard scissor-jaw mechanism using cable/thread tension.
3. **Vertical Lifting:** Winch spool winding to elevate objects to $15\text{ cm}$, $25\text{ cm}$, $35\text{ cm}$, and max height.
4. **Placement:** Controlled unwinding and grip release.

---

## 🧰 Materials & Tools List (Challenge Constraints)

### Provided Materials:
* A4 Paper (20 sheets) & Cardboard (1 sheet)
* Duct Tape (1 roll) & Double-sided Tape
* Wooden Skewers (10x), Thread (3m), Wire (2m)
* Cable Ties (15x), Rubber Bands (10x), Paperclips (10x)
* 4x TT DC Motors & Battery Pack ($6\text{V}$)

### Utility Tools Used:
* Hot Glue Gun & Cordless Drill
* Utility Knife, Scissors, Measuring Tape, Soldering Iron

---

## 🏆 Evaluation & Results

The prototype was tested and judged according to EBEC evaluation standards:
* **Functionality & Task Completion:** Successfully lifted and positioned targeted loads without dropping.
* **Structural Stability:** Zero tipping recorded under maximum cantilever load.
* **Aesthetics & Material Utility:** Innovative cardboard truss structure and effective counterweight integration.

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).
