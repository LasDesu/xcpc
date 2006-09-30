/*
 * XAreaP.h - Copyright (c) 2001, 2006 Olivier Poncet
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
#ifndef _XAreaP_h
#define _XAreaP_h

#include <X11/CoreP.h>
#include "XArea.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _XAreaClassPart {
  XtPointer extension;
} XAreaClassPart;

typedef struct _XAreaClassRec {
  CoreClassPart  core_class;
  XAreaClassPart xarea_class;
} XAreaClassRec;

externalref XAreaClassRec xAreaClassRec;

typedef struct _XAreaPart {
  void (*start_handler)(Widget widget, XtPointer data);
  void (*clock_handler)(Widget widget, XtPointer data);
  void (*close_handler)(Widget widget, XtPointer data);
  void (*keybd_handler)(Widget widget, XEvent *xevent);
  void (*mouse_handler)(Widget widget, XEvent *xevent);
  void (*paint_handler)(Widget widget, XEvent *xevent);
  XtIntervalId interval_id;
} XAreaPart;

typedef struct _XAreaRec {
  CorePart  core;
  XAreaPart xarea;
} XAreaRec;

#ifdef __cplusplus
}
#endif

#endif