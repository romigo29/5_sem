const http = require('http');

const jsonObject = JSON.stringify({
    "comment": "Запрос.Лабораторная работа 8/10",
    "x": 1,
    "y": 2,
    "s": "Сообщение",
    "m": ["a", "b", "c", "d"],
    "o": {
        "surname": "Иванов",
        "name": "Иван"
    }
});

const options = {
    hostname: 'localhost',
    port: 5000,
    path: '/4',
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
};

const request = http.request(options, (response) => {
    let data = '';

    console.log(`Статус ответа: ${response.statusCode}`);

    response.on('data', (chunk) => {
        data += chunk;
    });

    response.on('end', () => {
        console.log('Response body: ', JSON.parse(data));
    });

});

request.on('error', (error) => {
    console.error(`Ошибка запроса: ${error.message}`);
});

request.write(jsonObject);
request.end();