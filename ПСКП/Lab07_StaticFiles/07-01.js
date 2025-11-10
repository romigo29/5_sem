const http = require('http');
const url = require('url');
const staticHandler = require('./m07-01')('./static');

http.createServer((request, response) => {
    const parsedUrl = url.parse(request.url, true);
    const resourcePath = parsedUrl.pathname;

    console.log(`Request: ${request.method} ${resourcePath}`);

    if (request.method !== 'GET') {
        response.writeHead(405, { 'Content-Type': 'text/plain' });
        response.end("405: Method Not Allowed");
        return;
    }

    if (resourcePath === '/') {
        staticHandler.sendFile(request, response, { 'Content-Type': 'text/html' }, '/index.html');
        return;
    }

    if (staticHandler.isStatic('html', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'text/html' }, resourcePath);
    else if (staticHandler.isStatic('css', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'text/css' }, resourcePath);
    else if (staticHandler.isStatic('js', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'application/javascript' }, resourcePath);
    else if (staticHandler.isStatic('png', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'image/png' }, resourcePath);
    else if (staticHandler.isStatic('docx', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'application/msword' }, resourcePath);
    else if (staticHandler.isStatic('json', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'application/json' }, resourcePath);
    else if (staticHandler.isStatic('xml', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'application/xml' }, resourcePath);
    else if (staticHandler.isStatic('mp4', resourcePath)) staticHandler.sendFile(request, response, { 'Content-Type': 'video/mp4' }, resourcePath);
    else staticHandler.writeHTTP404(response);
}).listen(5000, () => {
    console.log("Server is running on http://localhost:5000");
});
