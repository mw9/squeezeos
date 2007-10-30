/*
 * OProfile User Interface
 *
 * Copyright (C) 2007 Nokia Corporation. All rights reserved.
 *
 * Author: Robert Bradford <rob@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */


#include "types.h"

#include "caller_symbol_instance.h"
#include "symbol_instance.h"

G_DEFINE_TYPE (CallerSymbolInstance, caller_symbol_instance, TYPE_SYMBOL_INSTANCE);

static void
caller_symbol_instance_dispose (GObject *object)
{
  if (G_OBJECT_CLASS (caller_symbol_instance_parent_class)->dispose)
    G_OBJECT_CLASS (caller_symbol_instance_parent_class)->dispose (object);
}

static void
caller_symbol_instance_finalize (GObject *object)
{
  G_OBJECT_CLASS (caller_symbol_instance_parent_class)->finalize (object);
}

static void
caller_symbol_instance_class_init (CallerSymbolInstanceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = caller_symbol_instance_dispose;
  object_class->finalize = caller_symbol_instance_finalize;
}

static void
caller_symbol_instance_init (CallerSymbolInstance *self)
{
}

CallerSymbolInstance*
caller_symbol_instance_new (void)
{
  return g_object_new (TYPE_CALLER_SYMBOL_INSTANCE, NULL);
}
