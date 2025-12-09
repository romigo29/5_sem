const http = require('http');
const querystring = require('querystring');

const postData = querystring.stringify({
    x: 5,
    y: 10,
    s: 'test'
});

const options = {
    hostname: 'localhost',
    port: 5000,
    path: '/3',
    method: 'POST'
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

request.write(postData);
request.end();