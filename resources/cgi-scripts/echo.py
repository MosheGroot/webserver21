#!/usr/bin/python3
import os
import sys

# echo
if (os.environ['REQUEST_METHOD'] == 'POST'):    
    data = sys.stdin.read()
else:
    data = "You didn't used POST method, so there is nothing to ECHO... :c"


response_body = """<html>
    <title> ECHO </title>

    <body>
        <h1>{}</h1>
    </body>
</html>""".format(data)

print("Content-Type: text/html", end='\r\n')
print("Content-Length: {}".format(len(response_body)), end='\r\n')
print(end='\r\n')
print(response_body, end='')

