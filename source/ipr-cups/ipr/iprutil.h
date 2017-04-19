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

#ifndef _iprutil_h_
#define _iprutil_h_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


//****************************************************************************
// structs, typedefs, enums


// WIN32 portability macros
#ifdef WIN32
#define open _open
#define stat _stat
#define fstat _fstat
#define close _close
#endif

struct IPROptions
{
    IPROptions();
    short verbose;
    short chunking;
    short trace;
    short traceFiles;
    long timeout;
    short validateJob;
    short nosend;
};

typedef struct IPROptions IPROptions;


//****************************************************************************
// exported functions
unsigned char* stdinToBuffer(long* length, long* offset);
const char* errCodeToString(int errCode);

/**
  Load attributes and options from a file.

  File format is as follows: <br>
<pre>
; operation attributes
; example of other job template attributes:
; 
[operation-attrs]
attributes-charset: utf-8
attributes-natural-language: en-us
printer-uri: http://localhost:631/
document-format: text/plain
job-name: no-job-name
document-name: no-document-name
requesting-user-name: zfischer

; job template attributes
; note: job template attributes don't need to be specified
[job-template-attrs]
job-priority: 50
copies: 1
sides: one-sided

[options]
; Internet Printing Protocol options
; under which ipp commands are executed
; 
; Progress information will be displayed during the printing process
verbose: true
; Chunking will be used to send or receive data
chunking: true
; A trace file will be created for the request and response.
; They will be save under the name req.trc, res.trc
trace: true
; 60 s timeout
timeout: 60
; 
</pre>

  @param filename file to load from
  @param handle message handle to write message-specific options into.
         If handle is 0, message-specific options should be ignored.
  @param programOptions on return, contains program options to be used during
    execution
  @param errLine if an error occured, contains the error line within filename
  @return 0 if success, error code otherwise
     errors: 
     1 = wrong attribute/value syntax
     2 = attribute without section
     3 = unrecongnized section type
     4 = incorrect attribute name or value
     5 = can't open file


*/
long loadOptions(const char* filename, ipp_t* handle, IPROptions* programOptions, int* errLine);

/**
   Similar to loadOptions function, but the first argument is a buffer.
   @see loadOptions

*/
long loadOptionsBuffer(const char* buffer, ipp_t* handle, IPROptions* programOptions, int* errLine);
/**
  Read a file into the buffer.

  @param filename filename of the file to load. If "stdin", we'll use stdin.
  @param length output parameters length of the return buffer 
  @return buffer filled with file contents, 0 of couldn't load file.

*/
unsigned char* fileToBuffer(const char* filename, int* length);

/**
  Write buffer into a file.

  @param filename filename of the file to write to. 
  @param data the data
  @param length length of data
  @return 0 if success, 1 otherwise

*/
long bufferToFile(const char* filename, const unsigned char* data, long length);

/**
  Add a definition into buffer. 
  The buffer is compliant with ipp-template format.
  @param defineString  must be of the form:
    group/attr=value
    Group must be one of those defined in ipp-template format:
      operation-attributes
      template-attributes
      option
  @return true if success, false otherwise
*/
bool defineAdd(const char* defineString);
const char* defineGet();
void defineClean();

ipp_t* ippCreateWithHeader();
void ippMessageSetAttr(ipp_t *msg, ipp_tag_t attrGroup, const char* name, const char* value);
ipp_tag_t ippNameToType(const char* name);
bool ippIsAttributeString(ipp_tag_t type);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _iprutil_h_
