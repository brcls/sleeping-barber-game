#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>
static GLint imagewidth;
static GLint imageheight;
static GLint pixellength;
static GLubyte *pixeldata;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "barber.h"
#include "semaphore.h"

#define FILENAME "bobage"
#define QTDIMGS 10

//Estados do barbeiro
#define EMPEH       2
#define CORTANDO    1
#define DESCANSANDO 0

//estados  do programa
#define EXECUTANDO 1
#define PARADO 0

//configuração inicial do barbeiro
int Barber::energy = 1000;
int Barber::state=EMPEH;

#define CHAIRS 3 /*quantidade de cadeiras para clientes*/

std::mutex mtx; // mutex que controla as regioes crirticas

unsigned int score = 0;
int executing = EXECUTANDO;

Barber barber; // classe que controla o barbeiro
Semaphore customers (0);/*clientes esperando pelo serviço*/
Semaphore mutex  (1); /*para exclusão mútua*/
int waiting = 0; /*quantidade de clientes*/

/*Funcao: display
Descricao: mostra as imagens e outras informações relevantes na tela
*/
void display(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //Draw a pixel
    glDrawPixels(imagewidth, imageheight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixeldata);
    
    //---------------------------------

    std::cout << "display energy: " << barber.energy;
    
    // Mostra a quantidade de energia na tela
	char barberEnergy[25]={'\0'};
	sprintf(barberEnergy, "Energia: %d", barber.energy);// Text
	glColor3f(1.0,1.0,1.0);// Text color
	glRasterPos2f(-0.9,-0.9);// Print position
	// Print each char
	for(int j=0;j<30;j++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, barberEnergy[j]);
    }

    glFlush();
    glutSwapBuffers();
}

/*Funcao: openAssets
Descricao: abre as imagens para podermos usar durante o funcionamento do jogo
*/
FILE** openAssets (void)
{
    //abre os assets
    FILE **images = (FILE **)malloc(QTDIMGS * sizeof(FILE*));
    images[0] = fopen("assets/cut1.bmp", "rb");
    
    if (images[0] == 0)
        {
            std::cout << "images not found";
            exit(0);
        }

    return images;
    // for (int i = 0; i < QTDIMGS; i++)
    //     if (images[i] == 0)
    //     {
    //         cout << "images not found";
    //         exit(0);
    //     }
}

/*Funcao: openAssets
Descricao: fecha todas as imagens
*/
void closeAssets (FILE** images)
{
    for (int i = 0; i < 10; i++)
    {
        fclose(images[i]);
    }
}

/*Funcao: Keyboard
Descricao: lista qual teclas o jogo ouve e o que fazer com tais teclas
*/
void Keyboard(unsigned char key, int x, int y)
{
    int w = x + y;
    w++;
    //ENTER
    if (key == 13){
	 	std::cout << "carambolas";
	 	glutPostRedisplay();
    }
}


/*Funcao: cutting
Descricao: faz toda a regulação de semaforos e mutex na hora decortar o cabelo
*/
void cutting ()
{
    customers.down();
    mtx.lock();
    waiting -= 1;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barber.cut_hair();
    mtx.unlock();
    std::cout << "Cabelo do cliente cortado\n";
    score += 500;
}

/*Funcao: customer
Descricao: faz toda a regulação de semaforos e mutex na hora de adicionar um novo cliete
*/
void customer ()
{
    mtx.lock();
   if (waiting < CHAIRS)
   {
      waiting = waiting + 1;
      customers.up();
      mtx.unlock();
      std::cout << "Um novo cliente chega\n";
    }
    else
    {
        executing = PARADO;
        barber.state=EMPEH;
    }

    glutPostRedisplay();
}

/*Funcao: generate_customer
Descricao: adiciona um novo cliente a cada 5-10s, esse valor diminui chegando a um cliente a cada 0-5s na pontuacao maxima
*/
void generate_customer() {
    std::cout << "Entrou na thread de gerar\n";
    int dificuldade = score/5000;
    if (dificuldade >=5)
        dificuldade = 5;
    int max = 5, cTime;
    srand(time(0));
    cTime = (rand() % max) + (5-dificuldade);
    std::cout << cTime;
    std::this_thread::sleep_for(std::chrono::seconds(cTime));
    customer();
    generate_customer();
}

/*Funcao: get_input
Descricao: fica rodando em uma thread propria detectando qualquer input que o player digite
Parâmetros: int* exec, ponteiro de inteiro que define se o jogo exta ou não sendo executado
*/
void get_input(int *exec) {
    // std::cout << "Entrou na thread de pegar input\n";
    char input;
    std::cin >> input;
    if(*exec == EXECUTANDO){
        if (input == 'z'){
            if (customers.get_sem() > 0){
                cutting();
                std::cout << "sua pontuacao é: " << score << '\n';
            }
            else{
                std::cout << "não havia nenhum cliente na loja";
                barber.state = EMPEH;
            }
        }
        else if (input == 'x'){
            barber.sleep();
        }
        else if (input == 'q'){
            *exec = PARADO;
        }
    }
    else
        *exec = PARADO;
    if (*exec == EXECUTANDO)
        get_input(exec);
}

/*Funcao: drenar_energia
Descricao: função que fica em uma thread executando constantemente drenando energia se o barbeiro esta de pé e nao fazendo nada se ele está em outro estado
*/
void drenar_energia(int *exec) {
    std::cout << barber.state << std::endl;
    if (*exec == PARADO)
        return;
    if (barber.state == EMPEH && *exec ==EXECUTANDO) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (barber.energy > 0){
            barber.energy -=50;
            std::cout << "sua energia é:" << barber.energy << "\n";
        } else {
            *exec = PARADO;
            return;
        }
    }
    while (barber.state != EMPEH && *exec == EXECUTANDO) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    glutPostRedisplay();
    drenar_energia(exec);
}

void initGame(int)
{
    std::thread drenar(drenar_energia, &executing);
    std::thread generation(generate_customer);
    
    while (executing) {
        glutMainLoopEvent();
    }

    //Ponto de atenção (continua reexecutando?) ====
    generation.join();
    drenar.join();
}

// função responasavel por toda execução do codigo
int main(int argc, char *argv[])
{

    //open a file
    FILE *pfile = fopen("assets/cut0.bmp", "rb");
    if (pfile == 0)
    {
        printf("image not found\n");
        exit(0);
    }

        
    //Read image size
    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
    fread(&imageheight, sizeof(imageheight), 1, pfile);
    //Calculate pixel data length
    pixellength = imagewidth * 3;
    while (pixellength % 4 != 0)
        pixellength++;
    pixellength *= imageheight;
    //Read pixel data
    pixeldata = (GLubyte *)malloc(pixellength);
    if (pixeldata == 0)
        exit(0);
    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);
    //Shut down file
    fclose(pfile);


    //Initialize GLUT operation
    glutInit(&argc, argv);   
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(imagewidth, imageheight);
    glutCreateWindow(FILENAME);
    glutDisplayFunc(&display);
    glutKeyboardFunc(Keyboard);
    // glutMainLoopEvent();
    initGame(0);
   
    //glutTimerFunc(0, initGame, 0);// Define qual será a função de loop
    // glutMainLoop(); //LOOP DO OPENGL

    
    //-------------------------------------
    free(pixeldata);
    std::cout << "GAME OVER!\n";
    return 0;
}
