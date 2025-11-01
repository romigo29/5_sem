// m07-01.js
const fs = require('fs');
const path = require('path');

const mimeTypes = {
    html: 'text/html',
    css: 'text/css',
    js: 'text/javascript',
    png: 'image/png',
    docx: 'application/msword',
    json: 'application/json',
    xml: 'application/xml',
    mp4: 'video/mp4'
};

// Экспортируем функцию, принимающую корневой каталог (например, "static")
module.exports = function(staticPath) {
    return function(req, res) {
        if (req.method !== 'GET') {
            res.writeHead(405, { 'Content-Type': 'text/plain; charset=utf-8' });
            res.end('405 Method Not Allowed');
            return;
        }

        // Удаляем ведущий / из пути, если он есть
        let reqPath = decodeURI(req.url);
        if (reqPath === '/') reqPath = '/index.html'; // по умолчанию отдаем index.html

        const filePath = path.join(staticPath, reqPath);
        const ext = path.extname(filePath).substring(1); // расширение без точки

        // Проверяем MIME-тип
        const mime = mimeTypes[ext];
        if (!mime) {
            res.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
            res.end('404 Not Found: unsupported file type');
            return;
        }

        // Читаем файл и отправляем
        fs.readFile(filePath, (err, data) => {
            if (err) {
                res.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end('404 Not Found');
            } else {
                res.writeHead(200, { 'Content-Type': mime });
                res.end(data);
            }
        });
    };
};
