// directory sensoren API
const urlSensorApi = "http://145.24.222.125/api/sensors/"

// detects when button is pressed
document.getElementById("add-sensor").addEventListener("click", () => {
    openForm();
});

function openForm() {
    document.getElementById("popup-form").style.display = "block";
}

function closeForm() {
    document.getElementById("popup-form").style.display = "none";
}

function addSensor() {
    let request = new XMLHttpRequest();
    let name = document.getElementById("sensName").value;
    let serialNumber = document.getElementById("sensSerialNumber").value;
    let diagram = document.getElementById("fileToBase64").value;
    let shortDescription = document.getElementById("sensShortDescription").value;
    let wiki = document.getElementById("sensWiki").value;
    let code = document.getElementById("sensCode").value;
    let token = localStorage.getItem('token');


    let data = {
        "name": name,
        "short_description": shortDescription,
        "serial_number": serialNumber,
        "diagram": diagram,
        "wiki": wiki,
        "code": code,
        "token": token
    };

    request.open("POST", urlSensorApi + "add/", true);
    request.setRequestHeader('Content-type', 'application/json');
    request.send(JSON.stringify(data));

    request.onload = function() {
        let json = JSON.parse(request.responseText);
        showResponse(json);
        closeForm();
    };
}

function showResponse(json) {
    let lbl = document.getElementById('lbl-status');

    if (Object.keys(json).includes("result")) {
        lbl.innerHTML = json["result"];
        lbl.style.color = "green";
    } else {
        lbl.innerHTML = json["error"];
        lbl.style.color = "red";
    }
}