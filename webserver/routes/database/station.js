var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

//GET method for /station
router.get('/station', function(req, res) {
    connection.query("SELECT * FROM Station", function(err, result) {
        if (err) {
            console.error(err);
            res.send(400);
        }
        res.send(result);
    });
});

router.get('/station/:id', function(req, res) {
    stationID = parseInt(req.params.id)
    connection.query("SELECT * FROM Station WHERE stationID=" + stationID, function(err, result) {
        if (err) {
            console.error(err);
            res.send(400);
        }
        res.send(result);
    });
});

//POST method for /station
router.post('/station', function(req, res) {
    connection.query("INSERT INTO Station (name, sampleRate, activeSensors) VALUES ('" + req.body.name + "', '" + req.body.sampleRate + "', '" + req.body.activeSensors + "')", function(err, result) {
        if (err) {
            console.error(err);
            res.send(400);
        } else
            res.send(202);
    });
});

//PUT method for /station
router.put('/station', function(req, res) {
    connection.query("SELECT * FROM Station WHERE stationID=" + req.body.stationID, function(err, result) {
        if (err) {
            console.error(err);
            res.send(400);
        }
        if (result.length == 0) {
            res.send(406);
        } else {
            connection.query("UPDATE Station SET name='" + req.body.name + "', sampleRate='" + req.body.sampleRate + "', activeSensors='" + req.body.activeSensors + "' WHERE stationID=" + req.body.stationID, function(err, result, fields) {
                if (err) {
                    console.log(err);
                    res.send(400);
                } else
                    res.send(202);
            });
        };
    });
});

//DELETE method for /station
router.delete('/station', function(req, res) {
    connection.query("SELECT * FROM Station WHERE stationID=" + req.body.stationID, function(err, result) {
        if (err) {
            console.error(err);
            res.send(400);
        } else if (result.length == 0) {
            res.send(406);
        } else {
            connection.query("SELECT *FROM Measurement WHERE stationID=" + req.body.stationID, function(err, result) {
                if (err) {
                    console.error(err);
                } else if (result.length == 0) {
                    connection.query("DELETE FROM Station WHERE stationID=" + parseInt(req.body.stationID), function(err, result) {
                        if (err) {
                            if (err.errno == 1451) {
                                console.error(err.message)
                                res.send(400);
                            } else
                                console.error(err);
                        } else
                            res.send(202);
                    });
                } else
                    res.send(403);
            })
        };
    });
})

module.exports = router;