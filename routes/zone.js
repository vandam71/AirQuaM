var express = require('express');
var router = express.Router();
var connection = require('./lib/db');

router.post('/zone', function (req, res) {
    var total = 0;
    var insert = 1;
    connection.query("SELECT ZoneLatitude, ZoneLongitude FROM Zone", function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        console.log
        for (var i = 0; i < result.length; i++) {
            if (result[i].ZoneLatitude == req.body.ZoneLatitude) {
                insert = 0;
                break;
            };
            if (result[i].ZoneLongitude == req.body.ZoneLongitude) {
                insert = 0;
                break;
            }
            insert = 1;
        };
        if (insert == 0) {
            console.log("Already in Table");
            connection.query("SELECT * FROM Zone", function (err, result, fields) {
                if (err) {
                    console.error(err);
                }
                res.send(result);
            });    
        } else if (insert == 1) {
            connection.query("INSERT INTO Zone (ZoneLatitude, ZoneLongitude) VALUES ('" + req.body.ZoneLatitude + "', '" + req.body.ZoneLongitude + "')", function (err, result) {
                if (err) {
                    console.error(err);
                }
            });
            connection.query("SELECT * FROM Zone", function (err, result, fields) {
                if (err) {
                    console.error(err);
                }
                res.send(result);
            });
        }
    });
});

module.exports = router;