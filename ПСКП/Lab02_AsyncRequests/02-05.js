var http = require('http')
var fs = require('fs');

const PORT = 5000;

http.createServer(function (request, response) {

    if (request.url === '/fetch') {

        let html = fs.readFileSync('./fetch.html');
        response.writeHead(200, { 'content-type': 'text-html; charset=utf-8' })
        response.end(html);
    }
    else if (request.url === '/api/name') {
        response.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        response.end('Романов Игорь Вячеславович');
    }
    else {
        response.writeHead(404, { 'Content-Type': 'text/plain' });
        response.end('Not found');
    }
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}/`);