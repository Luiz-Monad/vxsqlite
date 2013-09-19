
/**
 * Verix OS
 * Mauricio Aniche (T_Mauricio_A1)
 */

#if SQLITE_OS_VERIX               /* This file is used for Verix only */

#include <Logsys.h>
#include <stdlib.h>
#include <string.h>
#include <svc.h>
#include <svc_sec.h>
#include <svctxo.h>
#include <errno.h>

/*
** The verixFile structure is a subclass of sqlite3_file* specific to the Verix
** portability layer.
*/
#define MAX_VERIX_FILE_NAME	50
typedef struct verixFile verixFile;
struct verixFile {
  const sqlite3_io_methods *pMethod;/* Must be first */

  int h;				  /* Handle for accessing the file */
  short temporary;		  /* Temporary file? 1 or 0 */
  char filename[MAX_VERIX_FILE_NAME];	  /* File name! */
};

short verified = 1;
int last_errno = 0;

// 0 = registered
static int verixLicensedVersion() {
	int h;
	int rd;
	char sn[11+1];
	char buf[30];
	char license[12];
	char sha[20];
	int iOldGroup;

	if(verified) {
		memset(sn,0x0,sizeof(sn));
		SVC_INFO_SERLNO(sn);
		sprintf(buf,"%san1che%s", sn, SQLITE_VERSION);
		SHA1(0, (unsigned char*)buf, strlen(buf), (unsigned char*)sha);

		iOldGroup = get_group();
		set_group(15);
		h = open("vxsqlite.lic", O_RDONLY);
		set_group(iOldGroup);

		if(h<=0) return 1;
		rd = read(h, license, 12);
		close(h);
		if(rd!=12) return 1;

		verified = memcmp(license,&(sha[5]), 12);
	}

	return verified;
}

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

  rc = close(pFile->h);
  if(rc!=0) {
	  last_errno = errno;
	  return SQLITE_IOERR;
  }

  if(pFile->temporary==1) {
	  rc = _remove(pFile->filename);
  }

  LOG_PRINTF(("verixClose %d temp %s ret %d", pFile->h, pFile->filename, rc));

  if(rc==0) {
	return SQLITE_OK;
  }
  else {
	last_errno = errno;
	return SQLITE_IOERR;
  }
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
	  last_errno = errno;
	  LOG_PRINTF(("verixRead lseek %d %d errno %d", pFile->h, _offset, errno ));
	  memset(pBuf, 0, amt);
	  return SQLITE_IOERR_SHORT_READ;
  }

  rd = read(pFile->h,(char*)pBuf,amt);
  if(rd==amt) {
	  LOG_PRINTF(("verixRead amt %d read %d offset %d handle %d", amt, rd, _offset, pFile->h));
	  return SQLITE_OK;
  }
  else {
	  LOG_PRINTF(("verixRead short_read rd %d amt %d errno %d", rd, amt, errno ));
	  last_errno = errno;
	  memset(&((char*)pBuf)[rd], 0, amt-rd);
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
  int total = 0;
  int tries = 0;
#ifdef LOGSYS_FLAG
  int aamt = amt;
#endif

  verixFile *pFile = (verixFile*)id;

  assert( id!=0 );

  ls = lseek(pFile->h,_offset,SEEK_SET);
  if(ls==-1) {
	  LOG_PRINTF(("verixWrite lseek %d %d errno %d", pFile->h, _offset, errno ));
	  last_errno = errno;
	  return SQLITE_IOERR_WRITE;
  }

  assert( amt>0 );

  while(amt>0) {
	w = write(pFile->h,(char*)pBuf,amt);
	if(w==-1) {
		last_errno = errno;
		LOG_PRINTF(("verixWrite Write errno %d", errno ));
		return SQLITE_IOERR_WRITE;
	}

	total += w; tries++;
	amt -= w;
	pBuf = &((char*)pBuf)[w];
  }

  LOG_PRINTF(("verixWrite amt %d wrote %d in %d tries handle %d", aamt, total, tries, pFile->h));

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

  if (get_file_size(pFile->h,&size)!=0) {
	  LOG_PRINTF(("get_file_size errno %d", errno ));
	  last_errno = errno;
	  return SQLITE_IOERR_TRUNCATE;
  }

  if (lseek(pFile->h,_nByte,SEEK_SET)==-1) {
	  LOG_PRINTF(("verixTruncate lseek %d %d errno %d", pFile->h, _nByte, errno ));
	  last_errno = errno;
	  return SQLITE_IOERR_TRUNCATE;
  }

  if (delete_(pFile->h, size-_nByte)!=0) {
	  LOG_PRINTF(("verixTruncate delete_ errno %d", errno ));
	  last_errno = errno;
	  return SQLITE_IOERR_TRUNCATE;
  }

  LOG_PRINTF(("verixTruncate %d ret 0", _nByte));

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

  size = dir_get_file_sz(pFile->filename);
  if (size==-1) {
	  last_errno = errno;
	  LOG_PRINTF(("verixFileSize get_file_size errno %d", errno ));
	  return SQLITE_IOERR_FSTAT;
  }

  LOG_PRINTF(("verixFileSize handle %d filename %s size %d", pFile->h, pFile->filename, size));
  *pSize = ((sqlite3_int64)size);
  return SQLITE_OK;
}


/*
** Lock the file with the lock specified by parameter locktype.
*/
static int verixLock(sqlite3_file *id, int locktype){
  verixFile *pFile = (verixFile*)id;
  int ret;
  ret = lock(pFile->h,0,0);
  LOG_PRINTF(("verixLock %d ret %d", pFile->h, ret));

  if(ret==0) {
	  return SQLITE_OK;
  }
  else {
	  last_errno = errno;
	  return SQLITE_BUSY;
  }
}

/*
** This routine checks if there is a RESERVED lock held on the specified
** file by this or any other process. If such a lock is held, return
** non-zero, otherwise zero.
*/
static int verixCheckReservedLock(sqlite3_file *id, int *pResOut){
  verixFile *pFile = (verixFile*)id;
  int rc;

  if(lock(pFile->h,0,0)==0) {
	unlock(pFile->h,0,0);
	rc = 0;
  }

  rc = 1;
  *pResOut = rc;

  LOG_PRINTF(("verixCheckReservedLock %d", rc));
  
  return SQLITE_OK;
}

/*
** Lower the locking level on file descriptor id to locktype.
*/
static int verixUnlock(sqlite3_file *id, int locktype){
  verixFile *pFile = (verixFile*)id;
  
  int ret;
  ret = unlock(pFile->h,0,0);
  LOG_PRINTF(("verixUnlock %d ret %d", pFile->h, ret));

  if (ret==0) {
	return SQLITE_OK;
  }
  else {
	  last_errno = errno;
	  return SQLITE_IOERR_UNLOCK;
  }
}

/*
** Control and query of the open file handle.
*/
static int verixFileControl(sqlite3_file *id, int op, void *pArg){

	LOG_PRINTF(("verixFileControl 0"));

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
#define VERIX_SECTOR_SIZE SQLITE_DEFAULT_SECTOR_SIZE
//#define VERIX_SECTOR_SIZE 128
	LOG_PRINTF(("verixSectorSize %d", VERIX_SECTOR_SIZE));

  return VERIX_SECTOR_SIZE;
}

/*
** Return a vector of device characteristics.
*/
static int verixDeviceCharacteristics(sqlite3_file *id){
	UNUSED_PARAMETER(id);
	LOG_PRINTF(("verixDeviceCharacteristics 0"));

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
  short isTemp=0;
  char filename[50];
  verixFile *pFile = (verixFile*)id;

  if(verixLicensedVersion()) return SQLITE_CANTOPEN;

  if(!zName) {
	sprintf(filename,"tmp%d", read_ticks());
  }
  else {
	sprintf(filename,zName);
  }

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
  h = open(filename,openFlags);

  if( h==-1 ){
	LOG_PRINTF(("Open errno %d", errno ));
	last_errno = errno;
	return SQLITE_CANTOPEN;
  }
  
  LOG_PRINTF(("verixOpen %s flags %d handle %d", zName, openFlags, h ));

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
	int ret;
	ret = _remove(zFilename);
	LOG_PRINTF(("verixDelete %s %d", zFilename, ret));

	if(ret==0) {
		return SQLITE_OK;
	}
	else {
		last_errno = errno;
		return SQLITE_IOERR_DELETE;
	}
}

/*
** Check the existance and status of a file.
*/
static int verixAccess(
  sqlite3_vfs *pVfs,         /* Not used on win32 */
  const char *zFilename,     /* Name of file to check */
  int flags,                 /* Type of test to make on this file */
  int *pResOut               /* OUT: Result */
){
  int attr;
  int rc;

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

  LOG_PRINTF(("verixAccess %d", rc));

  *pResOut = rc;
  return SQLITE_OK;
}


/*
** Create a temporary file name in zBuf.  zBuf must be big enough to
** hold at pVfs->mxPathname characters.
*/
/* DEPRECATED
static int verixGetTempname(sqlite3_vfs *pVfs, int nBuf, char *zBuf){

	LOG_PRINTF(("verixGetTempname"));

  sprintf(zBuf,"tmp%d", read_ticks());
  return SQLITE_OK; 
}
*/

/*
** Turn a relative pathname into a full pathname.
*/
static int verixFullPathname(
  sqlite3_vfs *pVfs,            /* Pointer to vfs object */
  const char *zRelative,        /* Possibly relative input path */
  int nFull,                    /* Size of output buffer in bytes */
  char *zFull                   /* Output buffer */
){
	LOG_PRINTF(("verixFullPathname zRelative %s", zRelative));

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

	LOG_PRINTF(("verixSleep %d", microsec));

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

static int verixGetLastError(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
	char msg[20];
	
	sprintf(msg,"verix errno: %d", last_errno);
	if(strlen(msg)>=nBuf) {
		memcpy(zBuf,msg,nBuf);
		return 1;
	}
	else {
		memcpy(zBuf,msg,strlen(msg));
		return 0;
	}
}

int sqlite3_os_init(void){
  static sqlite3_vfs verixVfs = {
    1,                 /* iVersion */
    sizeof(verixFile), /* szOsFile */
    128,			/* mxPathname */
    0,                 /* pNext */
    "verix",           /* zName */
    0,                 /* pAppData */
 
    verixOpen,           /* xOpen */
    verixDelete,         /* xDelete */
    verixAccess,         /* xAccess */
    verixFullPathname,   /* xFullPathname */
    verixDlOpen,         /* xDlOpen */
    verixDlError,        /* xDlError */
    verixDlSym,          /* xDlSym */
    verixDlClose,        /* xDlClose */
    verixRandomness,     /* xRandomness */
    verixSleep,          /* xSleep */
    verixCurrentTime,    /* xCurrentTime */
    verixGetLastError    /* xGetLastError */
  };
  sqlite3_vfs_register(&verixVfs, 1);
  return SQLITE_OK; 
}
int sqlite3_os_end(void){ 
  return SQLITE_OK;
}

#endif /* OS_VERIX */
