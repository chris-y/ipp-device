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
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "cups/ipp.h"
#include "iprutil.h"

//****************************************************************************
// local macros and inlines
#define SLogError printf
//****************************************************************************
// local functions declarations


//****************************************************************************
// static objects and variables
struct ItemEntry
{
    const char* name;
    ipp_tag_t tag;
};

//****************************************************************************
// static members

static ItemEntry g_nameTagTable[] =
{

    // first entry is reserved
      { "reserved",                     IPP_TAG_ZERO     }
    , { "printer-uri",                  IPP_TAG_URI     }
    , { "requesting-user-name",         IPP_TAG_NAME       }
    , { "ipp-attribute-fidelity",       IPP_TAG_BOOLEAN }
    , { "document-name",                IPP_TAG_NAME }
    , { "document-format",              IPP_TAG_MIMETYPE }
    , { "document-natural-language",    IPP_TAG_LANGUAGE }
    , { "compression",                  IPP_TAG_KEYWORD}
    , { "message",                      IPP_TAG_TEXT }
    , { "requested-attributes",         IPP_TAG_KEYWORD}


    // job template attributes
    , { "job-priority",                 IPP_TAG_INTEGER}
    , { "job-hold-until",               IPP_TAG_KEYWORD}
    , { "job-sheets",                   IPP_TAG_INTEGER}
    , { "multiple-document-handling",   IPP_TAG_KEYWORD}
    , { "copies",                       IPP_TAG_INTEGER}
    , { "finishings",                   IPP_TAG_ENUM}
    , { "page-ranges",                  IPP_TAG_RANGE}
    , { "sides",                        IPP_TAG_INTEGER}
    , { "number-up",                    IPP_TAG_INTEGER}
    , { "orientation-requested",        IPP_TAG_ENUM}
    , { "media",                        IPP_TAG_KEYWORD}
    , { "printer-resolution",           IPP_TAG_RESOLUTION}
    , { "print-quality",                IPP_TAG_ENUM}

    // job template attributes - printer default
    , { "job-priority-default",         IPP_TAG_INTEGER}
    , { "job-hold-until-default",       IPP_TAG_KEYWORD}
    , { "job-sheets-default",           IPP_TAG_INTEGER}
    , { "multiple-document-handling-default",   IPP_TAG_KEYWORD}
    , { "copies-default",               IPP_TAG_INTEGER}
    , { "finishings-default",           IPP_TAG_ENUM}
    // this attr actually doesn't exist according to spec.
//    , { IPP_TAG_ATTR_PAGE_RANGES_DEFAULT,           "page-ranges-default",          IPP_TAG_RANGE}
    , { "sides-default",                IPP_TAG_INTEGER}
    , { "number-up-default",            IPP_TAG_INTEGER}
    , { "orientation-requested-default",IPP_TAG_ENUM}
    , { "media-default",                IPP_TAG_KEYWORD}
    , { "printer-resolution-default",   IPP_TAG_RESOLUTION}
    , { "print-quality-default",        IPP_TAG_ENUM}

    // job template attributes - printer supported
    , { "job-priority-supported",         IPP_TAG_INTEGER}
    , { "job-hold-until-supported",       IPP_TAG_KEYWORD}
    , { "job-sheets-supported",           IPP_TAG_INTEGER}
    , { "multiple-document-handling-supported",   IPP_TAG_KEYWORD}
    , { "copies-supported",               IPP_TAG_RANGE}
    , { "finishings-supported",           IPP_TAG_ENUM}
    , { "page-ranges-supported",          IPP_TAG_BOOLEAN}
    , { "sides-supported",                IPP_TAG_INTEGER}
    , { "number-up-supported",            IPP_TAG_INTEGER}
    , { "orientation-requested-supported",IPP_TAG_ENUM}
    , { "media-supported",                IPP_TAG_KEYWORD}
    , { "printer-resolution-supported",   IPP_TAG_RESOLUTION}
    , { "print-quality-supported",        IPP_TAG_ENUM}

    // job description attributes
    , { "job-uri", IPP_TAG_URI }
    , { "job-id", IPP_TAG_INTEGER }
    , { "job-printer-uri", IPP_TAG_URI }
    , { "job-more-info", IPP_TAG_URI }
    , { "job-name",                     IPP_TAG_NAME }
    , { "job-originating-user-name", IPP_TAG_NAME}
    , { "job-state", IPP_TAG_ENUM }
    , { "job-state-reasons", IPP_TAG_KEYWORD }
    , { "job-state-message", IPP_TAG_TEXT }
    , { "number-of-documents", IPP_TAG_INTEGER }
    , { "output-device-assigned", IPP_TAG_NAME }
    , { "time-at-creation", IPP_TAG_INTEGER }
    , { "time-at-processing", IPP_TAG_INTEGER }
    , { "time-at-completed", IPP_TAG_INTEGER }
    , { "number-of-intervening-jobs", IPP_TAG_INTEGER }
    , { "job-message-from-operator", IPP_TAG_TEXT }
    , { "job-k-octets", IPP_TAG_INTEGER }
    , { "job-impressions", IPP_TAG_INTEGER }
    , { "job-media-sheet", IPP_TAG_INTEGER }
    , { "job-k-octets-processed", IPP_TAG_INTEGER }
    , { "job-impressions-completed", IPP_TAG_INTEGER }
    , { "job-media-sheets-completed", IPP_TAG_INTEGER }
    , { "attributes-charset",           IPP_TAG_CHARSET  }
    , { "attributes-natural-language",  IPP_TAG_LANGUAGE }

    // get-printer-attributes (printer description attributes)
    , { "printer-uri-supported", IPP_TAG_URI }
    , { "uri-security-supported", IPP_TAG_KEYWORD }
    , { "printer-name", IPP_TAG_NAME }
    , { "printer-location", IPP_TAG_TEXT }
    , { "printer-info", IPP_TAG_TEXT }
    , { "printer-more-info", IPP_TAG_URI }
    , { "printer-driver-installer", IPP_TAG_URI }
    , { "printer-make-and-model", IPP_TAG_TEXT }
    , { "printer-more-info-manufacturer", IPP_TAG_URI }
    , { "printer-state", IPP_TAG_ENUM }
    , {  "printer-state-reasons", IPP_TAG_KEYWORD }
    , { "printer-state-message", IPP_TAG_TEXT }
    , { "operations-supported", IPP_TAG_ENUM }
    , { "charset-configured", IPP_TAG_CHARSET }
    , { "charset-supported", IPP_TAG_CHARSET }
    , { "natural-language-configured", IPP_TAG_LANGUAGE }
    , { "generated-natural-language-supported", IPP_TAG_LANGUAGE }
    , { "document-format-default", IPP_TAG_MIMETYPE }
    , { "document-format-supported", IPP_TAG_MIMETYPE }
    , { "printer-is-accepting-jobs", IPP_TAG_BOOLEAN }
    , { "queued-job-count", IPP_TAG_INTEGER }
    , { "printer-message-from-operator", IPP_TAG_TEXT }
    , { "color-supported", IPP_TAG_BOOLEAN }
    , { "reference-uri-schemes-supported", IPP_TAG_URISCHEME }
    , { "pdl-override-supported", IPP_TAG_KEYWORD }
    , { "printer-up-time", IPP_TAG_INTEGER }
    , { "printer-current-time", IPP_TAG_DATE }
    , { "multiple-operation-timeout", IPP_TAG_INTEGER }
    , { "compression-supported", IPP_TAG_KEYWORD }
    , { "job-k-octets-supported", IPP_TAG_RANGE }
    , { "job-impressions-supported", IPP_TAG_RANGE }
    , { "job-media-sheets-supported", IPP_TAG_RANGE }

    , { "status-message", IPP_TAG_TEXT }

    , { "limit", IPP_TAG_INTEGER }
    , { "which-jobs", IPP_TAG_KEYWORD }
    , { "my-jobs", IPP_TAG_BOOLEAN }

    , { "document-uri", IPP_TAG_URI }
    , { "last-document", IPP_TAG_BOOLEAN }
};

const short g_nameTagTableSize = sizeof(g_nameTagTable)/sizeof(g_nameTagTable[0]);






struct TableEntry
{
    int key;
    char* value;
};

static TableEntry errTable[] = {
    { 1   , "Wrong attribute/value syntax" },
    { 2   , "Attribute without section" },
    { 3   , "Unrecognized section type" },
    { 4   , "Incorrect attribute name or value" },
    { 5   , "Can't open file" }
};


//static const char* OP_ATTRS_KEY = "operation-attrs";
//static const char* TEMPL_ATTRS_KEY = "job-template-attrs";
//static const char* OPTIONS_KEY =  "options";


static const char* OP_ATTRS_SECTION = "[operation-attrs]";
static const char* TEMPL_ATTRS_SECTION = "[job-template-attrs]";
static const char* OPTIONS_SECTION =  "[options]";
const int OP_ATTRS = 1;
const int TEMPL_ATTRS = 2;
const int OPTIONS = 3;
const char* IPR_OPT_VERBOSE = "verbose";
const char* IPR_OPT_CHUNKING = "chunking";
const char* IPR_OPT_TRACE = "trace";
const char* IPR_OPT_NOSEND = "nosend";
const char* IPR_OPT_TRACEFILES = "tracefiles";
const char* IPR_OPT_TIMEOUT = "timeout";
const char* IPR_OPT_VALIDATE_JOB = "validate-job";



//****************************************************************************
// exported functions
IPROptions::IPROptions()
{
    verbose = 0;
    chunking = 0;
    trace = 0;
    traceFiles = 0;
    timeout = 10000;
    validateJob = 0;
    nosend = 0;
}

/**
  Read stdin into the buffer.

  @param length output: length of buffer
  @param offset output: offset of body (ipp data)


*/
unsigned char* stdinToBuffer(long* length, long* offset)
{
    unsigned char* buffer = 0;
	  FILE* stream;
    stream = stdin;
    long bufSize = 20000;
    const int bufInc = 10000; // buffer size increment
    *length = 0;
    *offset = 0;

    buffer = (unsigned char*)malloc(bufSize);


    int i = 0;
    do {
        if (!buffer)
            break;
        buffer[i] = getc(stream);
        i++;
        if (i >= bufSize)
        {
            bufSize += bufInc;
            buffer = (unsigned char*) realloc(buffer, bufSize);
        }
    } while (!feof(stream));

    if (buffer)
    {
        long filesize = i-1;
        //printf("filesize=%ld", filesize);
        *length = filesize;
        char* term = "\r\n\r\n";
        char* found = strstr((const char*)buffer, term);
        long index;
        if (found)
        {
            index = found - (const char*)buffer;
            index += strlen(term);
        }
        else
            index = 0;
        *offset = index;
        buffer[filesize] = '\0';
    }
    return buffer;
}


const char* errCodeToString(int errCode)
{
    for (int i = 0; i < (int)(sizeof(errTable)/sizeof(errTable[0])); i++)
        if (errCode == errTable[i].key)
            return errTable[i].value;
    return "unknown";
}

long loadOptions(const char* filename, ipp_t* handle, IPROptions* programOptions, int* errLine)
{
    int length;
    unsigned char* buffer = fileToBuffer(filename, &length);
    if (!buffer)
        return 5;
    long rc = loadOptionsBuffer((char*)buffer, handle, programOptions, errLine);
    delete []buffer;
    return rc;
}


long loadOptionsBuffer(const char* buffer, ipp_t *handle, IPROptions* programOptions, int* errLine)
{
//    unsigned long length = 0;
    static char COMMENT_CHAR = ';';
    char buf[128];
    long rc = 0;
    // parse the buffer
    // line starting with [ indicates new section and changes the section flag
    // any line starting with ";" is ignored
    // other lines have to be of the form x=y where
    // x is attribute name
    // y is attribute value
    int section = IPP_TAG_ZERO;
    int currentLine = 0;
    int bufOffset = 0;
    const char* newBuf = buffer;
    char prevName[128];
    prevName[0] = 0;

    do {
        currentLine++;
        // get line
        if (newBuf[bufOffset] == 0)
            break;
        const char* next = strstr(&newBuf[bufOffset] , "\n");
        if (next)
        {
            unsigned int llen = next-&newBuf[bufOffset];
            // handle lines that don't fit into buffer by truncating them.
            int tocopy;
            if (llen >= sizeof(buf))
                tocopy = sizeof(buf) -1;
            else
                tocopy = llen;

            strncpy(buf, &newBuf[bufOffset], tocopy);
            buf[tocopy] = 0;
            bufOffset += llen + 1;
        }
        else
        {
            // copy partial line and break;
            int llen = strlen(newBuf) - bufOffset;
            strncpy(buf, &newBuf[bufOffset], llen);
            buf[llen] = 0;
            bufOffset = strlen(newBuf);
        }

        // ignore comment
        if (buf[0] == COMMENT_CHAR || !strlen(buf) || buf[0] == '\n' || buf[0] == '\r')
            continue;
        // check new section
        if (buf[0] == '[')
        {
            if (!strncmp(buf, OP_ATTRS_SECTION, strlen(OP_ATTRS_SECTION)))
                section = IPP_TAG_OPERATION;
            else if (!strncmp(buf, TEMPL_ATTRS_SECTION, strlen(TEMPL_ATTRS_SECTION)))
                section = IPP_TAG_JOB;
            else if (!strncmp(buf, OPTIONS_SECTION, strlen(OPTIONS_SECTION)))
                section = OPTIONS;
            else
            {
                rc = 3;
                break;
            }

        }
        else
        {
            char* value = strstr(buf, "=");
            if (value)
            {
                *value = 0;
                value++;
                // trim new line
                char* nl = strstr(value, "\n");
                if (nl)
                    *nl = 0;
								nl = strstr(value, "\r");
                if (nl) // unix
                    *nl = 0;
                const char* name = buf;
                // set attribute without type
                if (section == IPP_TAG_JOB || section == IPP_TAG_OPERATION)
                {
                    if (handle)
                    {
                        if (!strcmp(prevName, name))
                            assert(0); // not implemented;//rc = IPPP_MessageSetAttrMulti(handle, (EAttrGroup) section, name, value);
                        else {
														ippMessageSetAttr(handle, (ipp_tag_t)section, name, value);
                            //ippAddString(handle, (ipp_tag_t)section, IPP_TAG_KEYWORD, name, NULL, value);
														//rc = IPPP_MessageSetAttr(handle, (EAttrGroup) section, name, value);
												}
                        if (rc)
                        {
                            *errLine = currentLine;
                            rc = 4;
                            break;
                        }
                        strcpy(prevName, name);
                    }
                }
                else if (section == OPTIONS)
                {
                    if (!strcmp(name, IPR_OPT_VERBOSE))
                        programOptions->verbose = atoi(value);
                    else if (!strcmp(name, IPR_OPT_CHUNKING))
                        programOptions->chunking = atoi(value);
                    else if (!strcmp(name, IPR_OPT_TRACE))
                        programOptions->trace = atoi(value);
                    else if (!strcmp(name, IPR_OPT_NOSEND))
                        programOptions->nosend = atoi(value);
                    else if (!strcmp(name, IPR_OPT_TRACEFILES))
                        programOptions->traceFiles = atoi(value);
                    else if (!strcmp(name, IPR_OPT_TIMEOUT))
                        programOptions->timeout = atoi(value);
                    else if (!strcmp(name, IPR_OPT_VALIDATE_JOB))
                        programOptions->validateJob = atoi(value);
                }
                else
                {
                    rc = 2;
                    break;
                }
            }
            else
            {
                // error
                *errLine = currentLine;
                rc = 1;
                break;
            }

        }

    } while(true);
    return rc;
}


long bufferToFile(const char* filename, const unsigned char* data, long length)
{
    FILE* stream;
    stream = fopen(filename, "w+b");
    if (!stream)
    {
        SLogError("Can't open file %s\n", filename);
        return 1;
    }
    fwrite(data, 1, length, stream);
    fclose(stream);


    return 0;
}

unsigned char* fileToBuffer(const char* filename, int* length)
{
    unsigned char* buffer = 0;

    FILE* stream;
    stream = fopen(filename, "r+b");

    if (!stream)
    {
        SLogError("Can't open file %s\n", filename);
        return 0;
    }
    if (fseek(stream, 0, SEEK_END))
        return 0;
    int filesize = ftell(stream);
    if (filesize == 0)
        return 0;

    int hasRead = 0;
    buffer = new unsigned char[filesize + 1];
    if (fseek(stream, 0, SEEK_SET))
        return 0;
    hasRead = fread(buffer, 1, filesize, stream);
    if (hasRead != filesize)
    {
        delete []buffer;
        SLogError("Can't read all the bytes from file %s, %d out of %d read\n", filename, hasRead, filesize);
        return 0;
    }
    *length = filesize;
    buffer[filesize] = '\0';
	fclose(stream);
    return buffer;
}


static char defineBuffer[4096];
static int bufOffset = 0;

void defineClean()
{
    defineBuffer[0] = 0;
    bufOffset = 0;
}

const char* defineGet()
{
    return defineBuffer;
}

bool defineAdd(const char* defineString)
{
    if (sizeof(defineBuffer) < bufOffset + strlen(defineString) + 5)
        return false;
    const char* attrOffset = strstr(defineString, "#");
    if (!attrOffset)
    {
        // assuming operation-attrs/ was ommited
        attrOffset = defineString;
        const char* valueOffset = strstr(attrOffset, "=");
        if (!valueOffset)
            return false;
        strcat(defineBuffer, OP_ATTRS_SECTION);
        strcat(defineBuffer, "\n");
    }
    else
    {
        attrOffset++;
        const char* valueOffset = strstr(attrOffset, "=");
        if (!valueOffset)
            return false;
        int olen = (attrOffset - defineString - 1) * sizeof(char);
        strcat(defineBuffer, "[");
        strncpy(&defineBuffer[strlen(defineBuffer)], defineString, olen);
        strcat(defineBuffer, "]\n");
    }
    strcat(defineBuffer, attrOffset);
    strcat(defineBuffer, "\n");
    return true;
}

//------------- ipr cups utilities
static int requestId = 100;

ipp_t* ippCreateWithHeader() {
	ipp_t* request = ippNew();
	request->request.op.version[0]	 = 0x01;
	request->request.op.version[1]	 = 0x01;
	request->request.op.request_id	 = requestId++;
	return request;
}


void ippMessageSetAttr(ipp_t *msg, ipp_tag_t attrGroup, const char* name, const char* value) {
	ipp_tag_t type = ippNameToType(name);
	if (ippIsAttributeString(type)) {
		ippAddString(msg, attrGroup, type, name, NULL, value);
	} else {
		assert(0); // not implemented
	}
}

bool ippIsAttributeString(ipp_tag_t type) {
	bool rc = false;
	switch(type) {
		case IPP_TAG_URI :
		case IPP_TAG_STRING :
		case IPP_TAG_TEXT :
		case IPP_TAG_NAME :
		case IPP_TAG_KEYWORD :
		case IPP_TAG_CHARSET :
		case IPP_TAG_LANGUAGE :
			rc = true;
			break;
		default:
			rc = false;
			break;
	}
	return rc;
}


ipp_tag_t ippNameToType(const char* name)
{
	ipp_tag_t type = IPP_TAG_ZERO;
  for (int i = 0; i < g_nameTagTableSize; i++) {
		if (!strcmp(g_nameTagTable[i].name, name)) {
      type = g_nameTagTable[i].tag;
			break;
		}
  }
  return type;
}


//****************************************************************************
// local functions



