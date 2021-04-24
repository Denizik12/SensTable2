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

                const showCode = document.createElement("td");
                let i_element = document.createElement('I');
                i_element.className = "fas far fa-eye show-code";
                i_element.name = `show-${sensor.id}`;
                i_element.onclick = function() {
                    getTableData(i_element.name, "showCode");
                };
                showCode.appendChild(i_element);

                const editCode = document.createElement("td");
                i_element = document.createElement('I');
                i_element.className = "fas fa-edit edit-code";
                i_element.name = `${sensor.id}`;
                i_element.onclick = function() {
                    getTableData(i_element.name, "editForm");
                };
                editCode.appendChild(i_element);

                entry.appendChild(id)
                entry.appendChild(name);
                entry.appendChild(serialNumber);
                entry.appendChild(showCode);
                entry.appendChild(editCode);
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

function getTableData(id, func) {
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
        "id": document.getElementById('edit-form-header').innerHTML.replace("Sensor code bewerken: #", ""),
        "name": "",
        "short_description": "",
        "serial_number": "",
        "diagram": "",
        "wiki": "",
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
    let hd = document.getElementById("edit-form-header");
    hd.innerHTML = `Sensor code bewerken: #`;
}

function closeEditForm() {
    document.getElementById("edit-popup-form").style.display = "none";
    let hd = document.getElementById("edit-form-header");
    hd.innerHTML = `Sensor code bewerken: #`;
}

function test() {
    window.alert("test");
}

function openCode() {
    document.getElementById("show-code-popup").style.display = "block";
}

function closeCode() {
    document.getElementById("show-code-popup").style.display = "none";
}