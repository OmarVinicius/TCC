
#include "sockets.h"

uint16_t sco_mtu;

int rfcomm_tel_socket = -1;
int rfcomm_fone_socket = -1;

int sco_tel_socket = -1;
int sco_fone_socket = -1;

char endr_adap_hs_literal[18];
char endr_fone_literal[18];

void definir_endr_adap_hs_literal(char* endr)
{
    strcpy(endr_adap_hs_literal, endr);
    printf("%s\n", endr_adap_hs_literal);
}

void definir_endr_fone_literal(char* endr)
{
    strcpy(endr_fone_literal, endr);
    printf("%s\n", endr_fone_literal);
}

int inicializa_sockets_tel()
{
    rfcomm_socket_tel(3);
    printf("Socket RFCOMM smartphone %d\n", rfcomm_tel_socket);

    sco_socket_tel();
    printf("Socket SCO smartphone %d\n", sco_tel_socket);

    if (rfcomm_tel_socket == -1 || sco_tel_socket == -1) {
        printf("%s\n", "Erro ao iniciar os sockets com o smartphone");
        return -1;
    } else {
        printf("%s\n", "Sockets com o smartphone, iniciados corretamente");
        return 0;
    }
}

int rfcomm_socket_tel(uint8_t porta)
{
    int sock;
    int cliente = -1;
    unsigned int comprimento_endr = sizeof (struct sockaddr_rc);
    struct sockaddr_rc endereco_local, endereco_tel;
    char endr_tel_literal[18];

    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* Configura o socket definindo o adaptador que simula o headset,
     * como o "servidor" da conexão */
    endereco_local.rc_family = AF_BLUETOOTH;
    str2ba(endr_adap_hs_literal,&endereco_local.rc_bdaddr);
    endereco_local.rc_channel = porta;

    if (bind(sock, (struct sockaddr*) &endereco_local, 
             sizeof (struct sockaddr_rc)) < 0) {
        perror("RFCOMM smartphone bind ERROR");
        return -1;
    }

    printf("Aguardando conexão do smartphone ao socket RFCOMM\n");

    if (listen(sock, 1) < 0) {
        perror("RFCOMM smartphone listen ERROR");
        return -1;
    }

    cliente = accept(sock, (struct sockaddr*) &endereco_tel, 
                     &comprimento_endr);

    ba2str(&endereco_tel.rc_bdaddr, endr_tel_literal);

    /* Verifica se a conexão foi estabelecida sem falhas e
     * efetua desbloqueio de I/O do socket */
    if (cliente != -1 && fcntl(cliente, F_SETFL, O_NONBLOCK) >= 0) {
        printf("%s %s %d\n", "RFCOMM conectado corretamente ao smartphone: ",
                endr_tel_literal, cliente);
        rfcomm_tel_socket = cliente;
        return cliente;
    } else {
        printf("RFCOMM smartphone accept ERROR\n");
        return -1;
    }
}

int sco_socket_tel()
{
    int sock, erro;
    int cliente = -1;
    unsigned int comprimento_endr = sizeof (struct sockaddr_sco);
    struct bt_voice codec_voz;
    struct sockaddr_sco endereco_local, endereco_tel;
    char endr_tel_literal[18];

    sock = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_SCO);

    /* Define a codificação de voz a ser utilizada durante a transmissão de voz,
     * que por padrão no Bluetooth, é a CVSD */
    codec_voz.setting = BT_VOICE_CVSD_16BIT;
    erro = setsockopt(sock, SOL_BLUETOOTH, BT_VOICE, &codec_voz, 
                      sizeof (codec_voz));
    if (erro < 0) {
        perror("SCO smartphone setsockopt voice ERROR");
        return -1;
    }

    /* Assim como na inicialização do socket RFCOMM, define o adaptador 
     * que simula o headset, como o "servidor" da conexão */
    endereco_local.sco_family = AF_BLUETOOTH;
    str2ba(endr_adap_hs_literal,&endereco_local.sco_bdaddr);

    if (bind(sock, (struct sockaddr*) &endereco_local, 
             sizeof (struct sockaddr_sco)) < 0) {
        perror("SCO smartphone bind ERROR");
        return -1;
    }

    printf("Aguardando conexão do smartphone ao socket SCO\n");

    if (listen(sock, 1) < 0) {
        perror("SCO smartphone listen ERROR");
        return -1;
    }

    cliente = accept(sock, (struct sockaddr*) &endereco_tel, 
                     &comprimento_endr);

    ba2str(&endereco_tel.sco_bdaddr, endr_tel_literal);

    if (cliente != -1) {
        printf("%s %s %d\n", "SCO conectado corretamente ao smartphone: ", 
                endr_tel_literal, cliente);
        sco_tel_socket = cliente;
    } else {
        printf("SCO smartphone accept ERROR\n");
        return -1;
    }

    /* Obtém o mtu definido pelo smartphone, para configurar, posteriormente, 
     * o socket com o headset, com o mesmo valor */
    struct sco_options so;
    int so_size = sizeof (so);
    erro = getsockopt(cliente, SOL_SCO, SCO_OPTIONS, &so, &so_size);
    if (erro < 0) {
        perror("SCO smartphone getsockopt mtu ERROR");
        return -1;
    }
    sco_mtu = so.mtu;

    close(sock);

    return cliente;
}

int inicializa_sockets_fone()
{
    rfcomm_socket_fone(4);
    printf("Socket RFCOMM headset %d\n", rfcomm_fone_socket);

    sco_socket_fone();
    printf("Socket SCO headset %d\n", sco_fone_socket);

    if (rfcomm_fone_socket == -1 || sco_fone_socket == -1) {
        printf("%s\n", "Erro ao iniciar os sockets com o headset");
        return -1;
    } else {
        printf("%s\n", "Sockets com o headset, iniciados corretamente");
        return 0;
    }
}

int rfcomm_socket_fone(uint8_t porta)
{
    int sock;
    int status = -1;
    struct sockaddr_rc endereco_fone = {0};

    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* Configura o socket, definindo o endereço do headset no qual
     * deseja conectar-se */
    endereco_fone.rc_family = AF_BLUETOOTH;
    endereco_fone.rc_channel = porta;
    str2ba(endr_fone_literal, &endereco_fone.rc_bdaddr);

    status = connect(sock, (struct sockaddr_rc *) &endereco_fone, 
                     sizeof (endereco_fone));

    /* Verifica se a conexão foi estabelecida sem falhas e
     * efetua desbloqueio de I/O do socket */
    if (status != -1 && fcntl(sock, F_SETFL, O_NONBLOCK) >= 0) {
        printf("%s %s %d\n", "RFCOMM conectado corretamente ao headset: ", 
                endr_fone_literal, sock);
        rfcomm_fone_socket = sock;
        return status;
    } else {
        printf("RFCOMM headset connect ERROR\n");
        return -1;
    }

    return status;
}

int sco_socket_fone()
{
    int sock, erro;
    int status = -1;
    struct bt_voice codec_voz;
    struct sockaddr_rc endereco_fone;

    sock = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_SCO);

    /* Define a codificação de voz a ser utilizada durante a transmissão de voz,
     * que por padrão no Bluetooth, é a CVSD */
    codec_voz.setting = BT_VOICE_CVSD_16BIT;
    erro = setsockopt(sock, SOL_BLUETOOTH, BT_VOICE, &codec_voz, 
                      sizeof (codec_voz));
    if (erro < 0) {
        perror("SCO headset setsockopt voice ERROR");
        return -1;
    }

    /* Configura o socket, definindo o endereço do headset no qual
     * deseja conectar-se */
    endereco_fone.rc_family = AF_BLUETOOTH;
    str2ba(endr_fone_literal, &endereco_fone.rc_bdaddr);

    status = connect(sock, (struct sockaddr_rc *) &endereco_fone, 
                     sizeof (endereco_fone));

    if (status != -1) {
        printf("%s %s %d\n", "SCO conectado corretamente ao headset: ", 
                endr_fone_literal, sock);
        sco_fone_socket = sock;
    } else {
        printf("SCO headset connect ERROR\n");
        return -1;
    }

    /* Define o mtu, com o valor configurado no momento da conexão com
     * o smartphone */
    struct sco_options so;
    int so_size = sizeof (so);
    so.mtu = sco_mtu;
    erro = setsockopt(sock, SOL_SCO, SCO_OPTIONS, &so, &so_size);
    if (erro < 0) {
        perror("SCO Vítima B setsockopt mtu ERROR");
        return -1;
    }

    return status;
}

int loop_chamada()
{
    uint8_t scobuffer[255];
    uint8_t rfcommbuffer[255];
    int compr_pact_tel_rfcomm, compr_pact_fone_rfcomm, 
        compr_pact_tel_sco, compr_pact_fone_sco;

    /* Define o formato de áudio a ser reproduzido de acordo
     * com o padrão de transmissão de voz por Bluetooth */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 8000,
        .channels = 1
    };
    
    pa_simple *reproducao = NULL;
    int error;

    /* Define e configura o dispositivo padrão de reprodução de áudio 
     * desta máquina */
    if (!(reproducao = pa_simple_new(NULL, "HeadsetBlueTest", PA_STREAM_PLAYBACK, 
                                     NULL, "playback", &ss, NULL, NULL, &error))) {
        printf("pa_simple_new() failed: %s\n", pa_strerror(error));
        return -1;
    }

    printf("\n\n");

    while (1) {

        /* Recebe os constantes pacotes de áudio provenientes do smartphone
         * e armazena no buffer para posterior execução e 
         * encaminhamento para o headset */
        if ((compr_pact_tel_sco = recv(sco_tel_socket, scobuffer, 
                                    sizeof (scobuffer), 0)) <= 0) {
            perror("RX sco smartphone ERROR");
            break;
        }
        printf("RX sco smartphone: %d sock: %d\n", compr_pact_tel_sco, sco_tel_socket);

        /* Reproduz o áudio recebido do smartphone */
        if (pa_simple_write(reproducao, scobuffer, compr_pact_tel_sco, &error) < 0) {
            printf("pa_simple_write() failed: %s\n", pa_strerror(error));
            break;
        }

        /* Recebe possíveis pacotes de controle do headset, 
         * enviados pelo smartphone, e armazena em buffer para posterior
         * encaminhamento ao headset */
        compr_pact_tel_rfcomm = recv(rfcomm_tel_socket, rfcommbuffer, 
                                  sizeof (rfcommbuffer), 0);
        printf("RX rfcomm smartphone: %d sock: %d\n", compr_pact_tel_rfcomm, rfcomm_tel_socket);

        /* Verifica se foi recebido algum pacote do smartphone ou se 
         * ocorreu alguma falha que tenha bloqueado o socket, 
         * exigindo a interrupção do loop */
        if (compr_pact_tel_rfcomm < 0 && errno != EWOULDBLOCK) {
            perror("RX rfcomm smartphone ERROR");
            break;
        } else if (compr_pact_tel_rfcomm > 0) {
            compr_pact_fone_rfcomm = send(rfcomm_fone_socket, rfcommbuffer, 
                                      compr_pact_tel_rfcomm, 0);
            printf("TX rfcomm headset: %d sock: %d\n", compr_pact_fone_rfcomm, rfcomm_fone_socket);
        }


        /* Envia os pacotes de áudio provenientes do smartphone
         * que estão armazenados no buffer, para o headset */
        if ((compr_pact_fone_sco = send(sco_fone_socket, scobuffer, 48, 0)) <= 0) {
            perror("TX sco headset ERROR");
            break;
        }
        printf("TX sco headset: %d sock: %d\n", compr_pact_fone_sco, sco_fone_socket);

        /* Recebe os constantes pacotes de áudio provenientes do headset
         * e armazena no buffer para posterior execução e 
         * encaminhamento para o smartphone */
        if ((compr_pact_fone_sco = recv(sco_fone_socket, scobuffer, 48, 0)) <= 0) {
            perror("RX sco headset ERROR: ");
            break;
        }
        printf("RX sco headset: %d sock: %d\n", compr_pact_fone_sco, sco_fone_socket);

        /* Reproduz o áudio recebido do headset */
        if (pa_simple_write(reproducao, scobuffer, compr_pact_fone_sco, &error) < 0) {
            printf("pa_simple_write() failed: %s\n", pa_strerror(error));
            break;
        }

        /* Recebe possíveis pacotes de controle do smartphone, 
         * enviados pelo headset, e armazena em buffer para posterior
         * encaminhamento ao smartphone */
        compr_pact_fone_rfcomm = recv(rfcomm_fone_socket, rfcommbuffer, 
                                  sizeof (rfcommbuffer), 0);
        printf("RX rfcomm headset: %d sock: %d\n", compr_pact_fone_rfcomm, rfcomm_fone_socket);

        /* Verifica se foi recebido algum pacote do headset ou se 
         * ocorreu alguma falha que tenha bloqueado o socket, 
         * exigindo a interrupção do loop */
        if (compr_pact_fone_rfcomm < 0 && errno != EWOULDBLOCK) {
            perror("RX rfcomm headset ERROR");
            break;
        } else if (compr_pact_fone_rfcomm > 0) {
            compr_pact_tel_rfcomm = send(rfcomm_tel_socket, rfcommbuffer, 
                                      compr_pact_fone_rfcomm, 0);
            printf("TX rfcomm smartphone: %d sock: %d\n", compr_pact_tel_rfcomm, rfcomm_tel_socket);
        }

        /* Envia os pacotes de áudio provenientes do headset
         * que estão armazenados no buffer, para o smartphone */
        if ((compr_pact_tel_sco = send(sco_tel_socket, scobuffer, 48, 0)) <= 0) {
            perror("TX sco smartphone ERROR");
            break;
        }
        printf("TX sco smartphone: %d sock: %d\n", compr_pact_tel_sco, sco_tel_socket);
    }

    pa_simple_free(reproducao);

    close(sco_fone_socket);
    close(sco_tel_socket);

    printf("A chamada foi finalizada\n");

    return 0;
}

