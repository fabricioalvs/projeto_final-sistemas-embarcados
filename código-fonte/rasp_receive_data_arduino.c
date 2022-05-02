#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <termios.h>
#define TTY "/dev/ttyS0"

int uart0_fd,num;
char comandoCompleto[100];
void ctrl_c(int sig)
{
        puts(" CLOSING " TTY "...");
        close(uart0_fd);
        exit(-1);
}
void disconnect_uart()
{
        puts("UART DISCONNECTED");
        close(uart0_fd);
        exit(-1);
}

int main(void)
{
        struct termios options;
        signal(SIGINT, ctrl_c);
        uart0_fd = open(TTY, O_RDWR); // | O_NOCTTY); // | O_NDELAY);
        if(uart0_fd==-1)
        {
                puts("ERROR OPEN UART");
                return -1;
        }
        tcgetattr(uart0_fd, &options);
        options.c_cflag = CS8 | CREAD | CLOCAL;
        options.c_iflag = 0;
        options.c_oflag = 0;
        options.c_lflag = 0;
        options.c_cc[VTIME] = 0;
        options.c_cc[VMIN]  = 1;
        cfsetospeed(&options, B115200);
        cfsetispeed(&options, B115200);
        tcflush(uart0_fd, TCIOFLUSH);
        tcsetattr(uart0_fd, TCSANOW, &options);
        unsigned char rx_buffer[6];
 	tcsetattr(uart0_fd, TCSANOW, &options);
        FILE *arq;
        arq=fopen("dados_ecg.xlsx", "wt");
        if(arq == NULL)
        {
                printf("ERRO NA ABERTURA DE ARQUIVO\n");
                return -1;
        }
	int cont_pico_R=0;
        int tempo = 0;
        double batimentos_card;
        int result;
        int rx_length;
        while(1)
        {
                int rx_length = read(uart0_fd, (void*)rx_buffer, 5);
		usleep(2000);
                cont_pico_R++;
                tempo++;
                if (rx_length < 0)
                {
                        disconnect_uart();
                        //An error occured (will occur if there are no bytes)
                }
                else if (rx_length == 0)
                {
                         printf("NO DATA");//No data waiting
                }
		result =fputs(rx_buffer,arq);
                if (result == EOF)
                {
                      printf("ERRO NA GRAVAÇÃO\n");
                }
		num = strtol(rx_buffer, NULL, 10); //Transformando para número inteiro de base 10
           	if (num > 100)
                {
                        printf("%d\n",cont_pico_R);
                        batimentos_card=60/(cont_pico_R*0.002);
                        printf("%.f\n",batimentos_card);
                        snprintf(comandoCompleto, 100, "python3 rasp_to_ubidots.py %.f", batimentos_card); // concatena>                        system(comandoCompleto);
                        cont_pico_R=0;
                }
		memset(rx_buffer,0,6);
		if (tempo == 15000){
                        fclose(arq);
                        close(uart0_fd);
                        exit(-1);
                }
	}
}
