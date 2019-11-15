var mysql = require('mysql');
var connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: '1234',
    database: 'airquam'
});

connection.connect(function(error){
    if(!!error){
        console.log(error);
    } else {
        console.log('Connected!');
    }
});
module.exports = connection;