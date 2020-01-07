var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get('/station', function(req, res) {
    var stationList = [];

    connection.query("SELECT * FROM Station", function(err, rows, fields) {
        if (err) {
            console.log(err.message);
        } else {
            for (var i = 0; i < rows.length; i++) {
                var station = {
                    'stationID': rows[i].stationID,
                    'name': rows[i].name,
                    'sampleRate': rows[i].sampleRate,
                    'state': rows[i].state
                }
                stationList.push(station);
            }
            res.render('station', { "stationList": stationList });
        }
    });
});

router.get('/measurement', function(req, res) {
    var measurementList = [];

    connection.query("SELECT * FROM Measurement", function(err, rows, fields) {
        if (err) {
            console.log(err.message)
        } else {
            for (var i = 0; i < rows.length; i++) {
                var measurement = {
                    'measurementID': rows[i].measurementID,
                    'stationID': rows[i].stationID,
                    'time': rows[i].time,
                    'date': rows[i].date,
                    'GPSlatitude': rows[i].GPSlatitude,
                    'GPSlongitude': rows[i].GPSlongitude,
                    'ZoneID': rows[i].ZoneID,
                    'NO2': rows[i].NO2,
                    'CO': rows[i].CO,
                    'CO2': rows[i].CO2,
                    'TVOC': rows[i].TVOC,
                    'Temperature': rows[i].Temperature,
                    'Humidity': rows[i].Humidity
                }
                measurementList.push(measurement);
            }
            res.render('measurement', { "measurementList": measurementList });
        }
    })
})

router.get('/zone', function(req, res) {
    var zoneList = [];

    connection.query("SELECT * FROM Zone", function(err, rows, fields) {
        if (err) {
            console.log(err.message)
        } else {
            for (var i = 0; i < rows.length; i++) {
                var zone = {
                    'ZoneID': rows[i].ZoneID,
                    'ZoneLatitude': rows[i].ZoneLatitude,
                    'ZoneLongitude': rows[i].ZoneLongitude
                }
                zoneList.push(zone);
            }
            res.render('zone', { 'zoneList': zoneList });
        }
    })
})

module.exports = router;