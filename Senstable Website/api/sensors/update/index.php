<?php
include_once '../../api.php';

$api = new API();
$api->setHeaders();

$json = file_get_contents('php://input');
$json = json_decode($json);

$token              = $json->token;
$id                 = $json->id;
$name               = $json->name;
$short_description  = $json->short_description;
$serial_number      = $json->serial_number;
$diagram            =$json->diagram;
$wiki               = $json->wiki;
$code               = $json->code;

// first check token
if ($api->authenticate($token)) {
    if ($diagram != ""){
        // update diagram
        $sql = "UPDATE sensors SET diagram=:diagram WHERE id=:id";
        $param = array(
            "id" => $id,
            "diagram" => $diagram,
        );
    } else if ($code != ""){
        // update code
        $sql = "UPDATE sensors SET code=:code WHERE id=:id";
        $param = array(
            "id" => $id,
            "code" => $code,
        );
    } else {
        // update sensor
        $sql = "UPDATE sensors SET name=:name, short_description=:short_description, serial_number=:serial_number, wiki=:wiki WHERE id=:id";
        $param = array(
            "id" => $id,
            "name" => $name,
            "short_description" => $short_description,
            "serial_number" => $serial_number,
            "wiki" => $wiki,
        );
    }

    // check if the SQL query was succesful
    if ($api->sendQuery($sql, $param)) {
        $data = [
            'result' => 'Sensor succesvol bijgewerkt!',
        ];
    } else {
        $data = [
            'error' => 'Updaten is mislukt!',
        ];
    }
} else {
    $data = [
        'error' => 'U moet opnieuw inloggen!',
    ];
}

echo json_encode($data);