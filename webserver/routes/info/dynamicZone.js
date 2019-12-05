var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get("/zone/:id", function (req, res) {
    zoneID = parseInt(req.params.id);
    var batch = [];
    var time = [];
    connection.query("SELECT * FROM Zone WHERE ZoneID=" + zoneID, function (err, result) {
        if (err) {
            console.log(err);
        }
        else {
            var zoneInfo = {
                'ZoneID': zoneID,
                'ZoneLatitude': result[0].ZoneLatitude,
                'ZoneLongitude': result[0].ZoneLongitude
            }
            connection.query("SELECT * FROM Measurement WHERE ZoneID=" + zoneID, function (err, rows) {
                if (err) {
                    console.log(err.message);
                }
                else {
                    for (var i = 0; i < rows.length; i++) {
                        for (var i = 0; i < rows.length; i++) {
                            var words = (rows[i].time).split(':');
                            var timeLiteral = parseInt(words[1]) + (parseInt(words[0])*60);

                            var measurement = {
                                'measurementID': rows[i].measurementID,
                                'stationID': rows[i].stationID,
                                'time': rows[i].time,
                                'timeLiteral': timeLiteral,
                                'date': rows[i].date,
                                'GPSlatitude': rows[i].GPSlatitude,
                                'GPSlongitude': rows[i].GPSlongitude,
                                'ZoneID':rows[i].ZoneID,
                                'NO2': rows[i].NO2,
                                'CO': rows[i].CO,
                                'CO2': rows[i].CO2,
                                'TVOC': rows[i].TVOC,
                                'Temperature': rows[i].Temperature,
                                'Humidity': rows[i].Humidity
                            }
                            batch.push(measurement);
                        }
                    }
                    res.render('zoneInfo', {
                        "zoneInfo": zoneInfo,
                        "batch": batch
                    })
                }
            })
        }
    })
});

module.exports = router;