var http = require('http')
var fs = require('fs');

const PORT = 5000;

http.createServer(function (request, response) {

    if (request.url === '/html') {

        let html = fs.readFileSync('./index.html');
        response.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        response.end(html);
    }

    else {
        response.writeHead(404, { 'Content-Type': 'text/plain' });
        response.end('Not found');
    }
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}/`);