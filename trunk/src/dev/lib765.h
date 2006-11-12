/* lib765.h - Copyright (c) 2002, 2003, 2004 John Elliott - 2005 Philip Kendall
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
#ifndef __LIB765_H__
#define __LIB765_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
    Functions which your program should provide are marked here by 
    the word EXT: 

    NOTE: This library emulates enough of a uPD765A controller (aka Intel 8272)
    for an Amstrad CP/M BIOS. Features the Amstrad BIOS does not use, such as:
 
    * DMA mode
    * Multitrack mode
    * Multisector reads/writes 
    * Read/Write deleted data

    are not implemented.

*/

#define MAX_SECTOR_LEN  8192	/* Max size of sectors this FDC can handle */
#define SHORT_TIMEOUT   1000	/* Timeouts, in machine cycles */
#define LONGER_TIMEOUT  1333333L

/* EXT: called by the FDC to print debugging messages
 * 
 * We assume that the normal debug level is 0. As we get more and more 
 * nitpicky, the debug level gets higher.
 *
 * Level 5 dumps out all command sequences sent to the FDC.
 * Level 4 logs all sector reads, writes & seeks.
 */
void fdc_dprintf(int debuglevel, char *fmt, ...);

typedef void (*lib765_error_function_t)(int debuglevel, char *fmt, va_list ap);

extern lib765_error_function_t lib765_error_function;

void lib765_default_error_function(int debuglevel, char *fmt, va_list ap);

void lib765_register_error_function(lib765_error_function_t ef);

/******************************* Types ************************************/
typedef unsigned char fdc_byte;	/* 8-bit byte */
typedef short         fd_err_t; /* Error code returned from disc routines */

/****************************** Defines ***********************************/

/* Floppy drive types */
#define FD_NC9256 (-1)	/* PCW9256 drives 2 and 3 */
#define FD_NONE   ( 0)	/* Drive B: on single-drive computers */
#define FD_30     ( 1)	/* 3"    */
#define FD_35     ( 2)	/* 3.5"  */
#define FD_525    ( 3)	/* 5.25" */

/* Floppy drive errors */
#define FD_E_OK	       (0) 	/* OK */
#define FD_E_SEEKFAIL (-1)	/* Seek fail */
#define FD_E_NOADDR   (-2)	/* Missing address mark */
#define FD_E_NODATA   (-3) 	/* No data */
#define FD_E_DATAERR  (-4)	/* CRC Data error */
#define FD_E_NOSECTOR (-5)	/* Sector not found */
#define FD_E_NOTRDY   (-6)	/* Drive not ready */
#define FD_E_READONLY (-7)	/* Read only */

/* Floppy disk dirty flags */
#define FD_D_UNAVAILABLE (-1)	/* Disk dirty information not available */
#define FD_D_CLEAN       ( 0)	/* Disk not written to */
#define FD_D_DIRTY       ( 1)	/* Disk written to */

struct fdc_765;

/********************************** CLASSES ********************************/

/* This is yet another attempt to write object-oriented C. 
 *
 * Variables in these structs are marked as PUBLIC, READONLY or PRIVATE. 
 * Your program can play with PUBLIC variables how it likes, but should
 * not alter READONLY ones, and it shouldn't touch the PRIVATE ones at all.
 *
 * Use FLOPPY_DRIVE for dummy drives (units which aren't connected at all) 
 * and DSK_FLOPPY_DRIVE / LIBDSK_FLOPPY_DRIVE for drives that are real.
 * 
 * These structures MUST be initialised (see fd_init() below) before use.
 */

/* lib765 v0.1.0: Implementation details hidden away behind opaque 
 * pointers; accessor functions used instead */

typedef struct floppy_drive *FDRV_PTR;

/* Accessor functions replace direct access to structure members */

int fd_gettype    (FDRV_PTR fd); /* 0 for none, 1 for 3", 2 for 3.5", 3 for 5.25" */
int fd_getheads   (FDRV_PTR fd); /* No. of heads in the drive: 1 or 2 */
int fd_getcyls    (FDRV_PTR fd); /* No. of cylinders the drive can access: 
                                  * eg: a nominally 40-track drive can usually go up
                                  * to 42 tracks with a bit of "persuasion" */
int fd_getreadonly(FDRV_PTR fd); /* Is the drive (or the disc therein) set to R/O? */

void fd_settype    (FDRV_PTR fd, int type);
void fd_setheads   (FDRV_PTR fd, int heads);
void fd_setcyls    (FDRV_PTR fd, int cyls);
void fd_setreadonly(FDRV_PTR fd, int ro);

int fd_getmotor    (FDRV_PTR fd); /* Is the motor on for this drive? */
int fd_getcurcyl   (FDRV_PTR fd); /* What cylinder is this drive on?
                                   * Current cylinder. Note that if the drive is
                                   * double-stepping, this is the "real" cylinder -
                                   * so it could = 24 and be reading cylinder 12
                                   * of a 40-track DSK file. */

/* Create a default (not-connected) drive */

FDRV_PTR fd_new(void);

/* Destroy any kind of drive */
void     fd_destroy(FDRV_PTR *fd);

/* Special emulation for PCW9256 drive 2 */

FDRV_PTR fd_newnc9(FDRV_PTR fd);
FDRV_PTR fd9_getproxy(FDRV_PTR self);
void     fd9_setproxy(FDRV_PTR self, FDRV_PTR PROXY);


/* Subclass of FLOPPY_DRIVE: a drive which emulates discs using the CPCEMU 
 * .DSK format */

FDRV_PTR fd_newdsk(void);

/* Get / set DSK file associated with this drive.
 * Note that doing fdd_setfilename() causes an implicit eject on the 
 * previous disc in the drive. */
char *   fdd_getfilename(FDRV_PTR fd);		
void	 fdd_setfilename(FDRV_PTR fd, const char *s);

/* This function is specific to DSK-type drives, and is not called by the 
 * FDC. It is intended for use by administration interfaces */
fd_err_t fdd_new_dsk(FDRV_PTR fd);


#ifdef DSK_ERR_OK	/* LIBDSK headers included */
/* Subclass of FLOPPY_DRIVE: a drive which emulates discs using LIBDSK
 * for disc I/O. */
FDRV_PTR fd_newldsk(void);

/* Get / set DSK file associated with this drive.
 * Note that doing fdd_setfilename() causes an implicit eject on the 
 * previous disc in the drive. */
char *   fdl_getfilename(FDRV_PTR fd);		
void	 fdl_setfilename(FDRV_PTR fd, const char *s);

/* Set the LIBDSK drive type. NULL for automatic */
const char *   fdl_gettype(FDRV_PTR fd);		
void	 fdl_settype(FDRV_PTR fd, const char *s);
/* Set the LIBDSK compression type. NULL for automatic */
const char *   fdl_getcomp(FDRV_PTR fd);		
void	 fdl_setcomp(FDRV_PTR fd, const char *s);
#endif



typedef struct fdc_765 *FDC_PTR;

/* This is called when the FDC interrupts */
typedef void (*FDC_ISR)(FDC_PTR self, int status);

FDC_PTR fdc_new(void);
void    fdc_destroy(FDC_PTR *p);

/* Reading FDC internal state */
fdc_byte fdc_readst0(FDC_PTR fdc);
fdc_byte fdc_readst1(FDC_PTR fdc);
fdc_byte fdc_readst2(FDC_PTR fdc);
fdc_byte fdc_readst3(FDC_PTR fdc);

int fdc_getunit(FDC_PTR fdc);	/* Get current unit */
int fdc_gethead(FDC_PTR fdc);	/* Get current head */

void fdc_setisr(FDC_PTR self, FDC_ISR isr);
FDC_ISR fdc_getisr(FDC_PTR self);

/* The FDC's four drives. You must set these. */
FDRV_PTR fdc_getdrive(FDC_PTR self, int drive);
void fdc_setdrive(FDC_PTR self, int drive, FDRV_PTR ptr);


/*********************** WRAPPER FUNCTIONS ********************************/ 

/* Wrappers for the floppy's member functions */
/* Seek to a cylinder */
fd_err_t fd_seek_cylinder(FDRV_PTR fd, int cylinder);
/* Read ID from current cylinder, head "head". "sector" is a number 
 * which should increase every time you call this function, to simulate 
 * the rotation of the disc. Fills the buffer at "buf" with the sector ID. */
fd_err_t fd_read_id(FDRV_PTR fd, int head, int sector, fdc_byte *buf);
/* Read a sector. xcylinder and xhead are values expected from the sector ID;
 * while "head" and "sector" are the actual place to look on the disc. 
 * Data will be returned to "buf", maximum "len" bytes. */
fd_err_t fd_read_sector(FDRV_PTR fd, int xcylinder,
                int xhead, int head, int sector, fdc_byte *buf, 
		int len, int *deleted, int skip_deleted, int mfm, int multi);
/* Write a sector; parameters as for "read sector" */
fd_err_t  fd_write_sector(FDRV_PTR fd, int xcylinder,
                int xhead, int head, int sector, fdc_byte *buf, int len, 
		int deleted, int skip_deleted, int mfm, int multi);
/* Read an entire track; parameters as for "read sector" */
fd_err_t  fd_read_track   (struct floppy_drive *fd, int xcylinder,
                int xhead, int head, fdc_byte *buf, int *len);
/* Format a track of "sectors" sectors, on the current cylinder. "track" 
 * holds the sector ID information, and "filler" is the filler byte */ 
fd_err_t  fd_format_track (struct floppy_drive *fd, int head, 
		int sectors, fdc_byte *track, fdc_byte filler);
/* Get the drive status - bits 7-3 of the DD_DRIVE_STATUS value */
fdc_byte fd_drive_status(FDRV_PTR fd);
/* Return 1 if the drive is ready, 0 if it is not */
fdc_byte fd_isready(FDRV_PTR fd);
/* Return 1 if the disc has been changed (defined as the disc being ejected 
 * since last FDC reset). For PcW16 changeline support. */
fdc_byte fd_changed(FDRV_PTR fd);
/* Has this floppy been written to since it was inserted? */
#define LIB765_EXPOSES_DIRTY
int fd_dirty(FDRV_PTR fd);
/* Eject the disc from the drive */
void fd_eject(FDRV_PTR fd);
/* Set the drive's data rate */
void fd_set_datarate(FDRV_PTR fd, fdc_byte rate);
/* Reset the drive */
void fd_reset(FDRV_PTR fd);



/* Interface to the FDC itself */ 

/* Reset the FDC. MUST be called to initialise the FDC structure */
void fdc_reset     (FDC_PTR self);
/* Write a byte to the FDC's data register */
void fdc_write_data(FDC_PTR self, fdc_byte value);
/* Read the FDC's data register */
fdc_byte fdc_read_data (FDC_PTR self);
/* Read the FDC's main control register */
fdc_byte fdc_read_ctrl (FDC_PTR self);
/* Raise / lower the FDC's terminal count line */
void fdc_set_terminal_count(FDC_PTR self, fdc_byte value);
/* Start/stop drive motors. Bit 0 corresponds to drive 1's motor, etc. */
void fdc_set_motor(FDC_PTR self, fdc_byte running);
/* Call this once every cycle round the emulator's main loop */
void fdc_tick(FDC_PTR self);
/* Write to the Digital Output Register. Write -1 to disable DOR emulation */
void fdc_write_dor(FDC_PTR self, int value);
/* Read from the Digital Input Register. */
fdc_byte fdc_read_dir(FDC_PTR self);
/* Write to the Data Rate Register. */
void fdc_write_drr(FDC_PTR self, fdc_byte value);

typedef struct floppy_drive_vtable
{
	fd_err_t  (*fdv_seek_cylinder)(FDRV_PTR fd, int cylinder);
	fd_err_t  (*fdv_read_id)      (FDRV_PTR fd, int head, 
                                 int sector, fdc_byte *buf);
	fd_err_t  (*fdv_read_sector  )(FDRV_PTR fd, int xcylinder, 
		int xhead, int head, int sector, fdc_byte *buf, int len, 
		int *deleted, int skip_deleted, int mfm, int multi);
	fd_err_t  (*fdv_read_track   )(FDRV_PTR fd, int xcylinder,
		int xhead, int head, fdc_byte *buf, int *len);
	fd_err_t  (*fdv_write_sector )(FDRV_PTR fd, int xcylinder,
		int xhead, int head, int sector, fdc_byte *buf, int len,
		int deleted, int skip_deleted, int mfm, int multi);
	fd_err_t (*fdv_format_track )(FDRV_PTR fd, int head, 
		int sectors, fdc_byte *buf, fdc_byte filler);
	fdc_byte (*fdv_drive_status )(FDRV_PTR fd);
	int      (*fdv_isready)(FDRV_PTR fd);
	int	 (*fdv_dirty  )(FDRV_PTR fd);
	void     (*fdv_eject  )(FDRV_PTR fd);
	void	 (*fdv_set_datarate)(FDRV_PTR fd, fdc_byte rate);
	void     (*fdv_reset  )(FDRV_PTR fd);
	void     (*fdv_destroy)(FDRV_PTR fd);
	int	 (*fdv_changed)(FDRV_PTR fd);
} FLOPPY_DRIVE_VTABLE;


typedef struct floppy_drive
{
/* PRIVATE variables 
 * The following points to the drive's method table. You should not need
 * to use this; instead, use the fd_*() wrapper functions below. */
	FLOPPY_DRIVE_VTABLE * fd_vtable;	
/* PUBLIC members */
	/* You should set the first three of these immediately after calling 
         * fd_init() or fdd_init() on a drive.*/
	int fd_type;	  /* 0 for none, 1 for 3", 2 for 3.5", 3 for 5.25" */
	int fd_heads;	  /* No. of heads in the drive: 1 or 2 */
	int fd_cylinders; /* No. of cylinders the drive can access: 
			   * eg: a nominally 40-track drive can usually go up
                           * to 42 tracks with a bit of "persuasion" */
	int fd_readonly;  /* Is the drive (or the disc therein) set to R/O? */
	int fd_changed;	  /* Default changeline implementation. This will be 
			   * set to 1 when drive is ejected, 0 at controller
			   * partial reset. If you can write a better 
			   * implementation of the changeline, override 
			   * fdv_changed(). */

/* READONLY variables */
	int fd_motor;	  /* Is the motor for this drive running? */
	int fd_cylinder;  /* Current cylinder. Note that if the drive is
			   * double-stepping, this is the "real" cylinder - 
			   * so it could = 24 and be reading cylinder 12 
                           * of a 40-track DSK file. */
} FLOPPY_DRIVE;

/* Subclass of FLOPPY_DRIVE: a drive which emulates discs using the CPCEMU 
 * .DSK format */

typedef struct dsk_floppy_drive
{
/* PUBLIC variables: */
	FLOPPY_DRIVE fdd;		/* Base class */
	char  fdd_filename[PATH_MAX];	/* Filename to .DSK file. Before 
					 * changing this call fd_eject() on
                                         * the drive */
/* PRIVATE variables: */
	FILE *fdd_fp;			/* File of the .DSK file */
	fdc_byte fdd_disk_header[256];	/* .DSK header */
	fdc_byte fdd_track_header[256];	/* .DSK track header */
	int fdd_dirty;			/* Has this disk been written to? */
} DSK_FLOPPY_DRIVE;

#ifdef DSK_ERR_OK	/* LIBDSK headers included */
typedef struct libdsk_floppy_drive
{
/* PUBLIC variables: */
	FLOPPY_DRIVE fdl;		/* Base class */
	char  fdl_filename[PATH_MAX];	/* Filename to .DSK file. Before 
					 * changing this call fd_eject() on
                                         * the drive */
	const char  *fdl_type;		/* LIBDSK drive type, NULL for auto */
	const char  *fdl_compress;	/* LIBDSK compression, NULL for auto */
/* PRIVATE variables: */
	DSK_PDRIVER  fdl_diskp;	
	DSK_GEOMETRY fdl_diskg;		/* Autoprobed geometry */
} LIBDSK_FLOPPY_DRIVE;
#endif	/* ifdef DSK_ERR_OK */

typedef struct nc9_floppy_drive
{
	FLOPPY_DRIVE fdd;		/* Base class */
	FLOPPY_DRIVE *nc9_fdd;		/* Pointer to the 9256's B drive */
} NC9_FLOPPY_DRIVE;


FDRV_PTR fd_inew(size_t size);


/* This class represents the controller itself. When you instantiate one, call
 * fdc_reset() on it before doing anything else with it. */

typedef struct fdc_765
{
    /* PRIVATE variables */
	int fdc_interrupting;	/* 0 => Not interrupting
				 * 1 => Entering result phase of 
				 *      Read/Write/Format/Scan
				 * 2 => Ready for data transfer 
				 *      (execution phase) 
				 * 4 => End of Seek/Recalibrate command */
	/* The results from the SPECIFY command */
	int fdc_specify[2];
	/* The last sector for which a DD READ ID request was made */
	int fdc_lastidread; 

	/* Current WRITE command is for deleted data? */
	int fdc_write_deleted;

	/* Command phase buffer */
	int fdc_cmd_id;   	/* Current command */
	int fdc_cmd_len;  	/* No. of bytes remaining to transfer */
	int fdc_cmd_pos;	/* Next buffer position to write */
	fdc_byte fdc_cmd_buf[20];	/* The command as a byte string */

	/* Execution phase buffer */
	fdc_byte fdc_exec_buf[MAX_SECTOR_LEN];
	int  fdc_exec_len;	/* No. of bytes remaining to transfer */
	int  fdc_exec_pos;	/* Position in buffer */

	/* Results phase buffer */
	fdc_byte fdc_result_buf[20];
	int fdc_result_len;	/* No. of bytes remaining to transfer */	
	int fdc_result_pos;	/* Position in buffer */

	int fdc_terminal_count;	/* Set to abort a transfer */	
	int fdc_isr_countdown;	/* Countdown to interrupt */

	int fdc_dor;		/* Are we using that horrible kludge, the
				 * Digital Output Register, rather than
				 * proper drive select lines? */
	/* Drive pointers after the DOR has had its wicked way */
	FLOPPY_DRIVE *fdc_dor_drive[4];

	/* READONLY variables - these can be used in status displays */
        /* The four uPD765A status registers */
        int fdc_st0, fdc_st1, fdc_st2, fdc_st3;
        /* The main status register */
        int fdc_mainstat;
	int fdc_curunit, fdc_curhead;   /* Currently active unit & head */

	/* Public variables */
	void (*fdc_isr)(struct fdc_765 *self, int status);
		/* EXT: Called when interrupt line is raised or lowered.
		 *     You must provide this if the FDC is to interrupt. */
	FLOPPY_DRIVE *fdc_drive[4];
		/* The FDC's four drives. You must set these pointers */

} FDC_765;

#ifdef __cplusplus
}
#endif                                                                          

#endif /* __LIB765_H__ */
