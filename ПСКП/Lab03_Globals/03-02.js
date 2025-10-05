var http = require('http');
var fs = require('fs');
var url = require('url');

const PORT = 5000;

function factorial(k) {
    if (isNaN(k) || k < 0) return null;
    if (k == 0 || k == 1) return 1;

    return k * factorial(k - 1);
}


http.createServer(function (request, response) {

    const parsedUrl = url.parse(request.url, true);
    const pathname = parsedUrl.pathname;
    const query = parsedUrl.query;


    if (pathname === '/' && request.method === 'GET') {
        let html = fs.readFileSync('./factorial.html');
        response.writeHead(200, { 'content-type': 'text/html' });
        response.end(html);
    }
    else if (pathname === '/fact' && request.method === 'GET') {

        const k = parseInt(query.k, 10);

        if (isNaN(k) || k < 0) {
            response.writeHead(400, { 'content-type': 'application/json; charset=utf-8' });
            response.end(JSON.stringify({ error: 'Invalid parameter k' }));
        }

        else {

            const fact = factorial(k);
            response.writeHead(200, { 'content-type': 'application/json; charset=utf-8' });
            response.end(JSON.stringify({ k: k, fact: fact }));

        }
    }
    else {
        response.writeHead(404, { 'content-type': 'text/plain; charset=utf-8' });
        response.end('Not found');
    }
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}/`);