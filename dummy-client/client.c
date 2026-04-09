#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include "protocol.h"
#include "buffer.h"

#define PORT 8080

// ─── Datos que recibe cada hilo ───────────────────────────────────────────────
typedef struct
{
    uint32_t burst;
    uint8_t priority;
} DatosProceso;

// ─── Función que conecta y envía un proceso al servidor ───────────────────────
void *hilo_proceso(void *arg)
{
    DatosProceso *datos = (DatosProceso *)arg;

    // Conectar al servidor
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error conectando al servidor");
        free(datos);
        return NULL;
    }

    // Serializar y enviar
    uint8_t buffer_data[REQUEST_SIZE];
    Buffer buffer;
    buffer_init(&buffer, buffer_data, REQUEST_SIZE);

    Request req = {datos->burst, datos->priority};
    serialize_request(&buffer, &req);
    send(sock, buffer_data, buffer.offset, 0);

    printf("[Proceso enviado] burst=%u, priority=%u\n",
           datos->burst, datos->priority);

    // Recibir PID
    uint8_t res_buffer[RESPONSE_SIZE];
    int bytes = recv(sock, res_buffer, RESPONSE_SIZE, 0);
    if (bytes > 0)
    {
        Buffer res_buf;
        buffer_init(&res_buf, res_buffer, RESPONSE_SIZE);
        Response res;
        deserialize_response(&res_buf, &res);
        printf("[PID recibido] PID=%u para proceso con burst=%u\n",
               res.pid, datos->burst);
    }

    close(sock);
    free(datos);
    return NULL;
}

// ─── Modo Manual ──────────────────────────────────────────────────────────────
void modo_manual(char *nombre_archivo, int burst_min, int burst_max)
{
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo)
    {
        perror("Error abriendo archivo");
        return;
    }

    int burst, priority;
    while (fscanf(archivo, "%d %d", &burst, &priority) == 2)
    {

        // Validar rango de burst
        if (burst < burst_min || burst > burst_max)
        {
            printf("[Ignorado] burst=%d fuera del rango [%d, %d]\n",
                   burst, burst_min, burst_max);
            continue;
        }

        // Crear datos para el hilo
        DatosProceso *datos = malloc(sizeof(DatosProceso));
        datos->burst = (uint32_t)burst;
        datos->priority = (uint8_t)priority;

        // Lanzar hilo
        pthread_t hilo;
        pthread_create(&hilo, NULL, hilo_proceso, datos);
        pthread_detach(hilo);

        // Sleep aleatorio 3-8 segundos antes del siguiente
        int espera = 3 + rand() % 6;
        printf("[Manual] Esperando %d segundos...\n", espera);
        sleep(espera);
    }

    fclose(archivo);
    printf("[Manual] Archivo leído completamente.\n");
}

// ─── Modo Automático ──────────────────────────────────────────────────────────
volatile int corriendo = 1;

void detener(int sig)
{
    (void)sig;
    corriendo = 0;
    printf("\n[Auto] Deteniendo cliente...\n");
}

void modo_automatico(int burst_min, int burst_max)
{
    signal(SIGINT, detener); // captura Ctrl+C

    while (corriendo)
    {
        DatosProceso *datos = malloc(sizeof(DatosProceso));
        datos->burst = burst_min + rand() % (burst_max - burst_min + 1);
        datos->priority = 1 + rand() % 10;

        pthread_t hilo;
        pthread_create(&hilo, NULL, hilo_proceso, datos);
        pthread_detach(hilo);

        // Pausa entre creación de procesos (1-4 segundos)
        int espera = 1 + rand() % 4;
        sleep(espera);
    }
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc < 4)
    {
        printf("Uso:\n");
        printf("  ./client manual <archivo> <burst_min> <burst_max>\n");
        printf("  ./client auto <burst_min> <burst_max>\n");
        return 1;
    }

    int burst_min, burst_max;

    if (strcmp(argv[1], "manual") == 0)
    {
        burst_min = atoi(argv[3]);
        burst_max = atoi(argv[4]);
        modo_manual(argv[2], burst_min, burst_max);
    }
    else if (strcmp(argv[1], "auto") == 0)
    {
        burst_min = atoi(argv[2]);
        burst_max = atoi(argv[3]);
        modo_automatico(burst_min, burst_max);
    }
    else
    {
        printf("Modo no reconocido. Use 'manual' o 'auto'\n");
        return 1;
    }

    return 0;
}