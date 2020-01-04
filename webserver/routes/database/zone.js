var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

//GET method for /zone
router.get('/zone', function(req, res) {
    //SELECT all values from Zone table
    connection.query("SELECT * FROM Zone", function(err, result, fields) {
        if (err) {
            console.error(err);
            res.send(400);
        }
        res.send(result);
    });
});

//POST method for /zone
router.post('/zone', function(req, res) {
    var insert = 1;
    //SELECT all Zone entries and put check if ZoneLatitude or Zonelongitude have a match
    connection.query("SELECT ZoneLatitude, ZoneLongitude FROM Zone", function(err, result, fields) {
        if (err) {
            console.error(err);
            res.send(400);
        } else {
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
                console.error(406);
                connection.query("SELECT * FROM Zone", function(err, result, fields) {
                    if (err) {
                        console.error(err);
                        res.send(400);
                    } else
                        res.send(406);
                });
            } else if (insert == 1) {
                //INSERT new values to table
                connection.query("INSERT INTO Zone (ZoneLatitude, ZoneLongitude) VALUES ('" + req.body.ZoneLatitude + "', '" + req.body.ZoneLongitude + "')", function(err, result) {
                    if (err) {
                        console.error(err);
                        res.send(400);
                    } else
                        res.send(201);
                });
            }
        }
    });
});

//DELETE method from /zone
router.delete('/zone', function(req, res) {
    connection.query("SELECT * FROM Zone WHERE ZoneID=" + req.body.ZoneID, function(err, result) {
        if (err) {
            console.error(err);
            res.send(400);
        }
        if (result.length == 0) {
            res.send(404);
        } else {
            connection.query("DELETE FROM Zone WHERE ZoneID=" + req.body.ZoneID, function(err, result) {
                if (err) {
                    console.log(err);
                    res.send(400);
                } else
                    res.send(202)
            });
        };
    });
})

module.exports = router;