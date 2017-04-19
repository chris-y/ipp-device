/* config.h.  Generated by configure.  */
/*
 * "$Id: config.h.in,v 1.75 2004/02/25 20:14:50 mike Exp $"
 *
 *   Configuration file for the Common UNIX Printing System (CUPS).
 *
 *   @configure_input@
 *
 *   Copyright 1997-2004 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Easy Software Products and are protected by Federal
 *   copyright law.  Distribution and use rights are outlined in the file
 *   "LICENSE.txt" which should have been included with this file.  If this
 *   file is missing or damaged please contact Easy Software Products
 *   at:
 *
 *       Attn: CUPS Licensing Information
 *       Easy Software Products
 *       44141 Airport View Drive, Suite 204
 *       Hollywood, Maryland 20636-3111 USA
 *
 *       Voice: (301) 373-9600
 *       EMail: cups-info@cups.org
 *         WWW: http://www.cups.org
 */

#ifndef _CUPS_CONFIG_H_
#define _CUPS_CONFIG_H_

/*
 * Version of software...
 */

#define CUPS_SVERSION "CUPS v1.1.21rc1"


/*
 * Default user and group...
 */

#define CUPS_DEFAULT_USER "lp"
#define CUPS_DEFAULT_GROUP "sys"


/*
 * Maximum number of file descriptors to support.
 */

#define CUPS_MAX_FDS 4096


/*
 * Where are files stored?
 */

#define CUPS_LOCALEDIR "/usr/share/locale"
#define CUPS_SERVERROOT "/etc/cups"
#define CUPS_SERVERBIN "/usr/lib/cups"
#define CUPS_DOCROOT "/usr/share/doc/cups"
#define CUPS_REQUESTS "/var/spool/cups"
#define CUPS_LOGDIR "/var/log/cups"
#define CUPS_DATADIR "/usr/share/cups"
#define CUPS_FONTPATH "/usr/share/cups/fonts"


/*
 * What is the format string for strftime?
 */

#define CUPS_STRFTIME_FORMAT "%c"


/*
 * Do we have various image libraries?
 */

#define HAVE_LIBPNG 1
#define HAVE_LIBZ 1
#define HAVE_LIBJPEG 1
#define HAVE_LIBTIFF 1


/*
 * Does this machine store words in big-endian (MSB-first) order?
 */

/* #undef WORDS_BIGENDIAN */


/*
 * Which directory functions and headers do we use?
 */

#define HAVE_DIRENT_H 1
/* #undef HAVE_SYS_DIR_H */
/* #undef HAVE_SYS_NDIR_H */
/* #undef HAVE_NDIR_H */


/*
 * Do we have PAM stuff?
 */

#ifndef HAVE_LIBPAM
#define HAVE_LIBPAM 1
#endif /* !HAVE_LIBPAM */

/* #undef HAVE_PAM_PAM_APPL_H */


/*
 * Do we have <shadow.h>?
 */

#define HAVE_SHADOW_H 1


/*
 * Do we have <crypt.h>?
 */

#define HAVE_CRYPT_H 1


/*
 * Use <string.h>, <strings.h>, and/or <bstring.h>?
 */

#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
/* #undef HAVE_BSTRING_H */


/*
 * Do we have the strXXX() functions?
 */

#define HAVE_STRDUP 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
/* #undef HAVE_STRLCAT */
/* #undef HAVE_STRLCPY */


/*
 * Do we have the vsyslog() function?
 */

#define HAVE_VSYSLOG 1


/*
 * Do we have the (v)snprintf() functions?
 */

#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1


/*
 * What signal functions to use?
 */

/* #undef HAVE_SIGSET */
/*#define HAVE_SIGACTION 1*/


/*
 * What wait functions to use?
 */

#define HAVE_WAITPID 1
#define HAVE_WAIT3 1


/*
 * Do we have the mallinfo function and malloc.h?
 */

/* #undef HAVE_MALLINFO */
#define HAVE_MALLOC_H 1


/*
 * Which encryption libraries do we have?
 */

/* #undef HAVE_CDSASSL */
/* #undef HAVE_GNUTLS */
/* #undef HAVE_LIBSSL */
/* #undef HAVE_SSL */


/*
 * Do we have the OpenSLP library?
 */

/* #undef HAVE_LIBSLP */


/*
 * Do we have libpaper?
 */

/* #undef HAVE_LIBPAPER */


/*
 * Do we have <sys/ioctl.h>?
 */

#define HAVE_SYS_IOCTL_H 1


/*
 * Do we have mkstemp() and/or mkstemps()?
 */

#define HAVE_MKSTEMP 1
/* #undef HAVE_MKSTEMPS */


/*
 * Does the "tm" structure contain the "tm_gmtoff" member?
 */

#define HAVE_TM_GMTOFF 1


/*
 * Do we have rresvport()?
 */

#define HAVE_RRESVPORT 1


/*
 * Do we have getifaddrs()?
 */

#define HAVE_GETIFADDRS 1


/*
 * Do we have hstrerror()?
 */

#define HAVE_HSTRERROR 1


/*
 * Do we have the <sys/sockio.h> header file?
 */

/* #undef HAVE_SYS_SOCKIO_H */


/*
 * Does the sockaddr structure contain an sa_len parameter?
 */

/* #undef HAVE_STRUCT_SOCKADDR_SA_LEN */


/*
 * Various scripting languages...
 */

/* #undef HAVE_JAVA */
#define CUPS_JAVA ""
#define HAVE_PERL 1
#define CUPS_PERL "/usr/bin/perl"
#define HAVE_PHP 1
#define CUPS_PHP "/usr/bin/php"
#define HAVE_PYTHON 1
#define CUPS_PYTHON "/usr/bin/python"


#endif /* !_CUPS_CONFIG_H_ */

/*
 * End of "$Id: config.h.in,v 1.75 2004/02/25 20:14:50 mike Exp $".
 */
