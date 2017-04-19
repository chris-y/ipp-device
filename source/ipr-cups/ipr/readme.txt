IPP printing - IPR family of programs - Release Notes

Copyright 2004, ShineSoft Systems
Version 2.00.00: 08/20/2004


IPR provides a series of client programs that enable the user to provide 
printing compliant with the Internet Printing Protocol (IPP). 

ipr,ipq,iprm,ipc follow to some extent the approach to printing of
lpr set of commands. However, ipr doesn't try to be generic or backwards
compatible, but focuses on the IPP protocol. It allows you to talk to
an arbitrary IPP printer or server and submit jobs and 
query and manage printers and jobs.


For more information about IPP, please see http://www.pwg.org. 
For up-to-date information about ipr, please see http://www.michalek.org/ipr/. 



History
-------

This program is being rolled out as open source.
It's based on CUPS ipp library and an article I wrote together
with Michael Sweet back in 1998.

ipr was originally available from my company Shinesoft Systems
as part of ShinePrint suite, but only in binary form. This release
is based on that tool, but slightly different.


Installation on Windows
-----------------------
1) unzip the package
2) run install.bat install_directory
3) modify your ~/.ipp-template to reflect the preferences, in particular 
your default IPP printer

Note for cygwin users:
 make sure HOME environment variable is set
 copy ipp-template to $HOME%/.ipp-template 

If you are using windows with cygwin, either copy ipp-template to your $HOME
directory or 

Installation on Linux
---------------------
1) unzip the package
2) log on as root and run install. The files will be installed in /usr/bin 
directory
3) log on as user and establish template:
cp /usr/bin/ipp-template ~/.ipp-template
cp /usr/bin/ipp-narrowjobs ~/.ipp-narrowjobs

run ipr_install_templates. This will copy templates 
   .ipp-template* to user's home directory
4) modify your ~/.ipp-template to reflect the preferences, in particular 
your default IPP printer

Testing the installation
------------------------

You'll need an IPP printer. One way to get one is to install
CUPS on linux which simulates an IPP printer even if your hardware
doesn't support IPP directly.

... or find out from your admin what the URL is.

Request printer attributes, e.g.:

ipq -p -P http://edel:631/printers/printer
(-P option override printer-uri setting of .ipp-template
It should show printer attributes for the IPP printer you specified.


A Brief Description of the IPR system
-------------------------------------

The ipr system comes with only one executable. Depending on the OS, different 
links (on linux or other Unixes) are established during installation process or 
(on Windows) separate copies with different names are made. The resulting set 
consists of the following programs:

Program     Description                     IPP operations
ipr         print IPP job                   print-job, print-uri
ipq         query printer and job status    get-jobs, get-job-attributes
                                            get-printer-attributes
ipc         control printer and jobs        cancel-job,
                                            pause-printer, resume-printer, 
                                            purge-jobs, 
                                            hold-job, release-job, restart-job
iprm        remove job                      cancel-job (can be done also with 
                                            ipc)
ipr -o	    request any operation           in syntax similar to print job, but
                                            send an operation specified in
                                            -o option. Does not require filename
When running the programs with -h or --help option, a detailed description 
of available options is displayed.


ipr sends a printjob to an IPP printer. The print data is specified on the 
command line or is read from stdin.
Other parameters for the print job can be specified on the command line or in 
the IPP template file.

The template file has the following sections:

[operation-attrs]

[job-template-attrs]

[options]

Each section contains name/value pairs of the form:
Name=value

The allowed names for [options] are specified and described in the sample ipp-
template file that comes with this distribution.
The acceptable name/value pairs of the other two sections are governed by the 
IPP specification and are different depending on the operation that is executed.

Therefore, if you want to use attibutes specific for ipq (see supplied template),
you'll need to manually copy the template onto a new file,
do the changes, and use -t option to force usage of that template.
e.g:

Default template files
Program     template file
ipr         ipp-template
ipq,        ipp-templateq
ipc, iprm   ipp-templatec
ipr -o      ipp-templateo


E.g. the command

ipr -t mytemplate.ipp printjob.ps

will use mytemplate.ipp as template file, which will specify
IPP operation or job-template attributes.

If the template file is not specified, the program attempts to
locate the file in:

HOME/.ipp-template on unix 
and
%HOMEPATH%\.ipp-template or %HOME%/ipp-template on windows


If not found, the program will use default
attributes. For a list of default options, please see the sample 
template file.

The attribute settings provided via the template files can be also provided on 
the command line using the -D option. The command line always overrides template 
attributes.

-D define    definition of attribute or option, in the form:
                  (operations-attrs|job-template-attrs|options)/name=value
                  e.g. -D operation-attrs#which-jobs=completed to be used
                  with lpq
                  This option is functionaly equivalent to this template file
                  section:
                [operation-attrs]
                which-jobs=completed


Requesting-user-name operation attribute will be extracted from the environment 
variable (USER on linux, USERNAME on Windows NT). If this environment variable 
is not set, the user will program will use the value specified in template. Note 
that command line option always overrides template or environment variables.

Overview of IPP operations 
--------------------------

This is a list of IPP operations directly supported by this release. For 
complete list of IPP operations, please see http://www.pwg.org. Other operations 
are supported indirectly by using -o options. When using option ipr -o, you can 
issue any IPP operation, even those that may be defined in the future. The 
following section is taken from the ShinePrint IPP SDK which can be downloaded 
from http://www.shinesoft.com/shineprint/.
Please note that these values are hex. If you are using -o option, you have to 
use a decimal value.

    IPPOP_PRINT_JOB =      0x2,            //      Print-Job               M
    IPPOP_PRINT_URI =      0x3,            //      Print-URI               O
    IPPOP_VALIDATE_JOB =   0x4,            //      Validate-Job            M
    IPPOP_CREATE_JOB =     0x5,            //      Create-Job              O
    IPPOP_SEND_DOCUMENT =  0x6,            //      Send-Document           O
    IPPOP_SEND_URI =       0x7,            //      Send-URI                O
    IPPOP_CANCEL_JOB =     0x8,            //      Cancel-Job              M
    IPPOP_GET_JOB_ATTRS =  0x9,            //      Get-Job-Attributes      M
    IPPOP_GET_JOBS =       0xA,            //      Get-Jobs                M
    IPPOP_GET_PRINTER_ATTRS =   0xB,       //      Get-Printer-Attributes  M
    // extended operations: set 1
    IPPOP_HOLD_JOB       =     0xC,        //      Hold-Job                O
    IPPOP_RELEASE_JOB    =     0xD,        //      Release-Job             O
    IPPOP_RESTART_JOB    =     0xE,        //      Restart-Job             O

    IPPOP_RESERVED_FUTURE1 =   0xF,        //      Reserved for future ops N/A

    IPPOP_PAUSE_PRINTER  =     0x10,       //      Pause-Printer           O
    IPPOP_RESUME_PRINTER =     0x11,       //      Resume-Printer          O
    IPPOP_PURGE_JOBS     =     0x12,       //      Purge-Jobs              O



Return codes
------------

ipr family of programs provides exit code that can be used in unix shell scripts 
and Windows batch files to determined if the command was executing successfully.
Here are the meanings of individual error returns:

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
    2008: connect error



IPP Status Codes
----------------

The following section was taken from the headers of ShinePrint IPP SDK which can 
be downloaded from http://www.shinesoft.com/shineprint/.

0 successful-OK
1 successful-ok-ignored-or-substituted-attributes
2 successful-ok-conflicting-attributes
0x400 client-error-bad-request
0x401 client-error-forbidden
0x402 client-error-not-authenticated
0x403 client-error-not-authorized
0x404 client-error-not-possible
0x405 client-error-timeout
0x406 client-error-not-found
0x407 client-error-gone
0x408 client-error-request-entity-too-large
0x409 client-error-request-URI-too-long
0x40A client-error-document-format-not-supported
0x40B client-error-attributes-or-values-not-supported
0x40C client-error-uri-scheme-not-supported
0x40D client-error-charset-not-supported
0x40E client-error-conflicting-attributes

0x500 server-error-internal-server-error
0x501 server-error-operation-not-supported
0x502 server-error-service-unavailable
0x503 server-error-version-not-supported
0x504 server-error-device-error
0x505 server-error-temporary-error
0x506 server-error-not-accepting-jobs


Common HTTP Reply Codes:
------------------------

The following codes are specified in the RFC.

     Status-Code    = "100"  ; Continue
                    | "101"  ; Switching Protocols
                    | "200"  ; OK
                    | "201"  ; Created
                    | "202"  ; Accepted
                    | "203"  ; Non-Authoritative Information
                    | "204"  ; No Content
                    | "205"  ; Reset Content
                    | "206"  ; Partial Content
                    | "300"  ; Multiple Choices
                    | "301"  ; Moved Permanently
                    | "302"  ; Found
                    | "303"  ; See Other
                    | "304"  ; Not Modified
                    | "305"  ; Use Proxy
                    | "307"  ; Temporary Redirect
                    | "400"  ; Bad Request
                    | "401"  ; Unauthorized
                    | "402"  ; Payment Required
                    | "403"  ; Forbidden
                    | "404"  ; Not Found
                    | "405"  ; Method Not Allowed
                    | "406"  ; Not Acceptable
                    | "407"  ; Proxy Authentication Required
                    | "408"  ; Request Time-out
                    | "409"  ; Conflict
                    | "410"  ; Gone
                    | "411"  ; Length Required
                    | "412"  ; Precondition Failed
                    | "413"  ; Request Entity Too Large
                    | "414"  ; Request-URI Too Large
                    | "415"  ; Unsupported Media Type
                    | "416"  ; Requested range not satisfiable
                    | "417"  ; Expectation Failed
                    | "500"  ; Internal Server Error
                    | "501"  ; Not Implemented
                    | "502"  ; Bad Gateway
                    | "503"  ; Service Unavailable
                    | "504"  ; Gateway Time-out
                    | "505"  ; HTTP Version not supported


Sample commands. 
---------------

Note: 
For more samples, please go to http://www.shinesoft.com/ipr. 
In some cases commands don't fit on one line and "\" is used as line 
continuation character. In real life, these cases need to be typed as one line.
Some samples use capture from linux which is indicated by the prompt
[zfischer@edel ipr]$ which is not part of the command.


ipr -D options#timeout=60 -D operation-attrs#attributes-charset=utf-8 \
-D operation-attrs#job-name="job34" test.dat
IPP operation successfully submitted.

Prints test.dat to the printer specified by printer-uri in the template file. 
The values for timeout, attributes-charset and job-name from the default 
template file are overridden.

ipr -P http://www.shinesoft.com/sp/ipp.cgi test.dat
IPP operation successfully submitted.

Prints test.dat to the printer specified on the command line. Other attributes, 
such as job-name, attributes-charset etc. are taken from the default template 
file in the home directory.

ipr test.dat
Loaded template file D:\util/ipp-template
Sending file test.dat
About to send the following request:

Message contents
Version=0x100
operation id/status code=2 (0x2) Print-Job
request id=100 (0x64)
Attributes
Attrs group: Op
charset: attributes-charset: utf-8
naturalLanguage: attributes-natural-language: en-us
uri: printer-uri: http://iris/scripts/ippis.dll
name: requesting-user-name: peterm


IPP operation successfully submitted.
Received the following response from IPP printer:

Message contents
Version=0x100
operation id/status code=0 (0x0) successful-OK
request id=100 (0x64)
Attributes
Attrs group: Op
charset: attributes-charset: utf-8
naturalLanguage: attributes-natural-language: en-us
text: status-message: successful-OK

Attrs group: Job0
uri: job-uri: http://iris/scripts/ippis.dll?329
integer: job-id: 329
enum: job-state: 3

Prints a file, and since verbose option was specified shows the IPP request and 
response on the screen (send it to stdout).

ipr -P http://nohost/sp/ipp.cgi test.dat
Error 1000 connecting to IPP server at http://nohost/sp/ipp.cgi.
The url may be wrong or the WEB server hosting IPP may be down

Attempts to print test.dat, but fails because the printer URL is incorrect.

ipr -P http://badprinter/ipp.cgi test.dat
IPP operation was not submitted successfully.
Response status code: 0x504 (1284): server-error-device-error

Attempts to print to a printer which is malfunctioning. Displays the error 
returned by printer.


[zfischer@edel ipr]$ ipq -D operation-attrs#limit=5
IPP operation successfully submitted.
Jobs for http://iris/scripts/ippis.dll

job-uri                           job-id job-state job-printer-uri               
time-at-creation job-priori
ty copies sides     attributes-charset attributes-natural-language job-name   
job-originating-user-name
http://iris/scripts/ippis.dll?297 297    3         http://iris/scripts/ippis.dll 
96525            1
   1      one-sided utf-8              en-us                       Noname job 
zfischer
http://iris/scripts/ippis.dll?298 298    3         http://iris/scripts/ippis.dll 
96568            1
   1      one-sided utf-8              en-us                       Noname job 
peterm
http://iris/scripts/ippis.dll?299 299    3         http://iris/scripts/ippis.dll 
96709            1
   1      one-sided utf-8              en-us                       Noname job 
peterm
http://iris/scripts/ippis.dll?300 300    3         http://iris/scripts/ippis.dll 
96758            1
   1      one-sided utf-8              en-us                       Noname job 
fred
http://iris/scripts/ippis.dll?301 301    3         http://iris/scripts/ippis.dll 
105563           1
   1      one-sided utf-8              en-us                       Noname job 
peterm

Shows the list of the last 5 not-completed jobs in the printer queue for the 
printer specified in the template file. The output doesn't fit into single lines 
so the lines are folded. The following example limits the number of attributes 
to get narrower output.

[zfischer@edel ipr]$ ipq -D operation-attrs#limit=5 -D operation-
attrs#requested-
attributes=job-id -D operatio
n-attrs/requested-attributes=job-name
IPP operation successfully submitted.
Jobs for http://iris/scripts/ippis.dll

job-id job-name
329    Noname job
328    Noname job
327    ippprint document
326    Noname job
325    job34


Shows the last 5 jobs, but limits the number of attributes returned. The same 
effect as by 5 -D operation-attrs#requested-attributes=job-id -D operation-
attrs/requested-attributes=job-name can be achieved by modifying the ipp-
template file.

The same effect as above can be achieved by omitting operation-attr keyword like 
this:

[zfischer@edel ipr]$ ipq -c -D limit=5 -D requested-attributes=job-id \
> -D requested-attributes=job-name
IPP operation successfully submitted.
Jobs for http://iris/scripts/ippis.dll

job-id job-name
329    Noname job
328    Noname job
327    ippprint document
326    Noname job
325    job34

[zfischer@edel ipr]$ ipq -c -D operation-attrs#limit=5 -D operation-
attrs/requested-attributes=job-id -D opera
tion-attrs/requested-attributes=job-name
IPP operation successfully submitted.
Jobs for http://iris/scripts/ippis.dll

job-id job-name
307    Noname job
305    Noname job
296    Noname job
295    Noname job
294    Noname job

Does the same as the previous command, but shows 'completed' jobs by specifying 
-c option.

[zfischer@edel ipr]$ ipq -p -P http://www.shinesoft.com/sp/ipp.cgi
IPP operation successfully submitted.
Printer attributes for http://www.shinesoft.com/sp/ipp.cgi

operations-supported: 2
: 4
: 8
: 9
: 10
: 11
: 12
: 13
: 14
: 16
: 17
printer-uri-supported: http://www.shinesoft.com/sp/ipp.cgi
printer-name: ShinePrint Printer Version 1.09
printer-location: Cupertino    CA
printer-info: This printer is used for Testing IPP. Some large jobs may be 
truncated during processing
uri-security-supported: none
printer-state: 3
printer-state-reasons: none
charset-configured: utf-8
charset-supported: utf-8
natural-language-configured: en-us
generated-natural-language-supported: en-us
document-format-default: application/vnd.hp-PCL
document-format-supported: application/vnd.hp-PCL
: text/plain
: application/postscript
: application/octet-stream
printer-is-accepting-jobs: 1
pdl-override-supported: not-attempted
job-hold-until-default: no-hold
job-hold-until-supported: indefinite
media-default: na-letter-white
media-supported: na-letter-white
printer-more-info: www.shinesoft.com/shineprint/
color-supported: 1
printer-make-and-model: ShinePrint 1.09
printer-up-time: 256521

Shows all printer attributes.

ipq -c -D operation-attrs#requesting-user-name="fred" \
    -D operation-attrs#my-jobs=1

IPP operation successfully submitted.
Jobs for http://iris/scripts/ippis.dll

job-uri                           job-id job-state job-printer-uri
 time-at-creation job-priority copies sides     attributes-charset attributes-na
tural-language job-name   job-originating-user-name
http://iris/scripts/ippis.dll?300 300    9         http://iris/scripts/ippis.dll
 96758            1            1      one-sided utf-8              en-us
               Noname job fred


Shows jobs and their attributes for user "fred" (this overrides the default user 
established from ipp-templateq or the environment variable)

[zfischer@edel ipr]$ ipc printer pause
IPP operation successfully submitted.
[zfischer@edel ipr]$ ipq -p -D operation-attrs#requested-attributes=printer-state
IPP operation successfully submitted.
Printer attributes for http://iris/scripts/ippis.dll

printer-state: 5

[zfischer@edel ipr]$ ipc printer resume
IPP operation successfully submitted.
[zfischer@edel ipr]$ ipq -p -D operation-attrs#requested-attributes=printer-state
IPP operation successfully submitted.
Printer attributes for http://iris/scripts/ippis.dll

printer-state: 3

The sequence of commands above pauses the printer, shows its state (requests 
printer attributes, but only one attribute: printer-state), then resumes the 
printer and shows the state again.
We see that the printer state changes to 5 (paused) and than back to 3 (idle) 
again.

[zfischer@edel ipr]$ ipr -o 4 -D document-format=text/plain
IPP operation successfully submitted to http://iris/scripts/ippis.dll.
[zfischer@edel ipr]$ ipr -o 4 -D document-format=fake/format
IPP operation to http://iris/scripts/ippis.dll was not submitted successfully.
Response status code: 0x40a (1034): client-error-document-format-not-supported

The two commands above request validate-job, first with a reasonable format, 
then with a format that probably won't be supported.

For more information on different attributes, see the IPP specification at 
http://www.pwg.org


Tips
----
1) For frequently used options, you can use batch files that point to customized 
template files, such as:
;ipp-narrow-jobs
; attributes for ipq: narrow output, only a few job attributes specified
;
[operation-attrs]
attributes-charset=utf-8
attributes-natural-language=en-us
printer-uri=http://iris/scripts/ippis.dll
requested-attributes=job-id
requested-attributes=job-state
requested-attributes=job-name
limit=10
; end of ipp-narrow-jobs

Then, the resulting output from ipq -c looks like this:

[zfischer@edel ipr]$ ipq -t ipp-narrow-jobs -c
IPP operation successfully submitted.
Jobs for http://iris/scripts/ippis.dll

job-id job-state job-name
329    9         Noname job
328    9         Noname job
327    9         ippprint document
326    9         Noname job
325    9         job34
324    9         job34
323    9         my job
322    9         Noname job
321    9         http://www.mercurycenter.com/pr
320    9         http://www.nytimes.com/yr/mo/da

2) Return code 411 from http transport usually indicates that a chunked request 
was attempted but the server is a cgi and doesn't support chunking.

3) You can use aliases and shell scripts that ease job submission, such as
  iprw provided with ipr distribution, which names the job according to
  the filename submitted 
  or aliases such as:
# shows jobs in narrow format
alias ipqn='ipq -c -t ~/.ipp-narrowjobs'
# shows printed jobs in narrow format
alias ipqnc='ipq -c -t ~/.ipp-narrowjobs'


# to print narrow job listing without template:
export IPP_PRINTER_URI="http://edel:631/printers/printer"
alias ipqn="ipq -P $IPP_PRINTER_URI -D operation-attrs#requested-attributes=job-id -D operation-attrs#requested-attributes=job-state -D operation-attrs#requested-attributes=job-name"

  You can customize the system to use just one template file and define default 
  values passed via scripts and aliases and -D option



Limitations
-----------
1) The option -D operation-attrs#name=value can be shortened also as -D 
name=value. If this is done, neither name nor value may contain the character 
"#". A work-around is to use the full syntax -D operation-attrs#name=value.
2) When option -o is used, it's the responsibility of the user to use correct 
arguments. It's recommended that -n (no validation) is used if problems occur 
that prevent client from submitting the request. To see the details of the 
request, please use the verbose option (-v).


Frequently Asked Questions (FAQ)
--------------------------


Q0: When running ipr, I get the message "printer-uri must be specified."
A0: Your printer uri needs to be specified on the command line, using -P option 
or in the ipp-template file.

Q1: I get the message "timed out". How do I make it work?
A1: Set options#timeout parameter in your template file or on the command line:
    -D options#timeout=600 sets the timeout to 10 minutes.

Q2: When running on Windows NT, I get the message " Error loading template file 
/ipp-templateq: Can't open file"


A2: Set your HOME environment variable to point to the location where the ipp-
templateq file resides.


Q3: When using ipq command, I don't see any jobs queued on the printer.

A3: ipq shows the not-completed jobs by default. If your job has been printed, 
it may be viewed by: ipq -c if the IPP printer you are using keeps printed job 
history.


Q4: When restarting job, I get client-error-not-authorized error (see below). 
How can I correct it?

> ipc job restart 284
IPP operation was not submitted successfully.
Response status code: 0x403 (1027): client-error-not-authorized

A4: Depending on the printer you are using, you need to have the same 
requesting-user-name as the user who submitted the job.
One way of restarting on IPP servers with flexible policies is:
> ipc -D requesting-user-name=zfischer job restart 284

Q5: When submiting requests, two files are always created in the current 
directory. How can I disable this feature? 

A5: This feature can be disabled by changing the ipp-template* files to contain:
[options]
tracefiles=1

or by using the option

-D options#filetrace=1

Q6: How do I issue a job-validate operation?

A5: Use -o option:
ipr -o 4 

Q7: I get an error complaining about missing library libstdc++.

A7: Install the correct library. This is how to do it in Debian Linux:

Note: Depending on the version of ipr, the needed library may be different.
 You can run ldd (as root) to find out ipr dependencies, e.g.:
root# ldd `which ipr`
        libstdc++.so.2.8 => /usr/lib/libstdc++.so.2.8 (0x40025000)
        libm.so.6 => /lib/libm.so.6 (0x40066000)
        libc.so.6 => /lib/libc.so.6 (0x40087000)
        /lib/ld-linux.so.2 => /lib/ld-linux.so.2 (0x40000000)


Bugs
----

Current list of bugs can be found at: http://www.shinesoft.com/ipr/bugs.html


