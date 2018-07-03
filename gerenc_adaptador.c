
#include <string.h>

#include "gerenc_adaptador.h"


int obter_endereco_local(const char *adapt_indice, char* endr)
{
    DBusConnection *dbuscon;
    DBusError erro;
    DBusMessage *requisicao, *resposta;
    DBusMessageIter iter, sub;
    
    char* endr_temp;
    
    /* Define os parâmetros necessários para a obtenção do endereço
     * a partir da API do BlueZ */
    const char *bus = "org.bluez";
    char *object_path_temp = "/org/bluez/hci";
    char *object_path = (char *) malloc(1 + strlen(object_path_temp) + strlen(adapt_indice));
    strcpy(object_path, object_path_temp);
    strcat(object_path, adapt_indice);
    const char *interface_dbus_prop = "org.freedesktop.DBus.Properties";
    const char *method = "Get";
    const char *interface_adapter = "org.bluez.Adapter1";
    const char *property = "Address";

    dbus_error_init(&erro);

    dbuscon = dbus_bus_get(DBUS_BUS_SYSTEM, &erro);
    if (erro.message != NULL) {
        //printf("%s\n", error.message);
        //dbus_connection_close(dbuscon);
        return -1;
    }
    requisicao = dbus_message_new_method_call(bus, object_path,
            interface_dbus_prop, method);

    dbus_message_append_args(requisicao, DBUS_TYPE_STRING, 
                             &interface_adapter, DBUS_TYPE_INVALID);
    dbus_message_append_args(requisicao, DBUS_TYPE_STRING, 
                             &property, DBUS_TYPE_INVALID);

    resposta = dbus_connection_send_with_reply_and_block(dbuscon, requisicao, 
                                                         1000, &erro);
    if (erro.message != NULL) {
        //printf("%s\n", error.message);
        dbus_message_unref(requisicao);
        dbus_message_unref(resposta);
        //dbus_connection_close(dbuscon);
        return -1;
    }
    
    /* Itera a resposta da requisção para descobrir o endereço */
    dbus_message_iter_init(resposta, &iter);
    dbus_message_iter_recurse(&iter, &sub);
    dbus_message_iter_get_basic(&sub, &endr_temp);
    
    strcpy(endr,endr_temp);

    dbus_message_unref(requisicao);
    dbus_message_unref(resposta);
    
    return 0;
}

