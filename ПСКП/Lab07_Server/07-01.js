// 07-01.js
const http = require('http');
const path = require('path');
const staticHandler = require('./m07-01');
const PORT = 5000;

// Параметризуем имя каталога
const staticDir = path.join(__dirname, 'static');
const handler = staticHandler(staticDir);

const server = http.createServer((req, res) => {
    handler(req, res);
});


server.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}/`);
});
