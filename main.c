
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "conexao.h"
#include "gerenc_adaptador.h"
#include "servicos.h"
#include "sockets.h"

/*
 * 
 */
int main(int argc, char* argv[]) 
{
    
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
        if (inicializa_sockets_tel() == -1)
            return EXIT_FAILURE;

        if (inicializa_sockets_fone() == -1)
            return EXIT_FAILURE;

        loop_chamada();
    }

    return EXIT_SUCCESS;
}
