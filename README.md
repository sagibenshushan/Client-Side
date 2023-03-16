
EX2 – HTTP client 
Authored by Sagi Ben Shushan
209351147

==Description for EX2==
An HTTP client that constructs an HTTP request based on the user’s command line input, sends the 
request to a Web server, receives the reply from the server, and displays the reply message on the 
screen.
The client side sends a request to a specific server in an exact format, when the format is not exact an error is printed to the screen.
There are several additional conditions for the format, if -r or -p is specified then a number must appear which will represent the number of arguments or the length of the rite for which -p appears.
when a port is not specified, the default HTTP port number of 80 is used.
The error that print to the screen is: "client [–p n <text>] [–r n <pr1=value1 pr2=value2 …>] <URL>".


--- the checker have to know---
If there is more than 1 show of '-p' or '-r' i print to the screen error.


==Program Files==
client.c

==How to compile EX2?==
compile: gcc -Wall -o client client.c

===Input:==
commands from the user

==Output:==
The request(by the asked format) and the response from the server.




