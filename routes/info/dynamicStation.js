var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get('/station/:id', function(req, res){
    stationID = parseInt(req.params.id);
    res.render('stationinfo', {'stationID': stationID});
})

module.exports = router;