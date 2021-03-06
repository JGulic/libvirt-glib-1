/*
 * libvirt-gconfig-domain-interface-network.c: libvirt domain interface configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_NETWORK, GVirConfigDomainInterfaceNetworkPrivate))

struct _GVirConfigDomainInterfaceNetworkPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainInterfaceNetwork, gvir_config_domain_interface_network, GVIR_CONFIG_TYPE_DOMAIN_INTERFACE);


static void gvir_config_domain_interface_network_class_init(GVirConfigDomainInterfaceNetworkClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainInterfaceNetworkPrivate));
}


static void gvir_config_domain_interface_network_init(GVirConfigDomainInterfaceNetwork *iface)
{
    iface->priv = GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK_GET_PRIVATE(iface);
}


GVirConfigDomainInterfaceNetwork *gvir_config_domain_interface_network_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_NETWORK,
                                    "interface", NULL);
    gvir_config_object_set_attribute(object, "type", "network", NULL);
    return GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK(object);
}

GVirConfigDomainInterfaceNetwork *gvir_config_domain_interface_network_new_from_xml(const gchar *xml,
                                                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_NETWORK,
                                             "interface", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "network") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK(object);
}

void gvir_config_domain_interface_network_set_source(GVirConfigDomainInterfaceNetwork *interface,
                                                     const char *source)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_NETWORK(interface));

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(interface),
                                                    "source", "network", source);
}
