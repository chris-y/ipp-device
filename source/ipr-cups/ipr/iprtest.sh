# adjust this env. var. to reflect your printer settings
PRINTER=http://edel:631/printers/printer 
OPTIONS=-v
set -x
ipq $OPTIONS -c -D requested-attributes=job-id  -P $PRINTER
echo Result: $?
ipq $OPTIONS -c -P $PRINTER
echo Result: $?
ipq $OPTIONS -p -P $PRINTER
echo Result: $?
# using default template:
# must install properly .ipp-template
ipq $OPTIONS -c
echo Result: $?
ipq $OPTIONS -c -D requested-attributes=job-id
echo Result: $?
ipr $OPTIONS -o 10 -D requested-attributes=job-id
echo Result: $?
ipr $OPTIONS -o 10 
echo Result: $?
ipr $OPTIONS -o 11
echo Result: $?
	