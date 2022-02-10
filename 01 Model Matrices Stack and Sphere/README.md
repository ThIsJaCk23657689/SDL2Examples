# SDL2 範例程式
### 01 Model Matrices Stack and Sphere
本範例程式使用 SDL2 並且使用 OpenGL 3.3 版本，實現近代 OpenGL 開發以及流程，其中包含 Shader 的撰寫以及【簡易的】分拆程式，盡可能的實現物件導向設計的概念，但為了避免過度複雜，絕大多數程式還是寫在了 `main.cpp` 程式當中。

這個範例程式主要是展現如何實作 Model Matrices Stack、球體頂點生成以及現代 OpenGL 必須所碰到的 VAO、VBO 和 EBO 的處理方式（設計方式），整個場景是一個很簡單的方式呈現，一個太陽、一個地球和一個月亮，透過不專業的自轉和公轉的速度還表現　Model Matrix 對於物件所帶來的影響差異。

![](https://i.imgur.com/UInMfAk.png)

## 安裝
需要使用 vcpkg 安裝以下套件：
* sdl2
* glad
* glm
* imgui[core,opengl3-binding,sdl2-binding]

> 圖學環境安裝教學請[看這](https://hackmd.io/@23657689/computer_graphics_env_settings) 。

## 操作
* `Ctrl` + `Q` 關閉程式
------------------------------------------------------------
國立臺灣海洋大學 資訊工程學系 電腦圖學實驗室 Lab 503

**課程名稱**：電腦圖學

**授課教授**：翁世光 教授

**課程助教**：張竣傑

**助教信箱**：10957037@email.ntou.edu.tw