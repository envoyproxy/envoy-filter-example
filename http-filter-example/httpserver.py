#!/usr/bin/python
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer

PORT = 8081

class doHandler(BaseHTTPRequestHandler):
    # Handler for the GET requests
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        print self.headers  # print the http header
        # Send the html message
        self.wfile.write("Hello World !")
        return

try:
    # Create a web server and define the handler to manage the incoming request
    server = HTTPServer(('', PORT), doHandler)
    print 'Started httpserver on port ' , PORT

    # Wait forever for incoming http requests
    server.serve_forever()

except KeyboardInterrupt:
    print 'shutting down the web server'
    server.socket.close()
