/*
 * fdc_765.c - Copyright (c) 2001, 2006 Olivier Poncet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "fdc_765.h"

FDC_765 fdc_765;

void fdc_765_init(void)
{
  fdc_765_reset();
}

void fdc_765_reset(void)
{
  fdc_765.status = 0x80;
  fdc_765.motor = 0x00;
}

void fdc_765_exit(void)
{
}

void fdc_765_set_motor(byte state)
{
  fdc_765.motor = state & 0x01;
}

byte fdc_765_get_motor(void)
{
  return(fdc_765.motor);
}

byte fdc_765_get_status(void)
{
  return(fdc_765.status);
}
