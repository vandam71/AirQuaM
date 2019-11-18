var express = require('express');
var router = express.Router();
var path = require('path');
var mysql = require('mysql');
var connection = require('./lib/db');

router.get('/test', function (req, res, next) {
    res.render('dbtest');
});

router.post('/db', function (req, res) {
    connection.query("INSERT INTO Zone (ZoneLatitude, ZoneLongitude) VALUES ('" + req.body.a + "', '" + req.body.b + "')", function(err, result){
        if (err) throw err;
        console.log("Inserted");
    });
});

module.exports = router;