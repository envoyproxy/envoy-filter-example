#!/usr/bin/python3
from http.server import BaseHTTPRequestHandler,HTTPServer

HTTP_PORT = 8081

# This class will handles any incoming request
class doHandler(BaseHTTPRequestHandler):

    # Handler for the GET requests
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        print(self.headers)
        # Send the message
        self.wfile.write(b"Hello World !\r\n")
        self.wfile.write(self.headers.as_bytes())
        return

try:
    # Create a http server and define the handler to manage the request
    server = HTTPServer(('', HTTP_PORT), doHandler)
    print('Started httpserver on port ' , HTTP_PORT)

    # Wait forever for incoming http requests
    server.serve_forever()

except KeyboardInterrupt:
    print('^C received, shutting down the web server')
    server.socket.close()
