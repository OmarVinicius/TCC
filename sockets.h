/** 
 * Contém as funções de inicialização e gerenciamento
 * das conexões via socket (RFCOMM e SCO) com os dispositivos
 * alvos do ataque.
 */

#ifndef SOCKETS_H
#define SOCKETS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <fcntl.h>
#include <errno.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sco.h>
    
    /** 
     * \brief Define a variável que armazena o endereço do adaptador que simula
     * o headset
     */
    void definir_endr_adap_hs_literal(char* endr);
    
    /** 
     * \brief Define a variável que armazena o endereço do headset
     */
    void definir_endr_fone_literal(char* endr);

    /** 
     * \brief Inicializa os sockets RFCOMM e SCO do smartphone
     * 
     * \return Indica se a conexão foi estabelecida com sucesso
     */
    int inicializa_sockets_tel();

    /** 
     * \brief Configura o socket RFCOMM e aguarda conexão do smartphone
     * 
     * \param porta Indica a porta que o socket aguardará conexão
     * 
     * \return Indica se a conexão foi estabelecida com sucesso
     */
    int rfcomm_socket_tel(uint8_t porta);

    /** 
     * \brief Configura o socket SCO e aguarda conexão do smartphone
     * 
     * \return Indica se a conexão foi estabelecida com sucesso
     */
    int sco_socket_tel();

    /** 
     * \brief Inicializa os sockets RFCOMM e SCO do headset
     * 
     * \return Indica se a conexão foi estabelecida com sucesso
     */
    int inicializa_sockets_fone();

    /** 
     * \brief Configura o socket RFCOMM e conecta-se ao headset
     *
     * \param porta Indica a porta na qual o socket irá conectar-se
     * 
     * \return Indica se a conexão foi estabelecida com sucesso
     */
    int rfcomm_socket_fone(uint8_t porta);

    /** 
     * \brief Configura o socket SCO e aguarda conexão do headset
     * 
     * \return Indica se a conexão foi estabelecida com sucesso
     */
    int sco_socket_fone();

    /** 
     * \brief Inicia o loop responsável pelo encaminhamento e
     *        captura dos pacotes
     * 
     * \return Indica se o loop foi encerrado normalmente ou devido alguma falha
     */
    int loop_chamada();


#ifdef __cplusplus
}
#endif

#endif /* SOCKETS_H */

