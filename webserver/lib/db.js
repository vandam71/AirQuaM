var mysql = require('mysql');
var connection = mysql.createPool({
    host: 'eu-cdbr-west-02.cleardb.net',
    user: 'b6b35d28509c7a',
    password: 'e5c4c077',
    database: 'heroku_d7884420be88c67',
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