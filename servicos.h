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

