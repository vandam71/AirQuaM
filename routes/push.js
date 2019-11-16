var express = require('express');
var router = express.Router();

router.get('/test', function(req, res, next){
    res.render('dbtest');
});

router.post('/db', function(req,res){
    res.send(req.body.dbcontent);
});

module.exports = router;