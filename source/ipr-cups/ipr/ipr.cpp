/*
 *
 *  Copyright (c) 2004 Peter Michalek All Rights Reserved.
 *
 *

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Attn: ipr@michalek.org
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <cups/string.h>
#include <cups/cups.h>
#include <cups/debug.h>
#include <errno.h>
#include <assert.h>
#include "cups/ipp.h"
#include <fcntl.h>
#ifdef WIN32
#  include <io.h>
#pragma  warning( disable : 4786 )
#else
#  include <unistd.h>
#  include <fcntl.h>
#endif /* WIN32 */
#include <string>
#include <vector>
#include "AttrGroups.h"
using namespace std;

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include "iprutil.h"


//****************************************************************************
// static members

// which IPP program is now running (was called from the command line)
// note: this same binary can run as ipr, ipq, iprm
enum { IPP_IPR = 1, IPP_IPO , IPP_IPQ, IPP_IPC, IPP_IPRM };

#ifdef WIN32
static const char* g_terminator = "\\";
char* userEnv = "USERNAME";
#else
static const char* g_terminator = "/";
char* userEnv = "USER";
#define strnicmp strncasecmp
#endif
char* IPP_PRINTER_ENV = "IPP_PRINTER";

static char* copyright = "Copyright (C) Peter Michalek 2004. All rights reserved.\n";
// template file
const char* g_tmplFile;


static const char* g_ipx;
static char* usageipr =
"usage: ipr [-h] [-v] [-t templateFile] [-D group#attr=value] [-P printerUri] \\\n"
"[-o operation] fileToPrint|documentUri\n"
"If no filename given, uses standard input.\n";

static char* usageipq =
"usage: ipq [-h] [-v] [-t templateFile] [-D group#attr=value] [-P printerUri] [-c|-p]\n";

static char* usageiprm =
"usage: iprm [-h] [-v] [-t templateFile] [-D group#attr=value] [-P printerUri] jobName1 jobNameX\n";

static char* usageipc =
"usage: ipc [-h] [-v] [-t templateFile] [-D group#attr=value] [-P printerUri]\\\n"
"(job (cancel|pause|release|restart)) | (printer (pause|resume|purge))\n";

static char* usage = usageipr;
static char* version = "v2.00.00";
static char* progName;



/*

Note: Either fileToPrint or documentUri can be used.
If the argument starts with http:// or ftp://, print-uri operation is requested.
Otherwise, a local file is sent using print-job operation.

*/
// macros

const char* g_options =
"Options:\n"
"     -h           help\n"
"     -v           verbose. Equivalent to -D options#verbose=1\n"
"     -P printer   printer uri\n"
"     -t tmplfile  use the given template file instead of the default\n"
//"     -n           don't validate attributes\n"
"     -o operation ipp operation (valid only for ipr)\n"
"     -p           with ipq: show printer attributes\n"
"                  (this option valid only for ipq)\n"
"     -D define    definition of attribute or option, in the form:\n"
"                  (operations-attrs|job-template-attrs|options)#name=value\n"
"                  e.g. -D operation-attrs#which-jobs=completed to be used\n"
"                  with ipq.\n"
"                  The keyword operation-attrs# may be omitted.\n"
"                  This option is functionaly equivalent to this template file\n"
"                  section:\n"
"                [operation-attrs]\n"
"                which-jobs=completed\n\n"
"Attributes and options are read from the template file\n"
"They can be overridden by defines specified in -D option\n";

void displayOptions()
{
		printf("%s %s - %s\n", g_ipx, version, progName);
		printf(copyright);
		printf(usage);

		printf("Please see http://www.michalek.org/ipr for the latest information.\n");

		printf(g_options);
		printf("The default template file is %s ", g_tmplFile);
#ifdef WIN32
		printf("in the directory set in the HOME or HOMEPATH environment variable\n");
#else
		printf("in the user's home directory\n");
#endif
}


/**
	Convert pathname to program name (last part of pathname after the terminator
	@param pathname the whole or partial path to the program
	@return: last part of path - program name
*/
const char* pathnameToFilename(const char* pathname) {
		const char* substr = pathname;
		const char* filename = pathname;
		while(1) {
				substr = strstr(substr, g_terminator);
				if (substr)
						{
						substr++;
						filename = substr;
						}
				else
						break;
		}
		return filename;
}


enum {
		IPR_ERR_CMDSYNTAX 					= 2001,
		IPR_ERR_OPENFILE						= 2002,
		IPR_ERR_FILESIZE						= 2003,
		IPR_ERR_LOADTEMPLATE				= 2004,
		IPR_ERR_NOVALIDURI					= 2005,
		IPR_ERR_INCORRECT_PARAMS		= 2006,
		IPR_ERR_CONFIGURATION 			= 2007,
		IPR_ERR_CONNECT				 			= 2008
};

ipp_uchar_t wbuffer[8192];			/* Write buffer */
int 	wused;				/* Number of bytes in buffer */

/*
 * 'hex_dump()' - Produce a hex dump of a buffer.
 */

void
hex_dump(ipp_uchar_t *buffer, 	/* I - Buffer to dump */
				 int				 bytes) 	/* I - Number of bytes */
{
	int i, j; 			/* Looping vars */
	int ch; 			/* Current ASCII char */


 /*
	* Show lines of 16 bytes at a time...
	*/

	for (i = 0; i < bytes; i += 16)
	{
	 /*
		* Show the offset...
		*/

		printf("%04x ", i);

	 /*
		* Then up to 16 bytes in hex...
		*/

		for (j = 0; j < 16; j ++)
			if ((i + j) < bytes)
				printf(" %02x", buffer[i + j]);
			else
				printf("   ");

	 /*
		* Then the ASCII representation of the bytes...
		*/

		putchar(' ');
		putchar(' ');

		for (j = 0; j < 16 && (i + j) < bytes; j ++)
		{
			ch = buffer[i + j] & 127;

			if (ch < ' ' || ch == 127)
				putchar('.');
			else
				putchar(ch);
		}

		putchar('\n');
	}
}


int 				/* O - Number of bytes written */
write_cb(void 			 *data, 	/* I - Data */
				 ipp_uchar_t *buffer, 	/* I - Buffer to write */
	 int				 bytes) 	/* I - Number of bytes to write */
{
	int count;				/* Number of bytes */


 /*
	* Loop until all bytes are written...
	*/

	for (count = bytes; count > 0 && wused < sizeof(wbuffer); count --, wused ++)
		wbuffer[wused] = *buffer++;

 /*
	* Return the number of bytes written...
	*/

	return (bytes - count);
}

void dumpMessage(ipp_t* message) {
	ipp_state_t state;
	message->state = IPP_IDLE;
	wused = 0;
	while ((state = ippWriteIO(wbuffer, write_cb, 1, NULL, message)) != IPP_DATA)
		if (state == IPP_ERROR)
			break;

	if (state != IPP_DATA)
		puts("ERROR writing collection attribute!");

	printf("%d bytes written:\n", wused);
	hex_dump(wbuffer, wused);
}



/**
	the main program.

	See options for deailed information.

	Uses standard input if no filename given on the command line.


	Sample ipq command:
		D:\projects\sipp\shineprint\ipr>win32\debug\ipq -t ipp-templateq -P http://iris/
		scripts/ippis.dll -D operation-attrs#which-jobs=completed -D options#verbose=1
		-D operation-attrs#requested-attributes=job-state -D operation-attrs#requested-a
		ttributes=job-id

	To do:
	* server-side validation

	@return 
		Note: errors 0 to 0x600 (1546 decimal) indicate an IPP layer or HTTP
			transport error code. Errors > 2000 indicate error code specific for
			ipr commands.

		0: the IPP server returned successful, 
		1: the IPP server returned successful-ok-ignored-or-substituted-attributes
		2: the IPP server returned successful-ok-conflicting-attributes

		>= 100 and < 600
				IPP request was executed, but the HTTP server indicated an error. The
				return value corresponds to the Status Code returned in the HTTP 
				response. 
				An example of this is error code 400, which indicates that
				the HTTP server is up, but there is no IPP server at the given 
				location.

		>= 0x400 (1024) and < 0x600 (1536)
				IPP request was executed, but the IPP server indicated an error. The
				return value corresponds to the Status Code returned in the IPP 
				response.
				An example of this is error code 1034 (0x40a) 
				client-error-document-format-not-supported.
		

		2001: a command line syntax error
		2002: can't open input file
		2003: print file size is 0
		2004: failed loading template file
		2005: no valid uri available
		2006: incorrect combination of parameters, e.g. chunking is false, but 
				stdin used
		2007: internal error: program not one of ipr, ipq, iprm, ipc

*/
int main(int argc, char **argv)
{ 
	// set default options
	//bool doOffsets = false;
	bool doCopyright = false;
	//bool doValidateAsRequest = true;
	//bool doValidateAsResponse = true;
	//bool doIncBinary = true;
	bool doPrintUri = false;
	//bool dontValidate = true;
	char printerUri[HTTP_MAX_URI] = "";
	char method[HTTP_MAX_URI],	/* Method/scheme name */
		username[HTTP_MAX_URI],	/* Username:password */
		hostname[HTTP_MAX_URI],	/* Hostname */
		resource[HTTP_MAX_URI];	/* Resource name */
  int		port;			/* Port number */
	
	int rc = 0;
	 
	const char* filename = "stdin";
	char destUrl[256];
	//bool doTracing = false;
	strcpy(destUrl, "");

	// build path to template file
	char tmplpath[IPP_MAX_NAME];
	// holds temporary string
//		char* str;
	// specify program name
	// on win32 and unix under gdb, argv[0] evaluates to
	// /path/ip{x}, therefore, we need to take only last part
	if (strlen(argv[0]) > sizeof(tmplpath)) {
			assert(0);
	}

	// *** create a message 
	ipp_t* request = ippCreateWithHeader();

	//if (request <= 0)
	//		return IPPE_INVALID_HANDLE;
	IPROptions options;
	int errLine = 0;

	// guarantee order of the first 3 mandatory attributes
	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_CHARSET,
							 "attributes-charset", NULL, "utf-8");
	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_LANGUAGE,
							 "attributes-natural-language", NULL, "en-us");
	//ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
	//						 "printer-uri", NULL, "");

	// create generic request
	
	g_ipx = pathnameToFilename(argv[0]);
	tmplpath[0] = 0;

	//const char* g_ipx = "ipc";
	int ipx = 0;
	//const int DBUF_SIZE = 4096;
	//char *defineBuffer = new char[DBUF_SIZE];
	ipp_op_t ippop = (ipp_op_t) 0; // IPPOP_RESERVED1; // init to invalid value (0)
	unsigned int jobId = 0; // job id used for ipc operations
	// test
	//g_ipx = "ipq";
	g_tmplFile = ".ipp-template";
	if (!strnicmp(g_ipx, "iprm", 4)) {
		progName = "IPP remove jobs"; 	 
		usage = usageiprm;
		ipx = IPP_IPRM;
		//g_tmplFile = "ipp-templatec";
		ippop = IPP_CANCEL_JOB;
	}
	else if (!strnicmp(g_ipx, "ipr", 3)) {
		progName = "IPP print"; 	 
		usage = usageipr;
		ipx = IPP_IPR;
		//g_tmplFile = "ipp-template";
		ippop = IPP_PRINT_JOB;
	}
	else if (!strnicmp(g_ipx, "ipq", 3)) {
		progName = "IPP queue status";		
		usage = usageipq;
		ipx = IPP_IPQ;
		//g_tmplFile = "ipp-templateq";
		ippop = IPP_GET_JOBS;
	}
	else if (!strnicmp(g_ipx, "ipc", 3)) {
		progName = "IPP control program"; 	 
		usage = usageipc;
		ipx = IPP_IPC;
		//g_tmplFile = "ipp-templatec";
	}
	else {
		printf("This program is running as %s. Exiting.\n", g_ipx);
		return IPR_ERR_CONFIGURATION;
	}

	const char* homeEnv = getenv("HOME");
	const char* homePath = getenv("HOMEPATH");
	const char* homeDrive= getenv("HOMEDRIVE");

	// careful for buffer overflow
	if (homeEnv) {
		if (strlen(homeEnv) > IPP_MAX_NAME-30) {
			printf("HOME environment variable too long: ");
			fprintf(stderr, homeEnv);
			return IPR_ERR_CONFIGURATION;
		} else {
			strcpy(tmplpath, homeEnv);
		}
	} else if (homePath) {
		if (strlen(homePath) > IPP_MAX_NAME-30) {
			printf("HOMEPATH environment variable too long: ");
			fprintf(stderr, homePath);
			return IPR_ERR_CONFIGURATION;
		} else if (strlen(homeDrive) > 2) {
			printf("HOMEDRIVE environment variable too long: ");
			fprintf(stderr, homeDrive);
			return IPR_ERR_CONFIGURATION;
		} else {
			strcpy(tmplpath, homeDrive);
			strcat(tmplpath, "\\");
			strcat(tmplpath, homePath);
		}

	}
//	strcat(tmplpath, "/");
	strcat(tmplpath, g_tmplFile);

	// 2. 

	/* Parse the command line */
	++argv;
	while(*argv != NULL) {
		if (strcmp(*argv,"--help")==0) {
			displayOptions();
			return 0;
		} else if (strcmp(*argv,"-h")==0) {
			displayOptions();
			return 0;
		//} else if (strcmp(*argv,"-n")==0) {
		//	dontValidate = true;
		} else if (strcmp(*argv,"-v")==0) {
			if (!defineAdd("options#verbose=1")) {
				rc = IPR_ERR_CMDSYNTAX;
				break;
			}
		} else if (strcmp(*argv,"-P")==0) {
			++argv;
			if (*argv != NULL) {
				strncpy(destUrl, *argv, sizeof(destUrl));
				destUrl[sizeof(destUrl)-1] = 0;
			}
			else {
				rc = IPR_ERR_CMDSYNTAX;
				break;
			}
		} else if (strcmp(*argv,"-D")==0) {
			++argv;
			if (*argv != NULL) {
				// parse define
				if (!defineAdd(*argv)) {
					rc = IPR_ERR_CMDSYNTAX;
					break;
				}
			} else {
				rc = IPR_ERR_CMDSYNTAX;
				break;
			}
		} else if (strcmp(*argv,"-t")==0)  {
			++argv;
			if (*argv != NULL) {
				strcpy(tmplpath, *argv);
			} else {
				rc = IPR_ERR_CMDSYNTAX;
				break;
			}
		} else if (strcmp(*argv,"-c")==0) { // do completed jobs rather than default
			if (ipx == IPP_IPQ) {
				if (!defineAdd("operation-attrs#which-jobs=completed")) {
					rc = IPR_ERR_CMDSYNTAX;
						break;
				}
			}
			else {
				rc = IPR_ERR_CMDSYNTAX;
				break;
			}
		}  else if (strcmp(*argv,"-o")==0) { 	// do completed jobs rather than default
			if (ipx == IPP_IPR) {
				++argv;
				if (*argv != NULL) {
					// this will be an arbitrary operation
					ippop = (ipp_op_t) atoi(*argv);
					// morph IPR into IPO
					ipx = IPP_IPO;
				} else {
					rc = IPR_ERR_CMDSYNTAX;
					break;
				}
			} else {
				rc = IPR_ERR_CMDSYNTAX;
				break;
			}
		} else if (strcmp(*argv,"-p")==0) { 	// do completed jobs rather than default
			if (ipx == IPP_IPQ)
				ippop=IPP_GET_PRINTER_ATTRIBUTES;
			else {
				rc = IPR_ERR_CMDSYNTAX;
				break;
			}
		} else {
			if (ipx == IPP_IPR) {
			// the argument without "-" present, must be filename
				if (*argv[0] == '-') {
					rc = IPR_ERR_CMDSYNTAX;
					break;
				} else {
					if (!strcmp(filename, "stdin"))
						filename = *argv;
					else { // already found argument 
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}
				}
			} else if (ipx == IPP_IPRM) {
				jobId = atoi(*argv);
					if (!jobId) {
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}
			} else if (ipx == IPP_IPQ) {
				jobId = atoi(*argv);
				if (!jobId) {
					rc = IPR_ERR_CMDSYNTAX;
					break;
				}
				// change operation to get-job-attrs
				ippop = IPP_GET_JOB_ATTRIBUTES;
			} else if (ipx == IPP_IPC) {
				// the argument without "-" present, must comply with:
				// job verb jobId 
				// or
				// printer verb

				if (strcmp(*argv,"job")==0)  {
					++argv;
					// must be one of supported verbs
					if (*argv == NULL) {
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}
					if (strcmp(*argv,"cancel")==0) 
						ippop = IPP_CANCEL_JOB;
					else if (strcmp(*argv,"hold")==0) 
						ippop = IPP_HOLD_JOB;
					else if (strcmp(*argv,"release")==0) 
						ippop = IPP_RELEASE_JOB;
					else if (strcmp(*argv,"restart")==0) 
						ippop = IPP_RESTART_JOB;
					else {
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}
					++argv;

					if (*argv == NULL) {
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}
					jobId = atoi(*argv);
					if (!jobId) {
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}

				} else if (strcmp(*argv,"printer")==0) {
					++argv;
					// must be one of supported verbs
					if (*argv == NULL) {
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}
					if (strcmp(*argv,"pause")==0) 
						ippop = IPP_PAUSE_PRINTER;
					else if (strcmp(*argv,"resume")==0) 
						ippop = IPP_RESUME_PRINTER;
					else if (strcmp(*argv,"purge")==0) 
						ippop = IPP_PURGE_JOBS;
					else {
						rc = IPR_ERR_CMDSYNTAX;
						break;
					}
				} else {
					rc = IPR_ERR_CMDSYNTAX;
					break;
				}
			}
		}
		++argv;
	}

	if (doCopyright)
			printf("%s\n", copyright);
	if (ippop == IPP_PRINT_JOB && 
				(filename == NULL || !strcmp(filename, "stdin"))) {
			//fprintf(stderr, copyright);
			fprintf(stderr, "Incorrect syntax: print operation requires a file argument\n");
			return IPR_ERR_CMDSYNTAX;
	}

	if (rc) {
			//fprintf(stderr, copyright);
			if (*argv)
					fprintf(stderr, "Incorrect syntax at %s\n", *argv);
			printf(usage);
			return rc;
	 }



	// add job-name to ipr. This will override template files, but may
	// be overridden by command line.

	if (ipx == IPP_IPR || ipx == IPP_IPO)
	{
		if (strncmp(filename, "http://", strlen("http://")) == 0 || 
						strncmp(filename, "ftp://", strlen("ftp://")) == 0)
			doPrintUri = true;
		// don't override operation if already set from -o options
	}
	if (ipx == IPP_IPR)
	{
		if (doPrintUri)
			ippop = IPP_PRINT_URI;
		else
			ippop = IPP_PRINT_JOB;
	}

	// if we still don't know the operation, something is missing:
	if (!ippop) { //	== IPP_RESERVED1)
		printf(usage);
		return IPR_ERR_CMDSYNTAX;
	}

	// 1. read ipp-template from user directory



	rc = loadOptions(tmplpath, request, &options, &errLine);

	if (rc)
	{
		fprintf(stderr, "Error loading template file %s: %s.\n"
				"Perhaps your HOME environment variable is not set?\n", tmplpath, errCodeToString(rc));
		if (errLine) // exit only if syntax error
		{
			fprintf(stderr, "Offending line %d\n", errLine);
			return IPR_ERR_LOADTEMPLATE;
		}
	}
	else if (options.verbose)
		printf("Loaded template file %s\n", tmplpath);

	// assign user name
	// this work on unix, on windows it needs to be specified in template.
	// template overrides environment variable if it's not defined 
	char* userName = getenv(userEnv);
	if (!userName)
	{
		userName = "anonymous";
		printf("Warning: user name not specified in %s environment variable. Using 'anonymous'\n", 
			userEnv);
	}
	if (strlen(userName))
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
			 "requesting-user-name", NULL, userName);

	// if IPP_PRINTER env variable set, it overrides template file,
	// but doesn't override -P setting from command line
	char* ippPrinter = getenv(IPP_PRINTER_ENV);
	if (ippPrinter && strlen(ippPrinter) && (destUrl[0] == 0))
		strcpy(destUrl, ippPrinter);
	
	if (ipx == IPP_IPR && (ippop == IPP_PRINT_JOB || ippop == IPP_PRINT_URI)) // only for "true ipr" command
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
						 "job-name", NULL, filename);
			//IPPP_MessageSetAttrString(request, IPP_OPERATION, IPP_NAME, "job-name", filename);

	rc = loadOptionsBuffer(defineGet(), request, &options, &errLine);
	if (rc) {
		fprintf(stderr, "Error loading command line define parameters %s\n", errCodeToString(rc));
		if (errLine) // exit only if syntax error
		{
				fprintf(stderr, "Offending line %d\n", errLine);
		}
		return IPR_ERR_LOADTEMPLATE;
	}

	if (options.verbose) {
		if (ipx == IPP_IPR || ipx == IPP_IPO) {
			if (doPrintUri)
				printf("Requesting to print uri %s\n", filename);
			else if (ippop == IPP_PRINT_JOB)
				printf("Sending file %s\n", filename);
		}
	}

	if (strlen(destUrl))
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
						 "printer-uri", NULL, destUrl);

	// check if printer uri set
	ipp_attribute_t* attr = ippFindAttribute(request, "printer-uri", IPP_TAG_URI);
	
	if (!attr || !strlen(attr->values[0].string.text)) {
		fprintf(stderr, "printer-uri must be specified.\n");
		return IPR_ERR_CMDSYNTAX;
	} else {
		strncpy(printerUri, attr->values[0].string.text, IPP_MAX_NAME);
		printerUri[IPP_MAX_NAME-1] = 0;
	}
	// read the file in
	long length = 0;

	// tracing level to 2.
	//todo: implement tracing
	/*
	int level = 0;
	bool tracking = false;
	if (options.trace > 0)
			level = options.trace;
	*/

	// get file size
	if (!doPrintUri)
	{
		if (strcmp(filename, "stdin"))
		{
			int fh = open(filename, O_RDONLY);
			struct stat st;
			if (fh > 0) {
				fstat(fh, &st);
				length = st.st_size;
				close(fh);
			}
			else {
				printf("Can't open file %s.\n", filename);
				return IPR_ERR_OPENFILE;
			}
		}
	}

	// *** build a message 
	request->request.op.operation_id = ippop;

	if (jobId)
		ippAddInteger(request, IPP_TAG_OPERATION, IPP_TAG_INTEGER, "job-id", jobId);

	if (doPrintUri)
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
							 "document-uri", NULL, filename);

	/*
	//todo: check internal error: wrong request:
	if (!dontValidate)
	{
			rc = IPPP_MessageValidateRequest(request, &str);
			if (rc)
			{
					printf("ipr: IPPP_MessageValidateRequest failed: code=%ld attribute name=%s", rc, str ? str: "unknown");
					if (str)
							IPP_Free(str);
					return IPR_ERR_INCORRECT_PARAMS;
			}
	}

	// todo: write request into a trace file
	if (options.traceFiles)
			IPPP_MessageWriteToFile(request, "req.trc");


	//todo: dump request
	if (options.verbose)
	{
			rc = IPPP_MessageToString(request, &str);
			printf("About to send the following request:\n\n");
			puts(str);
			IPP_Free(str);
	}
	*/

	// *** send message to printer
	// parse host, port
	httpSeparate(printerUri, method, username, hostname,
	             &port, resource);


//	const char* host = "edel";
//	const char* resource = "/printers/printer";
//	int port = 631;
	http_t	*http;			/* Connection to server */
	http = httpConnect(hostname, port);
	if (http == NULL)
	{
		perror(hostname);
		ippDelete(request);
		printf("Error opening IPP client stream");
		return IPR_ERR_CONNECT;
	}

	
	ipp_t* response = cupsDoFileRequest(http, request, resource, ippop == IPP_PRINT_JOB ? filename:NULL);
	//dumpMessage(response);


	// provide output
	rc = response->request.status.status_code; 
	if (options.verbose) {
		if (rc == 0 ) {
				printf("IPP operation successfully submitted to %s.\n", printerUri);
		} else if (rc == IPP_OK_SUBST){
				printf("IPP operation successfully submitted to %s.\nHowever, some attributes were substituted by the server\n", printerUri);
		} else if (rc == IPP_OK_CONFLICT){
				printf("IPP operation successfully submitted to %s.\nHowever, conflicting attributes were detected by the server\n", printerUri);
		} else  {
				printf("IPP operation to %s was not submitted successfully.\nResponse status code: 0x%x (%d): %s\n\n", 
						printerUri,
						rc, rc, "???");
		}
	}


	/*
	//todo: add message dump
	if (!rc && str)
	{
			if (options.verbose)
			{
					printf("Received the following response from IPP printer:\n\n");
					puts(str);
			}
	}
	*/

	// provide output
	switch (ippop)
	{
		case IPP_GET_JOBS: {
			AttrGroups attrGroups(response, IPP_TAG_JOB);
			attrGroups.print(printerUri, "Jobs for ", AttrGroups::OutHorizontal);
			break;
			}
		case IPP_GET_JOB_ATTRIBUTES: {
			char title[128];
			sprintf(title, "Job attributes for job %d on ", jobId);
			AttrGroups attrGroups(response, IPP_TAG_JOB);
			attrGroups.print(printerUri, title, AttrGroups::OutVertical);
			break;
			}
		case IPP_GET_PRINTER_ATTRIBUTES: {
			AttrGroups attrGroups(response, IPP_TAG_PRINTER);
			attrGroups.print(printerUri, "Printer attributes for ", AttrGroups::OutVertical);
			break;
			}
		default:
				break;
	}

	// request deleted by cupsDoFileRequest
	if (response) {
		ippDelete(response);
	}

	return rc;
}


extern "C" {
int ipp_print_file(const char *printerUri, const char *filename, const char *tmplpath)
{
		// set default options
	//bool doOffsets = false;
	bool doCopyright = false;
	//bool doValidateAsRequest = true;
	//bool doValidateAsResponse = true;
	//bool doIncBinary = true;
	bool doPrintUri = false;
	//bool dontValidate = true;
	char method[HTTP_MAX_URI],	/* Method/scheme name */
		username[HTTP_MAX_URI],	/* Username:password */
		hostname[HTTP_MAX_URI],	/* Hostname */
		resource[HTTP_MAX_URI];	/* Resource name */
  int		port;			/* Port number */
	
	int rc = 0;
	 
//	char destUrl[256];
	//bool doTracing = false;
//	strcpy(destUrl, "");

	// *** create a message 
	ipp_t* request = ippCreateWithHeader();

	//if (request <= 0)
	//		return IPPE_INVALID_HANDLE;
	IPROptions options;
	int errLine = 0;

	// guarantee order of the first 3 mandatory attributes
	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_CHARSET,
							 "attributes-charset", NULL, "utf-8");
	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_LANGUAGE,
							 "attributes-natural-language", NULL, "en-us");
	//ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
	//						 "printer-uri", NULL, "");

	// create generic request
	
	//const char* g_ipx = "ipc";
	int ipx = 0;
	//const int DBUF_SIZE = 4096;
	//char *defineBuffer = new char[DBUF_SIZE];
	ipp_op_t ippop = (ipp_op_t) 0; // IPPOP_RESERVED1; // init to invalid value (0)
	unsigned int jobId = 0; // job id used for ipc operations
	// test
	//g_ipx = "ipq";
//	g_tmplFile = ".ipp-template";

		progName = "IPP print"; 	 
		usage = usageipr;
		ipx = IPP_IPR;
		//g_tmplFile = "ipp-template";
		ippop = IPP_PRINT_JOB;


	// 2. 

	// add job-name to ipr. This will override template files, but may
	// be overridden by command line.

	if (ipx == IPP_IPR || ipx == IPP_IPO)
	{
		if (strncmp(filename, "http://", strlen("http://")) == 0 || 
						strncmp(filename, "ftp://", strlen("ftp://")) == 0)
			doPrintUri = true;
		// don't override operation if already set from -o options
	}
	if (ipx == IPP_IPR)
	{
		if (doPrintUri)
			ippop = IPP_PRINT_URI;
		else
			ippop = IPP_PRINT_JOB;
	}

	// 1. read ipp-template from user directory

	if(strlen(tmplpath)) {
		rc = loadOptions(tmplpath, request, &options, &errLine);
	}
	// assign user name
	// this work on unix, on windows it needs to be specified in template.
	// template overrides environment variable if it's not defined 
	char* userName = getenv("USER");
	if (!userName)
	{
		userName = "anonymous";
/*
		printf("Warning: user name not specified in %s environment variable. Using 'anonymous'\n", 
			userEnv);
*/
	}
	if (strlen(userName))
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
			 "requesting-user-name", NULL, userName);

	// if IPP_PRINTER env variable set, it overrides template file,
	// but doesn't override -P setting from command line
/*
	char* ippPrinter = getenv(IPP_PRINTER_ENV);
	if (ippPrinter && strlen(ippPrinter) && (destUrl[0] == 0))
		strcpy(destUrl, ippPrinter);
*/	
	if (ipx == IPP_IPR && (ippop == IPP_PRINT_JOB || ippop == IPP_PRINT_URI)) // only for "true ipr" command
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
						 "job-name", NULL, filename);
			//IPPP_MessageSetAttrString(request, IPP_OPERATION, IPP_NAME, "job-name", filename);

	rc = loadOptionsBuffer(defineGet(), request, &options, &errLine);
/*
	if (rc) {
		fprintf(stderr, "Error loading command line define parameters %s\n", errCodeToString(rc));
		if (errLine) // exit only if syntax error
		{
				fprintf(stderr, "Offending line %d\n", errLine);
		}
		return IPR_ERR_LOADTEMPLATE;
	}
*/

	if (strlen(printerUri))
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
						 "printer-uri", NULL, printerUri);

	// check if printer uri set
	ipp_attribute_t* attr = ippFindAttribute(request, "printer-uri", IPP_TAG_URI);
	
/*
	if (!attr || !strlen(attr->values[0].string.text)) {
		fprintf(stderr, "printer-uri must be specified.\n");
		return IPR_ERR_CMDSYNTAX;
	} else {
*/
//		strncpy(printerUri, attr->values[0].string.text, IPP_MAX_NAME);
//		printerUri[IPP_MAX_NAME-1] = 0;
//	}
	// read the file in
	long length = 0;

	// tracing level to 2.
	//todo: implement tracing
	/*
	int level = 0;
	bool tracking = false;
	if (options.trace > 0)
			level = options.trace;
	*/

	// get file size
	if (!doPrintUri)
	{
		if (strcmp(filename, "stdin"))
		{
			int fh = open(filename, O_RDONLY);
			struct stat st;
			if (fh > 0) {
				fstat(fh, &st);
				length = st.st_size;
				close(fh);

				if (length == 0) return 0;
			}
			else {
				printf("Can't open file %s.\n", filename);
				return IPR_ERR_OPENFILE;
			}
		}
	}

	// *** build a message 
	request->request.op.operation_id = ippop;

	if (jobId)
		ippAddInteger(request, IPP_TAG_OPERATION, IPP_TAG_INTEGER, "job-id", jobId);

	if (doPrintUri)
		ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
							 "document-uri", NULL, filename);

	/*
	//todo: check internal error: wrong request:
	if (!dontValidate)
	{
			rc = IPPP_MessageValidateRequest(request, &str);
			if (rc)
			{
					printf("ipr: IPPP_MessageValidateRequest failed: code=%ld attribute name=%s", rc, str ? str: "unknown");
					if (str)
							IPP_Free(str);
					return IPR_ERR_INCORRECT_PARAMS;
			}
	}

	// todo: write request into a trace file
	if (options.traceFiles)
			IPPP_MessageWriteToFile(request, "req.trc");


	//todo: dump request
	if (options.verbose)
	{
			rc = IPPP_MessageToString(request, &str);
			printf("About to send the following request:\n\n");
			puts(str);
			IPP_Free(str);
	}
	*/

	// *** send message to printer
	// parse host, port
	httpSeparate(printerUri, method, username, hostname,
	             &port, resource);


//	const char* host = "edel";
//	const char* resource = "/printers/printer";
//	int port = 631;
	http_t	*http;			/* Connection to server */
	http = httpConnect(hostname, port);
	if (http == NULL)
	{
		perror(hostname);
		ippDelete(request);
	//	printf("Error opening IPP client stream");
		return IPR_ERR_CONNECT;
	}

	
	ipp_t* response = cupsDoFileRequest(http, request, resource, ippop == IPP_PRINT_JOB ? filename:NULL);
	//dumpMessage(response);


	// provide output
	rc = response->request.status.status_code; 
/*
	if (options.verbose) {
		if (rc == 0 ) {
				printf("IPP operation successfully submitted to %s.\n", printerUri);
		} else if (rc == IPP_OK_SUBST){
				printf("IPP operation successfully submitted to %s.\nHowever, some attributes were substituted by the server\n", printerUri);
		} else if (rc == IPP_OK_CONFLICT){
				printf("IPP operation successfully submitted to %s.\nHowever, conflicting attributes were detected by the server\n", printerUri);
		} else  {
				printf("IPP operation to %s was not submitted successfully.\nResponse status code: 0x%x (%d): %s\n\n", 
						printerUri,
						rc, rc, "???");
		}
	}
*/
	// request deleted by cupsDoFileRequest
	if (response) {
		ippDelete(response);
	}

	return rc;
}
 }
