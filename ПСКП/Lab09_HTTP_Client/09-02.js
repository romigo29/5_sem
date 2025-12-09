const http = require('http');

const x = 1;
const y = 2;

const options = {
    hostname: 'localhost',
    port: 5000,
    path: `/2?x=${x}&&y=${y}`,
    method: 'GET'
};

const request = http.request(options, (response) => {
    let data = '';

    console.log(`Статус ответа: ${response.statusCode}`);

    response.on('data', (chunk) => {
        data += chunk;
    });

    response.on('end', () => {
        console.log('Данные ответа:');
        console.log(data);
    });
});

request.on('error', (error) => {
    console.error(`Ошибка запроса: ${error.message}`);
});

request.end();