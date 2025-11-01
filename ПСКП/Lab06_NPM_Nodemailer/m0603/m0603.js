// m0603.js
const nodemailer = require('nodemailer');

const EMAIL = "pigeon99905@gmail.com";
const APP_PASSWORD = "kjbjmvsbgadjcjxx";

function send(messageText) {
    const transporter = nodemailer.createTransport({
        service: 'gmail',
        auth: {
            user: EMAIL,
            pass: APP_PASSWORD
        }
    });

    const mailOptions = {
        from: EMAIL,
        to: EMAIL,
        subject: 'Сообщение из Node.js (06-03)',
        text: messageText          
    };

    transporter.sendMail(mailOptions, (error, info) => {
        if (error)
            console.log('Ошибка отправки:', error);
        else
            console.log('Письмо отправлено:', info.response);
    });
}

module.exports = { send };
