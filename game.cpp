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

#define JANELA1 "Barbeiro Sonolento"
#define JANELA2 "Info do jogo"
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

void newPixeldata() {
    
    FILE *pfile;
    
    //open the certain image
    if (barber.state == CORTANDO)
    {
        if (customers.get_sem() == 0)
            pfile = fopen("assets/cut0.bmp", "rb");
        else if (customers.get_sem() == 1)
            pfile = fopen("assets/cut1.bmp", "rb");
        else if (customers.get_sem() == 2)
            pfile = fopen("assets/cut2.bmp", "rb");
        else if (customers.get_sem() == 3)
            pfile = fopen("assets/cut3.bmp", "rb");
        else if (customers.get_sem() == 1)
            pfile = fopen("assets/cut4.bmp", "rb");
    }
    else //if (barber.state == DESCANSANDO || barber.state == EMPEH)
    {
        if (customers.get_sem() == 0)
            pfile = fopen("assets/sleep0.bmp", "rb");
        else if (customers.get_sem() == 1)
            pfile = fopen("assets/sleep1.bmp", "rb");
        else if (customers.get_sem() == 2)
            pfile = fopen("assets/sleep2.bmp", "rb");
        else if (customers.get_sem() == 3)
            pfile = fopen("assets/sleep3.bmp", "rb");
        else if (customers.get_sem() == 1)
            pfile = fopen("assets/sleep4.bmp", "rb");
    }

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
    {
        std::cout << "pixel data error";
        exit(0);
    }
        
    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);
    //Shut down file
    fclose(pfile);
}


/*Funcao: display
Descricao: mostra as imagens e outras informações relevantes na tela
*/
void displayMain()
{
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw the image pixel by pixel
    glDrawPixels(800, 600, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixeldata);
    
    glutSwapBuffers();
}

void displaySecondary()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Mostra a quantidade de energia na tela
	char barberEnergy[20]={'\0'};
	sprintf(barberEnergy, "Energia: %d", barber.energy);// Text
	glColor3f(1.0,1.0,1.0);// Text color
	glRasterPos2f(0.1,0.2);// Print position
    // glRasterPos3s(-0.9, -0.9, 1);// Print position
	// Print each char
	for(int j=0;j<20;j++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, barberEnergy[j]);
    }

    // Mostra a quantidade de pontos na tela
	char scoreDisplay[25]={'\0'};
	sprintf(scoreDisplay, "Pontos: %d", score);// Text
	glColor3f(1.0,1.0,1.0);// Text color
	glRasterPos2f(0.1, 0.5);// Print position
	// Print each char
	for(int j=0;j<25;j++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, scoreDisplay[j]);
    }

    glFlush();
    glutSwapBuffers();
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
    // std::cout << cTime;
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

/*Funcao: Keyboard
Descricao: lista qual teclas o jogo ouve e o que fazer com tais teclas
*/
void Keyboard(unsigned char key, int x, int y)
{
    //ignorar---
    int w = x + y;
    w++;
    //----------

    //X
    if (key == 'x'){
	 	std::cout << "(x)" << std::endl;
        if (customers.get_sem() > 0)
        {
            cutting();
        }
        else
        {
            std::cout << "não havia nenhum cliente na loja";
            barber.state = EMPEH;
        } 
    }

    //Z
    if (key == 'z'){
	 	barber.sleep();
    }

    //Q
    if (key == 'q'){
	 	exit(0);
    }
}

/*Funcao: drenar_energia
Descricao: função que fica em uma thread executando constantemente drenando energia se o barbeiro esta de pé e nao fazendo nada se ele está em outro estado
*/
void drenar_energia(int *exec) {
    // std::cout << barber.state << std::endl;
    if (*exec == PARADO)
        return;
    if (barber.state == EMPEH && *exec ==EXECUTANDO) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (barber.energy > 0){
            barber.energy -=50;
            // std::cout << "sua energia é:" << barber.energy << "\n";
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
        newPixeldata();
        glutSetWindow(1);
        glutPostRedisplay();
        glutSetWindow(2);
        glutPostRedisplay();
        glutMainLoopEvent();
    }

    //Ponto de atenção (continua reexecutando?) ====
    generation.join();
    drenar.join();
}

// função responasavel por toda execução do codigo
int main(int argc, char *argv[])
{
    newPixeldata();

    //Initialize GLUT operation
    glutInit(&argc, argv);   
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 50);
    glutInitWindowSize(imagewidth, imageheight);
    glutCreateWindow(JANELA1);
    glutDisplayFunc(&displayMain);
    glutKeyboardFunc(Keyboard);
    // gluOrtho2D(0, 800, 0, 600);
    glViewport(0, 0, 800, 600);
    glOrtho(0, 1, 0, 1, 0, 1);
    // glutMainLoopEvent();

    glutInitWindowPosition(900, 200);
    glutInitWindowSize(200, 200);
    glutCreateWindow(JANELA2);
    glutDisplayFunc(&displaySecondary);
    glOrtho(0, 1, 0, 1, 0, 1);

    initGame(0);
   
    //glutTimerFunc(0, initGame, 0);// Define qual será a função de loop
    // glutMainLoop(); //LOOP DO OPENGL

    //-------------------------------------
    free(pixeldata);
    std::cout << "GAME OVER!\n";
    return 0;
}
