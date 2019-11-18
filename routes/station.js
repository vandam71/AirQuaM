var express = require('express');
var router = express.Router();
var connection = require('./lib/db');

router.post('/station', function (req, res){
    connection.query("INSERT INTO Station (name) VALUES ('" + req.body.name + "')", function (err, result){
        if (err){
            console.error(err);
        }
    });
    connection.query("SELECT * FROM Station", function (err, result, fields){
        if(err){
            console.error(err);
        }
        res.send(result);
    });
});

module.exports = router;