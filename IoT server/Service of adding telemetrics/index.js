const AWS = require('aws-sdk');
const accessid = process.env.ACCESSID;
const sccretkey = process.env.SECRETACCESSKEY;
AWS.config.update({
    accessKeyId: accessid,
    secretAccessKey: sccretkey,
    region: 'eu-central-1'
});

const ddb = new AWS.DynamoDB({
    apiVersion: '2012-08-10'
});
const docClient = new AWS.DynamoDB.DocumentClient();



exports.handler = (event, context, callback) => {
    if (event.horizon_id === undefined || event.air_temperature === undefined || event.atmospheric_pressure === undefined || event.air_humidity === undefined || event.soil_moisture === undefined ||
        event.fine_particles25 === undefined || event.fine_particles100 === undefined || event.wind_direction === undefined || event.battery_voltage === undefined ||
        event.current_load === undefined || event.rssi === undefined

    ) {
        callback("All data is required");
    }

    checkDevice(event.horizon_id).then((data) => {
        console.log(data);
        if (!data.Items || !data.Items[0].device_id) {
            callback(null, {
                statusCode: 404,
                body: JSON.stringify({
                    message: 'error - Unknown ID',
                })
            });
        } else {
            const params = {
                TableName: 'horizon_measurement',
                Item: {
                    'device_id': {
                        S: event.horizon_id
                    },
                    'measurement_timestamp': {
                        N: Date.now().toString()
                    },
                    'air_temperature': {
                        N: event.air_temperature.toString()
                    },
                    'atmospheric_pressure': {
                        N: event.atmospheric_pressure.toString()
                    },
                    'air_humidity': {
                        N: event.air_humidity.toString()
                    },
                    'soil_moisture': {
                        N: event.soil_moisture.toString()
                    },
                    'fine_particles25': {
                        S: event.fine_particles25
                    },
                    'fine_particles100': {
                        S: event.fine_particles100
                    },
                    'wind_direction': {
                        S: event.wind_direction
                    },
                    'battery_voltage': {
                        N: event.battery_voltage.toString()
                    },
                    'current_load': {
                        N: event.current_load.toString()
                    },
                    'rssi': {
                        N: event.rssi.toString()
                    }
                }

            };
            ddb.putItem(params, function(err, data) {
                if (err) {
                    console.log("Error", err);
                } else {
                    console.log("Success", data);
                }
            });
        }
    })
        .catch((err) => {
            callback(null, {
                statusCode: 503,
                body: JSON.stringify({
                    message: err,

                })
            });
        });




};

function checkDevice(did) {
    const params = {
        TableName: "horizon_devices",
        KeyConditionExpression: "device_id = :did",
        ExpressionAttributeValues: {
            ":did": did
        },
        Limit: 1
    };
    return docClient.query(params).promise();
}