var express = require('express');
var router = express.Router();
var path = require('path');
var mysql = require('mysql');
var connection = require('./lib/db');

router.get('/test', function (req, res, next) {
    res.render('dbtest');
});

router.post('/Zone', function (req, res) {
    var total = 0;
    var insert = 1;
    connection.query("SELECT ZoneLatitude, ZoneLongitude FROM Zone", function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        for (var i = 0; i<result.length; i++) {
            if (result[i].ZoneLatitude == req.body.a) {
                insert = 0;
                break;
            };
            if (result[i].ZoneLongitude == req.body.b) {
                insert = 0;
                break;
            }
            insert = 1;
        };
        if (insert == 0) {
        } else if (insert == 1) {
            connection.query("INSERT INTO Zone (ZoneLatitude, ZoneLongitude) VALUES ('" + req.body.a + "', '" + req.body.b + "')", function (err, result) {
                if (err) {
                    console.error(err);
                }
            });
        }
    });
    connection.query("SELECT * FROM Zone", function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        res.send(result);
    });
});

module.exports = router;