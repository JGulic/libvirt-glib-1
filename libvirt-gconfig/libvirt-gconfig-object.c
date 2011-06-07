/*
 * libvirt-gconfig-config-object.c: base object for XML configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010 Red Hat
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <string.h>

#include "libvirt-gconfig/libvirt-gconfig.h"

//extern gboolean debugFlag;
gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONFIG_OBJECT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_OBJECT, GVirConfigObjectPrivate))

struct _GVirConfigObjectPrivate
{
    gchar *doc;
    gchar *schema;
};

G_DEFINE_TYPE(GVirConfigObject, gvir_config_object, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_DOC,
    PROP_SCHEMA,
};


#define GVIR_CONFIG_OBJECT_ERROR gvir_config_object_error_quark()


static GQuark
gvir_config_object_error_quark(void)
{
    return g_quark_from_static_string("vir-g-config_object");
}

static void gvir_config_object_get_property(GObject *object,
                                            guint prop_id,
                                            GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_DOC:
        g_value_set_string(value, priv->doc);
        break;

    case PROP_SCHEMA:
        g_value_set_string(value, priv->schema);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_config_object_set_property(GObject *object,
                                            guint prop_id,
                                            const GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_DOC:
        if (priv->doc)
            g_object_unref(priv->doc);
        priv->doc = g_value_dup_string(value);
        break;

    case PROP_SCHEMA:
        if (priv->schema)
            g_object_unref(priv->schema);
        priv->schema = g_value_dup_string(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_config_object_finalize(GObject *object)
{
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    DEBUG("Finalize GVirConfigObject=%p", conn);

    g_free(priv->doc);
    g_free(priv->schema);

    G_OBJECT_CLASS(gvir_config_object_parent_class)->finalize(object);
}


static void gvir_config_object_class_init(GVirConfigObjectClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = gvir_config_object_finalize;
    object_class->get_property = gvir_config_object_get_property;
    object_class->set_property = gvir_config_object_set_property;

    g_object_class_install_property(object_class,
                                    PROP_DOC,
                                    g_param_spec_string("doc",
                                                        "Doc",
                                                        "The XML document",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));
    g_object_class_install_property(object_class,
                                    PROP_SCHEMA,
                                    g_param_spec_string("schema",
                                                        "Schema",
                                                        "The doc RNG schema",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirConfigObjectPrivate));
}


static void gvir_config_object_init(GVirConfigObject *conn)
{
    GVirConfigObjectPrivate *priv;

    DEBUG("Init GVirConfigObject=%p", conn);

    priv = conn->priv = GVIR_CONFIG_OBJECT_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}


GVirConfigObject *gvir_config_object_new(const gchar *doc,
                                   const gchar *schema)
{
    return GVIR_CONFIG_OBJECT(g_object_new(GVIR_TYPE_CONFIG_OBJECT,
                                           "doc", doc,
                                           "schema", schema,
                                           NULL));
}

void gvir_config_object_validate(GVirConfigObject *config G_GNUC_UNUSED,
                                 GError **err)
{
    g_set_error(err,
                GVIR_CONFIG_OBJECT_ERROR,
                0,
                "%s",
                "Unable to validate config");
}

const gchar *gvir_config_object_get_doc(GVirConfigObject *config)
{
    GVirConfigObjectPrivate *priv = config->priv;
    return priv->doc;
}

const gchar *gvir_config_object_get_schema(GVirConfigObject *config)
{
    GVirConfigObjectPrivate *priv = config->priv;
    return priv->schema;
}
