#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// ================= ASM =================
extern "C" {

    int carta_valida(int jugador, int mesa);
    int reversa(int direccion);
    int verificar_uno(int cartas);
    int ganador(int cartas);
    int aplicar_robo(int valor);
    int siguiente_turno(int turno, int direccion, int jugadores);
    int efecto_especial(int valor);
    int ia_validar(int jugador, int mesa);
    int generar_valor();
    int generar_color();
    int castigo_uno(int antes, int dijoUNO);
}

// ================= CARTA =================
struct Carta {

    int color;
    int valor;
};

// ================= ARCHIVOS =================
string nFiles[] = {

    "0","1","2","3","4","5","6","7","8","9",
    "draw2","skip","reverse","wild","wild4"
};

string cFiles[] = {

    "",
    "rojo",
    "azul",
    "verde",
    "amarillo"
};

// ================= CARGAR CARTA =================
SDL_Texture* cargarTexturaCarta(
    Carta c,
    SDL_Renderer* renderer
) {

    string ruta;

    if (c.valor >= 13)
        ruta = "cartas/" + nFiles[c.valor] + ".png";

    else
        ruta =
            "cartas/" +
            cFiles[c.color] +
            "_" +
            nFiles[c.valor] +
            ".png";

    return IMG_LoadTexture(renderer, ruta.c_str());
}

// ================= GENERAR CARTA =================
Carta generarCarta() {

    Carta c;

    c.valor = generar_valor();

    if (c.valor >= 13)
        c.color = 0;

    else
        c.color = generar_color();

    return c;
}

// ================= TEXTO =================
void dibujarTexto(
    SDL_Renderer* renderer,
    TTF_Font* fuente,
    string texto,
    int x,
    int y
) {

    SDL_Color blanco = {255,255,255};

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            fuente,
            texto.c_str(),
            blanco
        );

    SDL_Texture* textura =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_Rect rect = {

        x,
        y,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(
        renderer,
        textura,
        NULL,
        &rect
    );

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(textura);
}

// ================= MAIN =================
int main(int argc, char* argv[]) {


    SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* window =
        SDL_CreateWindow(
            "UNO",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280,
            720,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI 
        );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
    
    SDL_RenderSetLogicalSize(renderer, 1280, 720);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    TTF_Font* fuente = TTF_OpenFont("Roboto-Regular.ttf", 28); 
    if (!fuente) {
        printf("Error: No se encontro Roboto-Regular.ttf en la carpeta del juego.\n");
        fuente = TTF_OpenFont("DejaVuSans.ttf", 28);
    }

    SDL_Event event;

    bool running = true;
    bool menuPrincipal = true;

    int numJ = 2;

    string textoTurno = "Tu turno";

    bool dijoUNO = false;
    int cartasARobar = 0;

    bool mostrarInventario = false;

    // ================= MENU =================
    while (menuPrincipal) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {

                running = false;
                menuPrincipal = false;
            }

            if (event.type ==
                SDL_MOUSEBUTTONDOWN) {

                int x, y;

                SDL_GetMouseState(&x, &y);

                if (x >= 450 &&
                    x <= 850 &&
                    y >= 220 &&
                    y <= 300) {

                    numJ = 2;
                    menuPrincipal = false;
                }

                if (x >= 450 &&
                    x <= 850 &&
                    y >= 340 &&
                    y <= 420) {

                    numJ = 3;
                    menuPrincipal = false;
                }

                if (x >= 450 &&
                    x <= 850 &&
                    y >= 460 &&
                    y <= 540) {

                    numJ = 4;
                    menuPrincipal = false;
                }
            }
        }

        SDL_SetRenderDrawColor(
            renderer,
            20,
            20,
            20,
            255
        );

        SDL_RenderClear(renderer);

        SDL_Rect b1 = {450,220,400,80};
        SDL_Rect b2 = {450,340,400,80};
        SDL_Rect b3 = {450,460,400,80};

        SDL_SetRenderDrawColor(renderer,200,0,0,255);
        SDL_RenderFillRect(renderer,&b1);

        SDL_SetRenderDrawColor(renderer,0,0,200,255);
        SDL_RenderFillRect(renderer,&b2);

        SDL_SetRenderDrawColor(renderer,0,180,0,255);
        SDL_RenderFillRect(renderer,&b3);

        dibujarTexto(renderer,fuente,"UNO",600,120);

        dibujarTexto(renderer,fuente,"2 JUGADORES",540,245);
        dibujarTexto(renderer,fuente,"3 JUGADORES",540,365);
        dibujarTexto(renderer,fuente,"4 JUGADORES",540,485);

        SDL_RenderPresent(renderer);
    }

    // ================= MANOS =================
    vector<vector<Carta>> manos(numJ);

    for (int i = 0; i < numJ; i++) {

        for (int j = 0; j < 7; j++) {

            manos[i].push_back(
                generarCarta()
            );
        }
    }

    Carta mesa = generarCarta();

    if (mesa.valor >= 13)
        mesa.color = generar_color();

    int turno = 0;
    int direccion = 1;
    int idGanador = -1;

    // ================= LOOP =================
    while (running) {

        if (cartasARobar > 0) {
        
            for (int k = 0; k < cartasARobar; k++) {
                manos[turno].push_back(generarCarta());
            }
            cartasARobar = 0; 
            turno = siguiente_turno(turno, direccion, numJ);
            continue; 
        }

        if (turno == 0)
            textoTurno = "Tu turno";

        else if (turno == 1)
            textoTurno = "Turno Jugador 1";

        else if (turno == 2)
            textoTurno = "Turno Jugador 2";

        else
            textoTurno = "Turno Jugador 3";

        while (SDL_PollEvent(&event)) {

            if (event.type ==
                SDL_QUIT)

                running = false;

            if (event.type ==
                SDL_MOUSEBUTTONDOWN &&
                turno == 0) {

                int x = event.button.x;
                int y = event.button.y;

                cout << "Clic logico en: X=" << x << " Y=" << y << endl;
                printf("Clic detectado en: %d, %d\n", x, y);

                // ================= CERRAR INVENTARIO =================
                if (mostrarInventario &&
                    x >= 1080 &&
                    x <= 1150 &&
                    y >= 90 &&
                    y <= 140) {

                    mostrarInventario = false;
                }

                // ================= ABRIR INVENTARIO =================
                if (manos[0].size() > 10 &&
                    x >= 1050 &&
                    x <= 1130 &&
                    y >= 575 &&
                    y <= 635) {

                    mostrarInventario = true;
                }

                // ================= INVENTARIO CLICK =================
                if (mostrarInventario) {

                    for (int i = 0;
                         i < manos[0].size();
                         i++) {

                        int fila = i / 8;
                        int columna = i % 8;

                        SDL_Rect r = {

                            140 + (columna * 120),
                            170 + (fila * 170),
                            100,
                            150
                        };

                        if (x >= r.x &&
                            x <= r.x + r.w &&
                            y >= r.y &&
                            y <= r.y + r.h) {

                            Carta c = manos[0][i];

                            int cj =
                                c.color * 100 +
                                c.valor;

                            int cm =
                                mesa.color * 100 +
                                mesa.valor;

                            if (c.valor >= 13 ||
                                carta_valida(cj, cm)) {

                                mesa = c;

                                manos[0].erase(
                                    manos[0].begin() + i
                                );

                                mostrarInventario = false;

                                turno =
                                    siguiente_turno(
                                        turno,
                                        direccion,
                                        numJ
                                    );
                            }
                        }
                    }
                }

                // ================= BOTON UNO =================
                if (x >= 1080 &&
                    x <= 1220 &&
                    y >= 560 &&
                    y <= 620) {

                    if (manos[0].size() == 2) {

                        dijoUNO = true;
                    }
                }

                // ================= ROBAR =================
                if (x >= 420 &&
                    x <= 520 &&
                    y >= 260 &&
                    y <= 410) {

                    manos[0].push_back(
                        generarCarta()
                    );
                }

                if (x >= 530 && x <= 630 && y >= 260 && y <= 310) {
                    turno = siguiente_turno(turno, direccion, numJ);
                }

                SDL_Rect rectUno = { 1080, 480, 120, 60 }; 

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);

                    if (mx >= rectUno.x && mx <= rectUno.x + rectUno.w && 
                        my >= rectUno.y && my <= rectUno.y + rectUno.h) {
                        
                        
                        if (manos[0].size() == 2) {
                            dijoUNO = true;
                            
                            SDL_Delay(100); 
                        }
                    }
                }

                // ================= JUGAR CARTA =================
                if (!mostrarInventario) {
                        int visibles = manos[0].size();
                        if (visibles > 10) visibles = 10;

                        for (int i = 0; i < visibles; i++) {
                            SDL_Rect r = { 120 + (i * 90), 540, 100, 150 };

                            if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
                                Carta c = manos[0][i];
                                int cj = c.color * 100 + c.valor;
                                int cm = mesa.color * 100 + mesa.valor;

                                if (c.valor >= 13 || carta_valida(cj, cm)) {
                                mesa = c;
                                int cartasAntes = manos[0].size(); 
                                manos[0].erase(manos[0].begin() + i);
                                
                                mostrarInventario = false;

                                    if (castigo_uno(cartasAntes, dijoUNO ? 1 : 0) == 1) {
                                        manos[0].push_back(generarCarta());
                                        manos[0].push_back(generarCarta());
                                    }
                                    dijoUNO = false; 

                                    int efecto = efecto_especial(c.valor);
                                    if (efecto == 2) { 
                                        direccion = reversa(direccion); 
                                    } 

                                    if (efecto == 1) { 
                                    turno = siguiente_turno(turno, direccion, numJ);
                                }

                                    int robo = aplicar_robo(c.valor);
                                    if (robo > 0) {
                                        cartasARobar += robo; 
                                    }


                                // ================= COMODIN =================
                                if (c.valor >= 13) {
                                    SDL_Delay(150);
                                    bool elegirColor = true;
                                    while (elegirColor && running) {
                                        while (SDL_PollEvent(&event)) {
                                            if (event.type == SDL_QUIT) { running = false; elegirColor = false; }
                                            if (event.type == SDL_MOUSEBUTTONDOWN) {

                                                int mx = event.button.x;
                                                int my = event.button.y;

                                            if (my >= 300 && my <= 400) {
                                            if (mx >= 250 && mx <= 350) { mesa.color = 1; elegirColor = false; }
                                            if (mx >= 400 && mx <= 500) { mesa.color = 2; elegirColor = false; }
                                            if (mx >= 550 && mx <= 650) { mesa.color = 3; elegirColor = false; }
                                            if (mx >= 700 && mx <= 800) { mesa.color = 4; elegirColor = false; }
                                        }
                                        }
                                    }
                                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                        SDL_RenderClear(renderer);
                                        dibujarTexto(renderer, fuente, "ELIGE UN COLOR", 430, 180);
                                        SDL_Rect rR = {250,300,100,100}, rA = {400,300,100,100}, rV = {550,300,100,100}, rAm = {700,300,100,100};
                                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); SDL_RenderFillRect(renderer, &rR);
                                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); SDL_RenderFillRect(renderer, &rA);
                                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); SDL_RenderFillRect(renderer, &rV);
                                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); SDL_RenderFillRect(renderer, &rAm);
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(16);
                                    }

                            
                                }

                                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

                                turno = siguiente_turno(turno, direccion, numJ);
                               
                                break; 
                            }
                        }
                    }
                }
            }
        }

        // ================= IA =================
        
        if (turno != 0 && running) {
            SDL_Delay(600); 
            
            bool jugo = false;
            int sizeMano = manos[turno].size();

            for (int i = 0; i < sizeMano; i++) {
                Carta c = manos[turno][i];
                int cj = c.color * 100 + c.valor;
                int cm = mesa.color * 100 + mesa.valor;

                if (ia_validar(cj, cm)) {
                    mesa = c;
                    manos[turno].erase(manos[turno].begin() + i);
                    jugo = true;

                    int efecto = efecto_especial(mesa.valor);
                    
                    if (efecto == 2) { 
                        direccion = reversa(direccion); 
                    }
                    
                    if (efecto == 1) { 
                        
                        turno = siguiente_turno(turno, direccion, numJ);
                    }

                    int robo = aplicar_robo(mesa.valor);
                    if (robo > 0) {
                        cartasARobar += robo;
                    }

                    if (mesa.valor >= 13) {
                        mesa.color = (rand() % 4) + 1;
                    }

                    break;
                }
            }

            if (!jugo) {
                manos[turno].push_back(generarCarta());
            }

            turno = siguiente_turno(turno, direccion, numJ);
            
            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
        }

        // ================= VERIFICAR GANADOR =================

        for (int i = 0; i < numJ; i++) {
            if (ganador(manos[i].size()) == 1) {
                idGanador = i;
                running = false; 
            }
        }

        // ================= DIBUJAR =================
        SDL_SetRenderDrawColor(
            renderer,
            0,
            120,
            0,
            255
        );

        SDL_RenderClear(renderer);

        // ============= Botón de UNO =============

        SDL_Rect rectUno = { 1080, 480, 120, 60 };

        if (dijoUNO) {
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // Verde si ya dijo
        } else {
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); // Rojo si falta decir
        }

        SDL_RenderFillRect(renderer, &rectUno);
        dibujarTexto(renderer, fuente, "UNO!", 1105, 495);

        // ================= TURNO =================
        SDL_Rect barra = {

            380,
            20,
            500,
            60
        };

        SDL_SetRenderDrawColor(
            renderer,
            20,
            20,
            20,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &barra
        );

        dibujarTexto(
            renderer,
            fuente,
            textoTurno,
            520,
            35
        );

        // ================= MAZO =================
        SDL_Texture* texMazo =
            IMG_LoadTexture(
                renderer,
                "cartas/mazo.png"
            );

        SDL_Rect mazo = {

            420,
            260,
            100,
            150
        };

        SDL_RenderCopy(
            renderer,
            texMazo,
            NULL,
            &mazo
        );

        SDL_DestroyTexture(texMazo);

        // ================= MESA =================
        SDL_Texture* texMesa =
            cargarTexturaCarta(
                mesa,
                renderer
            );

        SDL_Rect rMesa = {

            650,
            260,
            100,
            150
        };

        SDL_RenderCopy(
            renderer,
            texMesa,
            NULL,
            &rMesa
        );

        SDL_DestroyTexture(texMesa);

        SDL_Rect btnPasar = { 530, 260, 100, 50 };
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Color Gris
        SDL_RenderFillRect(renderer, &btnPasar);
        dibujarTexto(renderer, fuente, "PASO", 545, 270);

        // ================= CARTAS =================
        if (!mostrarInventario) {

            int visibles = manos[0].size();

            if (visibles > 10)
                visibles = 10;

            for (int i = 0;
                 i < visibles;
                 i++) {

                SDL_Texture* tex =
                    cargarTexturaCarta(
                        manos[0][i],
                        renderer
                    );

                SDL_Rect r = {

                    120 + (i * 90),
                    540,
                    100,
                    150
                };

                SDL_RenderCopy(
                    renderer,
                    tex,
                    NULL,
                    &r
                );

                SDL_DestroyTexture(tex);
            }

            if (manos[0].size() > 10) {

                SDL_Rect botonMas = {

                    1050,
                    575,
                    80,
                    60
                };

                SDL_SetRenderDrawColor(
                    renderer,
                    40,
                    40,
                    40,
                    255
                );
             
                SDL_RenderFillRect(
                    renderer,
                    &botonMas

                );

                dibujarTexto(

                    renderer,
                    fuente,
                    "...",
                    1075,
                    575

                );
            }
        }

        // ================= INVENTARIO =================
        if (mostrarInventario) {

            SDL_Rect fondo = {

                80,
                80,
                1120,
                560
            };

            SDL_SetRenderDrawColor(

                renderer,
                0,
                0,
                0,
                240

            );

            SDL_RenderFillRect(

                renderer,
                &fondo


            );

            dibujarTexto(

                renderer,
                fuente,
                "TUS CARTAS",
                500,
                100

            );

            SDL_Rect cerrar = {

                1080,
                90,
                70,
                50

            };

            SDL_SetRenderDrawColor(

                renderer,
                180,
                0,
                0,
                255

            );

            SDL_RenderFillRect(
                renderer,
                &cerrar

            );

            dibujarTexto(

                renderer,
                fuente,
                "X",
                1105,
                95

            );

            for (int i = 0;
                 i < manos[0].size();
                 i++) {

                SDL_Texture* tex =
                    cargarTexturaCarta(
                        manos[0][i],
                        renderer
                    );

                int fila = i / 8;
                int columna = i % 8;

                SDL_Rect r = {

                    140 + (columna * 120),
                    170 + (fila * 170),
                    100,
                    150
                };

                SDL_RenderCopy(
                    renderer,
                    tex,
                    NULL,
                    &r
                );

                SDL_DestroyTexture(tex);
            }
        }

        // ================= MOSTRAR CARTAS DE LAS IA =================

        for (int i = 1; i < numJ; i++) {
            string textoIA = " IA " + to_string(i) + ": " + to_string(manos[i].size()) + " cartas";
            
            int posY = 100 + (i * 40); 
            
            SDL_Rect fondoTexto = { 1040, posY - 5, 220, 35 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
            SDL_RenderFillRect(renderer, &fondoTexto);

            dibujarTexto(renderer, fuente, textoIA, 1050, posY);
        }

        SDL_RenderPresent(renderer);

    }

    bool mostrandoResultado = true;
    while (mostrandoResultado) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_MOUSEBUTTONDOWN) 
                mostrandoResultado = false;
        }
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        string mensaje = (idGanador == 0) ? "FELICIDADES HAS GANADO!" 
                                         : "EL JUGADOR " + to_string(idGanador) + " HA GANADO";

        SDL_Rect panel = {340, 260, 600, 200};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &panel);

        dibujarTexto(renderer, fuente, mensaje, 400, 330);
        dibujarTexto(renderer, fuente, "Haz clic para salir", 500, 400);
        SDL_RenderPresent(renderer);

    }

    TTF_CloseFont(fuente);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}