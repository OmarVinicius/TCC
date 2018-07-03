
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "conexao.h"
#include "gerenc_adaptador.h"
#include "servicos.h"
#include "sockets.h"

/*
#include <sys/socket.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dbus/dbus.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/sco.h>
 */

/*
 * 
 */
int main(int argc, char* argv[]) {

    char endr_fone[18];
    strcpy(endr_fone, argv[1]);
    definir_endr_fone_literal(endr_fone);

    char endr_adap_hs[18];
    obter_endereco_local("0",&endr_adap_hs);
    definir_endr_adap_hs_literal(endr_adap_hs);

    if (registra_perfil_hs(3) < 0) {
        perror("register_sdpHS ");
        return EXIT_FAILURE;
    }

    if (registra_perfil_hsag(4) < 0) {
        perror("register_sdpAG ");
        return EXIT_FAILURE;
    }

    /*
        if (initRfcommSockets() == -1)
            return EXIT_FAILURE;

        while (1) {
            if (initScoSockets() == -1)
                return EXIT_FAILURE;

            handle_connection();
        }
     */

    if (inicializa_sockets_tel() == -1)
        return EXIT_FAILURE;

    if (inicializa_sockets_fone() == -1)
        return EXIT_FAILURE;
    
    loop_chamada();

    return EXIT_SUCCESS;
}
