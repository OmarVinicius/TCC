
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gerenc_adaptador.h"
#include "servicos.h"
#include "sockets.h"

/*
 * 
 */
int main(int argc, char* argv[]) 
{
    if(argc==1){
	printf("Insira como argumento, o endereço MAC do headset\n");
	return EXIT_FAILURE;
    }else{
	if(argc>2){
            printf("Insira como argumento, apenas o endereço MAC do headset\n");
            return EXIT_FAILURE;
	}
    }
	
    char endr_fone[18];
    strcpy(endr_fone, argv[1]);
    definir_endr_fone_literal(endr_fone);

    char endr_adap_hs[18];
    obter_endereco_local("0", &endr_adap_hs);
    definir_endr_adap_hs_literal(endr_adap_hs);

    /* Requisita o registro do perfil de serviço Headset na mesma porta
     * configurada no smartphone para o perfil de Headset Audio Gateway */
    if (registra_perfil_hs(3) < 0) {
        perror("ERRO registra_perfil_hs");
        return EXIT_FAILURE;
    }

    /* Requisita o registro do perfil de serviço Headset Audio Gateway
     * na mesma porta configurada no headset para o perfil de Headset */
    if (registra_perfil_hsag(4) < 0) {
        perror("ERRO registra_perfil_hsag");
        return EXIT_FAILURE;
    }

    /* Loop infinito para manter a conexão dos sockets com os alvos e
     * permitir a captura de todas as chamadas efetuadas */
    while (1) {
        if (inicializa_sockets_rfcomm() == -1){
            printf("%s\n", "Erro ao iniciar os sockets rfcomm");
            return EXIT_FAILURE;
        }

        if (inicializa_sockets_sco() == -1){
            printf("%s\n", "Erro ao iniciar os sockets sco");
            return EXIT_FAILURE;
        }

        if (loop_chamada()==-1)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
