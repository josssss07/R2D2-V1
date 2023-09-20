let gateway = `ws://${window.location.hostname}/ws`;
let websocket;

function onLoad(){
    initWebSocket();
}

window.addEventListener('load', onLoad);

function sendForward(){
    websocket.send("f");
}

function stop(){
    websocket.send("s");
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
    document.querySelector("#ip").onmousedown = sendForward;
    document.querySelector("#ip").onmouseup = stop;
}

function onClose(){
    console.log("Connection lost: ");
    setTimeout(initWebSocket, 5000);
}

function onMessage(event){
    console.log("Message received: ");
    console.log
}
