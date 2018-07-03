
#include <string.h>

#include "gerenc_adaptador.h"


int obter_endereco_local(const char *adapt_indice, char* endr)
{
    DBusConnection *dconn;
    DBusError error;
    DBusMessage *msg, *reply;
    DBusMessageIter iter, sub;
    
    char* endr_temp;
    
    const char *bus = "org.bluez";
    char *object_path_temp = "/org/bluez/hci";
    char *object_path = (char *) malloc(1 + strlen(object_path_temp) + strlen(adapt_indice));
    strcpy(object_path, object_path_temp);
    strcat(object_path, adapt_indice);
    const char *interface_dbusProp = "org.freedesktop.DBus.Properties";
    const char *method = "Get";
    const char *interface_adapter = "org.bluez.Adapter1";
    const char *property = "Address";

    dbus_error_init(&error);

    dconn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (error.message != NULL) {
        //printf("%s\n", error.message);
        dbus_connection_close(dconn);
        return -1;
    }
    msg = dbus_message_new_method_call(bus, object_path,
            interface_dbusProp, method);

    dbus_message_append_args(msg, DBUS_TYPE_STRING, &interface_adapter, DBUS_TYPE_INVALID);
    dbus_message_append_args(msg, DBUS_TYPE_STRING, &property, DBUS_TYPE_INVALID);

    reply = dbus_connection_send_with_reply_and_block(dconn, msg, 1000, &error);
    if (error.message != NULL) {
        //printf("%s\n", error.message);
        dbus_message_unref(msg);
        dbus_message_unref(reply);
        dbus_connection_close(dconn);
        return -1;
    }
    dbus_message_iter_init(reply, &iter);
    dbus_message_iter_recurse(&iter, &sub);
    dbus_message_iter_get_basic(&sub, &endr_temp);
    
    strcpy(endr,endr_temp);

    dbus_message_unref(msg);
    dbus_message_unref(reply);
    //dbus_connection_close(dconn);
    
    return 0;
}

