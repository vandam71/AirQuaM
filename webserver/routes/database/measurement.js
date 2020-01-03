var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

var ID;
var exists;

//POST method for /measurement
router.post('/measurement', function (req, res) {
    var Latitude = (req.body.GPSlatitude).toFixed(2);
    var Longitude = (req.body.GPSlongitude).toFixed(2);
    connection.query("SELECT COUNT(*) AS value FROM Zone WHERE ZoneLatitude=" + Latitude + " AND ZoneLongitude=" + Longitude, function (err, result) {
        if (err) {
            console.error(err.message);
        }
        exists = result[0].value;
        if (exists == 0) {
            console.log("here");
            connection.query("INSERT INTO Zone (ZoneLatitude, ZoneLongitude) VALUES ('" + Latitude + "', '" + Longitude + "')", function (err, result) {
                if (err) {
                    console.error(err.message);
                }
            });
        }
        connection.query("SELECT COUNT(*) AS value FROM Station WHERE stationID=" + req.body.stationID, function (err, result) {
            if (result[0].value == 0) {
                console.log("No Station matching ID: " + req.body.stationID);
                res.send();
            }
            else {
                connection.query("SELECT (ZoneID) FROM Zone WHERE ZoneLatitude=" + Latitude + " AND ZoneLongitude=" + Longitude, function (err, result) {
                    if (err) {
                        console.error(err);
                    }
                    ID = result[0].ZoneID;
                    connection.query("INSERT INTO Measurement (stationID, time, date, GPSlatitude, GPSlongitude, ZoneID, NO2, CO, CO2, TVOC, Temperature, Humidity) VALUES ('" + req.body.stationID + "', '" + req.body.time + "', '" + req.body.date + "', '" + req.body.GPSlatitude + "', '" + req.body.GPSlongitude + "', '" + ID + "', '" + req.body.NO2 + "', '" + req.body.CO + "', '" + req.body.CO2 + "', '" + req.body.TVOC + "', '" + req.body.Temperature + "', '" + req.body.Humidity + "')", function (err, result) {
                        if (err) {
                            console.error(err.message);
                        }
                        connection.query("SELECT * FROM Measurement", function (err, result) {
                            if (err) {
                                console.error(err.message);
                            }
                            res.send(result);
                        });
                    });
                });
            }
        });
    })
});

//GET method for /measurement
router.get('/measurement', function (req, res) {
    connection.query("SELECT * FROM Measurement", function (err, result) {
        if (err) {
            console.error(err.message);
        }
        res.send(result);
    });
});

//DELETE method for /measurement
router.delete('/measurement', function (req, res) {
    connection.query("SELECT * FROM Measurement WHERE measurementID=" + req.body.measurementID, function (err, result) {
        if (err) {
            console.error(err);
        }
        if (result.length == 0) {
            res.send("No Matching ID");
        }
        else {
            connection.query("DELETE FROM Measurement WHERE measurementID=" + req.body.measurementID, function (err, result) {
                if (err) {
                    console.log(err);
                }
                connection.query("SELECT * FROM Measurement", function (err, result) {
                    if (err) {
                        console.error(err);
                    }
                    res.send(result);
                });
            });
        };
    });
});

module.exports = router;
