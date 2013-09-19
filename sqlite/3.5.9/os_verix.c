/**
 * Verix OS
 * Mauricio Aniche
 */

#include "sqliteInt.h"
#if OS_VERIX               /* This file is used for Verix only */

#include <Logsys.h>
extern int errno;

#include <stdlib.h>
#include <string.h>
#include <svc.h>
#include <svctxo.h>

/*
** Include code that is common to all os_*.c files
*/
#include "os_common.h"

/*
** The verixFile structure is a subclass of sqlite3_file* specific to the Verix
** portability layer.
*/
#define MAX_VERIX_FILE_NAME	50
typedef struct verixFile verixFile;
struct verixFile {
  const sqlite3_io_methods *pMethod;/* Must be first */

  int h;				  /* Handle for accessing the file */
  int temporary;		  /* Temporary file? 1 or 0 */
  char filename[MAX_VERIX_FILE_NAME];	  /* File name! */
};


/*****************************************************************************
** The next group of routines implement the I/O methods specified
** by the sqlite3_io_methods object.
******************************************************************************/

/*
** Close a file.
*/
static int verixClose(sqlite3_file *id){
  int rc;
  verixFile *pFile = (verixFile*)id;

  LOG_PRINTF(("verixClose"));
  rc = close(pFile->h);
  if(rc!=0) return SQLITE_IOERR;

  if(pFile->temporary==1) {
	  rc = _remove(pFile->filename);
  }

  return (rc==0?SQLITE_OK:SQLITE_IOERR);
}

/*
** Read data from a file into a buffer.  Return SQLITE_OK if all
** bytes were read successfully and SQLITE_IOERR if anything goes
** wrong.
*/
static int verixRead(
  sqlite3_file *id,          /* File to read from */
  void *pBuf,                /* Write content into this buffer */
  int amt,                   /* Number of bytes to read */
  sqlite3_int64 offset       /* Begin reading at this offset */
){
  //long upperBits = (offset>>32) & 0x7fffffff;
  //long lowerBits = offset & 0xffffffff;
  int _offset = (int)offset;
  int ls;
  int rd;
  verixFile *pFile = (verixFile*)id;

  assert( id!=0 );

  ls = lseek(pFile->h,_offset,SEEK_SET);
  if(ls==-1) {
	  LOG_PRINTF(("verixRead lseek %d %d errno %d", pFile->h, _offset, errno ));
	  return SQLITE_IOERR_READ;
  }

  rd = read(pFile->h,(char*)pBuf,amt);
  if(rd==amt) return SQLITE_OK;
  else {
	  LOG_PRINTF(("read rd %d amt %d errno %d", rd, amt, errno ));
	  return SQLITE_IOERR_SHORT_READ;
  }
}

/*
** Write data from a buffer into a file.  Return SQLITE_OK on success
** or some other error code on failure.
*/
static int verixWrite(
  sqlite3_file *id,         /* File to write into */
  const void *pBuf,         /* The bytes to be written */
  int amt,                  /* Number of bytes to write */
  sqlite3_int64 offset      /* Offset into the file to begin writing at */
){
  //long upperBits = (offset>>32) & 0x7fffffff;
  //long lowerBits = offset & 0xffffffff;
  int _offset = (int)offset;
  int w;
  int ls;
  verixFile *pFile = (verixFile*)id;

  LOG_PRINTF(("verixWrite handle %d", pFile->h));
  assert( id!=0 );

  ls = lseek(pFile->h,_offset,SEEK_SET);
  if(ls==-1) {
	  LOG_PRINTF(("LSeek errno %d", errno ));
	  return SQLITE_IOERR_WRITE;
  }

  assert( amt>0 );

  while(amt>0) {
	w = write(pFile->h,(char*)pBuf,amt);
	if(w==-1) {
		LOG_PRINTF(("Write errno %d", errno ));
		return SQLITE_IOERR_WRITE;
	}

	amt -= w;
	pBuf = &((char*)pBuf)[w];
  }

  return SQLITE_OK;
}

/*
** Truncate an open file to a specified size
*/
static int verixTruncate(sqlite3_file *id, sqlite3_int64 nByte){
  //long upperBits = (nByte>>32) & 0x7fffffff;
  //long lowerBits = nByte & 0xffffffff;
  int _nByte = (int)nByte;
  verixFile *pFile = (verixFile*)id;
  long size;


  LOG_PRINTF(("verixTruncate"));

  if (get_file_size(pFile->h,&size)!=0) {
	  LOG_PRINTF(("get_file_size errno %d", errno ));
	  return SQLITE_IOERR_TRUNCATE;
  }

  if (lseek(pFile->h,_nByte,SEEK_SET)==-1) {
	  LOG_PRINTF(("LSeek errno %d", errno ));
	  return SQLITE_IOERR_TRUNCATE;
  }

  if (delete_(pFile->h, size-_nByte)!=0) {
	  LOG_PRINTF(("delete_ errno %d", errno ));
	  return SQLITE_IOERR_TRUNCATE;
  }

  return SQLITE_OK;
}

/*
** Make sure all writes to a particular file are committed to disk.
*/
static int verixSync(sqlite3_file *id, int flags){
  return SQLITE_OK;
}

/*
** Determine the current size of a file in bytes
*/
static int verixFileSize(sqlite3_file *id, sqlite3_int64 *pSize){
  verixFile *pFile = (verixFile*)id;
  long size=0;

  LOG_PRINTF(("verixFileSize handle %d filename %s", pFile->h, pFile->filename));

  size = dir_get_file_sz(pFile->filename);
  if (size==-1) {
	  LOG_PRINTF(("get_file_size errno %d", errno ));
	  return SQLITE_IOERR_FSTAT;
  }

  LOG_PRINTF(("verixFileSize size %d", size));
  *pSize = ((sqlite3_int64)size);
  return SQLITE_OK;
}


/*
** Lock the file with the lock specified by parameter locktype.
*/
static int verixLock(sqlite3_file *id, int locktype){
  verixFile *pFile = (verixFile*)id;

  LOG_PRINTF(("verixLock"));

  return (lock(pFile->h,0,0)==0?SQLITE_OK:SQLITE_BUSY);
}

/*
** This routine checks if there is a RESERVED lock held on the specified
** file by this or any other process. If such a lock is held, return
** non-zero, otherwise zero.
*/
static int verixCheckReservedLock(sqlite3_file *id){
  verixFile *pFile = (verixFile*)id;

  LOG_PRINTF(("verixCheckReservedLock"));

  if(lock(pFile->h,0,0)==0) {
	unlock(pFile->h,0,0);
	return 0;
  }

  return 1;
}

/*
** Lower the locking level on file descriptor id to locktype.
*/
static int verixUnlock(sqlite3_file *id, int locktype){
  verixFile *pFile = (verixFile*)id;
  
  LOG_PRINTF(("verixUnlock"));

  return (unlock(pFile->h,0,0)==0?SQLITE_OK:SQLITE_IOERR_UNLOCK);
}

/*
** Control and query of the open file handle.
*/
static int verixFileControl(sqlite3_file *id, int op, void *pArg){

	LOG_PRINTF(("verixFileControl"));

  return SQLITE_OK;
}

/*
** Return the sector size in bytes of the underlying block device for
** the specified file. This is almost always 512 bytes, but may be
** larger for some devices.
**
** SQLite code assumes this function cannot fail. It also assumes that
** if two files are created in the same file-system directory (i.e.
** a database and its journal file) that the sector size will be the
** same for both.
*/
static int verixSectorSize(sqlite3_file *id){

	LOG_PRINTF(("verixSectorSize"));

  return SQLITE_DEFAULT_SECTOR_SIZE;
}

/*
** Return a vector of device characteristics.
*/
static int verixDeviceCharacteristics(sqlite3_file *id){
	LOG_PRINTF(("verixDeviceCharacteristics"));

  return 0;
}

/*
** This vector defines all the methods that can operate on an
** sqlite3_file for win32.
*/
static const sqlite3_io_methods verixIoMethod = {
  1,                        /* iVersion */
  verixClose,
  verixRead,
  verixWrite,
  verixTruncate,
  verixSync,
  verixFileSize,
  verixLock,
  verixUnlock,
  verixCheckReservedLock,
  verixFileControl,
  verixSectorSize,
  verixDeviceCharacteristics
};

/***************************************************************************
** Here ends the I/O methods that form the sqlite3_io_methods object.
**
** The next block of code implements the VFS methods.
****************************************************************************/

/*
** Open a file.
*/
static int verixOpen(
  sqlite3_vfs *pVfs,        /* Not used */
  const char *zName,        /* Name of the file (UTF-8) */
  sqlite3_file *id,         /* Write the SQLite file handle here */
  int flags,                /* Open mode flags */
  int *pOutFlags            /* Status return flags */
){
  int h;
  int openFlags=0;
  int isTemp=0;
  verixFile *pFile = (verixFile*)id;

  LOG_PRINTF(("verixOpen %s", zName));

  if( flags & SQLITE_OPEN_READWRITE ){
    openFlags = O_RDWR;
  }
  else{
    openFlags = O_RDONLY;
  }

  if( flags & SQLITE_OPEN_CREATE ){
    openFlags = openFlags | O_CREAT;
  }
  if( flags & SQLITE_OPEN_DELETEONCLOSE ){
    isTemp = 1;
  }

  LOG_PRINTF(("openFlags %d", openFlags));
  h = open(zName,openFlags);

  if( h==-1 ){
	LOG_PRINTF(("Open errno %d", errno ));
	return SQLITE_CANTOPEN;
  }

  LOG_PRINTF(("Open handle %d", h ));

  if( pOutFlags ){
    if( flags & SQLITE_OPEN_READWRITE ){
      *pOutFlags = SQLITE_OPEN_READWRITE;
    }else{
      *pOutFlags = SQLITE_OPEN_READONLY;
    }
  }

  memset(pFile, 0, sizeof(*pFile));
  pFile->pMethod = &verixIoMethod;
  pFile->h = h;
  pFile->temporary = isTemp;
  memset(pFile->filename,0x0,MAX_VERIX_FILE_NAME);
  strcpy(pFile->filename, zName);

  return SQLITE_OK;
}

/*
** Delete the named file.
**
*/
static int verixDelete(
  sqlite3_vfs *pVfs,          /* Not used on win32 */
  const char *zFilename,      /* Name of file to delete */
  int syncDir                 /* Not used on win32 */
){
	LOG_PRINTF(("verixDelete"));

  return _remove(zFilename)==0 ? SQLITE_OK : SQLITE_IOERR_DELETE;
}

/*
** Check the existance and status of a file.
*/
static int verixAccess(
  sqlite3_vfs *pVfs,         /* Not used on win32 */
  const char *zFilename,     /* Name of file to check */
  int flags                  /* Type of test to make on this file */
){
  int attr;
  int rc;
  
  LOG_PRINTF(("verixAccess"));

  attr = dir_get_attributes(zFilename);

  switch( flags ){
    case SQLITE_ACCESS_READ:
    case SQLITE_ACCESS_EXISTS:
      rc = attr!=-1;
      break;
    case SQLITE_ACCESS_READWRITE:
      rc = (attr & ATTR_READONLY)==0;
      break;
    default:
      assert(!"Invalid flags argument");
  }
  return rc;
}


/*
** Create a temporary file name in zBuf.  zBuf must be big enough to
** hold at pVfs->mxPathname characters.
*/
static int verixGetTempname(sqlite3_vfs *pVfs, int nBuf, char *zBuf){

	LOG_PRINTF(("verixGetTempname"));

  sprintf(zBuf,"tmp%d", read_ticks());
  return SQLITE_OK; 
}

/*
** Turn a relative pathname into a full pathname.
*/
static int verixFullPathname(
  sqlite3_vfs *pVfs,            /* Pointer to vfs object */
  const char *zRelative,        /* Possibly relative input path */
  int nFull,                    /* Size of output buffer in bytes */
  char *zFull                   /* Output buffer */
){
	LOG_PRINTF(("verixFullPathname"));

  sqlite3_snprintf(pVfs->mxPathname, zFull, "%s", zRelative);
  return SQLITE_OK;
}

/*
** Interfaces for opening a shared library, finding entry points
** within the shared library, and closing the shared library.
*/
#define verixDlOpen  0
#define verixDlError 0
#define verixDlSym   0
#define verixDlClose 0


/*
** Write up to nBuf bytes of randomness into zBuf.
*/
static int verixRandomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
  int n = nBuf;

  LOG_PRINTF(("verixRandomness nBuf %d", nBuf));

  while(nBuf>0) {
	zBuf[0] = rand()%255;
	
	nBuf--;
	zBuf = &((char*)zBuf)[1];
  }
  
  return n;
}


/*
** Sleep for a little while.  Return the amount of time slept.
*/
static int verixSleep(sqlite3_vfs *pVfs, int microsec){

	LOG_PRINTF(("verixSleep"));

  SVC_WAIT((microsec+999)/1000);
  return ((microsec+999)/1000)*1000;
}

/*
** Find the current time (in Universal Coordinated Time).  Write the
** current time and date as a Julian Day number into *prNow and
** return 0.  Return 1 if the time and date cannot be found.
*/
int verixCurrentTime(sqlite3_vfs *pVfs, double *prNow){

	char day[3]={0x0,0x0,0x0};
    char month[3]={0x0,0x0,0x0};
    char year[3]={0x0,0x0,0x0};
    char hour[3]={0x0,0x0,0x0};
    char minute[3]={0x0,0x0,0x0};
	char second[3]={0x0,0x0,0x0};
	char buf[16];

	int a, m, y;
	double jdn, jd;

	LOG_PRINTF(("verixCurrentTime"));

	memset(buf,0x0,sizeof(buf));
    read_clock ( buf );

    memcpy ( day, &(buf[6]), 2 );
    memcpy ( month, &(buf[4]), 2 );
    memcpy ( year, &(buf[2]), 2 );
    memcpy ( hour, &(buf[8]), 2 );
    memcpy ( minute, &(buf[10]), 2 );
	memcpy ( second, &(buf[12]), 2 );

	// Calc!
	a = (14-atoi(month))/12;
	y = atoi(year) + 4800 -a;
	m = atoi(month) + 12*a - 3;
	jdn = atoi(day) + ((153*m+2)/5) + 365*y + y/4 - y/100 + y/400 - 32045;
	jd = jdn + (atoi(hour)-12)/24 + atoi(minute)/1440 + atoi(second)/86400;

	*prNow = jd;

  return 0;
}


/*
** Return a pointer to the sqlite3DefaultVfs structure.   We use
** a function rather than give the structure global scope because
** some compilers (MSVC) do not allow forward declarations of
** initialized structures.
*/
sqlite3_vfs *sqlite3OsDefaultVfs(void){
  static sqlite3_vfs verixVfs = {
    1,                 /* iVersion */
    sizeof(verixFile),   /* szOsFile */
    20,          /* mxPathname */
    0,                 /* pNext */
    "verix",           /* zName */
    0,                 /* pAppData */
  
    verixOpen,           /* xOpen */
    verixDelete,         /* xDelete */
    verixAccess,         /* xAccess */
    verixGetTempname,    /* xGetTempName */
    verixFullPathname,   /* xFullPathname */
    verixDlOpen,         /* xDlOpen */
    verixDlError,        /* xDlError */
    verixDlSym,          /* xDlSym */
    verixDlClose,        /* xDlClose */
    verixRandomness,     /* xRandomness */
    verixSleep,          /* xSleep */
    verixCurrentTime     /* xCurrentTime */
  };
  
  return &verixVfs;
}

#endif /* OS_VERIX */
