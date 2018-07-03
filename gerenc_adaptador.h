/** 
 * Contém as funções de gerenciamento dos adaptadores utilizados nesta
 * máquina, que efetuam o ataque.
 */

#ifndef GERENC_ADAPTADOR_H
#define GERENC_ADAPTADOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <dbus/dbus.h>

    /** 
     * \brief Obtém o endereço físico de um determinado adaptador
     *
     * \param indice Indica o índice do adaptador definido pelo BlueZ
     * 
     * \return Indica se obteve o endereço com sucesso
     */
    int obter_endereco_local(const char *adapt_indice, char* endr);



#ifdef __cplusplus
}
#endif

#endif /* GERENC_ADAPTADOR_H */

