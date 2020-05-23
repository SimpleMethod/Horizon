var app = angular.module("login", ["ngCookies"]).controller("MultiController");


app.controller('CheckController', function ($scope, $http, $cookies) {
    let hash =$cookies.get("hash");
    let deviceID =$cookies.get("deviceid");
    console.log(hash+deviceID);

    if(hash===undefined || deviceID===undefined || deviceID.length===0 || hash.length===0)
    {
       //
    }
    else
    {
       window.location.href = "/";
    }
});

app.controller('LoginController', function ($scope, $http, $cookies) {
    $scope.update_settings = function (auth) {

        if(!auth.device_ID || !auth.check_code)
        {
            console.log("Problem with reading all data");
            return -1;
        }
        let obj = {};
        obj.device_id = auth.device_ID;
        obj.check_code  = auth.check_code;
        let jsonString= JSON.stringify(obj);

        console.error(jsonString);
        $http({
            url: 'https://sl1ty574c4.execute-api.eu-central-1.amazonaws.com/latest/login',
            method: 'POST',
            data: JSON.stringify(obj),
            contentType: 'application/json'
        }).then(
            function (response) {
                console.log("Stack:"+ JSON.stringify(response.data));
                $scope.statusCode = response.data.statusCode;
                console.log(response.data.statusCode);
                if(response.data.statusCode===200)
                {
                    $scope.hash=response.data.hash;
                    $cookies.put("hash",$scope.hash);
                    $cookies.put("deviceid", auth.device_ID);
                    window.location.href = "/";
                }
                else
                {
                    $scope.showModal=1;
                    console.error("Status code:"+$scope.statusCode);
                }
            },
            function () {
                $scope.errors = "Problem with getting data from a esp32";
            }
        );
    };
});
