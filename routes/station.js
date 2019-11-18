var express = require('express');
var router = express.Router();
var connection = require('./lib/db');

router.get('/station', function (req,res){
    connection.query("SELECT * FROM Station", function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        res.send(result);
    });
});

router.post('/station', function (req, res) {
    connection.query("INSERT INTO Station (name) VALUES ('" + req.body.name + "')", function (err, result) {
        if (err) {
            console.error(err);
        }
    });
    connection.query("SELECT * FROM Station", function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        res.send(result);
    });
});

router.put('/station', function (req, res) {
    connection.query("SELECT * FROM Station WHERE stationID=" + req.body.stationID, function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        if (result.length == 0) {
            res.send("No Matching ID");
        }
        else {
            connection.query("UPDATE Station SET name='" + req.body.name + "' WHERE stationID=" + req.body.stationID, function (err, result, fields) {
                if (err) {
                    console.log(err);
                }
                connection.query("SELECT * FROM Station", function (err, result, fields) {
                    if (err) {
                        console.error(err);
                    }
                    res.send(result);
                });
            });
        };
    });
});

router.delete('/station', function (req, res) {
    connection.query("SELECT * FROM Station WHERE stationID=" + req.body.stationID, function (err, result, fields) {
        if (err) {
            console.error(err);
        }
        if (result.length == 0) {
            res.send("No Matching ID");
        }
        else {
            connection.query("DELETE FROM Station WHERE stationID=" + req.body.stationID, function (err, result, fields) {
                if (err) {
                    console.log(err);
                }
                connection.query("SELECT * FROM Station", function (err, result, fields) {
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