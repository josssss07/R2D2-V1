let gateway = `ws://${window.location.hostname}/ws`;
let websocket;
let cmdMap = {};
const inputs = [...document.querySelectorAll("input.btn")];

function onLoad(){
    initWebSocket();
}
window.addEventListener('load', onLoad);

function sendCmd(arg){
    websocket.send(arg);
    document.querySelector("h1").textContent = arg;
}

function stop(){
    websocket.send("s");
    document.querySelector("h1").textContent = "s";
}

function initWebSocket(){
    console.log("Trying to open a websocket connection:");
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(){
    console.log("Connection etablished: ");
    inputs.forEach(elem => {
        elem.onmousedown = sendCmd.bind(this, elem.id);
        elem.ontouchstart = sendCmd.bind(this, elem.id);
        if (elem.id == "l" || elem.id == "r"){    
            elem.onmouseup = sendCmd.bind(this, "ts");
            elem.onmouseout = sendCmd.bind(this, "ts");
            elem.ontouchmove = sendCmd.bind(this, "ts");
            elem.ontouchend = sendCmd.bind(this, "ts");
        }
        else{
            elem.onmouseup = stop;
            elem.onmouseout = stop;
            elem.ontouchmove = stop;
            elem.ontouchend = stop;
        }
    })
}


function onClose(){
    console.log("Connection lost: ");
    setTimeout(initWebSocket, 5000);
}

function onMessage(event){
    console.log("Message received: ");
    console.log
}

