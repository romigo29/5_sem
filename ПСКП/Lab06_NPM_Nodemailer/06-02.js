const http = require('http');
const fs = require('fs');
const nodemailer = require('nodemailer');
const querystring = require('querystring');

const EMAIL = "pigeon99905@gmail.com";
const APP_PASSWORD = "kjbj mvsb gadj cjxx";

const PORT = 5000;

http.createServer((req, res) => {
    if (req.method === 'GET') {

        let html = fs.readFileSync('./index.html');
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        res.end(html);
    }
    else if (req.method === 'POST') {
        let body = '';
        req.on('data', chunk => body += chunk.toString());
        req.on('end', () => {
            const data = querystring.parse(body);

            const transporter = nodemailer.createTransport({
                service: 'gmail',
                auth: {
                    user: EMAIL,
                    pass: APP_PASSWORD
                }
            });

            const mailOptions = {
                from: data.from,
                to: data.to,
                subject: 'Сообщение с Node.js',
                text: data.message
            };

            transporter.sendMail(mailOptions, (error, info) => {
                res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                if (error) {
                    res.end(`<h3>Ошибка отправки: ${error}</h3><a href="/">Назад</a>`);
                } else {
                    res.end(`<h3>Письмо отправлено: ${info.response}</h3><a href="/">Назад</a>`);
                }
            });
        });
    }
}).listen(PORT, () => console.log(`Server running on http://localhost:${PORT}`));