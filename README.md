# AI-POWERED-HANDHELD-DEVICE-FOR-MALARIA-AND-ANEMIA-DETECTION
An internet-independent, TinyML point-of-care medical scanner built on the ESP32-CAM. Uses a quantized INT8 MobileNetV2 model via TensorFlow Lite for Microcontrollers to perform real-time, edge-based Malaria and Anemia screening from blood smear slides. Peer-reviewed and published in IJSET (2026).
# 🩸 Edge-AI Assisted Digital Hematoscope for Rapid Malaria & Anemia Screening

A TinyML-powered handheld diagnostic device that performs **real-time Malaria and Anemia detection** using microscopic blood smear images on an **ESP32-CAM**. The system executes AI inference entirely on-device, eliminating the need for cloud connectivity and enabling rapid, low-cost diagnostics in rural and resource-constrained healthcare environments.

---

## 📖 Project Overview

This project demonstrates the implementation of an **Edge AI-based portable diagnostic device** capable of identifying **Malaria (Plasmodium vivax)** and **Anemia** from Giemsa-stained blood smear images.

The diagnostic model is trained using **TensorFlow** and optimized with **TensorFlow Lite for Microcontrollers (TinyML)** before deployment on an **ESP32-CAM**. The device captures microscopic images, performs image classification locally, displays the diagnosis on an OLED display, and stores results for future reference.

This solution is designed for:

- Rural Primary Health Centers (PHCs)
- Mobile healthcare camps
- Community health workers
- Resource-constrained medical facilities
- Point-of-care diagnostics

---

# ✨ Features

- 🧠 TinyML-based Edge AI inference
- 📷 ESP32-CAM image acquisition
- 🔬 Malaria & Anemia detection
- 📟 OLED result display
- 💾 Local data storage
- ⚡ Offline operation (No Internet Required)
- 🔋 Battery-powered portable device
- 🚀 Fast real-time inference
- 📉 Low memory footprint using INT8 Quantization

---

# 🛠 Hardware Components

| Component | Description |
|-----------|-------------|
| ESP32-CAM | Main controller and camera module |
| OV2640 Camera | Blood smear image capture |
| OLED Display (128×64) | Display prediction results |
| FTDI Programmer | ESP32 programming |
| MicroSD Card | Image and result storage |
| Macro Lens | Microscopic imaging |
| LED Illumination | Uniform lighting |
| USB Power Bank | Portable power source |

---

# 💻 Software & Technologies

- Arduino IDE
- Edge Impulse
- TensorFlow Lite for Microcontrollers
- TensorFlow
- Keras
- Python
- TinyML
- Embedded C++
- ESP32 Camera Library

---

# 🧠 AI Model

| Parameter | Value |
|-----------|-------|
| Model | MobileNetV2 |
| Framework | TensorFlow Lite Micro |
| Optimization | INT8 Quantization |
| Deployment | ESP32-CAM |
| Inference | Edge AI |

---

# 📂 Repository Structure

```
├── Code/
│   ├── Arduino Source Code
│   ├── ESP32 Firmware
│   └── TinyML Integration
│
├── Dataset/
│   ├── Training Dataset
│   └── Dataset Information
│
├── examples/
│   └── Example Programs
│
├── Malaria_Detection__inferencing.h
│
├── library.properties
│
├── PROJECT REPORT.pdf
│
└── README.md
```

---

# ⚙️ System Workflow

```
Blood Smear Slide
        │
        ▼
ESP32-CAM Image Capture
        │
        ▼
Image Preprocessing
        │
        ▼
TinyML Inference
(TensorFlow Lite Micro)
        │
        ▼
Disease Classification
        │
 ┌────────────────────┐
 │ OLED Display Result │
 └────────────────────┘
        │
        ▼
Save Results (Optional)
```

---

# 🚀 Getting Started

## 1. Clone Repository

```bash
git clone https://github.com/VinodGowda651/Edge-AI-Digital-Hematoscope.git
```

---

## 2. Open Arduino IDE

Open the firmware located inside the **Code** directory.

---

## 3. Install Required Libraries

Install the following libraries:

- ESP32 Board Package
- TensorFlow Lite for Microcontrollers
- ESP32 Camera Library
- U8g2 OLED Library
- SPI
- Wire

---

## 4. Upload the Firmware

1. Connect ESP32-CAM using FTDI.
2. Select the correct COM Port.
3. Choose **AI Thinker ESP32-CAM**.
4. Upload the firmware.

---

## 5. Run

After uploading:

- Capture blood smear image
- AI processes image
- Result displayed on OLED
- Data stored locally (if enabled)

---

# 📊 Results

The prototype successfully demonstrates:

- Real-time malaria detection
- Anemia screening
- TinyML deployment on ESP32
- Offline inference
- Low-power operation
- Portable diagnostics

---

# 📸 Project Images

You can add images here.

```
Prototype Image

OLED Output

Healthy Sample

Infected Sample

Circuit Diagram

System Block Diagram
```

---

# 🎯 Applications

- Point-of-Care Diagnostics
- Rural Healthcare
- Mobile Clinics
- Healthcare Research
- Embedded AI
- Medical IoT
- TinyML Applications

---

# 🔮 Future Improvements

- Multi-disease detection
- Cloud synchronization
- Mobile application
- Higher resolution imaging
- Improved dataset
- Custom PCB
- Better optical system
- Clinical validation

---

# 📚 Documentation

Complete project documentation is available in:

```
PROJECT REPORT.pdf
```

The report includes:

- Introduction
- Literature Survey
- Methodology
- Hardware Design
- Software Implementation
- AI Model Development
- Results
- Future Scope

---

# 👨‍💻 Author

**Vinod Gowda**

Electronics & Communication Engineering

Embedded Systems | TinyML | Edge AI | IoT | Computer Vision

GitHub: **https://github.com/VinodGowda651**

---

# 🤝 Acknowledgements

Special thanks to:

- Akshaya Institute of Technology
- Department of Electronics & Communication Engineering
- Project Guide
- TensorFlow Lite Team
- Edge Impulse
- ESP32 Community
- NIH Malaria Dataset

---

# 📜 License

This project is developed for educational, research, and academic purposes.

Feel free to fork, learn, and contribute.

---

## ⭐ Support

If you found this project helpful,

⭐ Star this repository

🍴 Fork the repository

🐞 Report issues

🤝 Contribute improvements

---

> **"Bringing AI-powered healthcare diagnostics to resource-constrained communities through TinyML and Edge Computing."**
