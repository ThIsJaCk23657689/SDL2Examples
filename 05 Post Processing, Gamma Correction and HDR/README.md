# SDL2 範例程式
### 05 Post Processing, Gamma Correction and HDR
本範例程式使用 SDL2 並且使用 OpenGL 3.3 版本，實現近代 OpenGL 開發以及流程，其中包含 Shader 的撰寫以及分拆程式，盡可能的實現物件導向設計的概念。

本程式實作 Post Processing、HDR、Gamma Correction。

![](https://i.imgur.com/EWW6010.png)

## 安裝
需要使用 vcpkg 安裝以下套件：
* sdl2
* glad
* glm
* imgui[core,opengl3-binding,sdl2-binding]
* stb

> 圖學環境安裝教學請[看這](https://hackmd.io/@23657689/computer_graphics_env_settings) 。

## 操作
* `w`, `s`, `a`, `d` 移動攝影機
* `Ctrl` 增加移動速度
* `LShift` 攝影機下降
* `Space` 攝影機上降
* `滑鼠` 控制攝影機 pitch 和 yaw 旋轉
* `TAB` 切換滑鼠控制攝影機（預設是關閉的），開啟時鼠標會不見且可以控制攝影機
* `1`, `2`, `3`, `4`, `5` 控制視角畫面
* `x` 隱藏或顯示三軸 (包含物體的在地坐標系、眼睛坐標系和世界坐標系)
* `Ctrl` + `q` 關閉程式

## 備註
1. 如果使用 Mingw 編譯的話，請記得 vcpkg 的套件要安裝 `x64-mingw-dynamic` 的版本，以及 CMake 需要新增 `-DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic` 以及 shader file 的換行符號要改為 `LF` 才不會發生編譯錯誤。
2. 該程式的 HDR 實做是基於 Reinhard 和 Exposure 這兩種的 Tone Mapping，使用者可以透過 GUI 來切換。
3. 有使用 Gamma Correction，所以讀取材質圖片時請用 `GL_SRGB` 或 `GL_SRGB_ALPHA`，因為通常材質圖片（指是 `diffuse texture`）都是基於 `sRGB` 色彩空間來設計與繪製，而這些顏彩空間本身就是已經 Gamma 校正過了，所以說把圖片繪製進來後再經過程式內部的 Gamma 校正後，材質貼圖會直過亮（因為已經兩次校正）；但是 `specular texture` 或 `normal texture` 就不需要，因為這些圖檔的數值就不是用於顏色顯示。
------------------------------------------------------------
國立臺灣海洋大學 資訊工程學系 電腦圖學實驗室 Lab 503

**課程名稱**：電腦圖學

**授課教授**：翁世光 教授

**課程助教**：張竣傑

**助教信箱**：10957037@email.ntou.edu.tw