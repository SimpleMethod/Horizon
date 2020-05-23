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
    if (event.device_id === undefined || event.hash === undefined || event.fromdate === undefined || event.todate === undefined || event.sorttype === undefined || event.limit === undefined) {
        callback(null, {
            statusCode: 404,
            body: JSON.stringify({
                message: 'ERROR ALL DATA IS REQUIRED'
            }),
            stack: event
        });

    }
    
    checkDevice(event.device_id).then((data) => {
        console.log(data);
        if ((!data.Items || !data.Items[0].device_id)) {
            callback(null, {
                statusCode: 404,
                body: JSON.stringify({
                    message: 'ERROR UNKNOWN ID OR HASH',
                })
            });
        } else {
            if (data.Items[0].serial_number === event.hash && data.Items[0].device_id === event.device_id) {

                console.log("Data:" + event.fromdate + "\n\r");
                getData(event.device_id, event.fromdate, event.todate, event.sorttype, event.limit).then((measurement) => {
                    callback(null, {measurement});

                }).catch((err) => {
                    callback(null, {
                        statusCode: 404,
                        body: JSON.stringify({
                            message: "ERROR GETTING DATA",
                        })
                    });
                });

            } else {
                callback(null, {
                    statusCode: 403,
                    body: JSON.stringify({
                        message: "ERROR GETTING AUTH DATA",
                    })
                });
            }
        }
    })
        .catch((err) => {
            callback(null, {
                statusCode: 403,
                body: JSON.stringify({
                    message: "LAMBDA ERROR GETTING AUTH DATA",
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


function getData(did, fromDate, toDate, sortType, limit) {
    console.info("Stack: device_ID: " + did + "\n\r FromDate: " + fromDate + "\n\r ToDate: " + toDate + "\n\r SortType: " + sortType + "\n\r\ Limit: " + limit + "\n\r");

    const params = {
        TableName: "horizon_measurement",
        KeyConditionExpression: "device_id = :did AND measurement_timestamp BETWEEN  :fromDate AND :toDate",
        ExpressionAttributeValues: {
            ":did": did,
            ":fromDate": fromDate,
            ":toDate": toDate
        },
        Limit: limit,
        ScanIndexForward: sortType
    };
    return docClient.query(params).promise();
}