var http = require('http')

const PORT = 5000;

http.createServer(function (request, response) {

    if (request.url === '/api/name') {

        response.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        response.end('Романов Игорь Вячеславович');

    }
    else {
        response.writeHead(404, { 'Content-Type': 'text/plain' });
        response.end('Not found');
    }
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}/`);