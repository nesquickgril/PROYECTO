# PROYECTO
LENGUAJE DE INTERFAZ "JUEGO DEL UNO"
📌 Descripción del Proyecto

Este proyecto consiste en el desarrollo de una versión del juego UNO, utilizando una combinación de C++ y lenguaje ensamblador (NASM).

El objetivo principal es implementar la lógica del juego en bajo nivel (ensamblador), mientras que la interacción con el usuario se maneja en C++, permitiendo así comprender la integración entre ambos lenguajes.

El sistema actualmente funciona en modo consola, donde el jugador puede:

Visualizar sus cartas
Elegir una carta para jugar
Robar cartas
Validar jugadas según las reglas de UNO
Interactuar con jugadores controlados por el sistema (IA)

La validación de las jugadas se realiza en ensamblador, mediante funciones que determinan si una carta es válida dependiendo del color o valor de la carta en la mesa.

⚙️ Tecnologías Utilizadas
C++ → Interfaz del usuario
Ensamblador (NASM) → Lógica del juego
WSL (Windows Subsystem for Linux) → Entorno de desarrollo
G++ → Compilador de C++
🧠 ¿Cómo funciona?

El proyecto está dividido en dos partes principales:

main.cpp: Se encarga de la interacción con el usuario, control de turnos y visualización del juego.
uno.asm: Contiene la lógica principal del juego, como la validación de cartas.

Ambos archivos se conectan mediante el uso de extern "C", lo que permite que C++ pueda ejecutar funciones escritas en ensamblador.

🚧 Estado del Proyecto

Actualmente, el proyecto se encuentra en fase de desarrollo inicial con:

Integración funcional entre C++ y ensamblador
Validación de jugadas
Pruebas en consola

Próximamente se planea:

Implementar una interfaz gráfica con SFML
Mejorar la experiencia visual del juego
Agregar más reglas completas del UNO
🎯 Objetivo

Aplicar los conocimientos de Lenguajes de Interfaz mediante el desarrollo de un sistema interactivo, integrando programación de alto nivel (C++) con bajo nivel (ensamblador), creando una aplicación funcional y didáctica.
