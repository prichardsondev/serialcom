const mongoose = require('mongoose');


const sensorSchema = new mongoose.Schema({

    temp:{
        type:Number,
        default:0,
        required:false
    },
    humidity:{
        type:Number,
        default:0,
        required:false
    },
    wind:{
        type:Number,
        default:0,
        required:false
    },
    direction:{
        type:String,
        default:0,
        required:false
    },
    sound:{
        type:Number,
        default:0,
        required:false
    },
    light:{
        type:Number,
        default:0,
        required:false
    },
    date: { type: Date, default: Date.now }
    
});

module.exports = mongoose.model('Sensors', sensorSchema);