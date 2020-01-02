var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

//GET method for /station
router.get('/station', function (req, res) {
    connection.query("SELECT * FROM Station", function (err, result) {
        if (err) {
            console.error(err);
        }
        res.send(result);
    });
});

router.get('/station/:id', function (req, res) {
    stationID = parseInt(req.params.id)
    connection.query("SELECT * FROM Station WHERE stationID=" + stationID, function (err, result) {
        if (err) {
            console.error(err);
        }
        res.send(result);
    });
});

//POST method for /station
router.post('/station', function (req, res) {
    connection.query("INSERT INTO Station (name, sampleRate, activeSensors) VALUES ('" + req.body.name + "', '" + req.body.sampleRate + "', '" + req.body.activeSensors + "')", function (err, result) {
        if (err) {
            console.error(err);
        }
    });
    connection.query("SELECT * FROM Station", function (err, result) {
        if (err) {
            console.error(err);
        }
        res.send(result);
    });
});

//PUT method for /station
router.put('/station', function (req, res) {
    connection.query("SELECT * FROM Station WHERE stationID=" + req.body.stationID, function (err, result) {
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
                connection.query("SELECT * FROM Station", function (err, result) {
                    if (err) {
                        console.error(err);
                    }
                    res.send(result);
                });
            });
        };
    });
});

//DELETE method for /station
router.delete('/station', function (req, res) {
    connection.query("SELECT * FROM Station WHERE stationID=" + req.body.stationID, function (err, result) {
        if (err) {
            console.error(err);
        }
        if (result.length == 0) {
            res.send("No Matching ID");
        }
        else {
            connection.query("DELETE FROM Station WHERE stationID=" + req.body.stationID, function (err, result) {
                if (err) {
                    console.log(err);
                }
                connection.query("SELECT * FROM Station", function (err, result) {
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