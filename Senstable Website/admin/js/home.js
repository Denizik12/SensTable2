// directory sensoren API
const urlSensorApi = "http://145.24.222.125/api/sensors/"

function loadSensorTable() {
    const tbody = document.getElementById("table");

    //create a request variable
    const request = new XMLHttpRequest();

    //open a new connection
    request.open("GET", urlSensorApi + "get/", true);

    request.onload = function() {
        //parse the object
        let data = JSON.parse(this.response);

        //handle errors
        if (request.status >= 200 && request.status < 400) {
            data.forEach(sensor => {
                const entry = document.createElement("tr");

                const id = document.createElement("td");
                id.textContent = sensor.id;

                const name = document.createElement("td");
                name.textContent = sensor.name;

                const description = document.createElement("td");
                description.textContent = sensor.short_description;

                const serialNumber = document.createElement("td");
                serialNumber.textContent = sensor.serial_number;

                const dateAdded = document.createElement("td");
                dateAdded.textContent = sensor.date_added;

                const deleteSensor = document.createElement("td");
                let i_element = document.createElement('I');
                i_element.className = "fas fa-trash-alt delete-sensor";
                i_element.name = `delete-${sensor.id}`;
                i_element.onclick = function() {
                    removeSensor(i_element.name.replace("delete-", ""));
                };
                deleteSensor.appendChild(i_element);

                const editSensor = document.createElement("td");
                i_element = document.createElement('I');
                i_element.className = "fas fa-edit edit-sensor";
                i_element.name = `${sensor.id}`;
                i_element.onclick = function() {
                    getTableData(i_element.name);
                };
                editSensor.appendChild(i_element);

                entry.appendChild(id)
                entry.appendChild(name);
                entry.appendChild(description);
                entry.appendChild(serialNumber);
                entry.appendChild(dateAdded);
                entry.appendChild(deleteSensor);
                entry.appendChild(editSensor);
                tbody.appendChild(entry);
            });
        }
    }

    request.send();
}

function clearSensorTable() {
    let myTable = document.getElementById("sensorTable");
    let rowCount = myTable.rows.length;
    for (let x = rowCount - 1; x > 0; x--) {
        myTable.deleteRow(x);
    }
}

document.getElementById("add-sensor").addEventListener("click", () => {
    openForm();
});

function openForm() {
    document.getElementById("popup-form").style.display = "block";
}

function closeForm() {
    document.getElementById("popup-form").style.display = "none";
}

function removeSensor(sensorId) {
    if (confirm(`Weet u zeker dat u deze sensor, #${sensorId}, wilt verwijderen?`)) {
        let token = localStorage.getItem('token');
        let data = {
            "id": sensorId,
            "token": token
        };

        let request = new XMLHttpRequest();
        request.open("POST", urlSensorApi + "delete/", true);
        request.setRequestHeader('Content-type', 'application/json');
        request.send(JSON.stringify(data));

        request.onload = function() {
            let json = JSON.parse(request.responseText);
            showResponse(json);
        };
    }
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

    clearSensorTable();
    loadSensorTable();
}

function getTableData(id) {
    let request = new XMLHttpRequest();
    //open a new connection
    request.open("POST", urlSensorApi + "get/", true);
    request.setRequestHeader('Content-type', 'application/json');

    let data = {
        "id": id,
    };
    request.send(JSON.stringify(data));

    request.onload = function() {
        //parse the object
        let data = JSON.parse(this.responseText);
        openEditForm();

        let hd = document.getElementById("edit-form-header");
        hd.innerHTML = `${hd.innerHTML}${data[0]['id']}`;

        Object.keys(data[0]).forEach(function(key) {
            if (document.getElementById(`sens-${key}-edit`) !== null)
                document.getElementById(`sens-${key}-edit`).value = data[0][key];
        });
    }
}

function saveSensor() {
    let request = new XMLHttpRequest();
    request.open("PUT", urlSensorApi + "update/", true);
    request.setRequestHeader('Content-type', 'application/json');

    let token = localStorage.getItem('token');
    let data = {
        "id": document.getElementById('edit-form-header').innerHTML.replace("Sensor bewerken: #", ""),
        "name": document.getElementById(`sens-name-edit`).value,
        "short_description": document.getElementById(`sens-short_description-edit`).value,
        "serial_number": document.getElementById(`sens-serial_number-edit`).value,
        "wiki": document.getElementById(`sens-wiki-edit`).value,
        "code": document.getElementById(`sens-code-edit`).value,
        "token": token
    };
    request.send(JSON.stringify(data));

    request.onload = function() {
        let json = JSON.parse(request.responseText);
        showResponse(json);
        closeEditForm();
    }

}

function openEditForm() {
    document.getElementById("edit-popup-form").style.display = "block";
}

function closeEditForm() {
    document.getElementById("edit-popup-form").style.display = "none";
    let hd = document.getElementById("edit-form-header");
    hd.innerHTML = `Sensor bewerken: #`;
}