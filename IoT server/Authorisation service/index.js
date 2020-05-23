const AWS = require('aws-sdk');
const crypto = require('crypto');
const accessid = process.env.ACCESSID;
const sccretkey = process.env.SECRETACCESSKEY;
const hash = process.env.HASH;
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
    if (event.device_id === undefined || event.check_code === undefined) {
        callback("All data is required");
    }
    const hash_text=hash+event.device_id+event.check_code+hash;
    const crypto_sha256 = crypto.createHash('sha256').update(hash_text).digest('hex');

    checkDevice(event.device_id).then((data) => {
        console.log(data);
        if  (!data.Items || !data.Items[0].device_id)  {
            callback(null, {
                statusCode: 404,
                body: JSON.stringify({
                    message: 'ERROR GETTING UNKNOWN DATA',
                })
            });
        } else {

            if(data.Items[0].device_id===event.device_id && data.Items[0].check_code===event.check_code)
            {
                callback(null, {
                    statusCode: 200,
                    hash: crypto_sha256
                });
            }
            else
            {
                callback(null, {
                    statusCode: 403,
                    body: JSON.stringify({
                        message: "ERR",
                    })
                });
            }
        }
    }).catch((err) => {
            callback(null, {
                statusCode: 403,
                body: JSON.stringify({
                    message: "ERROR GETTING DATA FROM LAMBDA",
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