var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get("/zone/:id", function(req, res) {
    zoneID = parseInt(req.params.id);
    var batch = [];
    connection.query("SELECT COUNT(*) AS value FROM Zone WHERE ZoneID=" + zoneID, function(err, result) {
        if (err) {
            console.error(err.message);
            res.send(400);
        } else {
            exists = result[0].value;
            if (exists != 0) {
                connection.query("SELECT * FROM Zone WHERE ZoneID=" + zoneID, function(err, result) {
                    if (err) {
                        console.log(err);
                        res.send(err)
                    } else if (result.size != 0) {
                        var zoneInfo = {
                            'ZoneID': zoneID,
                            'ZoneLatitude': result[0].ZoneLatitude,
                            'ZoneLongitude': result[0].ZoneLongitude
                        }
                        connection.query("SELECT * FROM Measurement WHERE ZoneID=" + zoneID, function(err, rows) {
                            if (err) {
                                console.log(err.message);
                            } else {
                                for (var i = 0; i < rows.length; i++) {
                                    var time = (rows[i].time).split(':');
                                    var date = (rows[i].date).split('-');

                                    var measurement = {
                                        'measurementID': rows[i].measurementID,
                                        'stationID': rows[i].stationID,
                                        'time': rows[i].time,
                                        'date': rows[i].date,
                                        'datetime': new Date(date[0], date[1], date[2], time[0], time[1], time[2], 00),
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
                                    batch.push(measurement);
                                }

                                const sortedDate = batch.sort((a, b) => a.datetime - b.datetime);

                                res.render('zoneInfo', {
                                    "zoneInfo": zoneInfo,
                                    "batch": sortedDate
                                })
                            }
                        })
                    }
                })
            } else
                res.render('error', { "error": 404 });
        }
    });
});

module.exports = router;
