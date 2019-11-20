var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

//GET method for /zone
router.get('/zone', function (req, res){
    //SELECT all values from Zone table
    connection.query("SELECT * FROM Zone", function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        res.send(result);
    });
});

//POST method for /zone
router.post('/zone', function (req, res) {
    var total = 0;
    var insert = 1;
    //SELECT all Zone entries and put check if ZoneLatitude or Zonelongitude have a match
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
            //INSERT new values to table
            connection.query("INSERT INTO Zone (ZoneLatitude, ZoneLongitude) VALUES ('" + req.body.ZoneLatitude + "', '" + req.body.ZoneLongitude + "')", function (err, result) {
                if (err) {
                    console.error(err);
                }
            });
            //SELECT all values from Zone table
            connection.query("SELECT * FROM Zone", function (err, result, fields) {
                if (err) {
                    console.error(err);
                }
                res.send(result);
            });
        }
    });
});

//DELETE method from /zone
router.delete('/zone', function (req, res) {
    connection.query("SELECT * FROM Zone WHERE ZoneID=" + req.body.ZoneID, function (err, result) {
        if (err) {
            console.error(err);
        }
        if (result.length == 0) {
            res.send("No Matching ID");
        }
        else {
            connection.query("DELETE FROM Zone WHERE ZoneID=" + req.body.ZoneID, function (err, result) {
                if (err) {
                    console.log(err);
                }
                connection.query("SELECT * FROM Zone", function (err, result) {
                    if (err) {
                        console.error(err);
                    }
                    res.send(result);
                });
            });
        };
    });
})

module.exports = router;