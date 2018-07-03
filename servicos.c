
#include "servicos.h"


/*
int busca_servico()
{
    
}

int remove_servico()
{
    
}

int exibe_servicos()
{
    
}
*/

int registra_perfil_hs(uint8_t porta)
{
    
    const char *nome_servico = "Headset HS Service";
    const char *descricao_servico = "HS";
    const char *provedor_servico = "_";

    uuid_t hs_uuid, ga_uuid;

    sdp_profile_desc_t definicao_servico;

    uuid_t raiz_uuid, l2cap_uuid, rfcomm_uuid;
    sdp_list_t *lista_l2cap = 0,
            *lista_rfcomm = 0,
            *lista_raiz = 0,
            *lista_protocolo = 0,
            *lista_acesso_protocolo = 0;

    sdp_data_t *dado_porta = 0;

    int erro = 0;
    sdp_session_t *sessao = 0;

    sdp_record_t *registro = sdp_record_alloc();

    /* Define os atributos do serviço (nome, provedor e descrição) */
    sdp_set_info_attr(registro, nome_servico, 
                      provedor_servico, descricao_servico);

    /* Obtém o identificador do serviço Headset */
    sdp_uuid16_create(&hs_uuid, HEADSET_SVCLASS_ID);

    if (!(lista_raiz = sdp_list_append(0, &hs_uuid)))
        return -1;

    /* Obtém o identificador do serviço genérico de áudio */
    sdp_uuid16_create(&ga_uuid, GENERIC_AUDIO_SVCLASS_ID);

    if (!(lista_raiz = sdp_list_append(lista_raiz, &ga_uuid)))
        return -1;

    /* Define um uuid de acordo com o tipo de serviço e
     * os perfis já registrados */
    if (sdp_set_service_classes(registro, lista_raiz) < 0)
        return -1;

    sdp_list_free(lista_raiz, 0);
    lista_raiz = 0;

    /* Define que o serviço é público */
    sdp_uuid16_create(&raiz_uuid, PUBLIC_BROWSE_GROUP);

    lista_raiz = sdp_list_append(0, &raiz_uuid);
    sdp_set_browse_groups(registro, lista_raiz);

    /* Define as informações relativas ao L2CAP,
     * que no caso deste serviço, não há nenhuma específica, pois,
     * utiliza o RFCOMM */
    sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
    lista_l2cap = sdp_list_append(0, &l2cap_uuid);
    lista_protocolo = sdp_list_append(0, lista_l2cap);

    /* Define as informações relativas ao RFCOMM */
    sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
    dado_porta = sdp_data_alloc(SDP_UINT8, &porta);
    lista_rfcomm = sdp_list_append(0, &rfcomm_uuid);
    sdp_list_append(lista_rfcomm, dado_porta);
    sdp_list_append(lista_protocolo, lista_rfcomm);

    /* Adiciona as informações relativas aos protocolos utilizados, 
     * ao registro */
    lista_acesso_protocolo = sdp_list_append(0, lista_protocolo);
    sdp_set_access_protos(registro, lista_acesso_protocolo);
    sdp_uuid16_create(&definicao_servico.uuid, HEADSET_PROFILE_ID);

    /* Define a versão do perfil de serviço, 
     * que de acordo com a especificação, é 1.2 */
    definicao_servico.version = 0x0102;

    if (!(lista_raiz = sdp_list_append(NULL, &definicao_servico)))
        return -1;

    if (sdp_set_profile_descs(registro, lista_raiz) < 0)
        return -1;

    /* Conecta-se ao servidor SDP local e registra o serviço */
    sessao = sdp_connect(BDADDR_LOCAL, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
    erro = sdp_record_register(sessao, registro, 0);

    sdp_data_free(dado_porta);
    sdp_list_free(lista_l2cap, 0);
    sdp_list_free(lista_rfcomm, 0);
    sdp_list_free(lista_raiz, 0);
    sdp_list_free(lista_acesso_protocolo, 0);

    return erro;
}

int registra_perfil_hsag(uint8_t porta)
{
    
    const char *nome_servico = "Headset AG Service";
    const char *descricao_servico = "HAG";
    const char *provedor_servico = "_";

    uuid_t hs_uuid, ga_uuid;

    sdp_profile_desc_t definicao_servico;

    uuid_t raiz_uuid, l2cap_uuid, rfcomm_uuid;
    sdp_list_t *lista_l2cap = 0,
            *lista_rfcomm = 0,
            *lista_raiz = 0,
            *lista_protocolo = 0,
            *lista_acesso_protocolo = 0;

    sdp_data_t *dado_porta = 0;

    int erro = 0;
    sdp_session_t *sessao = 0;

    sdp_record_t *registro = sdp_record_alloc();

    /* Define os atributos do serviço (nome, provedor e descrição) */
    sdp_set_info_attr(registro, nome_servico, 
                      provedor_servico, descricao_servico);

    /* Obtém o identificador do serviço Headset Audio Gateway*/
    sdp_uuid16_create(&hs_uuid, HEADSET_AGW_SVCLASS_ID);

    if (!(lista_raiz = sdp_list_append(0, &hs_uuid)))
        return -1;

    /* Obtém o identificador do serviço genérico de áudio */
    sdp_uuid16_create(&ga_uuid, GENERIC_AUDIO_SVCLASS_ID);

    if (!(lista_raiz = sdp_list_append(lista_raiz, &ga_uuid)))
        return -1;

    /* Define um uuid de acordo com o tipo de serviço e
     * os perfis já registrados */
    if (sdp_set_service_classes(registro, lista_raiz) < 0)
        return -1;

    sdp_list_free(lista_raiz, 0);
    lista_raiz = 0;

    /* Define que o serviço é público */
    sdp_uuid16_create(&raiz_uuid, PUBLIC_BROWSE_GROUP);

    lista_raiz = sdp_list_append(0, &raiz_uuid);
    sdp_set_browse_groups(registro, lista_raiz);

    /* Define as informações relativas ao L2CAP,
     * que no caso deste serviço, não há nenhuma específica, pois,
     * utiliza o RFCOMM */
    sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
    lista_l2cap = sdp_list_append(0, &l2cap_uuid);
    lista_protocolo = sdp_list_append(0, lista_l2cap);

    /* Define as informações relativas ao RFCOMM */
    sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
    dado_porta = sdp_data_alloc(SDP_UINT8, &porta);
    lista_rfcomm = sdp_list_append(0, &rfcomm_uuid);
    sdp_list_append(lista_rfcomm, dado_porta);
    sdp_list_append(lista_protocolo, lista_rfcomm);

    /* Adiciona as informações relativas aos protocolos utilizados, 
     * ao registro */
    lista_acesso_protocolo = sdp_list_append(0, lista_protocolo);
    sdp_set_access_protos(registro, lista_acesso_protocolo);
    sdp_uuid16_create(&definicao_servico.uuid, HEADSET_PROFILE_ID);

    /* Define a versão do perfil de serviço, 
     * que de acordo com a especificação, é 1.2 */
    definicao_servico.version = 0x0102;

    if (!(lista_raiz = sdp_list_append(NULL, &definicao_servico)))
        return -1;

    if (sdp_set_profile_descs(registro, lista_raiz) < 0)
        return -1;

    /* Conecta-se ao servidor SDP local e registra o serviço */
    sessao = sdp_connect(BDADDR_LOCAL, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
    erro = sdp_record_register(sessao, registro, 0);

    sdp_data_free(dado_porta);
    sdp_list_free(lista_l2cap, 0);
    sdp_list_free(lista_rfcomm, 0);
    sdp_list_free(lista_raiz, 0);
    sdp_list_free(lista_acesso_protocolo, 0);

    return erro;
}

