var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get('/station/:id', function (req, res) {
    connection.query("SELECT * FROM Station WHERE StationID=" + parseInt(req.params.id), function (err, result) {
        if (err) {
            console.log(err);
        } else {
            var station = {
                'stationID': result[0].stationID,
                'name': result[0].name,
                'sampleRate': result[0].sampleRate,
                'activeSensors': result[0].activeSensors
            }
            res.render('stationinfo', {station})
        }
    })
})

router.post('/station/:id', function (req, res){
    connection.query("UPDATE Station SET name='" + req.body.stationName + "', sampleRate='" + parseInt(req.body.sampleRate) + "', activeSensors='" + parseInt(req.body.activeSensors) + "' WHERE stationID=" + parseInt(req.params.id), function (err, result, fields) {
        if (err) {
            console.log(err);
        }
        res.redirect('/info/station')
    });
})

router.post('/station/:id/delete', function (req, res){
    connection.query("DELETE FROM Station WHERE stationID=" + parseInt(req.params.id), function (err, result) {
        if (err) {
            console.log(err);
        }
        res.redirect('/info/station/')
    });
})

module.exports = router;