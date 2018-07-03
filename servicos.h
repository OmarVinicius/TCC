/** 
 * Contém as funções de busca, criação e manipulação de
 * perfis de serviço do Bluetooth, usando o SDP.
 */

#ifndef SERVICOS_H
#define SERVICOS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

    /** 
     * \brief Busca um determinado perfil de serviço
     * 
     * \param -- Nome do serviço procurado
     * 
     * \return Indica se o serviço foi encontrado ou não
     */
    int busca_servico();

    /** 
     * \brief Remove um determinado perfil de serviço local
     * 
     * \param -- Nome do serviço a ser removido
     * 
     * \return Indica se ocorreu uma falha durante a remoção
     */
    int remove_servico();

    /** 
     * \brief Exibe os perfis de serviços locais ou remotos disponíveis
     * 
     * \return Indica se ocorreu uma falha durante a obtenção das informações
     */
    int exibe_servicos();

    /** 
     * \brief Registra o perfil de serviço Headset, localmente
     * 
     * \param porta Número da porta que será utilizada pelo serviço
     * 
     * \return Indica se ocorreu uma falha durante o registro
     */
    int registra_perfil_hs(uint8_t porta);

    /** 
     * \brief Registra o perfil de serviço Headset Audio Gateway, localmente
     * 
     * \param porta Número da porta que será utilizada pelo serviço
     * 
     * \return Indica se ocorreu uma falha durante o registro
     */
    int registra_perfil_hsag(uint8_t porta);


#ifdef __cplusplus
}
#endif

#endif /* SERVICOS_H */

