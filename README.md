# PROYECTO
LENGUAJE DE INTERFAZ "JUEGO DEL UNO"

Descripción del Proyecto

Este proyecto consiste en el desarrollo de una versión del juego UNO, utilizando una combinación de C++ y lenguaje ensamblador (NASM).

## Características del Proyecto
* **Interfaz Gráfica:** Desarrollada con **SDL2**, permitiendo una experiencia visual fluida.
* **Lógica en Ensamblador:** Los cálculos de turnos, validación de cartas, efectos especiales y el castigo del "UNO" están implementados en **ASM (NASM)** para maximizar la eficiencia.
* **IA Competitiva:** Jugadores automáticos que validan sus jugadas mediante lógica integrada.
* **Inventario Dinámico:** Gestión de manos grandes mediante una interfaz de inventario expandible.

---

## 📁 Estructura del Repositorio
* `main.cpp`: Código fuente principal en C++.
* `uno.asm`: Lógica de juego escrita en Ensamblador x86_64.
* `cartas/`: Directorio con todos los assets gráficos (.png).
* `*.ttf`: Fuente tipográfica utilizada en la interfaz.

---

## 🛠️ Requisitos para Compilar
Para compilar este proyecto desde cero, necesitarás:
1.  **MinGW-w64** (Compilador `g++`).
2.  **NASM** (Netwide Assembler).
3.  **Librerías SDL2**:
    * SDL2 (Core)
    * SDL2_image
    * SDL2_ttf

---

## 💻 Instrucciones de Compilación (Windows)

1.  **Compilar el archivo de Ensamblador:**
    ```bash
    nasm -f win64 uno.asm -o uno.obj
    ```

2.  **Compilar el ejecutable final vinculando C++ y el objeto ASM:**
    ```bash
    g++ main.cpp uno.obj -o juego_uno.exe -lSDL2 -lSDL2_image -lSDL2_ttf
    ```
    o
    ```bash
    g++ main.cpp uno.obj -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lgdi32 -lusp10 -lole32 -luuid -lwinmm -lrpcrt4 -lsetupapi -limm32 -lversion -loleaut32 -lcfgmgr32 -o juego_uno.exe
    ```

---

## 🎮 Cómo Jugar
Si solo deseas probar el juego sin configurar el entorno de desarrollo:
1.  Ve a la sección de [**Releases**](https://github.com/nesquickgril/PROYECTO/releases/tag/v1.0) de este repositorio.
2.  Descarga el archivo `UNO.zip`.
3.  Descomprime el contenido en una carpeta.
4.  Ejecuta `juego_uno.exe`.

