var mysql = require('mysql');
var connection = mysql.createPool({
    host: 'eu-cdbr-west-02.cleardb.net',
    user: 'bc2f6bc64cfba9',
    password: '0e84f59c',
    database: 'heroku_61f6ec79e99ef9a',
    dateStrings: true
});

connection.on('connection', function(connection) {
    console.log('DB Connection established');

    connection.on('error', function(err) {
        console.error('MySQL error', err.code);
    });

    connection.on('close', function(err) {
        console.error('MySQL close', err)
    });
});

module.exports = connection;
